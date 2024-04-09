#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
static struct programma_di_viaggio_support * list_programmi_di_viaggio_segretari(MYSQL *conn);
static void aggiungi_guida(MYSQL *conn,struct guida * guida);
static void assegna_autobus(MYSQL *conn,struct autobus_privato * autobusPrivato,struct viaggio * viaggio);
static struct tappa_support *  list_tappe_for_a_program(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio);
static void aggiungi_localita(MYSQL *conn,struct localita *localita);
static void aggiungi_albergo(MYSQL *conn, struct albergo*albergo);
static struct viaggio_support * list_viaggi_by_status(MYSQL *conn,struct viaggio*viaggio);
static struct guida_support * lista_assegnamenti_guide(MYSQL *conn);
static struct autobus_support * lista_assegnamenti_autobus(MYSQL *conn);
static struct guida_support * list_info_guide(MYSQL *conn);
static struct autobus_support * list_info_autobus(MYSQL *conn);
static struct albergo_support * list_info_alberghi(MYSQL *conn);
static int aggiungi_tappa(MYSQL *conn,struct tappa*tappa);
static void aggiungi_programma_di_viaggio(MYSQL *conn,struct programma_di_viaggio*programmaDiViaggio);
static struct report_support * extract_partecipanti_info(MYSQL_STMT*prepared_stmt);
static void extract_costi_information(MYSQL_STMT*prepared_stmt,struct report_support*reportSupport);
static struct report_support *do_report(MYSQL *conn,struct viaggio*viaggio);
static void aggiungi_autobus_privato(MYSQL *conn,struct autobus_privato*autobusPrivato);
static void aggiungi_viaggio(MYSQL *conn,struct viaggio*viaggio);
