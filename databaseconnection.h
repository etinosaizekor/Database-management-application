#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <mysql/mysql.h>
#include <QString>
#include <iostream>

struct connection_details{
  const char *server = "localhost",
  *user="root",
  *password = "9889",
  *database = "universityDatabase";
};

MYSQL *mysql_connection_setup(struct connection_details mysql_details);

MYSQL_RES* mysql_execute_query(MYSQL* connection, const char *sql_query);

void QstringToCharArray(QString, char[]);
int QStringToInt(QString, int);

#endif // DATABASECONNECTION_H
