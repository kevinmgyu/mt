#pragma once
#include "types.hpp"
#include "jsonxx.h"

using namespace jsonxx;

struct one_balance {
    string available;
    string freeze;
    string pledge;
    
    double d_available() {
        return atof(available.c_str());
    }

    double d_freeze() {
        return atof(freeze.c_str());
    }

    double d_pledge() {
        return atof(pledge.c_str());
    }

    bool is_balance_zero() {
        if( d_available() > 0 || d_freeze() > 0 || d_pledge() > 0 ) {
            return false;
        }

        return true;
    }
};

struct running_order {
    order_id_type id;
    string market;
    string source;
    int    type;
    int    side;
    string user;
    double ctime;
    double mtime;
    double price;
    double amount;
    double taker_fee;
    double maker_fee;
    double left;
    double deal_stock;
    double deal_money;
    double deal_fee;
    
   
};

struct depth_node {

    string amount;
    string price;

    depth_node() {
        amount = "0.0";
        price = "0.0";
    }

    depth_node(string price,string amount) {
        this->amount = amount;
        this->price = price;
    }

    depth_node(const depth_node& d) {
        this->amount = d.amount;
        this->price = d.price;
    }

    double d_amount() {
         return atof(amount.c_str());
    }

     double d_price() {
         return atof(price.c_str());
    }
};

struct depth {
    vector<depth_node> asks;
    vector<depth_node> bids;

    depth(Object result) {
        parse(result);
    }

    void parse(Object result) {
       asks.clear();
       Array asks_array = result.get<Array>("asks");
	   for( size_t j = 0; j < asks_array.size(); ++j ) {
            Array ar = asks_array.get<Array>(j);
            asks.push_back(depth_node(ar.get<String>(0),ar.get<String>(1)));
       }

       bids.clear();
       Array bids_array = result.get<Array>("bids");
	   for( size_t j = 0; j < bids_array.size(); ++j ) {
           Array ar = bids_array.get<Array>(j);
            bids.push_back(depth_node(ar.get<String>(0),ar.get<String>(1)));
       }
    }

    void split(double price, double& bid_bigger_amount, double& bid_small_amount,double& ask_bigger_amount,double& ask_small_amount) {
        bid_bigger_amount = 0;
        bid_small_amount = 0;
        ask_bigger_amount = 0;
        ask_small_amount = 0;

        for( size_t i = 0; i < asks.size(); ++i ) {
            double p = asks[i].d_price();
            double a = asks[i].d_amount();
            if( p < price ) {
                ask_small_amount = ask_small_amount + a;
            }

            if( p > price) {
                ask_bigger_amount = ask_bigger_amount + a;
            }
        }


        for( size_t i = 0; i < bids.size(); ++i ) {
            double p = bids[i].d_price();
            double a = bids[i].d_amount();
            if( p < price ) {
                bid_small_amount = bid_small_amount + a;
            }

            if( p > price) {
                bid_bigger_amount = bid_bigger_amount + a;
            }
        }
    }
};

int rand(int m, int n);
int rand(int n);
double double_rand( double min, double max );

/*
"id": "3572d2416d2905bcb5ad1d9d6ba5390745a30aff00000",
                "market": "EOS/BTT",
                "source": "",
                "type": 1,
                "side": 2,
                "user": "BTTzQLqAmiddjsb2zpwWwCrFHJfeCgswTgf",
                "ctime": 1520243249.3665781,
                "mtime": 1520243249.3665781,
                "price": "0.00909551",
                "amount": "845.64290066",
                "taker_fee": "0",
                "maker_fee": "0",
                "left": "845.64290066",
                "deal_stock": "0",
                "deal_money": "0",
                "deal_fee": "0"
*/

 
 

FC_REFLECT( one_balance, (available)(freeze)(pledge) )
FC_REFLECT( running_order, (id)(market)(source)(type)(side)(user) (ctime)(mtime)(price)(amount)(taker_fee)(maker_fee) (left)(deal_stock)(deal_money)(deal_fee) )
