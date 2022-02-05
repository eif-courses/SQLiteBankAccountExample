//
// Created by Marius on 2/5/2022.
//

#ifndef SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H
#define SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H


#include <iostream>
using std::string;

class BankAccount
{
public:
    BankAccount(int, string, string, double);

    void SetAccountNumber(int accountNumber);
    void SetLastName(string lastName);
    void SetFirstName(string firstName);
    void SetBalance(double balance);

    int GetAccountNumber() const;
    string GetFirstName() const;
    string GetLastName() const;
    double GetBalance() const;

private:
    int _accountNumber;
    string _firstName;
    string _lastName;
    double _balance;
};




#endif //SQLITEBANKACCOUNTEXAMPLE_BANKACCOUNT_H
