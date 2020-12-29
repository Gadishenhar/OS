#ifndef BANK_DATA
#define BANK_DATA


#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


class Account {
	int id;
	int remainder;
	int password;

	public:

		/**
		 * @brief create an Account
		 */
		Account(int id_, int remainder_, int password_);
		/**
		 * @brief destroy an Account
		 */
		~Account();

		/**
		 * @brief destroy an Account
		 * returns 1 if amount is too big
		 */
		int withdrawal(int amount);

		/**
		 * @brief destroy an Account
		 * returns 1 if incorrect password
		 */

		bool check_password(int pass);

		int take_commision(int commision_perc);

		void add_to_balance(int amount);

		int get_id() const;

		int get_remainder() const;

		int get_password() const;

		void print_account() const;

		bool operator==(const Account& rhs);

		bool operator< (const Account &other) const;

};

class Bank {

	ofstream log_file;
	Account bank_account;
	vector <Account> accounts;


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
		 * @brief balance of bank account
		 */
		int get_balance();

		/**
		 * @brief take commision from all accounts of the bank
		 */
		void take_commision();

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
		void withdrawal(int id, int password, int amount, int atm_id);

};

#endif
