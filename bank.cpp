#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <bank_data.h>
#include <atm.h>
#include <fstream>

using namespace std;
pthread_mutex_t log_file_mutex;
pthread_mutex_t atm_cnt;
ofstream log_file;
int atm_counter;

int main (int argc, char *argv[]) {

    //Open a log file and initialize a mutex for it
    pthread_mutex_init(&log_file_mutex,NULL);
	log_file.open("log.txt");

    Bank* bank = new Bank();

    //Initialize the counter of the ATMs and initialize a mutex for it
	pthread_mutex_init(&atm_cnt,NULL);
	atm_counter = 0;

    //TODO - INPUT CHECKS(??)
    int num_atms = atoi(argv[1]);

    pthread_t atm_thr[num_atms];
    int rc = 0;

    int atm_ids[num_atms];
    //creating threads for the atms:
    for (int i = 0; i < num_atms; i++) {
    	atm_ids[i] = i;
        if (rc = pthread_create(&atm_thr[i], NULL, atm, (void*)argv[i + 2] )) {  //i+2 because we want the arguments of the input
            cerr << "error: pthread_create, rc: " << rc;
        }
    }
    //create a thread to take the comission for the bank:
    pthread_t comm_thr;
    if (rc = pthread_create(&comm_thr, NULL, bank.take_commision, NULL)) {
        cerr << "error: pthread_create, rc: " << rc;
    }

    //creating a thread for the prints:
    pthread_t prt_thr;
    if (rc = pthread_create(&prt_thr, NULL, bank.print_accounts, NULL)) {
        cerr << "error: pthread_create, rc: " << rc;
    }

    //Joining all the threads we have created
    for (int i=0; i < num_atms; i++) {
            pthread_join(atm_thr[i], NULL);
        }
    pthread_join(comm_thr, NULL);
    pthread_join(prt_thr, NULL);

    log_file.close();
    pthread_mutex_destroy(&log_file_mutex);
	pthread_mutex_destroy(&atm_cnt);
    delete bank;
    return 0;
}
