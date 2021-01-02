#ifndef MY_ATM
#define MY_ATM
#include "bank_data.h"

void* atm (void *);

extern Bank* bank;


typedef struct atm_ctx {
	int atm_id;
	char* filename;
}atm_ctx_t;

#endif


