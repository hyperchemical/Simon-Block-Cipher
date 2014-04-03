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
	uint64_t x;

	vector<uint64_t> keys(4);
	vector<uint64_t> plaintext(2);
	vector<uint64_t> ciphertext(2);

	//Using Simon 128/256 Example Vectors

	keys[3] = 2242261671028070680;
	keys[2] = 1663540288323457296;
	keys[1] = 1084818905618843912;
	keys[0] = 506097522914230528;

	cout << "Keys:       ";
	for(int i = keys.size()-1; i >=0; i--){
		cout << hex << keys[i] << " ";
	}
	cout << endl;

	plaintext[0] = 8367809505449045871;
	plaintext[1] = 7883959205594428265;

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