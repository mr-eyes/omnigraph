import mysql.connector
from mysql.connector import Error
from mysql.connector import errorcode

try:
    connection = mysql.connector.connect(host='localhost',
                                         database='universal_transcriptome',
                                         user='root',
                                         password='')

    mySql_insert_query = """INSERT INTO `unitigs_tracker` 
                            (`ID`, `unitig_ID`, `originalComponent_ID`, `collectiveComponent_ID`, `finalComponent_ID`) 
                            VALUES
                            (NULL, '1', '2', '3', '4')"""

    cursor = connection.cursor()
    cursor.autocommit = True

    for i in range(100000):
        cursor.execute(mySql_insert_query)

    connection.commit()
    print(cursor.rowcount, "Record inserted successfully into Laptop table")
    cursor.close()

except mysql.connector.Error as error:
    print("Failed to insert record into Laptop table {}".format(error))

finally:
    if (connection.is_connected()):
        connection.close()
        print("MySQL connection is closed")