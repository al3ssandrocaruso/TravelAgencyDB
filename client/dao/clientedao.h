#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
#include "../utils/db.h"
static void aggiungi_partecipante(MYSQL *conn,struct partecipante * partecipante,struct prenotazione*prenotazione);
static void disdici_prenotazione(MYSQL *conn,struct prenotazione*prenotazione);
static struct programma_di_viaggio_support *  lista_programmi_di_viaggio_cliente(MYSQL *conn);
static struct tappa_support *  list_tappe_for_a_program(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio);
static void prenota_viaggio(MYSQL *conn,struct prenotazione*prenotazione);
static struct viaggio_support * list_viaggi_for_a_program(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio);
static struct prenotazione_support * extract_partecipanti_prenotazione_info(MYSQL_STMT*prepared_stmt);
static void extract_prenotazione_general_information(MYSQL_STMT*prepared_stmt,struct prenotazione_support*prenotazioneSupport);
static struct prenotazione_support * load_prenotazione_info(MYSQL *conn,struct prenotazione*prenotazione);