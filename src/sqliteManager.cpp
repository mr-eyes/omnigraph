#include "sqliteManager.hpp"


int SQLiteManager::callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

SQLiteManager::SQLiteManager(string db_file) {

    this->rc = sqlite3_open(db_file.c_str(), &this->db);

    if (this->rc) {
        fprintf(stderr,
                "Can't open database: %s\n", sqlite3_errmsg(this->db));
        exit(1);
    } else {
        fprintf(stderr,
                "Opened database successfully\n");
    }

    string _sqlite_checkTable = "SELECT ID FROM reads LIMIT 1";
    this->rc = sqlite3_exec(this->db, _sqlite_checkTable.c_str(), this->callback, 0, &this->zErrMsg);

    if (this->rc) {

        fprintf(stderr, "`reads` table was not found.\n");
        fprintf(stderr, "Creating `reads` table...\n");

        const char *_sqlite_create_table = "CREATE TABLE IF NOT EXISTS `reads` ("
                                           "`ID`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                                           "`PE_seq1`	TEXT,"
                                           "`PE_seq2`	TEXT,"
                                           "`seq1_collective_component`	INTEGER,"
                                           "`seq2_collective_component`	INTEGER,"
                                           "`seq1_original_component`	INTEGER,"
                                           "`seq2_original_component`	INTEGER"
                                           ");";

        char *_sqlite_create_index = "CREATE INDEX components_index ON reads (seq1_collective_component, seq2_collective_component);";

        this->rc = sqlite3_exec(this->db, _sqlite_create_table, this->callback, 0, &this->zErrMsg);
        if (this->rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
            sqlite3_free(this->zErrMsg);
        } else {
            fprintf(stderr, "Table created successfully\n");
        }

        this->rc = sqlite3_exec(this->db, _sqlite_create_index, this->callback, 0, &this->zErrMsg);

        if (this->rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
            sqlite3_free(this->zErrMsg);
        } else {
            fprintf(stderr, "DB Index created successfully.\n");
        }

    } else {
        fprintf(stderr, "`reads` table found.\n");
    }

    fprintf(stderr, "Done initializing DB.\n");
}

void SQLiteManager::close() {
    sqlite3_close(this->db);
}