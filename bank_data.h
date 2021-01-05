#ifndef BANK_DATA
#define BANK_DATA


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <semaphore.h>
#include <mutex>
#include <fstream>
#include <condition_variable>

using namespace std;

class Account {
	int id;
	int remainder;
	int password;
	sem_t s_read_acc;
	sem_t s_write_acc;
	int readers_acc;

	public:

		/**
		 * @brief create an Account
		 */
		Account(int id_, int remainder_, int password_);
		/**
		 * @brief create an Account with 0 inputs
		 */
		Account();
		/**
		 * @brief destroy an Account
		 */
		~Account();

		/**
		 * @brief take money from account and
		 * returns 1 if amount is too big
		 */
		int withdrawal(int amount);

		/**
		 * @brief check if pass is the correct password of this account
		 * returns 1 if incorrect password
		 */
		int check_password(int pass);

		/**
		 * @brief destroy an Account
		 * returns 1 if incorrect password
		 */
		void add_to_balance(int amount);

		/**
		 * @brief destroy an Account
		 * returns 1 if incorrect password
		 */
		void Access_account(bool is_write);

		/**
		 * @brief destroy an Account
		 * returns 1 if incorrect password
		 */
		void Release_account(bool is_write);

		/**
		 * @brief destroy an Account
		 * returns 1 if incorrect password
		 */
		void print_account() const;


// ============ getters =====================

		int get_id() const;

		int get_remainder() const;

		int get_password() const;

// ============= operators overloading =======


		bool operator< (const Account &other) const;

};

class Bank {

	ofstream log_file;
	Account bank_account;
	vector <Account> accounts;

	sem_t s_readers;
	sem_t s_writers;
	pthread_mutex_t m_log;
	int readers;



	public:

		/**
		 * @brief constructor create the bank
		 */
		Bank();

		/**
		 * @brief destructor destroy the bank
		 */
		~Bank();

		/**
		 * @brief take commision from all accounts of the bank
		 */
		void take_commision(void);

		/**
		 * @brief prints all the accounts sorted by account number every half second
		 */
		void print_accounts(void);

		/**
		 * @brief add account to the bank
		 */
		void add_account(int id, int remainder, int password, int atm_id);

		/**
		 * @brief withdrawl money from account with given id password and a given amount
		 */
		int withdrawal(int id, int password, int amount, int atm_id, bool is_transfer);

		/**
		 * @brief get account object from the bank accounts by id
		 * returns 1 for failure if account doesn't exist and writes to log
		 * Note: mutex handling is inside for access to account list
		 */
		Account* get_account(int id, int atm_id);

		// TODO: do we need it?
		void remove_account(int id, int password, int atm_id);

		/**
		 * @brief add to the balance of a specific account in the account list
		 */
		void deposit(int id, int password, int amount, int atm_id);

		/**
		 * @brief transfer from one account to another
		 */
		void transfer(int src_id, int dst_id, int password, int amount, int atm_id);

		/**
		 * @brief doing semaphore handling according to readers/writers algorithm before critical section.
		 * @params bool if true we handle the way we handled writers else its readers.
		 */
		void Access_account_vec(bool is_write);

		/**
		 * @brief doing semaphore handling according to readers/writers on the accounts vector after crit section
		 * @params bool if true we handle the way we handled writers else its readers.
		 */
		void Release_account_vec(bool is_write);

		/**
		 * @brief catch lock of log file
		 */
		void Access_log_file();

		/**
		 * @brief release lock of log file
		 */
		void Release_log_file();

		/**
		 * @brief return remainder of account
		 */
		void get_account_balance(int id, int password, int atm_id);


};

#endif
