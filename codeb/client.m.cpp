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
    vector<double> history;
    double dividendRatio;
    double volatility;
    vector<double> prices;
    double avg;
    double sum;
    
    Stock(string ticker, double netWorth, double dividendRatio, double volatility): ticker(ticker), netWorth(netWorth), dividendRatio(dividendRatio), volatility(volatility){}
};

#define check_size 5
vector<Stock> allStocks;

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

double getAskPrice(string ticker){
    vector<MyOrder> v = orders("ALL", ticker);
    double sum = 0;
    int c = 0;
    double min = 1000000;
    for (int i = 0; i < v.size(); i++){

        if (v[i].type == "ASK"){
            if (min > v[i].price) {
                min = v[i].price;
            }
            //sum += v[i].price * v[i].shares;
            //c = c + v[i].shares;
        }
    }
    return min;
    //return sum/c;
}

double getBidPrice(string ticker){
    vector<MyOrder> v = orders("ALL", ticker);
    double sum = 0;
    int c = 0;
    double max = 0;

    for (int i = 0; i < v.size(); i++){


        if (v[i].type == "BID"){
            if (max < v[i].price) {
                max = v[i].price;
            }
            //sum += v[i].price * v[i].shares;
            //c = c + v[i].shares;
        }
    }
    return max;
    //return sum/c;
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
Stock findHighest(){
    double max = 0;
    Stock result = Stock("",1,1,1);
    for (int i=0; i < allStocks.size(); i++){
        double t = allStocks[i].netWorth * allStocks[i].dividendRatio;
        if (t>max){
            max = t;
            result = allStocks[i];
        }
    }
    return result;

}

void update(){
    vector<Stock> newS = securities();
    for (int i = 0;  i < newS.size(); i++){
        allStocks[i].netWorth = newS[i].netWorth;
        allStocks[i].history.push_back(newS[i].netWorth);
        allStocks[i].dividendRatio = newS[i].dividendRatio;
    }
}

void SellOrders(){
    vector<MyStock> my_securities = mySecurities();
    for (int i=0; i < my_securities.size(); i++){
        if (my_securities[i].shares > 0) {
            MyStock s = my_securities[i];
            double avg;
            for (int j =0; j<allStocks.size(); j++){
                if (allStocks[j].ticker == s.ticker){
                    avg = allStocks[j].avg;
                }
            }
            double currentBid = getBidPrice(s.ticker);

            if (avg < currentBid){
                ask(s.ticker, currentBid, s.shares);
            }
        }
    }
}


void BuyBasedOnHistory() {

    for (int i=0; i < allStocks.size(); i++){
        int sum = 0;
        Stock t = allStocks[i];
                int size = t.history.size();    
        if (size > check_size ) size = check_size;
        for (int j = t.history.size() - size; j < t.history.size(); j++){
            sum = sum + t.history[j] - t.history[j-1];   
        }

        if (sum > 1000) {
            double askp = getAskPrice(t.ticker);
            bid(t.ticker, askp, 5);
                allStocks[i].avg = (allStocks[i].avg*allStocks[i].prices.size() + askp) / (allStocks[i].prices.size()+1);
                allStocks[i].prices.push_back(askp);
                cout<< allStocks[i].ticker<<' ' <<askp<<endl;
        }
    }
}

int main(int argc, char** argv) {
	ss.open("codebb.cloudapp.net", 17429);
    ss << "TripleG sjj" << "\n";
    //--------------------------------------

    allStocks = securities();
    showMyCash();
    showSecurities();
    //bid("BUD",12,50);
    //ask("BUD",20,50);
    //showOrders("MY", "");
    //showMySecurities();
    //clearBid("BUD");
    //showOrders("ALL", "AAPL");

    while (true){

        update();
        BuyBasedOnHistory();
        cout<<allStocks[0].netWorth<<endl;
        SellOrders();
        Stock high = findHighest();
        double askp = getAskPrice(high.ticker);
        bool success = bid(high.ticker,askp, 5);
        for (int i=0; i < allStocks.size(); i++){
            if (allStocks[i].ticker == high.ticker){
                allStocks[i].avg = (allStocks[i].avg*allStocks[i].prices.size() + askp) / (allStocks[i].prices.size()+1);
                allStocks[i].prices.push_back(askp);
                cout<< allStocks[i].ticker<<' ' <<askp<<endl;
            }
        }


    }
    //cout<<getAskPrice("AAPL");
    //Stock temp = findHighest();
    //cout<< temp.ticker <<endl;
    //--------------------------------------
    ss << "\nCLOSE_CONNECTION" << endl;
	return 0;
}