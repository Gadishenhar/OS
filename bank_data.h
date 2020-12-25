#include <iostream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;


class Account {
	public:

		/**
		 * @brief create an Account
		 */
		Account();
		/**
		 * @brief destroy an Account
		 */
		~Account();

};

class Bank {

	int bank_commision;
	Account bank_account;
	vector <Account> my_vec;

	public:

		/**
		 * @brief create the bank
		 */
		Bank();

		/**
		 * @brief destroy the bank
		 */
		~Bank();



};
