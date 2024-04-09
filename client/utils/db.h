#include <stdbool.h>
#include <mysql.h>
extern void set_binding_param(MYSQL_BIND *param, enum enum_field_types type, void *buffer, unsigned long len);
extern void time_to_mysql_time(char *str, MYSQL_TIME *time);
extern void date_to_mysql_time(char *str, MYSQL_TIME *time);
extern void mysql_date_to_string(MYSQL_TIME *date, char *str);
extern void mysql_time_to_string(MYSQL_TIME *time, char *str);
