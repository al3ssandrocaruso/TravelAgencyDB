#include<stdio.h>
#include <stdio.h>
#include <string.h>
#include "../defines.h"
#include "../model/model.h"
char segretario_get_action();
static void get_programma_di_viaggio_info(struct programma_di_viaggio *programmaDiViaggio);
static void get_autobus_privato_info(struct autobus_privato *autobusPrivato);
static void get_viaggio_info(struct viaggio *viaggio);
static void get_guida_info(struct guida *guida);
static void get_albergo_info(struct albergo *albergo);
static void get_localita_info(struct localita *localita);