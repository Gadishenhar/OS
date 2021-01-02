#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <fstream>
#include <unistd.h>

#include "bank_data.h"
#include "atm.h"

using namespace std;
pthread_mutex_t log_file_mutex;
pthread_mutex_t atm_cnt;
ofstream log_file;

Bank* bank;

bool g_exit_prog = false;
pthread_mutex_t g_exit_mutex;

void* take_commision(void* ctx) {

	bool exit_prog = false;
	while (!exit_prog) {
		bank->take_commision();
		sleep(3);
		pthread_mutex_lock(&g_exit_mutex);
		exit_prog = g_exit_prog;
		pthread_mutex_unlock(&g_exit_mutex);
	}
	pthread_exit(NULL);
}

void* print_accounts(void* ctx) {
	bool exit_prog = false;
	while (!exit_prog) {
		bank->print_accounts();
		usleep(500 * 1000);

		pthread_mutex_lock(&g_exit_mutex);
		exit_prog = g_exit_prog;
		pthread_mutex_unlock(&g_exit_mutex);
	}

    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {

    //Open a log file and initialize a mutex for it
    pthread_mutex_init(&log_file_mutex, NULL);

	pthread_mutex_init(&g_exit_mutex, NULL);



	bank = new Bank();

    //Initialize the counter of the ATMs and initialize a mutex for it
	pthread_mutex_init(&atm_cnt,NULL);

    //TODO - INPUT CHECKS(??)
    int num_atms = atoi(argv[1]);

    pthread_t *atm_thr = new pthread_t[num_atms];
    atm_ctx_t *atm_ctx = new atm_ctx_t[num_atms];
    int rc = 0;

    //creating threads for the atms:
    for (int i = 0; i < num_atms; i++) {
    	atm_ctx[i].atm_id = i;
    	atm_ctx[i].filename = argv[i + 2];
        if ( (rc = pthread_create(&atm_thr[i], NULL, atm, (void*)&atm_ctx[i] ))) {  //i+2 because we want the arguments of the input
            cerr << "error: pthread_create, rc: " << rc << endl;
            exit(-1);
        }
    }
    //create a thread to take the comission for the bank:
    pthread_t comm_thr;
    if ( (rc = pthread_create(&comm_thr, NULL, take_commision, NULL) ) ) {
        cerr << "error: pthread_create, rc: " << rc << endl;
        exit(-1);
    }

    //creating a thread for the prints:
    pthread_t prt_thr;
    if ( (rc = pthread_create(&prt_thr, NULL, print_accounts, NULL) ) ) {
        cerr << "error: pthread_create, rc: " << rc << endl;
        exit(-1);
    }

    pthread_join(prt_thr, NULL);

    pthread_join(comm_thr, NULL);
    //Joining all the threads we have created
    for (int i=0; i < num_atms; i++) {
    	pthread_join(atm_thr[i], NULL);
    }

	pthread_mutex_lock(&g_exit_mutex);
	g_exit_prog = true;
	pthread_mutex_unlock(&g_exit_mutex);

    delete[] atm_ctx;
    delete[] atm_thr;
    delete[] bank;
    return 0;
}
