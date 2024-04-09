#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
#include "segretarioview.h"
char segretario_get_action(){
    char options[8] = {'1','2', '3', '4', '5','6','7','q'};
    char op;
    printf("\033[2J\033[H");
    printf("\n[*** WELCOME TO SEGRETERIA DASHBOARD ***]\n\n");
    printf("1) Gestisci Programmi di Viaggio\n");
    printf("2) Gestisci Viaggi\n");
    printf("3) Gestisci Località\n");
    printf("4) Gestisci Guide\n");
    printf("5) Gestisci Autobus Privati\n");
    printf("6) Gestisci Alberghi\n");
    printf("7) Genera Report di un viaggio\n");
    printf("q) Quit\n");
    op = multiChoice("Select an option", options, 8);
    return op;
}
char nested_get_action_1(){
    char options[4] = {'1','2', '3', 'b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Programmi di Viaggio\n");
    printf("2) Inserisci nuovo Programma di Viaggio\n");
    printf("3) Aggiungi tappe a un programma di viaggio\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 4);
    return op;
}
char nested_get_action_2(){
    char options[6] = {'1','2', '3','4','5','b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Viaggi\n");
    printf("2) Inserisci nuovo Viaggio\n");
    printf("3) Assegna una guida a un Viaggio\n");
    printf("4) Assegna un autobus privato a un Viaggio\n");
    printf("5) Assegna un albergo a un Viaggio\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 6);
    return op;
}
char nested_get_action_3(){
    char options[3] = {'1','2','b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Guide\n");
    printf("2) Aggiungi Guida\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 3);
    return op;
}
char nested_get_action_4(){
    char options[3] = {'1','2','b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Autobus Privati\n");
    printf("2) Aggiungi Autobus Privato\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 3);
    return op;
}
char nested_get_action_5(){
    char options[4] = {'1','2', '3','b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Alberghi\n");
    printf("2) Aggiungi Albergo\n");
    printf("3) Rimuovi Albergo\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 4);
    return op;
}
char nested_get_action_6(){
    char options[3] = {'1','2','b'};
    char op;
    printf("\033[2J\033[H");
    printf("******\n\n");
    printf("1) Lista Località\n");
    printf("2) Aggiungi nuova Località\n");
    printf("b) Back to previous menu\n");
    op = multiChoice("Select an option", options, 3);
    return op;
}
static void print_viaggi_confermati(struct viaggio_support *viaggioSupport,struct viaggio*viaggio)
{
    printf("\n[Viaggi nello stato '%s']\n\n",viaggio->stato_viaggio);
   if(!viaggioSupport->num_entries){
       printf("La lista è vuota!\n");
   }else {
       for (size_t i = 0; i < viaggioSupport->num_entries; i++) {
           printf("[ID Programma %d] Data Partenza: %s , Data Rientro: %s , Costo: %02.2f$",
                  viaggioSupport->viaggi[i].id_programma,
                  viaggioSupport->viaggi[i].data_partenza,
                  viaggioSupport->viaggi[i].data_rientro,
                  viaggioSupport->viaggi[i].costo
           );
           if (strcmp(viaggio->stato_viaggio, "prenotabile") != 0) {
               printf(", Numero Partecipanti: %d", viaggioSupport->viaggi[i].num_partecipanti_confermati);
           }
           puts("");
       }
   }
}

static void print_guide_assign(struct guida_support *guidaSupport)
{
    printf("\n[** Assegnamenti Guide **]\n\n");

    for(size_t i = 0; i < guidaSupport->num_entries; i++) {
        printf("[%s] Data Partenza: %s , Data Rientro: %s\n",
               guidaSupport->guide[i].cf,
               guidaSupport->guide[i].dataP,
               guidaSupport->guide[i].dataR
        );
    }
}
static void print_itinerario(struct tappa_support *tappaSupport)
{
    if(!tappaSupport->num_entries){
        printf("\nItinerario Vuoto: ancora nessuna tappa aggiunta!\n");
    }else {
        printf("\n[** Itinerario **]\n\n");
        for (size_t i = 0; i < tappaSupport->num_entries; i++) {
            printf("[Tappa %ld] %s \n ->Arrivo in giorno %d alle ore %s \n ->Partenza in giorno %d alle ore %s\n ->Trattamento %s\n",
                   i + 1,
                   tappaSupport->tappe[i].nome_località,
                   tappaSupport->tappe[i].giorno_arrivo,
                   tappaSupport->tappe[i].ora_arrivo,
                   tappaSupport->tappe[i].giorno_partenza,
                   tappaSupport->tappe[i].ora_partenza,
                   tappaSupport->tappe[i].trattamento
            );
        }
    }

}
static void print_autobus_assign(struct autobus_support *autobusSupport)
{
    printf("\n[** Assegnamenti Autobus **]\n\n");

    for(size_t i = 0; i < autobusSupport->num_entries; i++) {
        printf("[%s] Data Partenza: %s , Data Rientro: %s\n",
               autobusSupport->autobusPrivato[i].targa,
               autobusSupport->autobusPrivato[i].dataP,
               autobusSupport->autobusPrivato[i].dataR
        );
    }
}

static void print_info_guide(struct guida_support *guidaSupport)
{
    printf("\n[** Informazioni Guide **]\n\n");

    for(size_t i = 0; i < guidaSupport->num_entries; i++) {
        printf("[CF: %s] Nome: %s , Cognome: %s , Contatto: %s , Data Nascita: %s\n",
               guidaSupport->guide[i].cf,
               guidaSupport->guide[i].nome,
               guidaSupport->guide[i].cognome,
               guidaSupport->guide[i].email,
               guidaSupport->guide[i].data_nascita
        );
    }
}
static void print_info_localita(struct localita_support *localitaSupport)
{
    printf("\n[** Informazioni Località **]\n\n");

    for(size_t i = 0; i < localitaSupport->num_entries; i++) {
        printf("%ld) Nome: %s , Stato: %s \n",
               i+1,
               localitaSupport->localita[i].nome_località,
               localitaSupport->localita[i].stato
        );
    }
}

static void print_info_alberghi(struct albergo_support *albergoSupport)
{
    printf("\n[** Informazioni Alberghi **]\n\n");

    for(size_t i = 0; i < albergoSupport->num_entries; i++) {
        printf("[Località: %s , Indirizzo: %s ] Nome: %s , Costo: %.2f $, Capienza: %d \n[Contatti] Referente: %s, Fax : %s , E-mail : %s, Telefono: %s \n\n",
               albergoSupport->alberghi[i].nome_località,
               albergoSupport->alberghi[i].indirizzo,
               albergoSupport->alberghi[i].nome_albergo,
               albergoSupport->alberghi[i].costo_per_notte,
               albergoSupport->alberghi[i].capienza_max,
               albergoSupport->alberghi[i].referente,
               albergoSupport->alberghi[i].fax,
               albergoSupport->alberghi[i].email,
               albergoSupport->alberghi[i].telefono
        );
    }
}

static void print_report(struct report_support *reportSupport)
{
    printf("[** Report **]\n\n");
    printf("Numero Partecipanti: %d\nCosto Per Persona: %.2f $\nCosto Totale Autobus: %.2f $\nCosto Totale Alberghi: %.2f $\nProfitto: %.2f $\n",
           reportSupport->num_partecipanti,
           reportSupport->costo_per_persona,
           reportSupport->costo_tot_autobus,
           reportSupport->costo_tot_alberghi,
           reportSupport->profitto);
    printf("\n[** Dati sui partecipanti **]\n\n");
    for(size_t i = 0; i < reportSupport->num_entries; i++) {
        printf("[Partecipante %ld]  Nome: %s , Cognome: %s , Email: %s , Nazionalità: %s, Data Nascita: %s\n",
               i+1,
               reportSupport->partecipanti[i].nome,
               reportSupport->partecipanti[i].cognome,
               reportSupport->partecipanti[i].email,
               reportSupport->partecipanti[i].nazionalità,
               reportSupport->partecipanti[i].data_nascita
        );
    }
}

static void print_info_autobus(struct autobus_support *autobusSupport)
{
    printf("\n[** Informazioni Autobus Privati **]\n\n");

    for(size_t i = 0; i < autobusSupport->num_entries; i++) {
        printf("[Targa: %s]  Nome Deposito: %s , Costo Giornaliero: %.2f , Capienza: %d\n",
               autobusSupport->autobusPrivato[i].targa,
               autobusSupport->autobusPrivato[i].nome_deposito,
               autobusSupport->autobusPrivato[i].costo_giornaliero,
               autobusSupport->autobusPrivato[i].capienza_max
        );
    }
}

static void get_programma_di_viaggio_info(struct programma_di_viaggio *programmaDiViaggio){
    char giorni[SIZE_INPUT_NUMBER];
    printf("\nInserisci il nome del programma di viaggio: ");
    getInput(MAX_LEN, programmaDiViaggio->nome_programma, false);
    printf("\nInserisci il numero di giorni del programma di viaggio: ");
    getInput(SIZE_INPUT_NUMBER, giorni, false);
    programmaDiViaggio->num_giorni = atoi(giorni);
}
static void get_autobus_privato_info(struct autobus_privato *autobusPrivato){
    char support[sizeof(int)];
    printf("\nInserisci la targa dell'autobus privato: ");
    getInput(TARGA_LEN, autobusPrivato->targa, false);
    printf("\nInserisci il nome del suo deposito: ");
    getInput(MAX_LEN, autobusPrivato->nome_deposito, false);
    printf("\nInserisci la capienza massima: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    autobusPrivato->capienza_max= atoi(support);
    printf("\nInserisci il costo giornaliero: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    autobusPrivato->costo_giornaliero= strtof(support,NULL);
}
static void get_viaggio_info(struct viaggio *viaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id del programma di viaggio a cui si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_partenza, false);
    printf("\nInserisci la data di rientro del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_rientro, false);
    printf("\nInserisci il costo del viaggio: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->costo= atoi(support);
}
static void get_viaggio_info_for_report(struct viaggio *viaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id del programma di viaggio a cui si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_partenza, false);
}
static void get_guida_info(struct guida *guida){
    printf("\nInserisci il nome della guida: ");
    getInput(MAX_LEN, guida->nome, false);
    printf("\nInserisci il cognome della guida: ");
    getInput(MAX_LEN, guida->cognome, false);
    printf("\nInserisci il codice fiscale della guida: ");
    getInput(CF_LEN, guida->cf, false);
    printf("\nInserisci il email della guida: ");
    getInput(MAX_LEN, guida->email, false);
    printf("\nInserisci il la data di nascita della guida [YYYY-MM-DD]: ");
    getInput(DATE_LEN, guida->data_nascita, false);
}
static void print_programmi_available_segretari(struct programma_di_viaggio_support *programmaDiViaggioSupport)
{
    printf("\n\n[** Programmi Di Viaggio Disponibili **]\n\n");
    for(size_t i = 0; i < programmaDiViaggioSupport->num_entries; i++) {
        printf("[ID %d] Nome Programma: %s , Numero Giorni %d \n",
               programmaDiViaggioSupport->programmiDiViaggio[i].id_programma,
               programmaDiViaggioSupport->programmiDiViaggio[i].nome_programma,
               programmaDiViaggioSupport->programmiDiViaggio[i].num_giorni
        );
    }
}
static void get_info_for_assign_guida(struct guida *guida,struct viaggio *viaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci il codice fiscale della guida da assegnare: ");
    getInput(CF_LEN, guida->cf, false);
    printf("\nInserisci l'id del programma di viaggio a cui il viaggio si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_partenza, false);
}
static void get_info_for_assign_autobus(struct autobus_privato *autobusPrivato,struct viaggio *viaggio){
    char support[sizeof(int)];
    printf("\nInserisci la targa dell'autobus da assegnare: ");
    getInput(TARGA_LEN, autobusPrivato->targa, false);
    printf("\nInserisci l'id del programma di viaggio a cui il viaggio si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_partenza, false);
}
static void get_info_for_assign_albergo(struct albergo *albergo,struct viaggio *viaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci il numero di notti per cui è previsto il pernottamento: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    albergo->num_notti=atoi(support);
    printf("\nInserisci la località dell'albergo da assegnare: ");
    getInput(MAX_LEN, albergo->nome_località, false);
    printf("\nInserisci l'indirizzo dell'albergo da assegnare: ");
    getInput(MAX_LEN, albergo->indirizzo, false);
    printf("\nInserisci l'id del programma di viaggio a cui il viaggio si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    viaggio->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, viaggio->data_partenza, false);
}
static void get_idprogramma_info(struct  programma_di_viaggio*programmaDiViaggio){
    char input[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'ID del programma di viaggio a cui vuoi aggiungere una tappa: ");
    getInput(SIZE_INPUT_NUMBER, input, false);
    programmaDiViaggio->id_programma=atoi(input);
}
static int ask_for_another(){
    int ret=0;
    char options[2] = {'Y','N'};
    int r = multiChoice("\nVuoi inserire un altra tappa?\n"
                        "Inserire 'Y' per continuare oppure 'N' per concludere ", options, 2);

    switch(r) {
        case 'Y':
            ret=1;
            break;
        case 'N':
            break;
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }
    return ret;

}
static char ask_for_stato_richiesto(){
    char ret;
    char options[4]={'1','2','3','4'};
    printf("\nSeleziona lo stato dei viaggi da visualizzare: \n");
    printf("1) Viaggi in programma\n");
    printf("2) Viaggi attivi\n");
    printf("3) Viaggi terminati\n");
    printf("4) Viaggi prenotabili\n");
    ret = multiChoice("\nSeziona una possibilità", options, 4);
    return ret;

}
static void get_tappa_info(struct tappa *tappa){
    char options[5] = {'1','2', '3','4','5'};
    char r;
    char giornoarrivo[10];
    char giornopartenza[10];
    // Get the required info
    printf("\nInsert località  nuova della tappa: ");
    getInput(MAX_LEN, tappa->nome_località, false);
    printf("\nInsert giorno di arrivo: ");
    getInput(SIZE_INPUT_NUMBER, giornoarrivo, false);
    tappa->giorno_arrivo= atoi(giornoarrivo);

    printf("\nInsert ora arrivo [HH:MM] : ");
    getInput(TIME_LEN, tappa->ora_arrivo, false);

    printf("\nInsert giorno di partenza: ");
    getInput(SIZE_INPUT_NUMBER, giornopartenza, false);
    tappa->giorno_partenza= atoi(giornopartenza);

    printf("\nInsert ora partenza [HH:MM] : ");
    getInput(TIME_LEN, tappa->ora_partenza, false);
    printf("\nAssign a possible trattamento: \n");
    printf("\t1) FB\n");
    printf("\t2) HB\n");
    printf("\t3) BB\n");
    printf("\t4) OB\n");
    printf("\t5) NB\n");
    r = multiChoice("\nSelect trattamento", options, 5);

    // Convert role into enum value
    switch(r) {
        case '1':
            strcpy(tappa->trattamento, "FB");
            break;
        case '2':
            strcpy(tappa->trattamento, "HB");
            break;
        case '3':
            strcpy(tappa->trattamento, "BB");
            break;
        case '4':
            strcpy(tappa->trattamento, "OB");
            break;
        case '5':
            strcpy(tappa->trattamento, "NB");
            break;
        default:
            fprintf(stderr, "Invalid condition at %s:%d\n", __FILE__, __LINE__);
            abort();
    }
}

static void get_albergo_info(struct albergo *albergo){
    char capienza[MAX_LEN];
    char costo_pn_char[MAX_LEN];
    printf("\nInserisci indirizzo: ");
    getInput(MAX_LEN, albergo->indirizzo, false);
    printf("\nInserisci località indirizzo: ");
    getInput(MAX_LEN, albergo->nome_località, false);

    redo:
    printf("\nInserisci il costo per notte per persona: "); //deve essere un float
    getInput(SIZE_INPUT_NUMBER, costo_pn_char, false);
    if(!strtof(costo_pn_char,NULL)){
        printf("\nInput non valido, per favore ripeti");
        goto redo;
    }else{
        albergo->costo_per_notte= strtof(costo_pn_char,NULL);
    }

    printf("\nInserisci il nome di un referente: ");
    getInput(MAX_LEN, albergo->referente, false);
    printf("\nInserisci il nome dell'albergo: ");
    getInput(MAX_LEN, albergo->nome_albergo, false);
    printf("\nInserisci il fax: ");
    getInput(MAX_LEN, albergo->fax, false);
    printf("\nInserisci l'email: ");
    getInput(MAX_LEN, albergo->email, false);
    printf("\nInserisci un telefono: ");
    getInput(MAX_LEN, albergo->telefono, false);

    redo2:
    printf("\nInserisci capienza massima di persone: "); //deve essere un intero!
    getInput(SIZE_INPUT_NUMBER, capienza, false);
    if(!atoi(capienza)){
        printf("\nInput non valido, per favore ripeti");
        goto redo2;
    }else{
        albergo->capienza_max =atoi(capienza);
    }

}
static void get_localita_info(struct localita *localita){
    printf("\nNome località: ");
    getInput(MAX_LEN, localita->nome_località, false);
    printf("Stato località: ");
    getInput(MAX_LEN, localita->stato, false);
}
