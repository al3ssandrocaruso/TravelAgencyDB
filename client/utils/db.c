#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "../utils/db.h"
void set_binding_param(MYSQL_BIND *param, enum enum_field_types type, void *buffer, unsigned long len)
{
    memset(param, 0, sizeof(*param));

    param->buffer_type = type;
    param->buffer = buffer;
    param->buffer_length = len;
}
void time_to_mysql_time(char *str, MYSQL_TIME *time)
{
    memset(time, 0, sizeof(*time));
    sscanf(str, "%02d:%02d", &time->hour, &time->minute);
    time->time_type = MYSQL_TIMESTAMP_TIME;
}
void date_to_mysql_time(char *str, MYSQL_TIME *time)
{
    memset(time, 0, sizeof(*time));
    sscanf(str, "%4d-%2d-%2d", &time->year, &time->month, &time->day);
    time->time_type = MYSQL_TIMESTAMP_DATE;
}
void mysql_date_to_string(MYSQL_TIME *date, char *str)
{
    snprintf(str, DATE_LEN, "%4d-%02d-%02d", date->year, date->month, date->day);
}
void mysql_time_to_string(MYSQL_TIME *time, char *str)
{
    snprintf(str,TIME_LEN, "%02d:%02d", time->hour, time->minute);
}

