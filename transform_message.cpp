#include <string>
#include <stdlib.h>
#include <iostream>
#include <gmpxx.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdexcept> 
#include <cstdio>

using namespace std;

#define BIT_MOD 8
#define BIT_MOD_2 3

mpz_class string_to_mpz(const char* message){
	mpz_class output;
	output = "0";
	string input(message);
	for(int i = 0; i < input.size(); i++){
		output *= 256;
		int ascii = input[i];
		output += ascii;
	}
	return output;
}

string mpz_to_string(const mpz_class& mpz_input){
	mpz_class numchars, character, size, bit;
	character = "0";
	size = mpz_sizeinbase(mpz_input.get_mpz_t(), 2);
	mpz_cdiv_q_2exp(numchars.get_mpz_t(), size.get_mpz_t(), BIT_MOD_2);
	string output(mpz_get_ui(numchars.get_mpz_t()), 'X');
	for(; character < numchars; character = character+1){
		int cur_char = 0;
		//for(bit = character*BIT_MOD;  bit < (character+1)*BIT_MOD; bit = bit+1){
		for(bit = (character+1)*BIT_MOD-1;  bit >= character*BIT_MOD; bit = bit-1){
			cur_char*=2;
			cur_char += mpz_tstbit(mpz_input.get_mpz_t(), mpz_get_ui(bit.get_mpz_t()));
		}
		char ch = cur_char;
		output[mpz_get_ui(numchars.get_mpz_t())- 1 - mpz_get_ui(character.get_mpz_t())] 
			= cur_char;
	}
	return output;

}

vector<unsigned long> split_message(const char* message, int bit_length){
	if(bit_length% BIT_MOD != 0) {
		printf("ERROR: %s:%d - Bit length %d not a multiple of %d\n", 
				__FILE__, __LINE__, bit_length, BIT_MOD);
		exit(1);
	}
	string input(message);
	mpz_class size, chars_per_entry;
	chars_per_entry = bit_length;
	chars_per_entry = chars_per_entry / BIT_MOD;
	size = input.size();
	mpz_cdiv_q(size.get_mpz_t(), size.get_mpz_t(), chars_per_entry.get_mpz_t());
	vector<unsigned long> message_vec(mpz_get_ui(size.get_mpz_t()));
	//cout << message_vec.size() << endl;
	for(int i = 0; i < message_vec.size(); i++){
		message_vec[i] = 
			mpz_get_ui(string_to_mpz(input.substr(i*mpz_get_ui(chars_per_entry.get_mpz_t())
					, mpz_get_ui(chars_per_entry.get_mpz_t())).c_str()).get_mpz_t());
	}


	if(message_vec.size() == 0){
		printf("ERROR: %s:%d - Zero size vector\n", 
				__FILE__, __LINE__);
		exit(1);
	}
	if(message_vec.size() % 2 != 0){
		message_vec.push_back(mpz_get_ui(string_to_mpz("    ").get_mpz_t()));

	}



	return message_vec;
}

//|12345678|