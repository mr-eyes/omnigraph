#ifndef OMNIGRAPH_SQLITEMANAGER_HPP
#define OMNIGRAPH_SQLITEMANAGER_HPP

#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include "iostream"

using namespace std;

class SQLiteManager {

private:
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    static int callback(void *NotUsed, int argc, char **argv, char **azColName);


public:
    SQLiteManager(string db_file);

    void close();

};


#endif //OMNIGRAPH_SQLITEMANAGER_HPP
