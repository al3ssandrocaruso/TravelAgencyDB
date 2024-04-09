#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
#include "segretariodao.h"
static struct programma_di_viaggio_support * list_programmi_di_viaggio_segretari(MYSQL *conn){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    int status;
    int id;
    size_t row = 0;
    char nomeprogramma[MAX_LEN];
    int numgiorni;
    struct programma_di_viaggio_support *programmaDiViaggioSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_programmi_viaggi_segretari()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_programmi_viaggi_segretari statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Errore nel restituire lista dei programmi di viaggio\n", true);
    }
    mysql_stmt_store_result(prepared_stmt);

    programmaDiViaggioSupport= malloc(sizeof(*programmaDiViaggioSupport) + sizeof(struct programma_di_viaggio) * mysql_stmt_num_rows(prepared_stmt));

    if(programmaDiViaggioSupport == NULL)
        goto out;

    memset(programmaDiViaggioSupport, 0, sizeof(*programmaDiViaggioSupport) + sizeof(struct programma_di_viaggio) * mysql_stmt_num_rows(prepared_stmt));

    programmaDiViaggioSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &nomeprogramma, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_LONG, &numgiorni, sizeof (int));
    set_binding_param(&param[2], MYSQL_TYPE_LONG, &id, sizeof (int));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters lista_programmi_viaggi_segretari\n");
        free(programmaDiViaggioSupport);
        programmaDiViaggioSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        programmaDiViaggioSupport->programmiDiViaggio[row].id_programma=id;
        programmaDiViaggioSupport->programmiDiViaggio[row].num_giorni=numgiorni;
        strcpy(programmaDiViaggioSupport->programmiDiViaggio[row].nome_programma,nomeprogramma);
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return programmaDiViaggioSupport;



}

static void aggiungi_guida(MYSQL *conn,struct guida * guida){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    MYSQL_TIME datanascita;
    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_guida(?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_guidastatement\n", false);
    }

    date_to_mysql_time(guida->data_nascita, &datanascita);
    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,guida->cf,strlen(guida->cf));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,guida->nome,strlen(guida->nome));
    set_binding_param(&param[2],MYSQL_TYPE_VAR_STRING,guida->cognome,strlen(guida->cognome));
    set_binding_param(&param[3],MYSQL_TYPE_VAR_STRING,guida->email,strlen(guida->email));
    set_binding_param(&param[4],MYSQL_TYPE_DATE,&datanascita,sizeof(datanascita));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_guida\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'aggiunta della guida.\n");
    } else {
        printf("Guida correttamente aggiunta!\n");
    }

    mysql_stmt_close(prepared_stmt);
}
static void assegna_guida(MYSQL *conn,struct guida * guida,struct viaggio * viaggio){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    MYSQL_TIME datapartenza;
    if(!setup_prepared_stmt(&prepared_stmt, "call assegna_guida(?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize assegna_guida statement\n", false);
    }

    date_to_mysql_time(viaggio->data_partenza, &datapartenza);
    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,guida->cf,strlen(guida->cf));
    set_binding_param(&param[1],MYSQL_TYPE_LONG,&(viaggio->id_programma),sizeof(viaggio->id_programma));
    set_binding_param(&param[2],MYSQL_TYPE_DATE,&datapartenza,sizeof(datapartenza));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for assegna_guida\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'assegnare la guida\n");
    } else {
        printf("Guida correttamente assegnata! \n");
    }

    mysql_stmt_close(prepared_stmt);
}
static void assegna_autobus(MYSQL *conn,struct autobus_privato * autobusPrivato,struct viaggio * viaggio){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    MYSQL_TIME datapartenza;
    if(!setup_prepared_stmt(&prepared_stmt, "call assegna_autobus_privato(?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize assegna_autobus_privato statement\n", false);
    }

    date_to_mysql_time(viaggio->data_partenza, &datapartenza);
    set_binding_param(&param[2],MYSQL_TYPE_VAR_STRING,autobusPrivato->targa,strlen(autobusPrivato->targa));
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(viaggio->id_programma),sizeof(viaggio->id_programma));
    set_binding_param(&param[1],MYSQL_TYPE_DATE,&datapartenza,sizeof(datapartenza));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for assegna_autobus_privato\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'assegnare l'autobus privato\n");
    } else {
        printf("Autobus correttamente assegnata! \n");
    }

    mysql_stmt_close(prepared_stmt);
}
static void assegna_albergo(MYSQL *conn,struct albergo * albergo,struct viaggio * viaggio){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    MYSQL_TIME datapartenza;

    if(!setup_prepared_stmt(&prepared_stmt, "call assegna_albergo(?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize assegna_albergo statement\n", false);
    }

    date_to_mysql_time(viaggio->data_partenza, &datapartenza);
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(viaggio->id_programma),sizeof(viaggio->id_programma));
    set_binding_param(&param[1],MYSQL_TYPE_DATE,&datapartenza,sizeof(datapartenza));
    set_binding_param(&param[2],MYSQL_TYPE_VAR_STRING,albergo->indirizzo,strlen(albergo->indirizzo));
    set_binding_param(&param[3],MYSQL_TYPE_VAR_STRING,albergo->nome_località,strlen(albergo->nome_località));
    set_binding_param(&param[4],MYSQL_TYPE_LONG,&(albergo->num_notti),sizeof(int));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for assegna_albergo\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'assegnazione dell'albergo\n");
    } else {
        printf("Albergo correttamente assegnato! \n");
    }

    mysql_stmt_close(prepared_stmt);
}

static struct tappa_support *  list_tappe_for_a_program(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio){
    int status;
    size_t row = 0;
    MYSQL_STMT *prepared_stmt;
    char nome_localita[46];
    int data_arrivo;
    MYSQL_TIME ora_arrivo;
    int data_partenza;
    MYSQL_TIME ora_partenza;
    char trattamento[3];
    struct tappa_support*tappaSupport= NULL;

    MYSQL_BIND param[6];
    if(!setup_prepared_stmt(&prepared_stmt, "call get_itinerario_input_id(?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize get_itinerario_input_idstatement\n", false);
    }
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(programmaDiViaggio->id_programma),sizeof (programmaDiViaggio->id_programma));
    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for get_itinerario_input_id\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile resituire l'itinerario\n", true);
    }
    mysql_stmt_store_result(prepared_stmt);

    tappaSupport= malloc(sizeof(*tappaSupport) + sizeof(struct tappa) * mysql_stmt_num_rows(prepared_stmt));

    if(tappaSupport == NULL)
        goto out;

    memset(tappaSupport, 0, sizeof(*tappaSupport) + sizeof(struct tappa) * mysql_stmt_num_rows(prepared_stmt));
    tappaSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_localita, strlen(nome_localita));
    set_binding_param(&param[1], MYSQL_TYPE_LONG, &data_arrivo, sizeof (data_arrivo));
    set_binding_param(&param[2], MYSQL_TYPE_TIME, &ora_arrivo, sizeof (ora_arrivo));
    set_binding_param(&param[3], MYSQL_TYPE_LONG, &data_partenza, sizeof (data_partenza));
    set_binding_param(&param[4], MYSQL_TYPE_TIME, &ora_partenza, sizeof (ora_partenza));
    set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, trattamento, 3);

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for get_itinerario_input_id\n");
        free(tappaSupport);
        tappaSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(tappaSupport->tappe[row].nome_località,nome_localita);
        tappaSupport->tappe[row].giorno_arrivo=data_arrivo;
        mysql_time_to_string(&ora_arrivo,tappaSupport->tappe[row].ora_arrivo);
        tappaSupport->tappe[row].giorno_partenza=data_partenza;
        mysql_time_to_string(&ora_partenza,tappaSupport->tappe[row].ora_partenza);
        strcpy(tappaSupport->tappe[row].trattamento,trattamento);
        row++;
    }
    out:
    mysql_stmt_free_result(prepared_stmt);
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return tappaSupport;

}
static void aggiungi_localita(MYSQL *conn,struct localita *localita)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[2];

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_localita(?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_localita statement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,localita->nome_località,strlen(localita->nome_località));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,localita->stato,strlen(localita->stato));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_localita\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'aggiungere la località\n");
    } else {
        printf("Località correttamente aggiunta! \n");
    }

    mysql_stmt_close(prepared_stmt);
}

static void aggiungi_albergo(MYSQL *conn, struct albergo*albergo)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[9];

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_albergo(?, ?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_albergo statement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,albergo->indirizzo,strlen(albergo->indirizzo));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,albergo->nome_località,strlen(albergo->nome_località));
    set_binding_param(&param[2],MYSQL_TYPE_DOUBLE,&(albergo->costo_per_notte),sizeof(albergo->costo_per_notte));
    set_binding_param(&param[3],MYSQL_TYPE_VAR_STRING,albergo->referente,strlen(albergo->referente));
    set_binding_param(&param[4],MYSQL_TYPE_VAR_STRING,albergo->nome_albergo,strlen(albergo->nome_albergo));
    set_binding_param(&param[5],MYSQL_TYPE_VAR_STRING,albergo->fax,strlen(albergo->fax));
    set_binding_param(&param[6],MYSQL_TYPE_VAR_STRING,albergo->email,strlen(albergo->email));
    set_binding_param(&param[7],MYSQL_TYPE_VAR_STRING,albergo->telefono,strlen(albergo->telefono));
    set_binding_param(&param[8],MYSQL_TYPE_LONG,&(albergo->capienza_max),sizeof(albergo->capienza_max));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_albergo\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error(prepared_stmt, "Si è verificato un errore nell'aggiunta dell'albergo\n");
        goto out;
    } else {
        printf("Nuovo albergo correttamento aggiunto! \n");
    }
    out:
    mysql_stmt_close(prepared_stmt);
}
static struct viaggio_support * list_viaggi_by_status(MYSQL *conn, struct viaggio*viaggio){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    MYSQL_TIME giorno_partenza;
    MYSQL_TIME giorno_rientro;
    double costo;
    int num_partecipanti;
    int id_programma;

    struct viaggio_support *viaggioSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_viaggi_input_stato(?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_viaggi_input_stato statement\n", false);
    }
    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,&(viaggio->stato_viaggio),sizeof (viaggio->stato_viaggio));
    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for lista_viaggi_input_stato\n", true);
    }
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile restituire la lista dei viaggi\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    viaggioSupport= malloc(sizeof(*viaggioSupport) + sizeof(struct viaggio) * mysql_stmt_num_rows(prepared_stmt));

    if(viaggioSupport == NULL)
        goto out;

    memset(viaggioSupport, 0, sizeof(*viaggioSupport) + sizeof(struct viaggio) * mysql_stmt_num_rows(prepared_stmt));

    viaggioSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_DATE, &giorno_partenza, sizeof(giorno_partenza));
    set_binding_param(&param[1], MYSQL_TYPE_DATE, &giorno_rientro, sizeof (giorno_rientro));
    set_binding_param(&param[2], MYSQL_TYPE_DOUBLE, &costo, sizeof(costo));
    set_binding_param(&param[3], MYSQL_TYPE_LONG, &num_partecipanti, sizeof(num_partecipanti));
    set_binding_param(&param[4], MYSQL_TYPE_LONG, &id_programma, sizeof(id_programma));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_viaggi_input_stato\n");
        free(viaggioSupport);
        viaggioSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        mysql_date_to_string(&giorno_partenza,viaggioSupport->viaggi[row].data_partenza);
        mysql_date_to_string(&giorno_rientro,viaggioSupport->viaggi[row].data_rientro);
        viaggioSupport->viaggi[row].costo = costo;
        viaggioSupport->viaggi[row].id_programma = id_programma;
        viaggioSupport->viaggi[row].num_partecipanti_confermati = num_partecipanti;
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return viaggioSupport;

}
static struct guida_support * lista_assegnamenti_guide(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char cf[CF_LEN];
    MYSQL_TIME datapartenza;
    MYSQL_TIME datarientro;

    struct guida_support *guidaSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_assegnamenti_guide()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_assegnamenti_guide statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile  restituire gli assegnamenti delle guide\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    guidaSupport= malloc(sizeof(*guidaSupport) + sizeof(struct guida) * mysql_stmt_num_rows(prepared_stmt));

    if(guidaSupport == NULL)
        goto out;

    memset(guidaSupport, 0, sizeof(*guidaSupport) + sizeof(struct guida) * mysql_stmt_num_rows(prepared_stmt));

    guidaSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &cf, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_DATE, &datapartenza, sizeof (datapartenza));
    set_binding_param(&param[2], MYSQL_TYPE_DATE, &datarientro, sizeof (datarientro));


    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_assegnamenti_guide\n");
        free(guidaSupport);
        guidaSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(guidaSupport->guide[row].cf,cf);
        mysql_date_to_string(&datapartenza,guidaSupport->guide[row].dataP);
        mysql_date_to_string(&datarientro,guidaSupport->guide[row].dataR);
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return guidaSupport;

}
static struct autobus_support * lista_assegnamenti_autobus(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    char targa[TARGA_LEN];
    MYSQL_TIME datapartenza;
    MYSQL_TIME datarientro;

    struct autobus_support *autobusSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_assegnamenti_autobus()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_assegnamenti_autobus statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile resitituire gli assegnamenti degli autobus\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    autobusSupport= malloc(sizeof(*autobusSupport) + sizeof(struct autobus_privato) * mysql_stmt_num_rows(prepared_stmt));

    if(autobusSupport == NULL)
        goto out;

    memset(autobusSupport, 0, sizeof(*autobusSupport) + sizeof(struct autobus_privato) * mysql_stmt_num_rows(prepared_stmt));

    autobusSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &targa, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_DATE, &datapartenza, sizeof (datapartenza));
    set_binding_param(&param[2], MYSQL_TYPE_DATE, &datarientro, sizeof (datarientro));


    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_assegnamenti_autobus\n");
        free(autobusSupport);
        autobusSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(autobusSupport->autobusPrivato[row].targa,targa);
        mysql_date_to_string(&datapartenza,autobusSupport->autobusPrivato[row].dataP);
        mysql_date_to_string(&datarientro,autobusSupport->autobusPrivato[row].dataR);
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return autobusSupport;

}
static struct guida_support * list_info_guide(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[5];
    char cf[CF_LEN];
    char nome[MAX_LEN];
    char cognome[MAX_LEN];
    char email[MAX_LEN];
    MYSQL_TIME datanascita;

    struct guida_support *guidaSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_guide_all()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_guide_all statement\n", false);
    }
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile restituire la lista delle guide\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    guidaSupport= malloc(sizeof(*guidaSupport) + sizeof(struct guida) * mysql_stmt_num_rows(prepared_stmt));

    if(guidaSupport == NULL)
        goto out;

    memset(guidaSupport, 0, sizeof(*guidaSupport) + sizeof(struct guida) * mysql_stmt_num_rows(prepared_stmt));

    guidaSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &cf, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &nome, MAX_LEN);
    set_binding_param(&param[2], MYSQL_TYPE_VAR_STRING, &cognome, MAX_LEN);
    set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, &email, MAX_LEN);
    set_binding_param(&param[4], MYSQL_TYPE_DATE, &datanascita, sizeof (datanascita));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_guide_all\n");
        free(guidaSupport);
        guidaSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(guidaSupport->guide[row].nome,nome);
        strcpy(guidaSupport->guide[row].cf,cf);
        strcpy(guidaSupport->guide[row].cognome,cognome);
        strcpy(guidaSupport->guide[row].email,email);
        mysql_date_to_string(&datanascita,guidaSupport->guide[row].data_nascita);
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return guidaSupport;

}
static struct localita_support * list_info_localita(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[2];
    char nome[MAX_LEN];
    char stato[MAX_LEN];

    struct localita_support *localitaSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_localita_all()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_localita_all statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile restituire la lista delle guide\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    localitaSupport= malloc(sizeof(*localitaSupport) + sizeof(struct localita) * mysql_stmt_num_rows(prepared_stmt));

    if(localitaSupport == NULL)
        goto out;

    memset(localitaSupport, 0, sizeof(*localitaSupport) + sizeof(struct localita) * mysql_stmt_num_rows(prepared_stmt));

    localitaSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &nome, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &stato, MAX_LEN);

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_localita_all\n");
        free(localitaSupport);
        localitaSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(localitaSupport->localita[row].nome_località,nome);
        strcpy(localitaSupport->localita[row].stato,stato);
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return localitaSupport;

}
static struct autobus_support * list_info_autobus(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[4];
    char targa[TARGA_LEN];
    char nomedep[MAX_LEN];
    int capienza;
    double costo;

    struct autobus_support *autobusSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_autobus_all()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_autobus_all statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile restituire la lista degli autobus\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    autobusSupport= malloc(sizeof(*autobusSupport) + sizeof(struct autobus_privato) * mysql_stmt_num_rows(prepared_stmt));

    if(autobusSupport == NULL)
        goto out;

    memset(autobusSupport, 0, sizeof(*autobusSupport) + sizeof(struct autobus_privato) * mysql_stmt_num_rows(prepared_stmt));

    autobusSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &targa, TARGA_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &nomedep, MAX_LEN);
    set_binding_param(&param[2], MYSQL_TYPE_LONG, &capienza, sizeof(int));
    set_binding_param(&param[3], MYSQL_TYPE_DOUBLE, &costo, sizeof(double));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_autobus_all\n");
        free(autobusSupport);
        autobusSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(autobusSupport->autobusPrivato[row].targa,targa);
        strcpy(autobusSupport->autobusPrivato[row].nome_deposito,nomedep);
        autobusSupport->autobusPrivato[row].capienza_max=capienza;
        autobusSupport->autobusPrivato[row].costo_giornaliero=costo;
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return autobusSupport;

}
static struct albergo_support * list_info_alberghi(MYSQL *conn){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[9];
    char indirizzo[MAX_LEN];
    char localita[MAX_LEN];
    double costo;
    char referente[MAX_LEN];
    char nome[MAX_LEN];
    char fax[MAX_LEN];
    char email[MAX_LEN];
    char telefono[MAX_LEN];
    int capienza;

    struct albergo_support *albergoSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_alberghi_all()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_alberghi_all statement\n", false);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile restituire la lista degli alberghi\n", true);
    }

    mysql_stmt_store_result(prepared_stmt);

    albergoSupport= malloc(sizeof(*albergoSupport) + sizeof(struct albergo) * mysql_stmt_num_rows(prepared_stmt));

    if(albergoSupport == NULL)
        goto out;

    memset(albergoSupport, 0, sizeof(*albergoSupport) + sizeof(struct albergo) * mysql_stmt_num_rows(prepared_stmt));

    albergoSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &indirizzo, MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &localita, MAX_LEN);
    set_binding_param(&param[2], MYSQL_TYPE_DOUBLE, &costo, sizeof(double));
    set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, &referente, MAX_LEN);
    set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, &nome, MAX_LEN);
    set_binding_param(&param[5], MYSQL_TYPE_VAR_STRING, &fax,MAX_LEN);
    set_binding_param(&param[6], MYSQL_TYPE_VAR_STRING, &email, MAX_LEN);
    set_binding_param(&param[7], MYSQL_TYPE_VAR_STRING, &telefono, MAX_LEN);
    set_binding_param(&param[8], MYSQL_TYPE_LONG, &capienza, sizeof(int));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_alberghi_all\n");
        free(albergoSupport);
        albergoSupport = NULL;
        goto out;
    }

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(albergoSupport->alberghi[row].indirizzo,indirizzo);
        strcpy(albergoSupport->alberghi[row].nome_località,localita);
        strcpy(albergoSupport->alberghi[row].referente,referente);
        strcpy(albergoSupport->alberghi[row].nome_albergo,nome);
        strcpy(albergoSupport->alberghi[row].fax,fax);
        strcpy(albergoSupport->alberghi[row].telefono,telefono);
        albergoSupport->alberghi[row].capienza_max=capienza;
        albergoSupport->alberghi[row].costo_per_notte=costo;
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return albergoSupport;

}
static int aggiungi_tappa(MYSQL *conn,struct tappa*tappa)
{
    int ret=0;
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[7];
    MYSQL_TIME oraPart;
    MYSQL_TIME oraArr;

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_tappa(?, ?, ?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_tappa statement\n", false);
    }

    time_to_mysql_time(tappa->ora_partenza,&oraPart);
    time_to_mysql_time(tappa->ora_arrivo,&oraArr);

    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(tappa->giorno_arrivo),sizeof (tappa->giorno_arrivo));
    set_binding_param(&param[1],MYSQL_TYPE_TIME,&(oraArr),sizeof (oraArr));
    set_binding_param(&param[2],MYSQL_TYPE_LONG,&(tappa->id_programma),sizeof (tappa->id_programma));
    set_binding_param(&param[3],MYSQL_TYPE_LONG,&(tappa->giorno_partenza),sizeof (tappa->giorno_partenza));
    set_binding_param(&param[4],MYSQL_TYPE_TIME,&(oraPart),sizeof (oraPart));
    set_binding_param(&param[5],MYSQL_TYPE_VAR_STRING,&(tappa->trattamento),strlen(tappa->trattamento));
    set_binding_param(&param[6],MYSQL_TYPE_VAR_STRING,&(tappa->nome_località),strlen(tappa->nome_località));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_tappa\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "\n Non è stato possibile aggiungere questa tappa, riprova!\n ");
    } else {
        ret=1;
        printf("Tappa correctly added!\n");
    }

    mysql_stmt_close(prepared_stmt);
    return ret;
}

static void aggiungi_programma_di_viaggio(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_programma_di_viaggio(? ,?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_programma_di_viaggio statement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,&(programmaDiViaggio->nome_programma),strlen (programmaDiViaggio->nome_programma));
    set_binding_param(&param[1],MYSQL_TYPE_LONG,&(programmaDiViaggio->num_giorni),sizeof (programmaDiViaggio->num_giorni));
    set_binding_param(&param[2],MYSQL_TYPE_LONG,&(programmaDiViaggio->id_programma),sizeof (programmaDiViaggio->id_programma));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_programma_di_viaggio\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'aggiungere il programma di viaggio\n");
    } else {
        printf("Programma di viaggio correctly registered...\n");
    }
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(programmaDiViaggio->id_programma),sizeof (programmaDiViaggio->id_programma));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Could not retrieve output parameter for aggiungi_programma_di_viaggio");
        goto out;
    }
    if(mysql_stmt_fetch(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Could not buffer results for aggiungi_programma_di_viaggio");
        goto out;
    }else{
        printf("\nCreato programma di viaggio con successo! [ID: %d]\n",programmaDiViaggio->id_programma);
    }
    out:
    mysql_stmt_free_result(prepared_stmt);
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);

    mysql_stmt_close(prepared_stmt);
    return;
}
static struct report_support * extract_partecipanti_info(MYSQL_STMT*prepared_stmt){
    int status;
    size_t row = 0;

    MYSQL_BIND param[5];

    char nome_cliente[MAX_LEN];
    char cognome_cliente[MAX_LEN];
    MYSQL_TIME datanascita;
    char email[MAX_LEN];
    char nazionalità[MAX_LEN];
    struct report_support *reportSupport = NULL;

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &nome_cliente,MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &cognome_cliente, MAX_LEN);
    set_binding_param(&param[2], MYSQL_TYPE_DATE, &datanascita, sizeof(datanascita));
    set_binding_param(&param[3], MYSQL_TYPE_VAR_STRING, &email, MAX_LEN);
    set_binding_param(&param[4], MYSQL_TYPE_VAR_STRING, &nazionalità, MAX_LEN);
    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind column parameters for extract_partecipanti_info \n");
        free(reportSupport);
        reportSupport = NULL;
        goto out;
    }

    if (mysql_stmt_store_result(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Unable to store partecipanti information result set.");
        goto out;
    }
    reportSupport= malloc(sizeof(*reportSupport)+  sizeof(struct partecipante) * mysql_stmt_num_rows(prepared_stmt));
    if(reportSupport == NULL)
        goto out;
    memset(reportSupport, 0, sizeof(*reportSupport) + sizeof(struct partecipante) * mysql_stmt_num_rows(prepared_stmt));
    reportSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);
        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        mysql_date_to_string(&datanascita,reportSupport->partecipanti[row].data_nascita);
        strcpy(reportSupport->partecipanti[row].email,email);
        strcpy(reportSupport->partecipanti[row].nazionalità,nazionalità);
        strcpy(reportSupport->partecipanti[row].nome,nome_cliente);
        strcpy(reportSupport->partecipanti[row].cognome,cognome_cliente);
        row++;
    }
    out:
    return reportSupport;
}
static void extract_costi_information(MYSQL_STMT*prepared_stmt,struct report_support*reportSupport){
    MYSQL_BIND param[5];
    double costo_per_persona;
    double costo_tot_autobus;
    double costo_tot_alberghi;
    double profitto;
    int num_partecipanti;
    set_binding_param(&param[0], MYSQL_TYPE_DOUBLE, &costo_per_persona, sizeof (costo_per_persona));
    set_binding_param(&param[1], MYSQL_TYPE_DOUBLE, &costo_tot_autobus, sizeof (costo_tot_autobus));
    set_binding_param(&param[2], MYSQL_TYPE_DOUBLE, &costo_tot_alberghi, sizeof (costo_tot_alberghi));
    set_binding_param(&param[3], MYSQL_TYPE_DOUBLE, &profitto, sizeof (profitto));
    set_binding_param(&param[4], MYSQL_TYPE_LONG, &num_partecipanti, sizeof (num_partecipanti));
    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind column parameters for extract_costi_information \n");
        return;
    }
    if (mysql_stmt_store_result(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Unable to store general prenotazione information result set.\n");
        return;
    }
    mysql_stmt_fetch(prepared_stmt);
    reportSupport->num_partecipanti=num_partecipanti;
    reportSupport->costo_tot_alberghi=costo_tot_alberghi;
    reportSupport->costo_tot_autobus=costo_tot_autobus;
    reportSupport->profitto=profitto;
    reportSupport->costo_per_persona=costo_per_persona;
    return;
}
static struct report_support *do_report(MYSQL *conn,struct viaggio*viaggio)
{
    struct report_support*reportSupport=NULL;
    int status;
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[2];
    MYSQL_TIME datapartenza;

    date_to_mysql_time(viaggio->data_partenza, &datapartenza);

    if(!setup_prepared_stmt(&prepared_stmt, "call report_viaggio(? ,?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize report_viaggiostatement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(viaggio->id_programma),sizeof (viaggio->id_programma));
    set_binding_param(&param[1],MYSQL_TYPE_DATE,&(datapartenza),sizeof(datapartenza));
    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for report_viaggio\n", true);
        goto out;
    }
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nella generazione del Report\n");
        goto out;
    }
    reportSupport=extract_partecipanti_info(prepared_stmt);
    if(reportSupport==NULL)
        goto out;
    mysql_stmt_free_result(prepared_stmt);
    status = mysql_stmt_next_result(prepared_stmt);
    if (status > 0)
        finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", false);
    else if(!status) extract_costi_information(prepared_stmt,reportSupport);
    out:
    mysql_stmt_free_result(prepared_stmt);
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_close(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    return reportSupport;
}

static void aggiungi_autobus_privato(MYSQL *conn,struct autobus_privato*autobusPrivato)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[4];

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_autobus_privato(? ,?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_autobus_privato statement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,autobusPrivato->targa,strlen (autobusPrivato->targa));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,autobusPrivato->nome_deposito,strlen (autobusPrivato->nome_deposito));
    set_binding_param(&param[2],MYSQL_TYPE_LONG,&(autobusPrivato->capienza_max),sizeof (autobusPrivato->capienza_max));
    set_binding_param(&param[3],MYSQL_TYPE_DOUBLE,&(autobusPrivato->costo_giornaliero),sizeof (autobusPrivato->costo_giornaliero));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_autobus_privaton", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un errore nell'aggiungere l'autobus privato\n");
    } else {
        printf("Autobus privato correttamente registrato.\n");
    }
    mysql_stmt_close(prepared_stmt);
}

static void aggiungi_viaggio(MYSQL *conn,struct viaggio*viaggio)
{
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[4];
    MYSQL_TIME datapartenza;
    MYSQL_TIME datarientro;

    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_viaggio(?,?,?,?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_viaggio statement\n", false);
    }

    date_to_mysql_time(viaggio->data_rientro, &datarientro);
    date_to_mysql_time(viaggio->data_partenza, &datapartenza);

    set_binding_param(&param[0],MYSQL_TYPE_DATE,&datapartenza,sizeof(datapartenza));
    set_binding_param(&param[1],MYSQL_TYPE_LONG,&(viaggio->id_programma),sizeof (viaggio->id_programma));
    set_binding_param(&param[2],MYSQL_TYPE_DATE,&datarientro,sizeof(datarientro));
    set_binding_param(&param[3],MYSQL_TYPE_DOUBLE,&(viaggio->costo),sizeof (viaggio->costo));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_viaggio\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un erroe nell'aggiungere  il viaggio\n");
    } else {
        printf("Viaggio correttamente aggiunto\n");
    }
    mysql_stmt_close(prepared_stmt);
}
