 
#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>

#include "mysql_connection.h"
#include "driver.h"
#include "exception.h"
#include "resultset.h"
#include "statement.h"
#include "prepared_statement.h"

//#include <mysql.h>
#include <iostream>
#include <string>
#include <sstream>

class Database
{
private:
    std::string server;
    std::string db;
    std::string username;
    std::string password;

    int num_rows, num_fields, st;
    bool is_connected;
    // MYSQL *connection, mysql;
    // MYSQL_RES *res;
    // MYSQL_ROW row;
    
    sql::Driver *driver;
    sql::Connection *connection;
    sql::Statement *stmt;
    sql::ResultSet *res;

public:
    Database() : is_connected(true)
    {

        //mysql_init(&mysql);
    }

    void connect(std::string ip = "127.0.0.1", std::string d = "test", std::string u = "root", std::string p = "")
    {
        driver = get_driver_instance();
        server = ip; db = d; username = u; password = p;

        //connection = mysql_real_connect(&mysql, server.c_str(), username.c_str(), password.c_str(), db.c_str(), 3306, 0, 0);
        connection = driver->connect(server, username, password);

        stmt = connection->createStatement();

        //if (connection == NULL)
            //throw mysql_error(&mysql);

        connection->setSchema(db);
    }

    ~Database() 
    {
        //mysql_free_result(res);
        //mysql_close(connection);
    }

    int state() const { return st; }
    int rows() const { return num_rows; }
    void set_connected(bool b) { is_connected = b; }
    bool connected() const { return is_connected; }
    //MYSQL_RES* result() const { return res; }
    void query(std::string);
    std::string getQueryRows();
};

#endif
