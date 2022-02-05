//
// Created by Marius on 2/5/2022.
//

#include "BankTransaction.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "BankTransaction.h"
#include "BankAccount.h"


int BankTransaction::OpenConnection(const char* dbPath)
{
    return sqlite3_open(dbPath, &_connection);
}
void BankTransaction::CloseConnection() {
    sqlite3_close(_connection);
}

// Without Callback working
BankAccount BankTransaction::GetAccount(int accountNumber){

    OpenConnection("BANK.DB");

    BankAccount bankAccount(1, "", "", 1.2);

    sqlite3_stmt* statment;

    const char* sql ="SELECT * FROM BANK_ACCOUNT WHERE ACCOUNT_ID=?";

    int rc = (sqlite3_prepare_v2(_connection, sql, -1, &statment, nullptr));

    if (sqlite3_bind_int(statment, 1, accountNumber) != SQLITE_OK) {
        printf("\nCould not bind integer.\n");

    }

    while ((rc = sqlite3_step(statment)) == SQLITE_ROW) {
        bankAccount = BankAccount(accountNumber, string(reinterpret_cast<const char*>(sqlite3_column_text(statment, 1))), string(reinterpret_cast<const char*>(sqlite3_column_text(statment, 2))), sqlite3_column_int(statment, 3));
        printf("ACCOUNT_ID: %s, BALANCE: %d eur\n", sqlite3_column_text(statment, 0), sqlite3_column_int(statment, 3));
    }

    sqlite3_finalize(statment);

    CloseConnection();

    return bankAccount;
}


void BankTransaction::Withdraw(int accountNumber, double amount)
{
    BankAccount bankAccount = GetAccount(accountNumber);

    if (bankAccount.GetBalance() < amount)
        Message("Cannot withdraw. Try lower amount.");
    else {
        bankAccount.SetBalance(bankAccount.GetBalance() - amount);

        Deposit(accountNumber, bankAccount.GetBalance()); // TODO you can create other function for Withdraw update Statement

        string text = "Withdraw successfull: -" + to_string(amount) + " eur";
        Message(text);
        Message("Your current balance: " + to_string(bankAccount.GetBalance()) + " eur");
    }
}
// WORKING
void BankTransaction::Deposit(int accountNumber, double amount)
{
    OpenConnection("BANK.DB");

    sqlite3_stmt* statment;
    char* zErrMsg = nullptr;
    const char* data = "Callback function called";

    /* Create SQL statement */
    const char* sql = "UPDATE BANK_ACCOUNT SET BALANCE=? WHERE ACCOUNT_ID=?;";


    if (sqlite3_prepare_v2(_connection, sql, -1, &statment, nullptr)!= SQLITE_OK) {
        printf("\nCould not EXECUTE STATMENT.\n");
    }
    if (sqlite3_bind_double(
            statment,
            1,  // Index of wildcard
            amount
    ) != SQLITE_OK) {
        printf("\nCould not bind double.\n");
        return;
    }
    if (sqlite3_bind_int(
            statment,
            2,  // Index of wildcard
            accountNumber
    ) != SQLITE_OK) {
        printf("\nCould not bind double.\n");
        return;
    }

    if (sqlite3_step(statment) != SQLITE_DONE) {
        printf("\nCould not step (execute) stmt.\n");
        return;
    }

    sqlite3_finalize(statment);
    printf("\n");
    //select_stmt("select * from foo");

    CloseConnection();

}


// WORKING
void BankTransaction::CreateAccount(BankAccount bankAccount)
{
    OpenConnection("BANK.DB");
    sqlite3_stmt* statment;

    const char* sql = "INSERT INTO BANK_ACCOUNT VALUES (?,?,?,?)";

    if (sqlite3_prepare_v2(_connection, sql, -1, &statment, nullptr) != SQLITE_OK) {
        printf("\nCould NOT EXECUTE STATEMENT!.\n");
        return;
    }
    if (sqlite3_bind_int(statment, 1, bankAccount.GetAccountNumber()) != SQLITE_OK) {
        printf("\nCould not bind int.\n");
        return;
    }

    const char* name = bankAccount.GetFirstName().c_str();

    cout << "------------------ACCOUNT NAME----------------\n" << name << "\n---------------------------------\n";


    if (sqlite3_bind_text(
            statment,
            2,  // Index of wildcard
            name, // Data as -> const * char
            bankAccount.GetFirstName().length(), // Data length
            SQLITE_STATIC
    ) != SQLITE_OK) {
        printf("\nCould not bind double.\n");
        return;
    }
    if (sqlite3_bind_text(
            statment,
            3,  // Index of wildcard
            bankAccount.GetLastName().c_str(), // Data as -> const * char
            bankAccount.GetLastName().length(), // Data length
            SQLITE_STATIC
    ) != SQLITE_OK) {
        printf("\nCould not bind double.\n");
        return;
    }
    if (sqlite3_bind_double(
            statment,
            4,  // Index of wildcard
            bankAccount.GetBalance()
    ) != SQLITE_OK) {
        printf("\nCould not bind int.\n");
        return;
    }

    if (sqlite3_step(statment) != SQLITE_DONE) {
        printf("\nCould not step (execute) stmt.\n");
        return;
    }

    printf("\n");
    sqlite3_finalize(statment);
    CloseConnection();


}


// WORKING
void BankTransaction::CloseAccount(int accountNumber)
{

    OpenConnection("BANK.DB");
    const char* sql = "DELETE FROM BANK_ACCOUNT WHERE ACCOUNT_ID=?";
    sqlite3_stmt* statment;

    if (sqlite3_prepare_v2(_connection, sql, -1, &statment, nullptr) != SQLITE_OK) {
        printf("\nCANT OPEN CONNECTION\n");
    }

    if (sqlite3_bind_int(statment, 1, accountNumber) != SQLITE_OK) {
        printf("\nCould not bind integer.\n");
        return;
    }

    char* zErrMsg = nullptr;
    const char* data = "Callback function called";


    /* Execute SQL statement */
    int rc = sqlite3_exec(_connection, sql, Callback, (void*)data, &zErrMsg);


    if (sqlite3_step(statment) != SQLITE_DONE) {
        printf("\nCould not step (execute) stmt.\n");
        return;
    }

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_finalize(statment);
    CloseConnection();

}

void BankTransaction::Message(string msg)
{
    cout << msg << endl;
}
void BankTransaction::PrintAllAccounts()
{
    OpenConnection("BANK.DB");
    char* zErrMsg = nullptr;
    const char* data = "Callback function called";

    /* Create SQL statement */
    const char* sql = "SELECT * from BANK_ACCOUNT";

    /* Execute SQL statement */
    int rc = sqlite3_exec(_connection, sql, Callback, (void*)data, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Operation done successfully\n");
    }
    CloseConnection();
}

void BankTransaction::CreateDummyTable(const char* dbPath) {

    const char* sql;
    char* zErrMsg = nullptr;

    /* Open database */
    int rc = OpenConnection(dbPath);
    if (rc) {
        cout << "CAN'T OPEN DATABASE!" << endl;
    }
    else {
        cout << "DATABASE SUCCESSFULLY OPENED!" << endl;
    }

    /* Create SQL statement */
    sql = "CREATE TABLE BANK_ACCOUNT("  \
        "ACCOUNT_ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "FIRSTNAME           TEXT    NOT NULL," \
        "LASTNAME            TEXT     NOT NULL," \
        "BALANCE        REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(_connection, sql, Callback, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }
    CloseConnection();
}


int BankTransaction::Callback(void* context, int columnCount, char** columnValues, char** columnName) {

    for (int i = 0; i < columnCount; i++) {
        cout << columnName[i] << " " << columnValues[i] << " ";
    }
    cout << endl;

    return 0;
}