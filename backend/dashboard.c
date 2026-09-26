#include <stdio.h>
#include <stdlib.h>

#include "../common/db.h"
#include "../common/http.h"

int main()
{
    send_json_header();

    MYSQL* conn =
        db_connect();

    if (conn == NULL)
    {
        printf(
            "{\"error\":\"Database connection failed\"}"
        );

        return 1;
    }

    int totalProducts = 0;
    int lowStock = 0;
    int outStock = 0;
    int receipts = 0;
    int deliveries = 0;
    int transfers = 0;

    MYSQL_RES* result;

    mysql_query(
        conn,
        "SELECT COUNT(*) FROM products"
    );

    result =
        mysql_store_result(conn);

    if (result)
    {
        MYSQL_ROW row =
            mysql_fetch_row(result);

        if (row)
        {
            totalProducts =
                atoi(row[0]);
        }

        mysql_free_result(result);
    }

    mysql_query(
        conn,
        "SELECT COUNT(*) "
        "FROM products "
        "WHERE stock <= reorder_level "
        "AND stock > 0"
    );

    result =
        mysql_store_result(conn);

    if (result)
    {
        MYSQL_ROW row =
            mysql_fetch_row(result);

        if (row)
        {
            lowStock =
                atoi(row[0]);
        }

        mysql_free_result(result);
    }

    mysql_query(
        conn,
        "SELECT COUNT(*) "
        "FROM products "
        "WHERE stock = 0"
    );

    result =
        mysql_store_result(conn);

    if (result)
    {
        MYSQL_ROW row =
            mysql_fetch_row(result);

        if (row)
        {
            outStock =
                atoi(row[0]);
        }

        mysql_free_result(result);
    }

    printf(
        "{"
        "\"totalProducts\":%d,"
        "\"lowStock\":%d,"
        "\"outStock\":%d,"
        "\"receipts\":%d,"
        "\"deliveries\":%d,"
        "\"transfers\":%d"
        "}",
        totalProducts,
        lowStock,
        outStock,
        receipts,
        deliveries,
        transfers
    );

    db_close(conn);

    return 0;
}