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
#include "simon.h"
#include "transform_message.cpp"

using namespace std;

void print_binary(uint64_t num){
	for(int i = 63; i >= 0; i--){
		cout << ((num >> i) & 1);
	}
}

uint64_t circular_left_shift(uint64_t num, uint64_t shift, uint64_t size){
	assert(shift > 0 && shift < size);
	return (num << shift) | (num >> (size - shift));
}

uint64_t circular_right_shift(uint64_t num, uint64_t shift, uint64_t size){
	assert(shift > 0 && shift < size);
	return (num >> shift) | (num << (size - shift));
}

vector<uint64_t> Simon::encrypt_public(vector<uint64_t> key, vector<uint64_t> plaintext){
	assert(key.size() == 4);
	assert(plaintext.size() % 2 == 0);

	//keywords.resize(key.size());
	keywords = key;
	key_expansion();

	for(int i = 0; i < plaintext.size(); i+=2){
		encrypt(plaintext[i], plaintext[i+1]);
	}

	return plaintext;
}	

vector<uint64_t> Simon::decrypt_public(vector<uint64_t> key, vector<uint64_t> ciphertext){
	assert(key.size() == 4);
	assert(ciphertext.size() % 2 == 0);

	//keywords.resize(key.size());
	keywords = key;
	key_expansion();

	for(int i = 0; i < ciphertext.size(); i+=2){
		decrypt(ciphertext[i], ciphertext[i+1]);
	}

	return ciphertext;
}

void Simon::key_expansion(){
	assert(keywords.size() >= 2 && keywords.size() <= 4);
	keywords.resize(rounds);
	for(int i = z_m; i < rounds; i++){
		uint64_t tmp;
		tmp = circular_right_shift(keywords[i-1], 3, word_size);
		if(z_m == 4){
			tmp = keywords[i-3] ^ tmp;

		}
		tmp = tmp ^ circular_right_shift(tmp, 1, word_size);
		keywords[i] = (~keywords[i-z_m]) ^ tmp ^ z[z_m][(i-z_m) % 62] ^ 3;
	}
}

void Simon::encrypt(uint64_t& x, uint64_t& y){
	cout << "@@@Encryption phase...\n";
	for(int i = 0; i < rounds; i++){
		
		uint64_t tmp = x;
		x = y ^ 
			(circular_left_shift(x, 1, word_size) & circular_left_shift(x, 8, word_size)) ^ 
			circular_left_shift(x, 2, word_size) ^ keywords[i];
		y = tmp;
	}	

}

void Simon::decrypt(uint64_t &x, uint64_t &y){
	cout << "@@@Decryption phase...\n";
	for(int i = rounds-1; i >= 0; i--){
		uint64_t tmp = y;
		y = x ^ (circular_left_shift(y, 1, word_size) & circular_left_shift(y, 8, word_size)) ^ 
				circular_left_shift(y, 2, word_size) ^ keywords[i];
		x = tmp;
	}	
}

void Simon::print_all(bool str){
	for(auto i : x_y){
		if(str)
			print_long(i);
		else
			cout << i;
	}
}

void Simon::print_long(uint64_t input){
	mpz_class tmp;
	tmp = input;
	cout << mpz_to_string(tmp);
}
