#include <iostream>
#include <string>
#include <string.h>
#include "atm.h"

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


void * atm (void*)
{
	string cmd;
	char* args[MAX_ARGS];
	char* linesize;
	int rc = 0;

	while (getline(cin, cmd)) {
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

		if (cmd == "O") {
			int id = atoi(args[0]);
			int password = atoi(args[1]);
			int amount = atoi(args[2]);
			// TODO: pass atm_id from bank.cpp properly
			//TODO: add mutex protection here or inside
			bank->add_account(id, amount, password, atm_id);
		} else if(cmd == "D") {

		} else if (cmd == "W") {

		} else if (cmd == "B") {

		} else if (cmd == "Q") {

		} else if (cmd == "T") {

		} else {

			cerr << "Invalid operation" << endl;
		}
done:
		delete[] linesize;
	}


	return 0;
}

