#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include "atm.h"
#include "bank_data.h"
#include <pthread.h>

using namespace std;

#define CHECK_RC(rc, ...) if (rc) {printf(__VA_ARGS__); goto done;}

#define MAX_ARGS 5

static int check_input(string cmd, int num_arg, char* args[]) {
	if (cmd == "O") {
		if (num_arg == 3) {
			return 0;
		} else {
			cerr << "invalid num of arguments" << endl;
			return 1;
		}
	} else if(cmd == "D") {
		if (num_arg == 3) {
			return 0;
		} else {
			cerr << "invalid num of arguments" << endl;
			return 1;
		}

	} else if (cmd == "W") {
		if (num_arg == 3) {
			return 0;
		} else {
			cerr << "invalid num of arguments" << endl;
			return 1;
		}
	} else if (cmd == "B") {
		if (num_arg == 2) {
			return 0;
		} else {
			cerr << "invalid num of arguments" << endl;
			return 1;
		}

	} else if (cmd == "Q") {
		if (num_arg == 2) {
			return 0;
		} else {
		cerr << "invalid num of arguments" << endl;
		return 1;
	}

	} else if (cmd == "T") {
		if (num_arg == 4) {
			return 0;
		} else {
			cerr << "invalid num of arguments" << endl;
			return 1;
		}
	}
	return 0;
}


void* atm (void* ctx)
{
	string cmd;
	char* args[MAX_ARGS];
	char* linesize;
	int rc = 0;


	atm_ctx_t ctx_atm = (atm_ctx_t)ctx;
	ifstream atm_file(ctx_atm.filename);
	int atm_id = ctx_atm.atm_id;

	while (getline(atm_file, cmd)) {
		const char* delimiters = " \t\n";
		int i = 0, num_arg = 0;

		linesize = new char [cmd.length() + 1];
		strcpy(linesize, cmd.c_str());
		cmd = strtok(linesize, delimiters);
		if (linesize == NULL)
			return 0;
		for (i = 0; i < MAX_ARGS; i++)
		{
			args[i] = strtok(NULL, delimiters);
			if (args[i] != NULL) {
				num_arg++;
			}
		}

		rc = check_input(cmd, num_arg, args);
		int id = atoi(args[0]);
		int password = atoi(args[1]);

		if (cmd == "O") {
			int amount = atoi(args[2]);
			bank.add_account(id, amount, password, atm_id)
		} else if (cmd == "D") {
			int amount = atoi(args[2]);
			bank.deposit(id, password, amount, atm_id);
		} else if (cmd == "W") {
			int amount = atoi(args[2]);
			bank.withdrawal(id, password, amount, atm_id);
		} else if (cmd == "B") {
			bank.check_account_balance(id, password, atm_id);
		} else if (cmd == "Q") {
			bank.remove_account(id, atm_id);
		} else if (cmd == "T") {
			int dst_id = atoi(args[2]);
			int amount = atoi(args[3]);
			bank.transfer(id, dst_id, password, amount, atm_id);
		} else {
			cerr << "Invalid operation" << endl;
		}
	}

done:
		delete[] linesize;
		pthread_exit(NULL);

}

