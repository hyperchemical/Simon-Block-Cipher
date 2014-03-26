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
	s.encrypt_message("Test long message 1 2 3 4 5");
	s.decrypt_latest_message();
}