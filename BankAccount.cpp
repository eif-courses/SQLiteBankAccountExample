//
// Created by Marius on 2/5/2022.
//
#include "BankAccount.h"

BankAccount::BankAccount(int accountNumber, string firstName, string lastName, double balance)
    : _accountNumber(accountNumber), _firstName(std::move(firstName)), _lastName(std::move(lastName)),
      _balance(balance) {}

int BankAccount::GetAccountNumber() const {
  return _accountNumber;
}

const string &BankAccount::GetFirstName() const {
  return _firstName;
}

const string &BankAccount::GetLastName() const {
  return _lastName;
}

double BankAccount::GetBalance() const {
  return _balance;
}

void BankAccount::SetBalance(double balance) {
  _balance = balance;
}
