#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "model/model.h"
#include "utils/db.h"
#include "defines.h"
struct configuration conf;

static MYSQL *conn;

static role_t attempt_login(MYSQL *conn, char *username, char *password) {
	MYSQL_STMT *login_procedure;
	
	MYSQL_BIND param[3];
	int role = 0;

	if(!setup_prepared_stmt(&login_procedure, "call login_segretario(?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		goto err2;
	}
    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, username, strlen(username));
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, password, strlen(password));
    set_binding_param(&param[2], MYSQL_TYPE_LONG, &role, sizeof(role));

	if (mysql_stmt_bind_param(login_procedure, param) != 0) {
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}

	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}

    set_binding_param(&param[0], MYSQL_TYPE_LONG, &role, sizeof(role));

    if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}
	
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	}

	mysql_stmt_close(login_procedure);
	return role;

    err:
	mysql_stmt_close(login_procedure);
    err2:
	return FAILED_LOGIN;
}

int main(void) {
    char options[3] = {'1','2', '3'};
    char op;
    role_t role;
    printf("\033[2J\033[H");
    printf("*** CONNECT TO TRAVEL AGENCY ***\n\n");
    printf("1) Sono un cliente\n");
    printf("2) Sono un segretario\n");
    printf("3) Quit\n");

    op = multiChoice("Select an option", options, 3);

    switch(op) {
        case '1':
            role=CLIENTE;
        case '2':
            break;
        case '3':
            exit(0);
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }

	if(!parse_config("users/login.json", &conf)) {
		fprintf(stderr, "Unable to load login configuration\n");
		exit(EXIT_FAILURE);
	}

	conn = mysql_init (NULL);
	if (conn == NULL) {
		fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_real_connect(conn, conf.host, conf.db_username, conf.db_password, conf.database, conf.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf (stderr, "mysql_real_connect() failed\n");
		mysql_close (conn);
		exit(EXIT_FAILURE);
	}
    if(op=='2') {
        printf("\033[2J\033[H");
        printf("*** Identificati con le tue credenziali da segretario *** \n\n");
        printf("Username: ");
        getInput(128, conf.username, false);
        printf("Password: ");
        getInput(128, conf.password, true);

        role = attempt_login(conn, conf.username, conf.password);
    }

	switch(role) {
		case CLIENTE:
			run_as_cliente(conn);
			break;

		case SEGRETARIO:
			run_as_segretario(conn);
			break;
			
		case FAILED_LOGIN:
			fprintf(stderr, "Invalid credentials\n");
			exit(EXIT_FAILURE);

		default:
			fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
			abort();
	}

	printf("Bye!\n");

	mysql_close (conn);
	return 0;
}
