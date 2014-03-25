#Compiler Settings
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0./lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

#Shortcuts
SFLAG = -std=c++11 -O3
TFLAG = -lgmpxx -lgmp
DFLAG = -Wall -Wextra -pedantic -Wvla -g -ggdb  -pg
FILE = transform_message.cpp
OUT= -o app

default:
	g++ $(TFLAG) $(SFLAG) simon.cpp -o simon

debug:
	g++ $(TFLAG) -std=c++11 $(DFLAG) simon.cpp -o simon

#Submit
two:
	g++ $(TFLAG) $(SFLAG) $(FILE) $(OUT)
	g++ $(TFLAG) $(SFLAG) generate_key.cpp -o generate

objects:
	g++ $(TFLAG) $(SFLAG) -o simon.o -c simon.cpp

all:
	g++ $(TFLAG) $(SFLAG) simon.o testing.cpp  -o app

clean:
	rm -rf *o modex
