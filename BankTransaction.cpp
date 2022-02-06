#include "BankTransaction.h"
#include "BankAccount.h"

int BankTransaction::OpenConnection(const char *dbPath = "BANK.DB") {
  return sqlite3_open(dbPath, &_connection);
}

void BankTransaction::CloseConnection() {
  sqlite3_close(_connection);
}

void BankTransaction::Message(const string &msg) {
  cout << msg << endl;
}

BankTransaction::~BankTransaction() {
  sqlite3_close(_connection);
}

unique_ptr<BankAccount> BankTransaction::GetAccount(int accountNumber) {

  OpenConnection();
  unique_ptr<BankAccount> bankAccount;
  const char *sql = "SELECT * FROM BANK_ACCOUNT WHERE ACCOUNT_ID=?";
  int rc = (sqlite3_prepare_v2(_connection, sql, -1, &_statement, nullptr));

  if (sqlite3_bind_int(_statement, 1, accountNumber) != SQLITE_OK) {
    Message("\nCould not bind int ACCOUNT_ID in GET_ACCOUNT.\n");
    return nullptr;
  }
  while ((rc = sqlite3_step(_statement)) == SQLITE_ROW) {
    auto firstName = string(reinterpret_cast<const char *>(sqlite3_column_text(_statement, 1)));
    auto lastName = string(reinterpret_cast<const char *>(sqlite3_column_text(_statement, 2)));
    const auto balance = sqlite3_column_int(_statement, 3);
    bankAccount = make_unique<BankAccount>(accountNumber, firstName, lastName, balance);
  }
  sqlite3_finalize(_statement);
  CloseConnection();
  return bankAccount;
}

void BankTransaction::Withdraw(int accountNumber, double amount) {

  unique_ptr<BankAccount> bankAccount = GetAccount(accountNumber);

  if (bankAccount != nullptr) {
    if (bankAccount->GetBalance() < amount)
      Message("Cannot withdraw. Try lower amount.");
    else {
      bankAccount->SetBalance(bankAccount->GetBalance() - amount);

      // TODO you can create other function for Withdraw update Statement
      Deposit(accountNumber, bankAccount->GetBalance());
      string text = "Withdraw successfully: -" + to_string(amount) + " eur\n";
      Message(text + "Your current balance: " + to_string(bankAccount->GetBalance()) + " eur");
    }
  }
}

void BankTransaction::Deposit(int accountNumber, double amount) {

  OpenConnection();
  const char *sql = "UPDATE BANK_ACCOUNT SET BALANCE=? WHERE ACCOUNT_ID=?;";

  if (sqlite3_prepare_v2(_connection, sql, -1, &_statement, nullptr) != SQLITE_OK) {
    Message("\nCould not EXECUTE DEPOSIT STATEMENT.\n");
    return;
  }

  if (sqlite3_bind_double(_statement, 1, amount) != SQLITE_OK) {
    Message("\nCould not bind double BALANCE in DEPOSIT.\n");
    return;
  }
  if (sqlite3_bind_int(_statement, 2, accountNumber) != SQLITE_OK) {
    Message("\nCould not bind int ACCOUNT_ID in DEPOSIT.\n");
    return;
  }
  if (sqlite3_step(_statement) != SQLITE_DONE) {
    Message("\nCould not step (execute) stmt. DEPOSIT!\n");
    return;
  }
  sqlite3_finalize(_statement);

  CloseConnection();
}

void BankTransaction::CreateAccount(unique_ptr<BankAccount> bankAccount) {
  OpenConnection();

  const char *sql = "INSERT INTO BANK_ACCOUNT VALUES (?,?,?,?)";

  if (sqlite3_prepare_v2(_connection, sql, -1, &_statement, nullptr) != SQLITE_OK) {
    Message("\nCould NOT EXECUTE STATEMENT! CREATE_ACCOUNT\n");
    return;
  }

  if (sqlite3_bind_int(_statement, 1, bankAccount->GetAccountNumber()) != SQLITE_OK) {
    Message("\nCould not bind int ACCOUNT_ID in CREATE_ACCOUNT.\n");
    return;
  }

  if (sqlite3_bind_text(_statement, 2, bankAccount->GetFirstName().c_str(),
                        (int) bankAccount->GetFirstName().length(),
                        SQLITE_STATIC) != SQLITE_OK) {
    Message("\nCould not bind text FIRST_NAME in CREATE_ACCOUNT.\n");
    return;
  }

  if (sqlite3_bind_text(_statement, 3, bankAccount->GetLastName().c_str(),
                        (int) bankAccount->GetLastName().length(),
                        SQLITE_STATIC) != SQLITE_OK) {
    Message("\nCould not bind text LAST_NAME in CREATE_ACCOUNT.\n");
    return;
  }

  if (sqlite3_bind_double(_statement, 4, bankAccount->GetBalance()) != SQLITE_OK) {
    Message("\nCould not bind double BALANCE in CREATE_ACCOUNT.\n");
    return;
  }

  if (sqlite3_step(_statement) != SQLITE_DONE) {
    Message("\nCould not step (execute) stmt. CREATE_ACCOUNT!\n");
    return;
  }

  sqlite3_finalize(_statement);
  CloseConnection();
}

void BankTransaction::CloseAccount(int accountNumber) {
  OpenConnection();

  char *zErrMsg = nullptr;
  const char *data = "Callback function called";
  const char *sql = "DELETE FROM BANK_ACCOUNT WHERE ACCOUNT_ID=?";

  if (sqlite3_prepare_v2(_connection, sql, -1, &_statement, nullptr) != SQLITE_OK) {
    Message("\nCould NOT EXECUTE STATEMENT! CLOSE_ACCOUNT\n");
    return;
  }

  if (sqlite3_bind_int(_statement, 1, accountNumber) != SQLITE_OK) {
    Message("\nCould not bind int ACCOUNT_ID in CLOSE_ACCOUNT.\n");
    return;
  }
  int rc = sqlite3_exec(_connection, sql, Callback, (void *) data, &zErrMsg);

  if (sqlite3_step(_statement) != SQLITE_DONE) {
    Message("\nCould not step (execute) stmt. CLOSE_ACCOUNT!\n");
    return;
  }
  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "Account with id: " << accountNumber << "was successfully closed!" << endl;
  }
  sqlite3_finalize(_statement);
  CloseConnection();
}

void BankTransaction::PrintAllAccounts() {
  OpenConnection();
  char *zErrMsg = nullptr;
  const char *data = "Callback function called";

  const char *sql = "SELECT * from BANK_ACCOUNT";

  int rc = sqlite3_exec(_connection, sql, Callback, (void *) data, &zErrMsg);

  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "Operation Display accounts successfully!" << endl;
  }
  CloseConnection();
}

void BankTransaction::CreateDummyTable(const char *dbPath = "BANK.DB") {

  const char *sql;
  char *zErrMsg = nullptr;

  int rc = OpenConnection(dbPath);
  if (rc) {
    cerr << "Can't open " << dbPath <<" database!"<< endl;
  } else {
    cout << "Database" <<dbPath<<"successfully opened!" << endl;
  }

  sql = "CREATE TABLE IF NOT EXISTS BANK_ACCOUNT("  \
        "ACCOUNT_ID INTEGER PRIMARY KEY AUTOINCREMENT," \
        "FIRSTNAME           TEXT    NOT NULL," \
        "LASTNAME            TEXT     NOT NULL," \
        "BALANCE        REAL );";

  rc = sqlite3_exec(_connection, sql, Callback, nullptr, &zErrMsg);

  if (rc != SQLITE_OK) {
    cerr << "SQL error: " << zErrMsg << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "Table BANK_ACCOUNT was created successfully!" << endl;
  }
  CloseConnection();
}

int BankTransaction::Callback(void *context, int columnCount, char **columnValues, char **columnName) {

  for (int i = 0; i < columnCount; i++) {
    cout << columnName[i] << " " << columnValues[i] << " ";
  }
  cout << endl;

  return 0;
}


