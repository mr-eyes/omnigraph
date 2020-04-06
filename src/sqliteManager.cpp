#include "sqliteManager.hpp"
#include "sqlite3pp.h"


int SQLiteManager::callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

SQLiteManager::SQLiteManager(string db_file) {

    this->db = sqlite3pp::database (db_file.c_str());

    string _sqlite_checkTable = "SELECT ID FROM reads LIMIT 1";
    this->rc = sqlite3_exec(this->db.db_, _sqlite_checkTable.c_str(), this->callback, 0, &this->zErrMsg);

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

        char *_sqlite_create_index = "CREATE INDEX components_index ON reads (seq1_collective_component);";

        this->rc = sqlite3_exec(this->db.db_, _sqlite_create_table, this->callback, 0, &this->zErrMsg);
        if (this->rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
            sqlite3_free(this->zErrMsg);
        } else {
            fprintf(stderr, "Table created successfully\n");
        }

        this->rc = sqlite3_exec(this->db.db_, _sqlite_create_index, this->callback, 0, &this->zErrMsg);

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
    string _sqlite_sync = "PRAGMA synchronous = OFF;";
    this->rc = sqlite3_exec(this->db.db_, _sqlite_sync.c_str(), this->callback, 0, &this->zErrMsg);
    if (this->rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", this->zErrMsg);
        sqlite3_free(this->zErrMsg);
    }

}

void SQLiteManager::close() {
    sqlite3_close(this->db.db_);
}

void SQLiteManager::insert_PE(string &R1, string &R2, int &collectiveComp1, int &collectiveComp2) {

    const string _sqlite_insert = "INSERT INTO reads"
                            "(PE_seq1, PE_seq2, seq1_collective_component, seq2_collective_component, seq1_original_component, seq2_original_component)"
                            "VALUES"
                            "('"+ R1 +"', '"+ R2 +"', " + to_string(collectiveComp1) +", "+ to_string(collectiveComp2) +", 0, 0);";

    this->rc = sqlite3_exec(this->db.db_, _sqlite_insert.c_str(), this->callback, 0, &this->zErrMsg);

}