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

    if (strcmp(path, "/receipt") == 0)
    {
        char supplier[150];
        char productId[30];
        char quantity[30];

        get_param(
            post,
            "supplier",
            supplier,
            sizeof(supplier)
        );

        get_param(
            post,
            "product_id",
            productId,
            sizeof(productId)
        );

        get_param(
            post,
            "quantity",
            quantity,
            sizeof(quantity)
        );

        char query[500];

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO receipts "
            "(supplier,status) "
            "VALUES('%s','Done')",
            supplier
        );

        if (mysql_query(conn, query) == 0)
        {
            unsigned long receiptId =
                mysql_insert_id(conn);

            snprintf(
                query,
                sizeof(query),
                "INSERT INTO receipt_items "
                "(receipt_id,product_id,quantity) "
                "VALUES(%lu,%s,%s)",
                receiptId,
                productId,
                quantity
            );

            mysql_query(conn, query);

            snprintf(
                query,
                sizeof(query),
                "UPDATE products "
                "SET stock = stock + %s "
                "WHERE product_id = %s",
                quantity,
                productId
            );

            mysql_query(conn, query);

            snprintf(
                query,
                sizeof(query),
                "INSERT INTO stock_ledger "
                "(product_id,operation_type,quantity,reference_id) "
                "VALUES(%s,'Receipt',%s,%lu)",
                productId,
                quantity,
                receiptId
            );

            mysql_query(conn, query);

            printf(
                "{\"success\":true,"
                "\"message\":\"Receipt validated successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not create receipt\"}"
            );
        }
    }

    else if (strcmp(path, "/delivery") == 0)
    {
        char customer[150];
        char productId[30];
        char quantity[30];

        get_param(
            post,
            "customer",
            customer,
            sizeof(customer)
        );

        get_param(
            post,
            "product_id",
            productId,
            sizeof(productId)
        );

        get_param(
            post,
            "quantity",
            quantity,
            sizeof(quantity)
        );

        char query[600];

        snprintf(
            query,
            sizeof(query),
            "SELECT stock "
            "FROM products "
            "WHERE product_id=%s",
            productId
        );

        if (mysql_query(conn, query) == 0)
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            MYSQL_ROW row =
                mysql_fetch_row(result);

            int stock = 0;

            if (row)
            {
                stock =
                    atoi(row[0]);
            }

            mysql_free_result(result);

            if (stock < atoi(quantity))
            {
                printf(
                    "{\"success\":false,"
                    "\"message\":\"Insufficient stock\"}"
                );

                db_close(conn);

                return 0;
            }
        }

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO deliveries "
            "(customer,status) "
            "VALUES('%s','Done')",
            customer
        );

        if (mysql_query(conn, query) == 0)
        {
            unsigned long deliveryId =
                mysql_insert_id(conn);

            snprintf(
                query,
                sizeof(query),
                "INSERT INTO delivery_items "
                "(delivery_id,product_id,quantity) "
                "VALUES(%lu,%s,%s)",
                deliveryId,
                productId,
                quantity
            );

            mysql_query(conn, query);

            snprintf(
                query,
                sizeof(query),
                "UPDATE products "
                "SET stock = stock - %s "
                "WHERE product_id=%s",
                quantity,
                productId
            );

            mysql_query(conn, query);

            snprintf(
                query,
                sizeof(query),
                "INSERT INTO stock_ledger "
                "(product_id,operation_type,quantity,reference_id) "
                "VALUES(%s,'Delivery',-%s,%lu)",
                productId,
                quantity,
                deliveryId
            );

            mysql_query(conn, query);

            printf(
                "{\"success\":true,"
                "\"message\":\"Delivery validated successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not create delivery\"}"
            );
        }
    }

    else if (strcmp(path, "/transfer") == 0)
    {
        char productId[30];
        char quantity[30];
        char fromLocation[100];
        char toLocation[100];

        get_param(
            post,
            "product_id",
            productId,
            sizeof(productId)
        );

        get_param(
            post,
            "quantity",
            quantity,
            sizeof(quantity)
        );

        get_param(
            post,
            "from_location",
            fromLocation,
            sizeof(fromLocation)
        );

        get_param(
            post,
            "to_location",
            toLocation,
            sizeof(toLocation)
        );

        char query[700];

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO transfers "
            "(product_id,quantity,from_location,to_location,status) "
            "VALUES(%s,%s,'%s','%s','Done')",
            productId,
            quantity,
            fromLocation,
            toLocation
        );

        if (mysql_query(conn, query) == 0)
        {
            unsigned long transferId =
                mysql_insert_id(conn);

            snprintf(
                query,
                sizeof(query),
                "INSERT INTO stock_ledger "
                "(product_id,operation_type,quantity,reference_id) "
                "VALUES(%s,'Transfer',0,%lu)",
                productId,
                transferId
            );

            mysql_query(conn, query);

            printf(
                "{\"success\":true,"
                "\"message\":\"Stock transferred successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not create transfer\"}"
            );
        }
    }

    else if (strcmp(path, "/adjustment") == 0)
    {
        char productId[30];
        char systemQuantity[30];
        char countedQuantity[30];
        char reason[200];

        get_param(
            post,
            "product_id",
            productId,
            sizeof(productId)
        );

        get_param(
            post,
            "system_quantity",
            systemQuantity,
            sizeof(systemQuantity)
        );

        get_param(
            post,
            "counted_quantity",
            countedQuantity,
            sizeof(countedQuantity)
        );

        get_param(
            post,
            "reason",
            reason,
            sizeof(reason)
        );

        int difference =
            atoi(countedQuantity) -
            atoi(systemQuantity);

        char query[800];

        snprintf(
            query,
            sizeof(query),
            "INSERT INTO adjustments "
            "(product_id,system_quantity,counted_quantity,"
            "difference,reason) "
            "VALUES(%s,%s,%s,%d,'%s')",
            productId,
            systemQuantity,
            countedQuantity,
            difference,
            reason
        );

        if (mysql_query(conn, query) == 0)
        {
            snprintf(
                query,
                sizeof(query),
                "UPDATE products "
                "SET stock=%s "
                "WHERE product_id=%s",
                countedQuantity,
                productId
            );

            mysql_query(conn, query);

            printf(
                "{\"success\":true,"
                "\"message\":\"Inventory adjusted successfully\"}"
            );
        }
        else
        {
            printf(
                "{\"success\":false,"
                "\"message\":\"Could not apply adjustment\"}"
            );
        }
    }

    else if (strcmp(path, "/receipts") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT receipt_id,supplier,status,created_at "
                "FROM receipts "
                "ORDER BY receipt_id DESC"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            MYSQL_ROW row;
            int first = 1;

            while (
                result != NULL &&
                (row = mysql_fetch_row(result)) != NULL
            )
            {
                if (!first)
                {
                    printf(",");
                }

                printf(
                    "{\"receipt_id\":%s,"
                    "\"supplier\":\"%s\","
                    "\"status\":\"%s\","
                    "\"created_at\":\"%s\"}",
                    row[0],
                    row[1],
                    row[2],
                    row[3]
                );

                first = 0;
            }

            printf("]");

            if (result != NULL)
            {
                mysql_free_result(result);
            }
        }
        else
        {
            printf("[]");
        }
    }

    else if (strcmp(path, "/deliveries") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT delivery_id,customer,status,created_at "
                "FROM deliveries "
                "ORDER BY delivery_id DESC"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            MYSQL_ROW row;
            int first = 1;

            while (
                result != NULL &&
                (row = mysql_fetch_row(result)) != NULL
            )
            {
                if (!first)
                {
                    printf(",");
                }

                printf(
                    "{\"delivery_id\":%s,"
                    "\"customer\":\"%s\","
                    "\"status\":\"%s\","
                    "\"created_at\":\"%s\"}",
                    row[0],
                    row[1],
                    row[2],
                    row[3]
                );

                first = 0;
            }

            printf("]");

            if (result != NULL)
            {
                mysql_free_result(result);
            }
        }
        else
        {
            printf("[]");
        }
    }

    else if (strcmp(path, "/transfers") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT transfer_id,product_id,quantity,"
                "from_location,to_location,status "
                "FROM transfers "
                "ORDER BY transfer_id DESC"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            MYSQL_ROW row;
            int first = 1;

            while (
                result != NULL &&
                (row = mysql_fetch_row(result)) != NULL
            )
            {
                if (!first)
                {
                    printf(",");
                }

                printf(
                    "{\"transfer_id\":%s,"
                    "\"product_id\":%s,"
                    "\"quantity\":%s,"
                    "\"from_location\":\"%s\","
                    "\"to_location\":\"%s\","
                    "\"status\":\"%s\"}",
                    row[0],
                    row[1],
                    row[2],
                    row[3],
                    row[4],
                    row[5]
                );

                first = 0;
            }

            printf("]");

            if (result != NULL)
            {
                mysql_free_result(result);
            }
        }
        else
        {
            printf("[]");
        }
    }

    else if (strcmp(path, "/adjustments") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT adjustment_id,product_id,"
                "system_quantity,counted_quantity,"
                "difference,reason "
                "FROM adjustments "
                "ORDER BY adjustment_id DESC"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            MYSQL_ROW row;
            int first = 1;

            while (
                result != NULL &&
                (row = mysql_fetch_row(result)) != NULL
            )
            {
                if (!first)
                {
                    printf(",");
                }

                printf(
                    "{\"adjustment_id\":%s,"
                    "\"product_id\":%s,"
                    "\"system_quantity\":%s,"
                    "\"counted_quantity\":%s,"
                    "\"difference\":%s,"
                    "\"reason\":\"%s\"}",
                    row[0],
                    row[1],
                    row[2],
                    row[3],
                    row[4],
                    row[5]
                );

                first = 0;
            }

            printf("]");

            if (result != NULL)
            {
                mysql_free_result(result);
            }
        }
        else
        {
            printf("[]");
        }
    }

    else if (strcmp(path, "/movements") == 0)
    {
        if (
            mysql_query(
                conn,
                "SELECT ledger_id,product_id,"
                "operation_type,quantity,"
                "reference_id,created_at "
                "FROM stock_ledger "
                "ORDER BY ledger_id DESC"
            ) == 0
        )
        {
            MYSQL_RES* result =
                mysql_store_result(conn);

            printf("[");

            MYSQL_ROW row;
            int first = 1;

            while (
                result != NULL &&
                (row = mysql_fetch_row(result)) != NULL
            )
            {
                if (!first)
                {
                    printf(",");
                }

                printf(
                    "{\"ledger_id\":%s,"
                    "\"product_id\":%s,"
                    "\"operation_type\":\"%s\","
                    "\"quantity\":%s,"
                    "\"reference_id\":%s,"
                    "\"created_at\":\"%s\"}",
                    row[0],
                    row[1],
                    row[2],
                    row[3],
                    row[4],
                    row[5]
                );

                first = 0;
            }

            printf("]");

            if (result != NULL)
            {
                mysql_free_result(result);
            }
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