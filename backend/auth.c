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

    MYSQL* conn = db_connect();

    if (conn == NULL)
    {
        printf(
            "{\"success\":false,"
            "\"message\":\"Database connection failed\"}"
        );

        return 1;
    }

    if (strcmp(path, "/signup") == 0)
    {
        char name[100];
        char email[150];
        char password[255];

        get_param(
            post,
            "name",
            name,
            sizeof(name)
        );

        get_param(
            post,
            "email",
            email,
            sizeof(email)
        );

        get_param(
            post,
            "password",
            password,
            sizeof(password)
        );

        char query[700];

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO users "
            "(name,email,password) "
            "VALUES('%s','%s','%s')",
            name,
            email,
            password
        );

        if (mysql_query(conn, query) == 0)
        {
            printf(
                "{\"success\":true,"
                "\"message\":\"Account created successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not create account\"}"
            );
        }
    }

    else if (strcmp(path, "/login") == 0)
    {
        char email[150];
        char password[255];

        get_param(
            post,
            "email",
            email,
            sizeof(email)
        );

        get_param(
            post,
            "password",
            password,
            sizeof(password)
        );

        char query[700];

        snprintf(
            query,
            sizeof(query),
            "SELECT user_id "
            "FROM users "
            "WHERE email='%s' "
            "AND password='%s'",
            email,
            password
        );

        if (mysql_query(conn, query) == 0)
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            if (
                result != NULL &&
                mysql_num_rows(result) > 0
            )
            {
                printf(
                    "{\"success\":true,"
                    "\"message\":\"Login successful\"}"
                );
            }
            else
            {
                printf(
                    "{\"success\":false,"
                    "\"message\":\"Invalid email or password\"}"
                );
            }

            if (result != NULL)
            {
                mysql_free_result(result);
            }
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