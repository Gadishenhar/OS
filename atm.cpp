#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "atm.h"
#include "bank_data.h"
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <iterator>

using namespace std;

#define CHECK_RC(rc) if (rc) { goto done;}

#define MAX_ARGS 5




void* atm (void* ctx)
{
	string cmd;
	


	atm_ctx_t ctx_atm = *((atm_ctx_t*)ctx);
	ifstream atm_file(ctx_atm.filename);
	int atm_id = ctx_atm.atm_id;

	while (getline(atm_file, cmd)) {

		std::istringstream ss(cmd);
		std::istream_iterator<std::string> begin(ss), end;
		std::vector<std::string> args(begin, end);

		int id = atoi(args[1].c_str());
		int password = atoi(args[2].c_str());

		if (args[0] == "O") {
			int amount = atoi(args[3].c_str());
			bank->add_account(id, amount, atm_id, password);
		} else if (args[0] == "D") {
			int amount = atoi(args[3].c_str());
			bank->deposit(id, password, amount, atm_id);
		} else if (args[0] == "W") {
			int amount = atoi(args[3].c_str());
			bank->withdrawal(id, password, amount, atm_id, false);
		} else if (args[0] == "B") {
			bank->get_account_balance(id, password, atm_id);
		} else if (args[0] == "Q") {
			bank->remove_account(id, password, atm_id);
		} else if (args[0] == "T") {
			int dst_id = atoi(args[3].c_str());
			int amount = atoi(args[4].c_str());
			bank->transfer(id, dst_id, password, amount, atm_id);
		} else {
			cerr << "Invalid operation" << endl;
		}

		usleep(100 * 1000);
	}

	pthread_exit(NULL);

}

