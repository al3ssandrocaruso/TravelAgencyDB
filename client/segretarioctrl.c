#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "model/model.h"
#include "utils/db.c"
#include "view/segretarioview.c"
#include "dao/segretariodao.c"

static void register_localita(MYSQL *conn)
{
    struct localita localita;
    memset(&localita, 0, sizeof(localita));
    get_localita_info(&localita);
    aggiungi_localita(conn,&localita);
    return;
}

static void do_report_ctrl(MYSQL *conn)
{
    struct viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    get_viaggio_info_for_report(&viaggio);
    struct report_support*reportSupport=do_report(conn,&viaggio);
    print_report(reportSupport);
    return;
}

static void register_albergo(MYSQL *conn)
{
    struct albergo albergo;
    memset(&albergo, 0, sizeof(albergo));
    get_albergo_info(&albergo);
    aggiungi_albergo(conn,&albergo);
    return;
}

static int  register_tappa(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio)
{
    struct tappa tappa;
    memset(&tappa, 0, sizeof(tappa));
    tappa.id_programma=programmaDiViaggio->id_programma;
    get_tappa_info(&tappa);
    return aggiungi_tappa(conn,&tappa);
}

static void aggiungi_tappe_programma_di_viaggio(MYSQL *conn){
    struct programma_di_viaggio programmaDiViaggio;
    memset(&programmaDiViaggio, 0, sizeof(programmaDiViaggio));
    get_idprogramma_info(&programmaDiViaggio);
    struct tappa_support *tappaSupport = list_tappe_for_a_program(conn, &programmaDiViaggio);
    if (tappaSupport != NULL) {
        print_itinerario(tappaSupport);
        free(tappaSupport);
    }
    while (1) {
        register_tappa(conn, &programmaDiViaggio);
        if(!ask_for_another())break;
    }
}
static void register_guida(MYSQL *conn)
{
    struct  guida guida;
    memset(&guida, 0, sizeof(guida));
    get_guida_info(&guida);
    aggiungi_guida(conn,&guida);
    return;
}
static void register_programma_di_viaggio(MYSQL *conn)
{
    struct  programma_di_viaggio programmaDiViaggio;
    memset(&programmaDiViaggio, 0, sizeof(programmaDiViaggio));;
    get_programma_di_viaggio_info(&programmaDiViaggio);
    aggiungi_programma_di_viaggio(conn,&programmaDiViaggio);
    return;
}
static void register_autobus_privato(MYSQL *conn)
{
    struct  autobus_privato autobusPrivato;
    memset(&autobusPrivato, 0, sizeof(autobusPrivato));;
    get_autobus_privato_info(&autobusPrivato);
    aggiungi_autobus_privato(conn,&autobusPrivato);
    return;
}
static void register_viaggio(MYSQL *conn)
{
    list_programmi_di_viaggio_segretari(conn);
    struct  viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    get_viaggio_info(&viaggio);
    aggiungi_viaggio(conn,&viaggio);
    return;
}


static void assegna_guida_ctrl(MYSQL *conn){
    struct guida_support * guidaSupport=lista_assegnamenti_guide(conn);
    if(guidaSupport != NULL) {
        print_guide_assign(guidaSupport);
        free(guidaSupport);
    }
    struct guida guida;
    struct viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    memset(&guida, 0, sizeof(guida));
    get_info_for_assign_guida(&guida,&viaggio);
    assegna_guida(conn,&guida,&viaggio);
}
static void assegna_autobus_ctrl(MYSQL *conn){
    struct autobus_support*autobusSupport=lista_assegnamenti_autobus(conn);
    if(autobusSupport != NULL) {
        print_autobus_assign(autobusSupport);
        free(autobusSupport);
    }
    struct autobus_privato autobusPrivato;
    struct viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    memset(&autobusPrivato, 0, sizeof(autobusPrivato));
    get_info_for_assign_autobus(&autobusPrivato,&viaggio);
    assegna_autobus(conn,&autobusPrivato,&viaggio);
}
static void assegna_albergo_ctrl(MYSQL *conn){
    struct albergo albergo;
    struct viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    memset(&albergo, 0, sizeof(albergo));
    get_info_for_assign_albergo(&albergo,&viaggio);
    assegna_albergo(conn,&albergo,&viaggio);
}
static void list_available_viaggi(MYSQL *conn)
{
    struct viaggio viaggio;
    memset(&viaggio, 0, sizeof(viaggio));
    char r=ask_for_stato_richiesto();
    switch(r) {
        case '1':
            strcpy(viaggio.stato_viaggio, "in programma");
            break;
        case '2':
            strcpy(viaggio.stato_viaggio, "attivo");
            break;
        case '3':
            strcpy(viaggio.stato_viaggio, "terminato");
            break;
        case '4':
            strcpy(viaggio.stato_viaggio, "prenotabile");
            break;
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }
    struct viaggio_support * viaggioSupport=list_viaggi_by_status(conn,&viaggio);
    if(viaggioSupport != NULL) {
        print_viaggi_confermati(viaggioSupport,&viaggio);
        free(viaggioSupport);
    }
}
static void list_info_guide_ctrl(MYSQL *conn)
{
    struct guida_support*guidaSupport= list_info_guide(conn);
    if(guidaSupport != NULL) {
        print_info_guide(guidaSupport);
        free(guidaSupport);
    }
}
static void list_info_localita_ctrl(MYSQL *conn)
{
    struct localita_support*localitaSupport= list_info_localita(conn);
    if(localitaSupport != NULL) {
        print_info_localita(localitaSupport);
        free(localitaSupport);
    }
}
static void list_info_autobus_ctrl(MYSQL *conn)
{
    struct autobus_support*autobusSupport= list_info_autobus(conn);
    if(autobusSupport != NULL) {
        print_info_autobus(autobusSupport);
        free(autobusSupport);
    }
}
static void list_info_alberghi_ctrl(MYSQL *conn)
{
    struct albergo_support*albergoSupport= list_info_alberghi(conn);
    if(albergoSupport != NULL) {
        print_info_alberghi(albergoSupport);
        free(albergoSupport);
    }
}
static void list_programmi_di_viaggio_segretari_ctrl(MYSQL *conn){
    struct programma_di_viaggio_support*programmaDiViaggioSupport= list_programmi_di_viaggio_segretari(conn);
    if(programmaDiViaggioSupport != NULL) {
        print_programmi_available_segretari(programmaDiViaggioSupport);
        free(programmaDiViaggioSupport);
    }
}
void run_as_segretario(MYSQL *conn) //segretario view
{

    printf("Switching to segretario role...\n");

    if(!parse_config("users/segretario.json", &conf)) {
        fprintf(stderr, "Unable to load administrator configuration\n");
        exit(EXIT_FAILURE);
    }

    if(mysql_change_user(conn, conf.db_username, conf.db_password, conf.database)) {
        fprintf(stderr, "mysql_change_user() failed\n");
        exit(EXIT_FAILURE);
    }

    while(true) {
        char op=segretario_get_action();
        switch(op) {
            case '1':
                op=nested_get_action_1();
                switch (op) {
                    case'1':
                        list_programmi_di_viaggio_segretari_ctrl(conn);
                        break;
                    case '2':
                        register_programma_di_viaggio(conn);
                        break;
                    case '3':
                        aggiungi_tappe_programma_di_viaggio(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '2':
                op=nested_get_action_2();
                switch (op) {
                    case '1':
                        list_available_viaggi(conn);
                        break;
                    case '2':
                        register_viaggio(conn);
                        break;
                    case '3':
                        assegna_guida_ctrl(conn);
                        break;
                    case '4':
                        assegna_autobus_ctrl(conn);
                        break;
                    case '5':
                        assegna_albergo_ctrl(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '3':
                op=nested_get_action_6();
                switch (op) {
                    case '1':
                        list_info_localita_ctrl(conn);
                        break;
                    case '2':
                        register_localita(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '4':
                op=nested_get_action_3();
                switch (op) {
                    case '1':
                        list_info_guide_ctrl(conn);
                        break;
                    case '2':
                        register_guida(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '5':
                op=nested_get_action_4();
                switch (op) {
                    case '1':
                        list_info_autobus_ctrl(conn);
                        break;
                    case '2':
                        register_autobus_privato(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '6':
                op=nested_get_action_5();
                switch (op) {
                    case '1':
                        list_info_alberghi_ctrl(conn);
                        break;
                    case '2':
                        register_albergo(conn);
                        break;
                    case 'b':
                        break;
                }
                break;
            case '7':
                do_report_ctrl(conn);
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

