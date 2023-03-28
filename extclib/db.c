#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <postgresql/libpq-fe.h>
#include "db.h"

#define UNUSED(x) (void)(x)

static PGconn *conn = NULL;
static PGresult *res = NULL;

static void _terminate(int code)
{
    if (code != 0)
        fprintf(stderr, "%s\n", PQerrorMessage(conn));

    if (res != NULL)
        PQclear(res);

    if (conn != NULL)
        PQfinish(conn);
}

static void _clearRes()
{
    PQclear(res);
    res = NULL;
}

static void _processNotice(void *arg, const char *message)
{
    UNUSED(arg);
    UNUSED(message);
    // do nothing
}

static int _startup() {
    // int libpq_ver = PQlibVersion();
    // printf("Version of libpq: %d\n", libpq_ver);
    conn = PQconnectdb("user=postgres password=postgres host=127.0.0.1 dbname=price");
    if (PQstatus(conn) != CONNECTION_OK)
    {
        _terminate(1);
        return 0;
    }
    PQsetNoticeProcessor(conn, _processNotice, NULL);
/*
    int server_ver = PQserverVersion(conn);
    char *user = PQuser(conn);
    char *db_name = PQdb(conn);

    printf("Server version: %d\n", server_ver);
    printf("User: %s\n", user);
    printf("Database name: %s\n", db_name);
*/
    return 1;
}

extern size_t get_table(FILE *tbl_file, char *tbl_name)
{
    static int8_t launched = 0;
    if (launched == 0) {
        if ((launched = _startup()) == 0) {
            return -1;
        }
    }
/*  res = PQexec(conn, "SET CLIENT_ENCODING TO UTF8");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        _terminate(1);
        return -2;
    }
*/
    char query[BUFSIZ];
    strcpy(query, "SELECT * FROM ");
    strcat(query, tbl_name);
    res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        _terminate(1);
        return -3;
    }

    PQprintOpt opt = {0};res = PQexec(conn, query);
    opt.header = 1; /* печатать заголовки полей и счётчик строк */
    opt.align = 1;      /* выравнивать поля */
    opt.html3 = 1, /* выводить HTML-таблицы */
    // opt.expanded = 1;   /* расширять таблицы */
    // opt.tableOpt = "";    /* атрибуты для HTML-таблицы */ /* insert to HTML <table ...> */
    opt.caption = tbl_name;     /* заголовок HTML-таблицы */ /* HTML <caption> */
    opt.fieldSep = "|"; /* Use a pipe as the field separator */

/*
    int ncols = PQnfields(res);
    int nrows = PQntuples(res);
    char *str;

    printf("Total: %d rows\n", nrows);
    printf("There are %d columns:\n", ncols);

    printf("<TR>\n");
    for (int i = 0; i < ncols; i++)
    {
        printf("<TH>");
        str = PQfname(res, i);
        printf("%30.30s ", str);
        printf("</TH>");
    }
    printf("\n");

    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {;
            str = PQgetvalue(res, i, j);
            printf("%30.30s", str);
        }
        printf("\n");
    }
*/
        

    PQprint(tbl_file, res, &opt);
    size_t len = ftell(tbl_file);
    fseek(tbl_file, 0, SEEK_SET);
    _clearRes();

    _terminate(0);
    return len;
}