//
// Created by Marius on 2/5/2022.
//

#include "BankAccount.h"


using std::string;

BankAccount::BankAccount(int accountNumber, string firstName, string lastName, double balance) :
        _accountNumber(accountNumber), _firstName(firstName), _lastName(lastName), _balance(balance) {}

void BankAccount::SetAccountNumber(int accountNumber)
{
    _accountNumber = accountNumber;
}

void BankAccount::SetLastName(string lastName)
{
    _lastName = lastName;
}

void BankAccount::SetFirstName(string firstName)
{
    _firstName = firstName;
}


void BankAccount::SetBalance(double balance)
{
    _balance = balance;
}

int BankAccount::GetAccountNumber() const
{
    return _accountNumber;
}

string BankAccount::GetFirstName() const
{
    return _firstName;
}

string BankAccount::GetLastName() const
{
    return _lastName;
}

double BankAccount::GetBalance() const
{
    return _balance;
}
