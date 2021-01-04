#include "bank_data.h"
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define DEBUG(...)  printf("Line %d Function %s:\n", __LINE__, __FUNCTION__ ); printf(__VA_ARGS__);
using namespace std;

//TODO:: add log_access function that wrap the mutex implementation and specific msg
//TODO:: add mutes protection for different read/write operations

//===================== Account ===================

Account::Account(int id_, int remainder_, int password_): id(id_), remainder(remainder_), password(password_) {
	sem_init(&s_read_acc , 0, 1);
	sem_init(&s_write_acc , 0, 1);
	readers_acc = 0;
}

Account::~Account() {

}

int Account::withdrawal(int amount) {
	if (amount > remainder) {
		return -1;
	} else {
		remainder = remainder - amount;
		return 0;
	}
}



void Account::add_to_balance(int amount) {
	remainder = remainder + amount;
}


int Account::check_password(int pass) {
	if(pass == password) {
		return 0;
	} else {
		return -1;
	}
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


bool Account::operator< (const Account &other) const {
	return (id < other.id);
}

void Account::Access_account(bool is_write) {
	if (is_write) {
		sem_wait(&s_write_acc);
	}
	else {
		sem_wait(&s_read_acc);
		readers_acc++;
		if (readers_acc==1)
			sem_wait(&s_write_acc);
		sem_post(&s_read_acc);
	}
}

void Account::Release_account(bool is_write) {
	if (is_write) {
		sem_post(&s_write_acc);
	}
	else {
		sem_wait(&s_read_acc);
		readers_acc--;
		if (!readers_acc)
			sem_post(&s_write_acc);
		sem_post(&s_read_acc);
	}
}


//================ Bank ====================

Bank::Bank(): log_file("log.txt"), bank_account(0,0,0)  {
	sem_init(&s_readers , 0, 1);
	sem_init(&s_writers , 0, 1);
	pthread_mutex_init(&m_log, NULL);
	readers = 0;
}

Bank::~Bank() {
	sem_destroy(&s_readers);
	sem_destroy(&s_writers);
	pthread_mutex_destroy(&m_log);
}

// DONE: locks
void Bank::take_commision() {
	int com = rand() % 3 + 2; // 2 or 3 or 4

	Access_account_vec(false);

	for (vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		it->Access_account(true);
		int id = it->get_id();
		int remainder = it->get_remainder();
		int bank_profit = round(double(com * double(remainder)) / 100);
		it->withdrawal(bank_profit);
		it->Release_account(true);

		bank_account.Access_account(true);
		bank_account.add_to_balance(bank_profit);
		bank_account.Release_account(true);
        int remainder_after = it->get_remainder();

        log_file << "account: " << id << " remainder before commission: " << remainder << " remainder after commission: " << remainder_after << endl;

        Access_log_file();
		log_file << "Bank comissions of " << com << " % were charged, the bank gained " << bank_profit <<" $ from account " << id << endl;
		Release_log_file();
	}

	Release_account_vec(false);
}

// DONE: locks
void Bank::print_accounts() {

	Access_account_vec(false);

	sort(accounts.begin(), accounts.end());

	printf("\033[2J");
	printf("\033[1;1H");
	cout << "Current Bank Status" << endl;

	for (vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		it->Access_account(false);
		it->print_account();
		it->Release_account(false);

	}

	Release_account_vec(false);

	bank_account.Access_account(false);
	int balance = bank_account.get_remainder();
	bank_account.Release_account(false);

	cout << "The Bank has " << balance << "$" << endl;


}

// DONE: locks
void Bank::add_account(int id, int remainder, int atm_id, int password) {
	vector<Account>::iterator it;
	Access_account_vec(true);
	sleep(1);

	for (it = accounts.begin(); it != accounts.end(); ++it) {
		if (it->get_id() == id) {
			Access_log_file();
			log_file << "Error << "  << atm_id << ": Your  transaction failed - account with the same id exists" << endl;
			Release_log_file();
			Release_account_vec(true);
			return;
		}
	}
	accounts.push_back(Account(id, remainder, password));
	Access_log_file();
	log_file << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << remainder << endl;
	Release_log_file();
	Release_account_vec(true);

}

// DONE: locks
int Bank::withdrawal(int id, int password, int amount, int atm_id) {

	int rc = 0;
	Account acc(0,0,0);

	Access_account_vec(false);
	rc = get_account(id, atm_id, acc);
	if(rc) {
		sleep(1);
		Release_account_vec(false);
		return -1;
	}
	acc.Access_account(true);
	sleep(1);
	rc = acc.check_password(password);
	if (rc) {
		Access_log_file();
		log_file << "Error << "  << atm_id << ": Your  transaction failed - password for account id " << id <<" is incorrect" << endl;
		Release_log_file();
		acc.Release_account(true);
		Release_account_vec(false);
		return -1;
	}

	rc = acc.withdrawal(amount);
	acc.Release_account(true);
	if (rc) {
		Access_log_file();
		log_file << "Error << "  << atm_id << ": Your  transaction failed - account id " << id <<" balance is lower than " << amount << endl;
		Release_log_file();
		acc.Release_account(true);
		Release_account_vec(false);
		return -1;
	}
	int curr_balance = acc.get_remainder();

	acc.Release_account(true);
	Release_account_vec(false);

	Access_log_file();
	log_file << atm_id << ": account " << id <<" new balance is " << curr_balance << " after "<< amount << " $ was withdrew" << endl;
	Release_log_file();

	return 0;
}

// DONE: locks
int Bank::get_account(int id, int atm_id, Account& acc) {
	//No use in access_account_vec on purpose - we will do it in the envlope functions
	for (vector<Account>::iterator it = accounts.begin(); it != accounts.end(); ++it) {
		if (it->get_id() == id) {
			acc = *it;
			return 0;
		}
	}
	Access_log_file();
	log_file << "Error << " << atm_id << ": Your  transaction failed - account id " << id << " does not exist" << endl; // TODO: is it to log or stdout
	Release_log_file();
	return -1;

}

void Bank::remove_account(int id, int atm_id) {
	Account acc(0,0,0);
	vector<Account>::iterator it;
	Access_account_vec(true);
	for (it = accounts.begin(); it != accounts.end(); ++it) {
		if (it->get_id() == id) {
			acc = *it;
			break;
		}
	}
	if (it == accounts.end()) {
		sleep(1);
		Access_log_file();
		log_file << "Error << " << atm_id << ": Your  transaction failed - account id " << id << " does not exist" << endl;
		Release_log_file();
		Release_account_vec(true);
		return;
	}
	it->Access_account(false);
	sleep(1);
	it->Release_account(false);
	accounts.erase(it);
	Release_account_vec(true);
}

void Bank::get_account_balance(int id, int password, int atm_id) {

	Account acc(0,0,0);
	Access_account_vec(false);
	int rc = get_account(id, atm_id, acc);
	if (rc) {
		sleep(1);
		Release_account_vec(false);
		return;
	}
	acc.Access_account(false);
	rc = acc.check_password(password);
	sleep(1);
	if (rc) {
		acc.Release_account(false);
		Release_account_vec(false);
		Access_log_file();
		log_file << "Error << "  << atm_id << ": Your  transaction failed - password for account id " << id <<" is incorrect" << endl;
		Release_log_file();
		return;
	}
	int curr_balance = acc.get_remainder();

	acc.Release_account(false);
	Release_account_vec(false);

	Access_log_file();
	log_file << atm_id << ": Account " << id << " balance is " << curr_balance << endl;
	Release_log_file();
}

// DONE:check if we need to write to log only if the first id doesn't exist or both of them : we need to print once only
//DONE: locks
void Bank::transfer(int src_id, int dst_id, int password, int amount, int atm_id) {
	Account src_account(0,0,0);
	Account dst_account(0,0,0);


	Access_account_vec(false);
	int rc_src = get_account(src_id, atm_id, src_account);
	if (rc_src) {
		sleep(1);
		Release_account_vec(false);
		return;
	}
	int rc_dst = get_account(dst_id, atm_id, dst_account);
	if (rc_dst) {
		sleep(1);
		Release_account_vec(false);
		return;
	}
	Release_account_vec(false);

	if (src_id == dst_id) {
		// same account
		return;
	}

	// locks and sleep in this method already. BUG FIXED
	rc_src = withdrawal(src_id, password, amount, atm_id);

	if (rc_src) {
		return;
	}

	src_account.Access_account(true);
	dst_account.Access_account(true);

	dst_account.add_to_balance(amount);
	int src_balance = src_account.get_remainder();
	int dst_balance = dst_account.get_remainder();

	dst_account.Release_account(true);
	src_account.Release_account(true);


	Access_log_file();
	log_file << atm_id << ": Transfer " << amount << " from account " << src_id << " to account "
			<< dst_id <<" new account balance is " << src_balance << " new target account balance is " << dst_balance << endl;

	Release_log_file();

}

// DONE: locks
void Bank::deposit(int id, int password, int amount, int atm_id) {
	Account acc(0,0,0);
	int rc = get_account(id, atm_id, acc);
	if (rc < 0 ) {
		return;
	}
	acc.Access_account(false);
	rc = acc.check_password(password);
	acc.Release_account(false);
	if (rc) {
		Access_log_file();
		log_file << "Error << "  << atm_id << ": Your  transaction failed - password for account id " << id <<" is incorrect" << endl;
		Release_log_file();
		return;
	}

	acc.Access_account(true);
	int curr_balance = acc.get_remainder();
	acc.add_to_balance(amount);
	acc.Release_account(true);

	Access_log_file();
	log_file << atm_id << ": Account " << id << " new balance is " << curr_balance << " after " << amount << " $ was deposited" << endl;
	Release_log_file();

}

void Bank::Access_account_vec(bool is_write) {
	if (is_write) {
		sem_wait(&s_writers);
	}
	else {
		sem_wait(&s_readers);
		readers++;
		if (readers==1)
			sem_wait(&s_writers);
		sem_post(&s_readers);
	}
}

void Bank::Release_account_vec(bool is_write) {
	if (is_write) {
		sem_post(&s_writers);
	}
	else {
		sem_wait(&s_readers);
		readers--;
		if (readers == 0)
			sem_post(&s_writers);
		sem_post(&s_readers);
	}
}

void Bank::Access_log_file() {
	int rc = pthread_mutex_lock(&m_log);
	if (rc) {
		cerr << "error in m_log lock " << rc << endl;
		exit(-1);
	}
}

void Bank::Release_log_file(){
	int rc = pthread_mutex_unlock(&m_log);

 	if (rc) {
		cerr << "error in m_log unlock " << rc << endl;
		exit(-1);
	}
}
