#include "db.h"
#include <stdio.h>

MYSQL* db_connect()
{
    MYSQL* conn = mysql_init(NULL);

    if (conn == NULL)
    {
        return NULL;
    }

    if (mysql_real_connect(
            conn,
            DB_HOST,
            DB_USER,
            DB_PASSWORD,
            DB_NAME,
            DB_PORT,
            NULL,
            0) == NULL)
    {
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

void db_close(MYSQL* conn)
{
    if (conn != NULL)
    {
        mysql_close(conn);
    }
}