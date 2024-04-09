#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "model/model.h"
#include "defines.h"
#include "view/clienteview.c"
#include "dao/clientedao.c"

static void list_programmi_di_viaggio_ctrl(MYSQL *conn)
{
    struct programma_di_viaggio_support*programmaDiViaggioSupport=lista_programmi_di_viaggio_cliente(conn);
    if(programmaDiViaggioSupport != NULL) {
        print_programmi_available(programmaDiViaggioSupport);
        free(programmaDiViaggioSupport);
    }
        struct programma_di_viaggio programmaDiViaggio;
        memset(&programmaDiViaggio, 0, sizeof(programmaDiViaggio));
        get_id_info_for_tappe(&programmaDiViaggio);
        struct tappa_support *tappaSupport = list_tappe_for_a_program(conn, &programmaDiViaggio);
        if (tappaSupport != NULL) {
            print_itinerario(tappaSupport);
            free(tappaSupport);
        }
    }

static void list_available_viaggi(MYSQL *conn)
{
    struct programma_di_viaggio programmaDiViaggio;
    memset(&programmaDiViaggio, 0, sizeof(programmaDiViaggio));
    get_id_info_for_viaggi(&programmaDiViaggio);
    struct viaggio_support * viaggioSupport=list_viaggi_for_a_program(conn,&programmaDiViaggio);
    if(viaggioSupport != NULL) {
        print_viaggi_available(viaggioSupport);
        free(viaggioSupport);
    }
}

static void prenota_viaggio_ctrl(MYSQL *conn)
{
    struct prenotazione prenotazione;
    memset(&prenotazione,0,sizeof (prenotazione));
    get_prenotazione_info(&prenotazione);
    prenota_viaggio(conn,&prenotazione);
}
static void visualizza_prenotazione_ctrl(MYSQL *conn)
{
    struct prenotazione prenotazione;
    memset(&prenotazione,0,sizeof (prenotazione));
    get_prenotazione_to_manage_info(&prenotazione);
    struct prenotazione_support*prenotazioneSupport=load_prenotazione_info(conn,&prenotazione);
    if(prenotazioneSupport != NULL ) {
        print_info_prenotazione(prenotazioneSupport);
        free(prenotazioneSupport);
    }
}
static void gestisci_prenotazione_ctrl(MYSQL *conn)
{
    char op = prenotazione_get_action();
    struct partecipante partecipante;
    struct prenotazione prenotazione;
    if(op=='b') return;
    get_prenotazione_to_manage_info(&prenotazione);
    struct prenotazione_support*prenotazioneSupport=load_prenotazione_info(conn,&prenotazione);
    if(prenotazioneSupport!=NULL) {
        print_info_prenotazione(prenotazioneSupport);
        free(prenotazioneSupport);
        switch (op) {
            case '1':
                memset(&partecipante, 0, sizeof(partecipante));
                get_partecipante_info(&partecipante);
                aggiungi_partecipante(conn, &partecipante, &prenotazione);
                break;
            case '2':
                disdici_prenotazione(conn, &prenotazione);
                break;
        }
    }else{
        printf("\nQualcosa è  andato storto..Riprovare\n");
    }
}


void run_as_cliente(MYSQL *conn)
{
	printf("Switching to cliente role...\n");

	if(!parse_config("users/cliente.json", &conf)) {
		fprintf(stderr, "Unable to load student configuration\n");
		exit(EXIT_FAILURE);
	}

	if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	while(true) {
		char op=cliente_get_action();
		switch(op) {
			case '1':
                list_programmi_di_viaggio_ctrl(conn);
                break;
				
			case '2':
                list_available_viaggi(conn);
                break;

			case '3':
                prenota_viaggio_ctrl(conn);
                break;
			case '4':
                gestisci_prenotazione_ctrl(conn);
                break;
            case '5':
                visualizza_prenotazione_ctrl(conn);
                break;
            case 'q':
                return;
			default:
				fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
				abort();
		}

		getchar();
	}
}
