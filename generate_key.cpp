#include <string>
#include <stdlib.h>
#include <iostream>
#include <gmpxx.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdexcept> 
#include <cstdio>
#include <fstream>
#include <ctime>
#include <cassert>

using namespace std;

int main(int argc, char const *argv[]){
	ofstream myfile;
	myfile.open ("private_key.txt");

	assert(argc == 2);
	int key_size = atoi(argv[1]);
	gmp_randstate_t r_state;
	mpz_class key;
	gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, time(NULL));
    mpz_urandomb(key.get_mpz_t(), r_state, key_size);
    mpz_setbit(key.get_mpz_t(), key_size-1);
	myfile << key << endl;
	
	myfile.close();
}