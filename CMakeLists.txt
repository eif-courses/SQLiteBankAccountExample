cmake_minimum_required(VERSION 3.21)
project(SQLiteBankAccountExample)

set(CMAKE_CXX_STANDARD 14)

add_executable(SQLiteBankAccountExample main.cpp BankAccount.cpp BankAccount.h BankTransaction.cpp BankTransaction.h sqlite3.h sqlite3.c)
