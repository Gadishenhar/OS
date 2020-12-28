#include <bank_data.h>
#include <algorithm>

using namespace std;

//===================== Account ===================
Account::Account(int id_, int remainder_): id(id_), remainder(remainder_) {

}

Account::~Account() {

}

bool Account::operator==(const Account& lhs, const Account& rhs) {
	return (lhs.id == rhs.id);
}



//================ Bank ====================
Bank::Bank() {

}
Bank::~Bank() {

}

void Bank::take_commision() {

}

void Bank::add_account(int id, int remainder, int atm_id) {

	if(find(accounts.begin(), accounts.end(), id) != accounts.end()) {
		accounts.push_back(Account(id, remainder));
	} else {
		cerr << "Error << "  << atm_id <<
	}

}





