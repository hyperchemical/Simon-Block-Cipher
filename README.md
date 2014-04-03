#EECS475P5-Simon
Implementation of SIMON128/256 in C++

##Usage
To use, include `simon.h` and have `simon.cpp` and `simon.h` in the same directory

###Encryption Usage

There are two public methods availiable:

	vector<uint64_t> encrypt_public(vector<uint64_t> key, vector<uint64_t> plaintext);
	vector<uint64_t> decrypt_public(vector<uint64_t> key, vector<uint64_t> ciphertext);

The vector key must be of size 4.
The vector plaintext must be of size 2.




	

