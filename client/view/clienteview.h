#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
char cliente_get_action();
char prenotazione_get_action();
static void get_partecipante_info(struct partecipante *partecipante);
static void get_prenotazione_info(struct prenotazione *prenotazione);
static void get_prenotazione_to_manage_info(struct prenotazione *prenotazione);
static void get_id_info_for_tappe(struct programma_di_viaggio* programmaDiViaggio);
static void get_id_info_for_viaggi(struct programma_di_viaggio* programmaDiViaggio);
static void print_viaggi_available(struct viaggio_support *viaggioSupport);
static void print_programmi_available(struct programma_di_viaggio_support *programmaDiViaggioSupport);
static void print_itinerario(struct tappa_support *tappaSupport);
static void print_info_prenotazione(struct prenotazione_support *prenotazioneSupport);