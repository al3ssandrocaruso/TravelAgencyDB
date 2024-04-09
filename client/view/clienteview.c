#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
#include "clienteview.h"
char cliente_get_action(){
    char options[6] = {'1','2', '3', '4', '5','q'};
    char op;
    printf("\033[2J\033[H");
    printf("*** WELCOME TO CLIENTE DASHBOARD ***\n\n");
    printf("1) Vedi i programmi di viaggio disponibili\n");
    printf("2) Vedi i viaggi disponibili per un certo programma\n");
    printf("3) Prenota un Viaggio\n");
    printf("4) Gestisci una Prenotazione\n");
    printf("5) Visualizza una Prenotazione\n");
    printf("q) Quit\n");
    op = multiChoice("Select an option", options, 6);
    return op;
}
char prenotazione_get_action(){
    char options[3] = {'1','2', 'b'};
    char op;
    printf("\033[2J\033[H");
    printf("*** Cosa vuoi fare ***\n\n");
    printf("1) Aggiungi Partecipante\n");
    printf("2) Disdici Prenotazione\n");
    printf("b) Back\n");
    op = multiChoice("Select an option", options, 3);
    return op;
}
static void get_partecipante_info(struct partecipante *partecipante){
    printf("\n**Inserisci per favore i dati del nuovo partecipante**\n");
    printf("\nInserisci il nome: ");
    getInput(MAX_LEN, partecipante->nome, false);
    printf("\nInserisci il cognome: ");
    getInput(MAX_LEN, partecipante->cognome, false);
    printf("\nInserisci il codice fiscale: ");
    getInput(CF_LEN, partecipante->cf, false);
    printf("\nInserisci la email: ");
    getInput(MAX_LEN, partecipante->email, false);
    printf("\nInserisci il la data di nascita [YYYY-MM-DD]: ");
    getInput(DATE_LEN, partecipante->data_nascita, false);
    printf("\nInserisci la nazionalità : ");
    getInput(MAX_LEN, partecipante->nazionalità, false);
}
static void get_prenotazione_info(struct prenotazione *prenotazione){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id del programma di viaggio a cui il viaggio si riferisce: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    prenotazione->id_programma=atoi(support);
    printf("\nInserisci la data di partenza del viaggio [YYYY-MM-DD] : ");
    getInput(DATE_LEN, prenotazione->data_partenza, false);
    printf("\nInserisci il numero di persone per cui è  prevista  la prenotazione: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    prenotazione->num_partecipanti=atoi(support);
}
static void get_prenotazione_to_manage_info(struct prenotazione *prenotazione){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id della prenotazione: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    prenotazione->id_prenotazione=atoi(support);
    printf("\nInserisci il codice che ti era stato rilasciato per gestire la prenotazione: ");
    getInput(MAX_LEN, prenotazione->codice_per_gestire, false);
}
static void get_id_info_for_tappe(struct programma_di_viaggio* programmaDiViaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id del prorgramma di viaggio che vuoi vedere in dettaglio: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    programmaDiViaggio->id_programma=atoi(support);
}
static void get_id_info_for_viaggi(struct programma_di_viaggio* programmaDiViaggio){
    char support[SIZE_INPUT_NUMBER];
    printf("\nInserisci l'id del programma di viaggio di cui vuoi vedere i viaggi in programma: ");
    getInput(SIZE_INPUT_NUMBER, support, false);
    programmaDiViaggio->id_programma=atoi(support);
}
static void print_viaggi_available(struct viaggio_support *viaggioSupport)
{
    printf("\n[** Edizioni Viaggi Disponibili **]\n\n");

    for(size_t i = 0; i < viaggioSupport->num_entries; i++) {
        printf("[%ld]  Data Partenza: %s , Data Rientro: %s , Costo: %02.2f$ \n",
               i+1,
               viaggioSupport->viaggi[i].data_partenza,
               viaggioSupport->viaggi[i].data_rientro,
               viaggioSupport->viaggi[i].costo);
    }
}
static void print_programmi_available(struct programma_di_viaggio_support *programmaDiViaggioSupport)
{
    printf("[** Programmi di Viaggi Disponibili **]\n\n");
    for(size_t i = 0; i < programmaDiViaggioSupport->num_entries; i++) {
        printf("[ID: %d] Nome Programma: %s , Numero Giorni %d , Costo a partire da: %02.2f$ \n",
               programmaDiViaggioSupport->programmiDiViaggio[i].id_programma,
               programmaDiViaggioSupport->programmiDiViaggio[i].nome_programma,
               programmaDiViaggioSupport->programmiDiViaggio[i].num_giorni,
               programmaDiViaggioSupport->programmiDiViaggio[i].min_price
               );
    }
}
static void print_itinerario(struct tappa_support *tappaSupport)
{
    printf("\n[** Itinerario **]\n\n");

    for(size_t i = 0; i < tappaSupport->num_entries; i++) {
        printf("[Tappa %ld] %s \n ->Arrivo in giorno %d alle ore %s \n ->Partenza in giorno %d alle ore %s\n ->Trattamento %s\n",
               i+1,
               tappaSupport->tappe[i].nome_località,
               tappaSupport->tappe[i].giorno_arrivo,
               tappaSupport->tappe[i].ora_arrivo,
               tappaSupport->tappe[i].giorno_partenza,
               tappaSupport->tappe[i].ora_partenza,
               tappaSupport->tappe[i].trattamento
               );
    }

}
static void print_info_prenotazione(struct prenotazione_support *prenotazioneSupport)
{
    printf("\n\n[**Informazioni Prenotazione**]");
    printf("\nRiferita a: %s , Data Partenza: %s\nAvvenuta in data: %s\nCosto per persona %02.2f$\n",
           prenotazioneSupport->nome_programma,
           prenotazioneSupport->data_partenza,
           prenotazioneSupport->data_prenotazione,
           prenotazioneSupport->prezzo_per_persona
    );
    if(!prenotazioneSupport->num_entries){
        printf("\nAncora nessun partecipante aggiunto\n");
    }else {
        for (size_t i = 0; i < prenotazioneSupport->num_entries; i++) {
            printf("[Partecipante %ld]: %s %s\n",
                   i + 1,
                   prenotazioneSupport->partecipanti[i].cognome,
                   prenotazioneSupport->partecipanti[i].nome
            );
        }
    }
}