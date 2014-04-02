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
	//s.init();
	// s.encrypt_message("Test long message 1 2 3 4 5");
	// s.decrypt_latest_message();

	vector<unsigned long> keys;
	vector<unsigned long> plaintext;
	vector<unsigned long> ciphertext;

	keys.push_back(1084818905618843912);
	keys.push_back(506097522914230528);

	plaintext.push_back(7166182912792162917);
	plaintext.push_back(7812731012696405024);

	ciphertext = s.encrypt_public(keys,plaintext);
	
	for(int i = 0; i < ciphertext.size(); i++){
		cout << hex << ciphertext[i] << " ";
	}
	cout << endl;

	ciphertext = s.decrypt_public(keys,ciphertext);

	for(int i = 0; i < ciphertext.size(); i++){
		assert(ciphertext[i] == plaintext[i]);
	}

	cout << "Asserted!" << endl;

}