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

using namespace std;

int main(){
	Simon s;
	unsigned long x;

	vector<unsigned long> keys;
	vector<unsigned long> plaintext;
	vector<unsigned long> ciphertext;

	keys.push_back(506097522914230528);
	keys.push_back(1084818905618843912);

	cout << "Keys:       " << hex << keys[1] << " ";
	cout << hex << keys[0] << endl;

	plaintext.push_back(7166182912792162917);
	plaintext.push_back(7812731012696405024);

	cout << "Plaintext:  " << hex << plaintext[0] << " ";
	cout << hex << plaintext[1] << endl;

	ciphertext = s.encrypt_public(keys,plaintext);
	
	cout << "Ciphertext: " << hex << ciphertext[0] << " ";
	cout << hex << ciphertext[1] << endl;

	ciphertext = s.decrypt_public(keys,ciphertext);

	cout << "Plaintext:  " << hex << ciphertext[0] << " ";
	cout << hex << ciphertext[1] << endl;
	
	for(int i = 0; i < ciphertext.size(); i++){
		assert(ciphertext[i] == plaintext[i]);
	}

	cout << "Complete!" << endl;

}