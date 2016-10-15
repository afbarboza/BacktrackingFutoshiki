all:
	g++ parser.cpp futoshiki.cpp main.cpp -o prog -Wall -std=c++11
clear:
	rm prog
