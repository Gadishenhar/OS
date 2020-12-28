#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


class Account {
	int id;
	int remainder;
	int password

	public:

		/**
		 * @brief create an Account
		 */
		Account(int id_, int remainder_, int password_);
		/**
		 * @brief destroy an Account
		 */
		~Account();

		bool operator==(const Account& lhs, const Account& rhs);

};

class Bank {

	Account bank_account;
	vector <Account> accounts;

	public:

		/**
		 * @brief create the bank
		 */
		Bank();

		/**
		 * @brief destroy the bank
		 */
		~Bank();

		/**
		 * @brief take commision from all accounts of the bank
		 */
		void take_commision(void);

		/**
		 * @brief add account to the bank
		 */
		void add_account(int id, int remainder, int password, int atm_id);



};
