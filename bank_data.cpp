#include <bank_data.h>
#include <algorithm>


using namespace std;

//===================== Account ===================
Account::Account(int id_, int remainder_, int password_): id(id_), remainder(remainder_), password(password_) {

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

void Bank::add_account(int id, int remainder, int atm_id, int password) {

	if (find(accounts.begin(), accounts.end(), id) != accounts.end()) {
		accounts.push_back(Account(id, remainder));

	} else {
		log_file << "Error << "  << atm_id << ": Your  transaction failed - account with the same id exists" << endl;
	}

}





