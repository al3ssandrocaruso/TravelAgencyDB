#pragma once
#include <stdbool.h>
#include <stdlib.h>
#define TIME_LEN 6
#define DATE_LEN 11
#define MAX_LEN 45
#define CF_LEN 17
#define TARGA_LEN 8
#define USERNAME_LEN 45
#define PASSWORD_LEN 45
#define MAX_LEN_STATO 13
#define TRATTAMENTO_LEN 3
#define SIZE_INPUT_NUMBER 11
#define CODE_LEN 8

typedef enum {
    FAILED_LOGIN=0,
    SEGRETARIO,
    CLIENTE
} role_t;

struct viaggio {
    char data_partenza[DATE_LEN];
    int id_programma;
    char data_rientro[DATE_LEN];
    double costo;
    char stato_viaggio[MAX_LEN_STATO];
    int num_partecipanti_confermati;
};
struct viaggio_support {
    unsigned num_entries;
    struct viaggio viaggi[];
};

struct credentials {
	char username[USERNAME_LEN];
	char password[PASSWORD_LEN];
};

struct autobus_privato{
    char targa[TARGA_LEN];
    char nome_deposito[MAX_LEN];
    int capienza_max;
    double costo_giornaliero;
    char dataP[DATE_LEN];
    char dataR[DATE_LEN];
};
struct autobus_support{
    unsigned num_entries;
    struct autobus_privato autobusPrivato[];
};

struct guida{
    char cf[CF_LEN];
    char nome[MAX_LEN];
    char cognome[MAX_LEN];
    char email[MAX_LEN];
    char data_nascita[DATE_LEN];
    char dataP[DATE_LEN];
    char dataR[DATE_LEN];
};
struct partecipante{
    char cf[CF_LEN];
    char nome[MAX_LEN];
    char cognome[MAX_LEN];
    char email[MAX_LEN];
    char data_nascita[DATE_LEN];
    char dataP[DATE_LEN];
    char dataR[DATE_LEN];
    char nazionalità[MAX_LEN];
};

struct report_support{
    unsigned num_entries;
    double costo_per_persona;
    double costo_tot_autobus;
    double costo_tot_alberghi;
    double profitto;
    int num_partecipanti;
    struct partecipante partecipanti[];
};

struct guida_support{
    unsigned num_entries;
    struct guida guide[];
};
struct programma_di_viaggio {
    int id_programma;
    char nome_programma[MAX_LEN];
    int num_giorni;
    double min_price;
};
struct programma_di_viaggio_support{
    unsigned num_entries;
    struct programma_di_viaggio programmiDiViaggio[];
};
struct tappa{
    int giorno_arrivo;
    char ora_arrivo[TIME_LEN];
    int id_programma;
    int giorno_partenza;
    char ora_partenza[TIME_LEN];
    char trattamento[TRATTAMENTO_LEN];
    char nome_località[MAX_LEN];
};
struct tappa_support {
    unsigned num_entries;
    struct tappa tappe[];
};
struct prenotazione{
    int id_prenotazione;
    char codice_per_gestire[MAX_LEN];
    char data_partenza[DATE_LEN];
    int id_programma;
    int num_partecipanti;
    double prezzo_tot_prenotazione;
};
struct prenotazione_support {
    unsigned num_entries;
    char data_prenotazione[DATE_LEN];
    char nome_programma[MAX_LEN];
    char data_partenza[MAX_LEN];
    double prezzo_per_persona;
    struct partecipante partecipanti[];
};
struct albergo {
    char indirizzo[MAX_LEN];
    char nome_località[MAX_LEN];
    double costo_per_notte;
    char referente[MAX_LEN];
    char nome_albergo[MAX_LEN];
    char fax[MAX_LEN];
    char email[MAX_LEN];
    char telefono[MAX_LEN];
    int capienza_max;
    int num_notti;
};
struct albergo_support {
    unsigned num_entries;
    struct albergo alberghi[];
};
struct localita {
    char nome_località[MAX_LEN];
    char stato[MAX_LEN];
};

struct localita_support {
    unsigned num_entries;
    struct localita localita[];
};
