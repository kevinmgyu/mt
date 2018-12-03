#pragma once
#include "types.hpp"

   //extern const int64_t scaled_precision_lut[];
extern uint64_t core_asset_precision;
extern uint64_t core_asset_min_amount_precision;
extern uint64_t core_asset_fee_precision ;

# define BALANCE_TYPE_AVAILABLE 1
# define BALANCE_TYPE_FREEZE    2
# define BALANCE_TYPE_PLEDGE    3
# define BALANCE_TYPE_INCOME    4

struct asset {
    asset_id_type 		 id;

    string				 symbol;
    string 				 name;

    uint8_t		         show_precision = 0;
    uint8_t				 save_precision = 0;
    uint64_t             base_precision = core_asset_precision;

    account_id_type		     creator;	
    bool                     is_native = true;
    
    optional<uint32_t>       chain_type;
    optional<string> 	     chain_contract_address;
    optional<uint64_t>       min_trade_amount; //= 100000;
    optional<uint64_t>       min_transfer_amount ;
    
    
    optional<share_type>     total_supply;

    vector<account_id_type>  init_account;
    vector<share_type>       init_amount;
    vector<uint8_t>          init_balance_type;
    optional<uint64_t>       external_precision;

    asset() {
    }

    asset(const asset& a) {
        id = a.id;
        symbol = a.symbol;
        name = a.name;
        show_precision = a.show_precision;
        save_precision = a.save_precision;
        base_precision = a.base_precision;

        creator = a.creator;
        is_native = a.is_native;
        
        chain_type = a.chain_type;
        chain_contract_address = a.chain_contract_address;
        min_trade_amount = a.min_trade_amount;
        min_transfer_amount = a.min_transfer_amount;
     
        total_supply = a.total_supply;

        for( account_id_type ac : a.init_account ) {
            init_account.push_back(ac);
        }

        for( share_type wc : a.init_amount ) {
            init_amount.push_back(wc);
        }

        for( uint8_t bt : a.init_balance_type ) {
            init_balance_type.push_back(bt);
        }
        
        external_precision = a.external_precision;
    }
    
    friend bool operator < ( const asset& a, const asset& b )
    {
        //FC_ASSERT( a.asset_id == b.asset_id );
        return a.id < b.id;
    }

    friend bool operator == ( const asset& a, const asset& b )
    {
        return a.id == b.id;
    }

    friend bool operator != ( const asset& a, const asset& b )
    {
        return !(a == b);
    }
};

struct market {
  //  market_id_type      id;
    string              name;
    asset_id_type       stock;
    asset_id_type       money;

    uint8_t             stock_precision = 0;
    uint8_t             money_precision = 0;
    uint8_t             fee_precision = 0;
   // uint64_t            base_precision = 100000000;
    uint64_t            min_trade_amount   = 100000;
    //uint64_t            min_transfer_amount   = 100000; 
     

    market() {
        
    }

    market(const market& a) {
     //   id = a.id;
        name = a.name;
        stock = a.stock;
        money = a.money;

        stock_precision = a.stock_precision;
        money_precision = a.money_precision;
        fee_precision = a.fee_precision;
        
      //  base_precision = a.base_precision;
        min_trade_amount = a.min_trade_amount;
       // min_transfer_amount = a.min_transfer_amount;

      //  taker_fee_percent = a.taker_fee_percent;s
      //  maker_fee_percent = a.maker_fee_percent;

    }
};

FC_REFLECT( asset,  (id)(symbol)(name)(show_precision)(save_precision)(base_precision)(creator)(is_native)(chain_type)(chain_contract_address)(min_trade_amount)(min_transfer_amount)(total_supply)(init_account)(init_amount)(init_balance_type)(external_precision))
//FC_REFLECT( market, (id)(name)(stock)(money)(stock_precision)(money_precision)(fee_precision)(base_precision)(min_trade_amount)(min_transfer_amount))
FC_REFLECT( market, (name)(stock)(money)(stock_precision)(money_precision)(fee_precision)(min_trade_amount))

