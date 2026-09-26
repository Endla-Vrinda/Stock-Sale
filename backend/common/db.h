#ifndef DB_H
#define DB_H

#include <mysql.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWORD "YOUR_PASSWORD"
#define DB_NAME "stocksense"
#define DB_PORT 3306

MYSQL* db_connect();

void db_close(MYSQL* conn);

#endif