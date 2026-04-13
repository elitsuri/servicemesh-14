/* database.c — SQLite database layer for servicemesh */
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static sqlite3 *g_db = NULL;

int db_init(const char *path) {
    if (sqlite3_open(path, &g_db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(g_db));
        return -1;
    }
    const char *schema =
        "CREATE TABLE IF NOT EXISTS servicemesh_records ("
        "  id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name      TEXT NOT NULL,"
        "  value     TEXT,"
        "  created   INTEGER DEFAULT (strftime('%s', 'now'))"
        ");";
    char *err = NULL;
    if (sqlite3_exec(g_db, schema, NULL, NULL, &err) != SQLITE_OK) {
        fprintf(stderr, "Schema error: %s\n", err);
        sqlite3_free(err);
        return -1;
    }
    return 0;
}

int db_insert(const char *name, const char *value) {
    const char *sql = "INSERT INTO servicemesh_records (name, value) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL) != SQLITE_OK) return -1;
    sqlite3_bind_text(stmt, 1, name,  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt) == SQLITE_DONE ? 0 : -1;
    sqlite3_finalize(stmt);
    return rc;
}

void db_close(void) {
    if (g_db) { sqlite3_close(g_db); g_db = NULL; }
}
