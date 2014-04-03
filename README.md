#EECS475P5-Simon
Implementation of [SIMON128/256](https://eprint.iacr.org/2013/404.pdf) in C++11, gcc-4.7.0 with libgmp

##Usage
To use, include `simon.h` and have `simon.cpp` and `simon.h` in the same directory

###Encryption Usage

There are two public methods availiable:

	vector<uint64_t> encrypt_public(vector<uint64_t> key, vector<uint64_t> plaintext);
	vector<uint64_t> decrypt_public(vector<uint64_t> key, vector<uint64_t> ciphertext);

Both functions return a vector that is the size of `plaintext` or `ciphertext`.

The vector `key` must be of size 4.
The vector `plaintext` must a multiple of 2 (recommended size 2).

### Compilation 

To compile with your own app.cc:

	g++ -lgmpxx -lgmp -std=c++11 -O3 simon.cpp app.cc -o app

### Example

`testing.cpp` is an example run of the encryption/decryption with the test vectors for SIMON128/256 provided in the [paper](https://eprint.iacr.org/2013/404.pdf).
	

