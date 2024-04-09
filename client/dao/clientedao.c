#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
#include "../utils/db.h"
#include "clientedao.h"

static void aggiungi_partecipante(MYSQL *conn,struct partecipante * partecipante,struct prenotazione*prenotazione){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[7];
    MYSQL_TIME datanascita;
    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_partecipante(?, ?, ?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_partecipante statement\n", false);
    }
    date_to_mysql_time(partecipante->data_nascita, &datanascita);
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(prenotazione->id_prenotazione),sizeof (prenotazione->id_prenotazione));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,partecipante->cf,strlen(partecipante->cf));
    set_binding_param(&param[2],MYSQL_TYPE_VAR_STRING,partecipante->nome,strlen(partecipante->nome));
    set_binding_param(&param[3],MYSQL_TYPE_VAR_STRING,partecipante->cognome,strlen(partecipante->cognome));
    set_binding_param(&param[4],MYSQL_TYPE_VAR_STRING,partecipante->email,strlen(partecipante->email));
    set_binding_param(&param[5],MYSQL_TYPE_DATE,&datanascita,sizeof(datanascita));
    set_binding_param(&param[6],MYSQL_TYPE_VAR_STRING,partecipante->nazionalità,strlen(partecipante->nazionalità));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_partecipante\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è  verificato un problema nell'aggiunta del partecipante");
    } else {
        printf("Partecipante correttamente aggiunto\n");
    }

    mysql_stmt_close(prepared_stmt);
}
static void disdici_prenotazione(MYSQL *conn,struct prenotazione*prenotazione){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[2];
    if(!setup_prepared_stmt(&prepared_stmt, "call disdici_prenotazione(?,?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize disdici_prenotazione statement\n", false);
    }

    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(prenotazione->id_prenotazione),sizeof (prenotazione->id_prenotazione));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,&(prenotazione->codice_per_gestire),strlen(prenotazione->codice_per_gestire));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for disdici_prenotazione\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è  verificato un problema nel disdire la prenotazione");
    } else {
        printf("\nPrenotazione correttamente disdetta \n");
    }

    mysql_stmt_close(prepared_stmt);
}
static struct programma_di_viaggio_support *  lista_programmi_di_viaggio_cliente(MYSQL *conn){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[4];
    int status;
    size_t row = 0;
    double costomin;
    int id;
    char nomeprogramma[MAX_LEN];
    int numgiorni;

    struct programma_di_viaggio_support *programmaDiViaggioSupport = NULL;
    if(!setup_prepared_stmt(&prepared_stmt, "call lista_programmi_di_viaggio_cliente()", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_programmi_di_viaggio_cliente statement\n", false);
    }
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt,"Si è  verificato un problema nel mostrare i programmi di viaggio", true);
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
    set_binding_param(&param[3], MYSQL_TYPE_DOUBLE, &costomin, sizeof(costomin));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_programmi_di_viaggio_cliente\n");
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
        programmaDiViaggioSupport->programmiDiViaggio[row].min_price=costomin;
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
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize get_itinerario_input_id statement\n", false);
    }
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(programmaDiViaggio->id_programma),sizeof (programmaDiViaggio->id_programma));
    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for get_itinerario_input_id\n", true);
    }

    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Si è  verificato un problema nell' ottenere l'itinerario\n", true);
    }
    mysql_stmt_store_result(prepared_stmt);

    tappaSupport= malloc(sizeof(*tappaSupport) + sizeof(struct tappa) * mysql_stmt_num_rows(prepared_stmt));

    if(tappaSupport == NULL)
        goto out;

    memset(tappaSupport, 0, sizeof(*tappaSupport) + sizeof(struct tappa) * mysql_stmt_num_rows(prepared_stmt));

    tappaSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);

    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, nome_localita, MAX_LEN);
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
static void prenota_viaggio(MYSQL *conn,struct prenotazione*prenotazione){
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[6];
    MYSQL_TIME datapartenza;
    if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_prenotazione(? ,?, ?, ?, ?, ?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize aggiungi_prenotazione statement\n", false);
    }
    date_to_mysql_time(prenotazione->data_partenza, &datapartenza);

    set_binding_param(&param[0],MYSQL_TYPE_DATE,&datapartenza,sizeof (datapartenza));
    set_binding_param(&param[1],MYSQL_TYPE_LONG,&(prenotazione->id_programma),sizeof (prenotazione->id_programma));
    set_binding_param(&param[2],MYSQL_TYPE_LONG,&(prenotazione->num_partecipanti),sizeof (prenotazione->num_partecipanti));
    set_binding_param(&param[3],MYSQL_TYPE_VAR_STRING,&(prenotazione->codice_per_gestire), CODE_LEN);
    set_binding_param(&param[4],MYSQL_TYPE_LONG,&(prenotazione->id_prenotazione), sizeof (prenotazione->id_prenotazione));
    set_binding_param(&param[5],MYSQL_TYPE_DOUBLE,&(prenotazione->prezzo_tot_prenotazione), sizeof (prenotazione->prezzo_tot_prenotazione));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for aggiungi_prenotazione\n", true);
    }

    // Run procedure
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt, "Si è verificato un problema nell'aggiungere la prenotazione");
    }
    set_binding_param(&param[0],MYSQL_TYPE_VAR_STRING,&(prenotazione->codice_per_gestire), CODE_LEN);
    set_binding_param(&param[1],MYSQL_TYPE_LONG,&(prenotazione->id_prenotazione), sizeof (prenotazione->id_prenotazione));
    set_binding_param(&param[2],MYSQL_TYPE_DOUBLE,&(prenotazione->prezzo_tot_prenotazione), sizeof (prenotazione->prezzo_tot_prenotazione));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Could not retrieve output parameter");
        goto out;
    }

    if(mysql_stmt_fetch(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Could not buffer results");
        goto out;
    }else{
        printf("\nPrenotazione avvenuta con successo!\n");
        printf("\n[Prezzo Stimato Prenotazione] %.2f $ (per %d partecipanti)\n",prenotazione->prezzo_tot_prenotazione,prenotazione->num_partecipanti);
        printf("[ID: %d] [codice %s] con queste credenziali potrai aggiunge i dati relativi ai partecipanti oppure disdire la tua prenotazione\n",prenotazione->id_prenotazione,prenotazione->codice_per_gestire);
    }
    out:
    mysql_stmt_close(prepared_stmt);
}
static struct viaggio_support * list_viaggi_for_a_program(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio){
    int status;
    size_t row = 0;

    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[3];
    MYSQL_TIME giorno_partenza;
    MYSQL_TIME giorno_rientro;
    double costo;

    struct viaggio_support *viaggioSupport = NULL;

    if(!setup_prepared_stmt(&prepared_stmt, "call lista_viaggi_prenotabili_input_id(?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize lista_viaggi_prenotabili_input_id statement\n", false);
    }
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(programmaDiViaggio->id_programma),sizeof (programmaDiViaggio->id_programma));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for lista_viaggi_prenotabili_input_id\n", true);
    }
    // Run procedure
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Non è possibile mostrare la lista\n", true);
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

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind output parameters for lista_viaggi_prenotabili_input_id\n");
        free(viaggioSupport);
        viaggioSupport = NULL;
        goto out;
    }

    /* assemble occupancy general information */
    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);

        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        mysql_date_to_string(&giorno_partenza,viaggioSupport->viaggi[row].data_partenza);
        mysql_date_to_string(&giorno_rientro,viaggioSupport->viaggi[row].data_rientro);
        viaggioSupport->viaggi[row].costo = costo;
        row++;
    }
    out:
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_free_result(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    mysql_stmt_close(prepared_stmt);
    return viaggioSupport;

}

static struct prenotazione_support * extract_partecipanti_prenotazione_info(MYSQL_STMT*prepared_stmt){
    int status;
    size_t row = 0;

    MYSQL_BIND param[2];
    char nome_cliente[MAX_LEN];
    char cognome_cliente[MAX_LEN];
    struct prenotazione_support *prenotazioneSupport = NULL;
    set_binding_param(&param[0], MYSQL_TYPE_VAR_STRING, &nome_cliente,MAX_LEN);
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &cognome_cliente, MAX_LEN);
    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind column parameters \n");
        free(prenotazioneSupport);
        prenotazioneSupport = NULL;
        goto out;
    }

    if (mysql_stmt_store_result(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Unable to store partecipanti info to result set.");
        goto out;
    }
    prenotazioneSupport= malloc(sizeof(*prenotazioneSupport)+  sizeof(struct partecipante) * mysql_stmt_num_rows(prepared_stmt));
    if(prenotazioneSupport == NULL)
        goto out;
    memset(prenotazioneSupport, 0, sizeof(*prenotazioneSupport) + sizeof(struct partecipante) * mysql_stmt_num_rows(prepared_stmt));
    prenotazioneSupport->num_entries = mysql_stmt_num_rows(prepared_stmt);
    while (true) {
        status = mysql_stmt_fetch(prepared_stmt);
        if (status == 1 || status == MYSQL_NO_DATA)
            break;
        strcpy(prenotazioneSupport->partecipanti[row].nome,nome_cliente);
        strcpy(prenotazioneSupport->partecipanti[row].cognome,cognome_cliente);
        row++;
    }
    out:
    return prenotazioneSupport;
}
static void extract_prenotazione_general_information(MYSQL_STMT*prepared_stmt,struct prenotazione_support*prenotazioneSupport){
    MYSQL_BIND param[4];
    MYSQL_TIME dataprenotazione;
    char nomeprogramma[MAX_LEN];
    MYSQL_TIME datapartenza;
    double prezzo_per_persona;

    set_binding_param(&param[0], MYSQL_TYPE_DATE, &dataprenotazione, sizeof (dataprenotazione));
    set_binding_param(&param[1], MYSQL_TYPE_VAR_STRING, &nomeprogramma, MAX_LEN);
    set_binding_param(&param[2], MYSQL_TYPE_DATE, &datapartenza, sizeof (datapartenza));
    set_binding_param(&param[3], MYSQL_TYPE_DOUBLE, &prezzo_per_persona, sizeof (prezzo_per_persona));

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
        print_stmt_error(prepared_stmt, "Unable to bind column parameters\n");
        return;
    }
    if (mysql_stmt_store_result(prepared_stmt)) {
        print_stmt_error(prepared_stmt, "Unable to store general information to result set.");
        return;
    }
    mysql_stmt_fetch(prepared_stmt);
    mysql_date_to_string(&dataprenotazione,prenotazioneSupport->data_prenotazione);
    mysql_date_to_string(&datapartenza,prenotazioneSupport->data_partenza);
    strcpy(prenotazioneSupport->nome_programma,nomeprogramma);
    prenotazioneSupport->prezzo_per_persona=prezzo_per_persona;
    return;
}
static struct prenotazione_support * load_prenotazione_info(MYSQL *conn,struct prenotazione*prenotazione){
    struct prenotazione_support*prenotazioneSupport=NULL;
    int status;
    MYSQL_STMT *prepared_stmt;
    MYSQL_BIND param[2];

    if(!setup_prepared_stmt(&prepared_stmt, "call get_prenotazione_info(?,?)", conn)) {
        finish_with_stmt_error(conn, prepared_stmt, "Unable to initialize get_prenotazione_info statement\n", false);
    }
    set_binding_param(&param[0],MYSQL_TYPE_LONG,&(prenotazione->id_prenotazione), sizeof (prenotazione->id_prenotazione));
    set_binding_param(&param[1],MYSQL_TYPE_VAR_STRING,&(prenotazione->codice_per_gestire), strlen(prenotazione->codice_per_gestire));

    if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
        finish_with_stmt_error(conn, prepared_stmt, "Could not bind parameters for get_prenotazione_info\n", true);
    }
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        print_stmt_error (prepared_stmt,"");
        goto out;
    }
    prenotazioneSupport=extract_partecipanti_prenotazione_info(prepared_stmt);
    if(prenotazioneSupport==NULL) goto out;
    mysql_stmt_free_result(prepared_stmt);
    status = mysql_stmt_next_result(prepared_stmt);
    if (status > 0)
        finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", false);
    else if(!status) extract_prenotazione_general_information(prepared_stmt,prenotazioneSupport);
    out:
    mysql_stmt_free_result(prepared_stmt);
    while(mysql_stmt_next_result(prepared_stmt) != -1) {}
    mysql_stmt_close(prepared_stmt);
    mysql_stmt_reset(prepared_stmt);
    return prenotazioneSupport;
}