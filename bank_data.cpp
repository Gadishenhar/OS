#include "bank_data.h"
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

//TODO:: add log_access function that wrap the mutex implementation and specific msg
//TODO:: add mutes protection for different read/write operations

//===================== Account ===================

Account::Account(int id_, int remainder_, int password_): id(id_), remainder(remainder_), password(password_) {

}

Account::~Account() {

}

int Account::withdrawal(int amount) {
	if (amount > remainder) {
		return 1;
	} else {
		remainder -= amount;
		return 0;
	}
}

int Account::take_commision(int commision_perc) {

	int bank_profit = (commision_perc * remainder) / 100;

	remainder -= bank_profit;

	return bank_profit;
}

void Account::add_to_balance(int amount) {
	remainder += amount;
}


bool Account::check_password(int pass) {
	return (pass == password);
}
 int Account::get_id() const {
	return id;
}

int Account::get_remainder() const {
	return remainder;
}

int Account::get_password() const {
	return password;
}

void Account::print_account() const {
	cout << "Account " << id <<": Balance - " << remainder << " $ , Account Password - " << password << endl;
}

bool Account::operator==(const Account& lhs, const Account& rhs) {
	return (id == rhs.id);
}

bool Account::operator< (const Account &other) const {
	return (id < other.id);
}



//================ Bank ====================

Bank::Bank(): log_file("log.txt"), bank_account(0,0,0)  {

}

Bank::~Bank() {

}

int Bank::get_balance() {
	return bank_account.get_remainder();
}


void Bank::take_commision() {
	int com = rand() % 3 + 2; // 2 or 3 or 4
	while(true) {
		sleep(3);
		for (vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
			int profit  = *it->take_commision(com);
			int password = *it->get_password();
			int id = *it->get_id();
			bank_account.add_to_balance(profit);

			log_file << "Bank comissions of " << com << " % were charged, the bank gained " << profit <<" $ from account " << id << endl;
		}
	}
}

void Bank::print_accounts() {
	while(true) {
		sleep(0.5);
		sort(accounts.begin(), accounts.end());

		cout << "Current Bank Status" << endl;

		for (vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
			*it->print_account();
		}
		int balance = get_balance();
		cout << "The Bank has " << balance << "$" << endl;

	}
}

//TODO:: should return 1 on fail?
void Bank::add_account(int id, int remainder, int atm_id, int password) {

	if (find(accounts.begin(), accounts.end(), id) != accounts.end()) {
		accounts.push_back(Account(id, remainder, password));
		log_file << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << remainder << endl;

	} else {
		log_file << "Error << "  << atm_id << ": Your  transaction failed - account with the same id exists" << endl;
	}
}

//TODO:: should return 1 on fail?
void Bank::withdrawal(int id, int password, int amount, int atm_id) {
	vector<Account>:: iterator it = find_if(accounts.begin(), accounts.end(), [](const Account& account) { return account.get_id() == id; });

	if (it == accounts.end()) {
		log_file << "Error << "  << atm_id << ": Your  transaction failed - account id " << id <<" does not exist" << endl; // TODO: is it to log or stdout
	}
	if (*it->check_password(password)) {
		log_file << "Error << "  << atm_id << ": Your  transaction failed - password for account id " << id <<" is incorrect" << endl;
		return;
	}
	if (*it->withdrawal(amount)) {
		log_file << "Error << "  << atm_id << ": Your  transaction failed - account id " << id <<" balance is lower than " << amount << endl;
		return;
	}
	int curr_balance = *it->get_remainder();

	log_file << atm_id << "account " << id <<" new balance is " << curr_balance << "after "<< amount << " $ was withdrew" << endl;
}

void Bank::get_account(int id, int atm_id) {
	vector<Account>::iterator it = find_if(accounts.begin(), accounts.end(), [](const Account& account) { return account.get_id() == id; });
	if (it == accounts.end()) {
		log_file << "Error << " << atm_id << ": Your  transaction failed - account id " << id << " does not exist" << endl; // TODO: is it to log or stdout
		return nullptr;
	}
	return it;
}

void Bank::remove_account(int id, int atm_id) {
	Account curr_account = bank->get_account(id, atm_id);
	if (it != nullptr) {
		int curr_balance = *curr_account->get_remainder();
		accounts.erase(curr_account);
		logfile << "Error " << atm_id << ": Account " << id << " is now closed. Balance was " << curr_balance << endl;
	}
}

void Bank::deposit(int id, int amount, int atm_id) {
	Account curr_account = bank->get_account(id, atm_id);
	curr_account->add_to_balance(amount);
	int curr_balance = *curr_account->get_remainder();

	log_file << atm_id << ": Account " << id << " new balance is " << curr_balance << " after " << amount << " $ was deposited" << endl;
}

void Bank::check_account_balance(int id, int atm_id) {
	Account curr_account = bank->get_account(id, atm_id);
	int curr_balance = *curr_account->get_remainder();
	log_file << atm_id << ": Account " << id << " balance is " << curr_balance << endl;
}

void Bank::check_account_pass(int id, int password, int atm_id) {
	Account curr_account = bank->get_account(id, atm_id);
	if !(*curr_account->check_password(password)) {
		log_file << "Error << " << atm_id << ": Your  transaction failed - password for account id " << id << " is incorrect" << endl;
		return false;
	}
	else {
		return true;
	}
}