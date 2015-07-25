/*
* File:   main.cpp
* Author: atamarkin2
*
* Created on June 26, 2014, 5:11 PM
*/

#include <string>
#include "galik_socketstream.h"
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

using namespace galik;
using namespace galik::net;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	size_t start = 0, end = 0;
	while ((end = s.find(delim, start)) != string::npos) {
		elems.push_back(s.substr(start, end - start));
		start = end + 1;
	}
	elems.push_back(s.substr(start));
	return elems;
}

vector<stock> stocks;
socketstream ss;
string name = "TripleG";
string password = "sjj";

struct stock {
	string ticker;
	double earnings;
	int shareNum;
	double dividenRatio;
};

void InitialStocks() {

}

void fetchStocks() {

}

stock getHighestDividen() {

}

double getCurrentCash() {

}

void bid(string ticker, double price, int shareNum) {

}

void ask(string ticker, double price, int shareNum) {

}


/*
*
*/
int main(int argc, char** argv) {
	if (argc < 5) {
		cout << "args: <user> <password> <host> <port> <command>" << endl;
		cout << argc;
		exit(1);
	}


	string host = "codebb.cloudapp.net";
	int port = 17429;


	ss.open(host, port);

	// string command("");
	// for (int i = 5; i < argc; i++) {
	// 	command += argv[i];
	// 	command += " ";
	// }
	// command;

	//ss << name << " " << password << "\n" << endl;

	InitialStocks(); //Call security

	while(true) {
		fetchStocks();
		stock s = getHighestDividen();

	}

	//ss << name << " " << password << "\n" << command << "\nCLOSE_CONNECTION" << endl;

	while (ss.good() && !ss.eof()) {
		string line;
		getline(ss, line);
		cout << line << endl;
	}
	return 0;
}
