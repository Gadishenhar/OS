#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <
using namespace std;

int main (int argc, char *argv[]) {

    //TODO - INPUT CHECKS(??)
    int num_atms = atoi(argv[1]);

    pthread_t atm_thr[num_atms];
    int rc = 0;

    //creating threads for the atms:
    for (int i=2; i <= num_atms+1; i++) {
        if ((rc = pthread_create((&atm_thr[i], NULL, atm, (void*)argv[i] ))) {
            fprintf(sderr, "error: pthread_create, rc: %d\n", rc);
        }
    }
    //create a thread to take the comission for the bank:
    pthread_t comm_thr;
    if ((rc = pthread_create((&comm_thr, NULL, bank.take_commision, NULL))) {
        fprintf(sderr, "error: pthread_create, rc: %d\n", rc);
    }

    //creating a thread for the prints:
    pthread_t prt_thr;
    if ((rc = pthread_create((&prt_thr, NULL, bank.print_accounts, NULL))) {
        fprintf(sderr, "error: pthread_create, rc: %d\n", rc);
    }

    //Joining all the threads we have created
    for (int i=0; i < num_atms; i++) {
            pthread_join(atm_thr[i], NULL);
        }
    pthread_join(comm_thr[i], NULL);
    pthread_join(prt_thr[i], NULL);
    }

}
