//
// Created by Marius on 2/5/2022.
//

#ifndef SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H
#define SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H

#include <iostream>

using std::string;

class BankAccount {
public:
  BankAccount(int accountNumber, string firstName, string lastName, double balance);

  int GetAccountNumber() const;

  const string &GetFirstName() const;

  const string &GetLastName() const;

  double GetBalance() const;

  void SetBalance(double balance);

private:
  int _accountNumber;
  string _firstName;
  string _lastName;
  double _balance;
};

#endif //SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H
