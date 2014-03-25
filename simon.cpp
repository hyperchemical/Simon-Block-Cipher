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
#include "transform_message.cpp"

using namespace std;

//mpz_xor (mpz_t rop, const mpz_t op1, const mpz_t op2)
//mpz_and (mpz_t rop, const mpz_t op1, const mpz_t op2)

mpz_class mpz_left_circular_shift(mpz_class num, const mpz_class& shift)
{

	// cout << "Shifting " << num << endl;
	// cout << "by " << shift << endl;
	mpz_class size, mask_right, mask_left, lshift, rshift, shift_val, diff;
	size = mpz_sizeinbase(num.get_mpz_t(), 2);
	//Get lshift val


	mpz_mod(shift_val.get_mpz_t(), shift.get_mpz_t(), size.get_mpz_t());
	diff = size-shift_val;

	//Calculate both shifters for left and right parts
	mpz_ui_pow_ui(lshift.get_mpz_t(), 2, mpz_get_ui(shift_val.get_mpz_t()));
	mpz_ui_pow_ui(rshift.get_mpz_t(), 2, mpz_get_ui(diff.get_mpz_t()));

	//Create mask for shifted area
	mpz_ui_pow_ui(mask_right.get_mpz_t(), 2, mpz_get_ui(shift_val.get_mpz_t()));
	mask_right = mask_right - 1;

	//cout << "mask_right: " << mask_right << endl;

	//shift mask_right left by size - shift bits
	mpz_class size_m_shift;
	mpz_ui_pow_ui(size_m_shift.get_mpz_t(), 2, mpz_get_ui(diff.get_mpz_t()));
	mask_right = mask_right * size_m_shift;

	//cout << "shift mask_right: " << mask_right << endl;

	//save leftmost shift bits
	mpz_class shifted_off;
	mpz_and(shifted_off.get_mpz_t(), num.get_mpz_t(), mask_right.get_mpz_t());
	
	//cout << "shifted off: " << shifted_off << endl;

	//shift num left by shift bits
	mpz_com(mask_right.get_mpz_t(), mask_right.get_mpz_t());
	mpz_and(num.get_mpz_t(), num.get_mpz_t(), mask_right.get_mpz_t());
	num = lshift * num;
	//cout << "num: " << num << endl;
	//shift saved bits to right
	mpz_fdiv_q (shifted_off.get_mpz_t(), shifted_off.get_mpz_t(), rshift.get_mpz_t());
	//cout << "shifted off: " << shifted_off << endl;
	//Restore number
	mpz_ior(num.get_mpz_t(), shifted_off.get_mpz_t(), num.get_mpz_t());

	return num;
}

unsigned long circular_left_shift(unsigned long num, unsigned long shift, unsigned long size){
	return (num << shift) | (num >> (size - shift));
}

unsigned long circular_right_shift(unsigned long num, unsigned long shift, unsigned long size){
	return (num >> shift) | (num << (size-shift));
}


class Simon{
	public:

	Simon(){

	}

	void init(){
		//generate_keys();
		read_keys();
		key_expansion();
		print_keywords_to_file();
		vector<unsigned long> yo = split_message("tst message", block_size);
		for (int i = 0; i < yo.size(); ++i)
		{
			cout << yo[i] << endl;
			mpz_class tmp;
			tmp = yo[i];
			cout << mpz_to_string(tmp) << endl;
		}
	
		encrypt(yo[0], yo[1]);

		decrypt(yo[0], yo[1]);
		for (int i = 0; i < yo.size(); ++i)
		{
			cout << yo[i] << endl;
			mpz_class tmp;
			tmp = yo[i];
			cout << mpz_to_string(tmp) << endl;
		}
	}

	vector<vector<int>> z {
		{1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
		{1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
		{1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
		{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
		{1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
	};
	
	int block_size = 64;
	int key_size = 64;
	int rounds = 68;
	int z_m = 2;
	int word_size = 64;

	mpz_class key;
	vector<unsigned long> keywords;

	void generate_keys(){
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

	void read_keys(){
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

	void key_expansion(){

		keywords.resize(rounds);
		for(int i = z_m; i < rounds; i++){
			//cout << "=====BEGINLOOP\n========\n";
			//mpz_class tmp;
			unsigned long tmp;
			//tmp = mpz_left_circular_shift(keywords[i-1], neg3);
			//tmp = circular_left_shift(keywords[i-1], block_size-3, block_size);
			tmp = circular_right_shift(keywords[i-1], 3, block_size);
			if(z_m == 4){
				tmp = keywords[i-3] ^ tmp;
				//tmp = mpz_get_ui(keywords[i-3].get_mpz_t()) ^ mpz_get_ui(tmp.get_mpz_t()) ;
				//mpz_xor(tmp.get_mpz_t(), keywords[i-3].get_mpz_t(), tmp.get_mpz_t());
			}
			//tmp = tmp ^ circular_left_shift(tmp, block_size-1, block_size);
			tmp = tmp ^ circular_right_shift(tmp, 1, block_size);

			//tmp = mpz_get_ui(tmp.get_mpz_t()) ^  mpz_get_ui(mpz_left_circular_shift(tmp, neg1).get_mpz_t());

			keywords[i] = (~keywords[i-z_m]) ^ tmp ^ z[z_m][(i-z_m) & 62] ^ 3;

		}
	}

	void encrypt(unsigned long& x, unsigned long& y){
	
		for(int i = 0; i < rounds; i++){
			
			unsigned long tmp = x;
			x = y ^ (circular_left_shift(x, 1, block_size) & circular_left_shift(x, 8, block_size)) ^ 
					circular_left_shift(x, 2, block_size) ^ keywords[i];
			y = tmp;
		}
	}

	void decrypt(unsigned long &x, unsigned long &y){
		// mpz_class one, eight, two;
		// one = "1", eight = "8", two = "2";
		for(int i = rounds-1; i >= 0; i--){
			unsigned long tmp = y;
			y = x ^ (circular_left_shift(y, 1, block_size) & circular_left_shift(y, 8, block_size)) ^ 
					circular_left_shift(y, 2, block_size) ^ keywords[i];
			x = tmp;
		}
	}

	void print_keywords_to_file(){
		ofstream myfile;
		myfile.open ("keywords.txt");

	    for(auto i : keywords){
			myfile << i << endl;
		}

		myfile.close();

	}
};




int main(){
	Simon s;
	s.init();
}

