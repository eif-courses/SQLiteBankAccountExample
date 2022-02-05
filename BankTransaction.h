//
// Created by Marius on 2/5/2022.
//

#ifndef SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H
#define SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H


#include "sqlite3.h"
#include "BankAccount.h"
#include <iostream>
using namespace std;
class BankTransaction{
private:
    sqlite3* _connection;
public:
    int OpenConnection(const char* dbPath);
    void CloseConnection();
    void CreateDummyTable(const char* dbPath);
    BankAccount GetAccount(int accountNumber);
    void CreateAccount(BankAccount bankAccount);
    void CloseAccount(int acc);
    void Deposit(int id, double amount);
    void Withdraw(int id, double amount);
    void PrintAllAccounts();
    void Message(string message);
    static int Callback(void* context,  // user-provided object (4th param to sqlite3_exec)
                        int columnCount,      // number of columns
                        char** columnValues,  // array of column values as C-style strings
                        char** columnName);    // array of column names as C-style strings);
};

#endif //SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H
