
#include "Database.h"

void Database::query(std::string q)
{
    //st = mysql_query(connection, q.c_str());

    //if (st != 0)
        //throw mysql_error(&mysql);

    // IF the query does not contain INSERT statement
    if (q.find("INSERT") == std::string::npos)
    {
        // res = mysql_store_result(connection);
        // num_rows = mysql_num_rows(res);
        // num_fields = mysql_num_fields(res);
        //std::cout << "query executing..." << std::endl;
        res = stmt->executeQuery(q.c_str());
        //std::cout << "query executed!" << std::endl;
        num_rows = res->rowsCount();
        num_fields = res->getRow();
    }
    else
    {
        //std::cout << "insert statement executing..." << std::endl;
        stmt->execute(q.c_str());
        //std::cout << "insert statement executed!" << std::endl;
    }
}

std::string Database::getQueryRows()
{
    std::stringstream str;

    while (res->next())
    {
        std::string name = res->getString(2);
        int score = res->getInt(3);
        double accuracy = res->getDouble(4);

        str << name << ';' << score << ';' << accuracy << ';';
        str << '\n';
    }

    // while ((row = mysql_fetch_row(res)))
    // {
    //     // loop through fields excluding id
    //     for (int i = 1; i < num_fields; i++)
    //     {
    //         if (row[i] != NULL)
    //             str << row[i] << ';';
    //     }
    //     str << '\n';
    // }

    return str.str();
}
