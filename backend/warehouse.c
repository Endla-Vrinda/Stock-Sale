#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/db.h"
#include "../common/http.h"

int main()
{
    send_json_header();

    char* pathInfo = getenv("PATH_INFO");

    char path[100] = "";

    if (pathInfo != NULL)
    {
        strncpy(
            path,
            pathInfo,
            sizeof(path) - 1
        );
    }

    char post[2048];

    read_post(
        post,
        sizeof(post)
    );

    MYSQL* conn =
        db_connect();

    if (conn == NULL)
    {
        printf(
            "{\"success\":false,"
            "\"message\":\"Database connection failed\"}"
        );

        return 1;
    }

    if (strcmp(path, "/create") == 0)
    {
        char name[100];
        char location[200];

        get_param(
            post,
            "name",
            name,
            sizeof(name)
        );

        get_param(
            post,
            "location",
            location,
            sizeof(location)
        );

        char query[600];

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO warehouses "
            "(warehouse_name,location) "
            "VALUES('%s','%s')",
            name,
            location
        );

        if (mysql_query(conn, query) == 0)
        {
            printf(
                "{\"success\":true,"
                "\"message\":\"Warehouse added successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not add warehouse\"}"
            );
        }
    }

    else if (strcmp(path, "/list") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT warehouse_id,"
                "warehouse_name,"
                "location "
                "FROM warehouses"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            if (result != NULL)
            {
                MYSQL_ROW row;
                int first = 1;

                while (
                    (row =
                        mysql_fetch_row(result))
                    != NULL
                )
                {
                    if (!first)
                    {
                        printf(",");
                    }

                    printf(
                        "{"
                        "\"warehouse_id\":%s,"
                        "\"warehouse_name\":\"%s\","
                        "\"location\":\"%s\""
                        "}",
                        row[0],
                        row[1],
                        row[2]
                    );

                    first = 0;
                }

                mysql_free_result(result);
            }

            printf("]");
        }
        else
        {
            printf("[]");
        }
    }

    else
    {
        printf(
            "{\"success\":false,"
            "\"message\":\"Unknown operation\"}"
        );
    }

    db_close(conn);

    return 0;
}