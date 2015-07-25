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
#include <sstream>

using namespace std;

using namespace galik;
using namespace galik::net;

socketstream ss;

struct Stock {
    string ticker;
    double netWorth;
    double dividendRatio;
    double volatility;
    
    Stock(string ticker, double netWorth, double dividendRatio, double volatility): ticker(ticker), netWorth(netWorth), dividendRatio(dividendRatio), volatility(volatility){}
};

struct MyStock {
    string ticker;
    int shares;
    double dividendRatio;
    
    MyStock(string ticker, int shares, double dividendRatio): ticker(ticker), shares(shares), dividendRatio(dividendRatio){}
};

struct MyOrder {
    string type;
    string ticker;
    double price;
    int shares;
    
    MyOrder(string type, string ticker, double price, int shares): type(type), ticker(ticker), price(price), shares(shares){}
};

//struct Order {
//    string type;
//    double price;
//    int shares;
//    
//    Order(string type, double price, int shares): type(type), price(price), shares(shares){}
//};

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	size_t start = 0, end = 0;
	while ((end = s.find(delim, start)) != string::npos) {
		elems.push_back(s.substr(start, end - start));
		start = end + 1;
	}
	elems.push_back(s.substr(start));
	return elems;
}

// COMMANDS
double myCash() {
    double n;
    ss << "MY_CASH " << endl;
    string line;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        s << line;
        string t;
        s >> t;
        s.clear();
        s.ignore();
        s << line;
        s >> n;
    }
    return n;
}

void showMyCash() {
    cout << myCash() << endl;
}

vector<MyStock> mySecurities() {
    ss << "MY_SECURITIES " << endl;
    string line;
    vector<MyStock> v;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        s << line;
        string t;
        s >> t;
        s.ignore();
        s.clear();
        string ticker;
        int shares;
        double dividendRatio;
        while (s << line) {
            s >> ticker;
            s << line;
            s >> shares;
            s << line;
            s >> dividendRatio;
            v.push_back(MyStock(ticker, shares, dividendRatio));
        }
        v.pop_back();
    }
    return v;
}

void showMySecurities() {
    vector<MyStock> v = mySecurities();
    for (int i=0; i<v.size(); i++) {
        cout << "!" << v[i].ticker << " " << v[i].shares << " " << v[i].dividendRatio << endl;
    }
}

bool bid(string ticker, double price, int shares) {
    ss << "BID " << ticker << " " << price << " " << shares << " " << endl;
    string line;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        string t;
        s << line;
        s >> t;
        if (t == "BID_OUT") {
            cout << "bidding done" << endl;
            return true;
        }
        else {
            cout << "bidding failed" << endl;
            return false;
        }
    }
    cout << "bidding failed" << endl;
    return false;
}

bool clearBid(string ticker) {
    ss << "CLEAR_BID " << ticker << " " << endl;
    string line;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        string t;
        s << line;
        s >> t;
        if (t == "CLEAR_BID_OUT") {
            cout << "clearing bid done" << endl;
            return true;
        }
        else {
            cout << "clearing failed" << endl;
            return false;
        }
    }
    cout << "clearing failed" << endl;
    return false;
}

bool ask(string ticker, double price, int shares) {
    ss << "ASK " << ticker << " " << price << " " << shares << endl;
    if (ss.good() && !ss.eof()) {
        string line;
        getline(ss, line);
        stringstream s;
        string t;
        s << line;
        s >> t;
        if (t == "ASK_OUT") {
            cout << "asking done" << endl;
            return true;
        }
        else {
            cout << "asking failed" << endl;
            return false;
        }
    }
    cout << "asking failed" << endl;
    return false;
}

bool clearAsk(string ticker) {
    ss << "CLEAR_ASK " << ticker << " " << endl;
    string line;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        string t;
        s << line;
        s >> t;
        if (t == "CLEAR_ASK_OUT") {
            cout << "clearing ask done" << endl;
            return true;
        }
        else {
            cout << "clearing failed" << endl;
            return false;
        }
    }
    cout << "clearing failed" << endl;
    return false;
}

vector<MyOrder> orders(string who, string ticker) {
    if (who == "MY") ss << "MY_ORDERS " << endl;
    else ss << "ORDERS " << ticker << " " << endl;
    string line;
    vector<MyOrder> v;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        s << line;
        string t;
        s >> t;
        s.ignore();
        s.clear();
        string type;
        string ticker;
        double price;
        int shares;
        while (s << line) {
            s >> type;
            s << line;
            s >> ticker;
            s << line;
            s >> price;
            s << line;
            s >> shares;
            v.push_back(MyOrder(type, ticker, price, shares));
        }
        v.pop_back();
    }
    return v;
}

void showOrders(string who, string ticker) {
    vector<MyOrder> v = orders(who, ticker);
    for (int i=0; i<v.size(); i++) {
        cout << "?" << v[i].type << " " << v[i].ticker << " " << v[i].price << " " << v[i].shares << endl;
    }
}

vector<Stock> securities() {
    ss << "SECURITIES " << endl;
    string line;
    vector<Stock> v;
    if (ss.good() && !ss.eof()) {
        getline(ss, line);
        stringstream s;
        s << line;
        string t;
        s >> t;
        s.ignore();
        s.clear();
        string ticker;
        double netWorth;
        double dividendRatio;
        double volatility;
        while (s << line) {
            s >> ticker;
            s << line;
            s >> netWorth;
            s << line;
            s >> dividendRatio;
            s << line;
            s >> volatility;
            v.push_back(Stock(ticker, netWorth, dividendRatio, volatility));
        }
        v.pop_back();
    }
    return v;
}

void showSecurities() {
    vector<Stock> v = securities();
    for (int i=0; i<v.size(); i++) {
        cout << "-" << v[i].ticker << " " << v[i].netWorth << " " << v[i].dividendRatio << " " << v[i].volatility << endl;
    }
}

//vector<Order> orders(string ticker) {
//    ss << "ORDERS " << ticker << " " << endl;
//    string line;
//    vector<Order> v;
//    if (ss.good() && !ss.eof()) {
//        getline(ss, line);
//        stringstream s;
//        s << line;
//        string t;
//        s >> t;
//        s.ignore();
//        s.clear();
//        string type;
//        double price;
//        int shares;
//        while (s << line) {
//            s >> type;
//            s << line;
//            s >> price;
//            s << line;
//            s >> shares;
//            v.push_back(Order(type, price, shares));
//        }
//        v.pop_back();
//    }
//    return v;
//}
//
//void showOrders(string ticker) {
//    vector<Order> v = orders(ticker);
//    for (int i=0; i<v.size(); i++) {
//        cout << "+" << v[i].type << " " << v[i].price << " " << v[i].shares << endl;
//    }
//}

/*
*
*/
int main(int argc, char** argv) {
	ss.open("codebb.cloudapp.net", 17429);
    ss << "TripleG sjj" << "\n";
    //--------------------------------------
    
    showMyCash();
    showSecurities();
    bid("BUD",12,50);
    ask("BUD",20,50);
    showOrders("MY", "");
    showMySecurities();
    clearBid("BUD");
    showOrders("ALL", "BUD");
    //--------------------------------------
    ss << "\nCLOSE_CONNECTION" << endl;
	return 0;
}