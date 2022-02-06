//
// Created by Marius on 2/5/2022.
//

#ifndef SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H
#define SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H


#include "sqlite3.h"
#include "BankAccount.h"
#include <iostream>
#include <memory>
#include "sqlite3.h"

using namespace std;

class BankTransaction {
private:
  sqlite3 *_connection{};
  sqlite3_stmt *_statement{};
public:
  virtual ~BankTransaction();

  int OpenConnection(const char *dbPath);

  void CloseConnection();

  void CreateDummyTable(const char *dbPath);

  unique_ptr<BankAccount> GetAccount(int accountNumber);

  void CreateAccount(unique_ptr<BankAccount> bankAccount);

  void CloseAccount(int accountNumber);

  void Deposit(int accountNumber, double amount);

  void Withdraw(int accountNumber, double amount);

  void PrintAllAccounts();

  static void Message(const string &message);

  static int Callback(void *context,  // user-provided object (4th param to sqlite3_exec)
                      int columnCount,      // number of columns
                      char **columnValues,  // array of column values as C-style strings
                      char **columnName);    // array of column names as C-style strings);
};

#endif //SQLITEBANKACCOUNTEXAMPLE_BANKTRANSACTION_H
