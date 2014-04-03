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



void Simon::encrypt_message(string input){
	read_keys();	
	key_expansion();
	x_y = split_message(input.c_str(), block_size);

	assert(x_y.size()%2 == 0);

	cout << "Orig: ";
	print_all(true);
	cout << endl;

	cout << "Orig Num: ";
	print_all(false);
	cout << endl;

	for(int i = 0; i < x_y.size(); i+=2){
		encrypt(x_y[i], x_y[i+1]);
	}

	cout << "Encr: ";
	print_all(true);
	cout << endl;

	cout << "Encr Num: ";
	print_all(false);
	cout << endl;

}

vector<uint64_t> Simon::encrypt_public(vector<uint64_t> key, vector<uint64_t> plaintext){
	assert(key.size() >= 2 && key.size() <= 4);
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
	assert(key.size() >= 2 && key.size() <= 4);
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

void Simon::decrypt_latest_message(){
	for(int i = 0; i < x_y.size(); i+=2){
		decrypt(x_y[i], x_y[i+1]);
	}

	cout << "Decr Num: ";
	print_all(false);
	cout << endl;

	cout << "Decr: ";
 	print_all(true);
 	cout << endl;
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


void Simon::init(){
	read_keys();
	key_expansion();
	print_keywords_to_file();
	vector<uint64_t> yo = split_message("tst message", block_size);
	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
		mpz_class tmp;
		tmp = yo[i];
		cout << mpz_to_string(tmp) << endl;
	}
	
	cout << "Encrypt:\n======\n";
	encrypt(yo[0], yo[1]);

	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
	}

	cout << "Decrypt:\n======\n";
	decrypt(yo[0], yo[1]);

	for (int i = 0; i < yo.size(); ++i)
	{
		cout << yo[i] << endl;
		mpz_class tmp;
		tmp = yo[i];
		cout << mpz_to_string(tmp) << endl;
	}
}

void Simon::generate_keys(){
	ofstream myfile;
	myfile.open ("private_key.txt");

	gmp_randstate_t r_state;
	mpz_class key;
	gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, time(NULL));
    for(int i = 0; i < z_m; i++){
	    mpz_urandomb(key.get_mpz_t(), r_state, key_size);
	    mpz_setbit(key.get_mpz_t(), key_size-1);
		myfile << key << endl;
	}

	myfile.close();
}

void Simon::read_keys(){
	keywords.clear();
	string line;
	ifstream myfile ("private_key.txt");
	for(int i = 0; i < z_m; i++){
		getline(myfile,line);
		mpz_class key;
		key = line;
		keywords.push_back(mpz_get_ui(key.get_mpz_t()));
	}

	myfile.close();
}



void Simon::print_keywords_to_file(){
	ofstream myfile;
	myfile.open ("keywords.txt");

    for(auto i : keywords){
		myfile << i << endl;
	}

	myfile.close();

}

