#pragma once
#include "asset.hpp"
#include "base_operation.hpp"

namespace bytetrade {

enum OrderStatus {
    OS_INIT = 0,
    OS_FINISHED = 1,
    OS_CANCELED = 2,
    OS_RUNNING = 3,
    OS_CANCELING = 4,
};

struct deal {

    deal_id_type deal_id;
    account_id_type ask_user_id;
    uint8_t ask_role;
    order_id_type ask_order_id;
    asset_id_type ask_asset;
    share_type ask_fee;
    asset_id_type ask_fee_asset;
    account_id_type ask_dapp_id;
    bool is_ask_last;
    optional<share_type> ask_btt_fee_price;

    account_id_type bid_user_id;
    uint8_t bid_role;
    order_id_type bid_order_id;
    asset_id_type bid_asset;
    share_type bid_fee;
    asset_id_type bid_fee_asset;
    account_id_type bid_dapp_id;
    bool is_bid_last;
    optional<share_type> bid_btt_fee_price;

    share_type price;
    share_type amount;
    share_type dealed;
    account_id_type witness;
    string market;
    

    deal() {
    }

    deal(const deal &o) {
        deal_id = o.deal_id;
        ask_user_id = o.ask_user_id;
        ask_role = o.ask_role;
        ask_order_id = o.ask_order_id;
        ask_asset = o.ask_asset;
        ask_fee = o.ask_fee;
        ask_fee_asset = o.ask_fee_asset;
        ask_dapp_id = o.ask_dapp_id;
        is_ask_last = o.is_ask_last;
        ask_btt_fee_price = o.ask_btt_fee_price;

        bid_user_id = o.bid_user_id;
        bid_role = o.bid_role;
        bid_order_id = o.bid_order_id;
        bid_asset = o.bid_asset;
        bid_fee = o.bid_fee;
        bid_fee_asset = o.bid_fee_asset;
        bid_dapp_id = o.bid_dapp_id;
        is_bid_last = o.is_bid_last;
        bid_btt_fee_price = o.bid_btt_fee_price;

        price = o.price;
        amount = o.amount;
        dealed = o.dealed;
        witness = o.witness;
        market = o.market;
    }
};

struct order {
    order_id_type id;
    account_id_type user_id;
    transaction_id_type transaction_id;
    bool use_btt_as_fee;
    share_type freeze_btt_fee;
    share_type freeze_left_btt_fee;
    uint32_t block_num;
    uint32_t type;
    uint32_t side;

    double create_time;
    double update_time;
    string market;
    optional<account_id_type> dapp;
    share_type price;
    share_type amount;
    share_type taker_fee;
    share_type maker_fee;
    share_type left;
    share_type freeze;
    share_type freeze_fee;
    share_type deal_stock;
    share_type deal_money;
    share_type deal_fee;
    int status;
    vector<deal> deals;

    void add_deal(deal dd) {
        for( auto d : deals) {
            if( d.deal_id.str() == dd.deal_id.str() ) {
                return;
            }
        }

        deals.push_back(dd);
    }

    order() {
        id = order_id_type();
        user_id = account_id_type();
        transaction_id = transaction_id_type();
        use_btt_as_fee = false;
        freeze_btt_fee = 0;
        freeze_left_btt_fee =0 ;
        type = 0;
        side = 0;
        block_num = 0;
        create_time = 0;
        update_time = 0;
        market = "";
        dapp = optional<account_id_type>();
        price = 0;
        amount = 0;
        taker_fee = 0;
        maker_fee = 0;
        left = 0;
        freeze = 0;
        freeze_fee = 0;
        deal_stock = 0;
        deal_money = 0;
        deal_fee = 0;
        status = OS_INIT;
        deals.clear();
    }

    order(const order &o) {
        id = o.id;
        user_id = o.user_id;
        transaction_id = o.transaction_id;
        use_btt_as_fee = o.use_btt_as_fee;
        freeze_btt_fee = o.freeze_btt_fee;
        freeze_left_btt_fee = o.freeze_left_btt_fee;
        type = o.type;
        side = o.side;
        block_num = o.block_num;
        create_time = o.create_time;
        update_time = o.update_time;
        market = o.market;
        dapp = o.dapp;
        price = o.price;
        amount = o.amount;
        taker_fee = o.taker_fee;
        maker_fee = o.maker_fee;
        left = o.left;
        freeze = o.freeze;
        freeze_fee = o.freeze_fee;
        deal_stock = o.deal_stock;
        deal_money = o.deal_money;
        deal_fee = o.deal_fee;
        status = o.status;
        deals.clear();
        for (deal d : o.deals) {
            deals.push_back(d);
        }
    }
};
}

struct market_t;

struct order_create_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 5 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    account_id_type creator;
    //order_id_type order_id;
    //  asset           amount_to_sell;
    //  asset           min_to_receive;
    uint8_t side;       // MARKET_ORDER_SIDE_ASK   or MARKET_ORDER_SIDE_BID
    uint8_t order_type; // MARKET_ORDER_TYPE_LIMIT or MARKET_ORDER_TYPE_MARKET
                        //  int             taker_fee;
                        //  int             maker_fee;
                        //  int             fee;
    string market_name;
    share_type amount;
    share_type price;
    bool use_btt_as_fee;
    optional<share_type> freeze_btt_fee;
    /// The order will be removed from the books if not filled by expiration
    /// Upon expiration, all unsold asset will be returned to seller
    time_point_sec now = fc::time_point::now();
    time_point_sec expiration = time_point_sec::maximum();

    account_id_type fee_payer() const { return creator; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string order_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        const uint16_t& btt_fee_rate,
        const uint16_t& no_btt_fee_rate,
        market_t *market
        );

     operation_result evaluate1(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string order_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
    //  price           get_price()const { return amount_to_sell / min_to_receive; }
};

struct order_create2_operation : public order_create_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    optional<uint16_t> custom_btt_fee_rate;
    optional<uint16_t> custom_no_btt_fee_rate;

    operation_result evaluate2(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string order_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
};

struct order_create3_operation : public order_create2_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    asset_id_type money_id;
    asset_id_type stock_id;

     operation_result evaluate3(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string order_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};


struct order_cancel_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 0;
    };

    share_type fee;
    order_id_type order_id;
    string market_name;
    /** must be order->seller */
    account_id_type creator;
    //      extensions_type     extensions;

    account_id_type fee_payer() const { return creator; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        uint64_t market_id
        );
};

struct order_cancel2_operation : public order_cancel_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    asset_id_type money_id;
    asset_id_type stock_id;

    operation_result evaluate2(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
};

struct order_cancel_settlement_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 0;
    };

    share_type fee;
    order_id_type order_id;
    account_id_type creator;
    asset_id_type asset_id;
    share_type amount;
    share_type freeze_btt;

    //      extensions_type     extensions;

    account_id_type fee_payer() const { return creator; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};

struct order_freeze_btt_fee_settlement_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 0;
    };

    share_type fee;
    order_id_type order_id;
    account_id_type creator;
    share_type freeze_btt;

    //      extensions_type     extensions;

    account_id_type fee_payer() const { return creator; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};



struct deal_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 0;
    };

    share_type fee;
    deal_id_type deal_id;
    account_id_type ask_user_id;
    uint8_t ask_role;
    order_id_type ask_order_id;
    asset_id_type ask_asset;
    share_type ask_fee;
    account_id_type ask_dapp_id;
    bool is_ask_last;
    asset_id_type ask_fee_asset;
    optional<share_type> ask_btt_fee_price;

    account_id_type bid_user_id;
    uint8_t bid_role;
    order_id_type bid_order_id;
    asset_id_type bid_asset;
    share_type bid_fee;
    account_id_type bid_dapp_id;
    bool is_bid_last;
    asset_id_type bid_fee_asset;
    optional<share_type> bid_btt_fee_price;

    share_type price;
    share_type amount;
    share_type deal;
    account_id_type witness;
    
     string market;
    //      uint8_t                  side;        // MARKET_ORDER_SIDE_ASK   or MARKET_ORDER_SIDE_BID
    //      uint8_t                  order_type;  // MARKET_ORDER_TYPE_LIMIT or MARKET_ORDER_TYPE_MARKET
    //      extensions_type     extensions;

    account_id_type fee_payer() const { return ask_user_id; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        account_id_type &ask_user,
        asset_id_type &ask_asset,
        share_type &ask_fee,
        account_id_type &bid_user,
        asset_id_type &bi_asset,
        share_type &bid_fee);
};

FC_REFLECT(bytetrade::deal, (deal_id)(ask_user_id)(ask_role)(ask_order_id)(ask_asset)(ask_fee)(ask_fee_asset)(ask_dapp_id)(is_ask_last)(ask_btt_fee_price)(bid_user_id)(bid_role)(bid_order_id)(bid_asset)(bid_fee)(bid_fee_asset)(bid_dapp_id)(is_bid_last)(bid_btt_fee_price)(price)(amount)(dealed)(witness)(market))
FC_REFLECT(bytetrade::order, (id)(user_id)(transaction_id)(use_btt_as_fee)(freeze_btt_fee)(freeze_left_btt_fee)(block_num)(type)(side)(create_time)(update_time)(market)(dapp)(price)(amount)(taker_fee)(maker_fee)(left)(freeze)(freeze_fee)(deal_stock)(deal_money)(deal_fee)(status)(deals))

FC_REFLECT(order_create_operation::fee_parameters_type, (fee))
FC_REFLECT(order_cancel_operation::fee_parameters_type, (fee))
//FC_REFLECT( call_order_update_operation::fee_parameters_type, (fee) )
//FC_REFLECT( bid_collateral_operation::fee_parameters_type, (fee) )
//FC_REFLECT( fill_order_operation::fee_parameters_type,  ) // VIRTUAL
//FC_REFLECT( execute_bid_operation::fee_parameters_type,  ) // VIRTUAL

//FC_REFLECT( order_create_operation,(fee)(seller)(order_id)(side)(order_type)(taker_fee)(maker_fee)(amount)(price)(expiration)(extensions))
FC_REFLECT(order_create_operation, (fee)(creator)(side)(order_type)(market_name)(amount)(price)(use_btt_as_fee)(freeze_btt_fee)(now)(expiration))
FC_REFLECT_DERIVED(order_create2_operation, (order_create_operation), (custom_btt_fee_rate)(custom_no_btt_fee_rate))
FC_REFLECT_DERIVED(order_create3_operation, (order_create2_operation), (money_id)(stock_id))

FC_REFLECT(deal_operation, (fee)(deal_id)(ask_user_id)(ask_role)(ask_order_id)(ask_asset)(ask_fee)(ask_dapp_id)(is_ask_last)(ask_fee_asset)(ask_btt_fee_price)(bid_user_id)(bid_role)(bid_order_id)(bid_asset)(bid_fee)(bid_dapp_id)(is_bid_last)(bid_fee_asset)(bid_btt_fee_price)(price)(amount)(deal)(witness)(market))
FC_REFLECT(order_cancel_settlement_operation, (fee)(order_id)(creator)(asset_id)(amount)(freeze_btt))
FC_REFLECT(order_freeze_btt_fee_settlement_operation, (fee)(order_id)(creator)(freeze_btt))

// FC_REFLECT_DERIVED(order_create_operation, (base_operation), (fee)(creator)(side)(order_type)(market_name)(amount)(price)(use_btt_as_fee)(freeze_btt_fee)(now)(expiration))
// FC_REFLECT_DERIVED(deal_operation, (base_operation), (fee)(deal_id)(ask_user_id)(ask_role)(ask_order_id)(ask_asset)(ask_fee)(ask_dapp_id)(bid_user_id)(bid_role)(bid_order_id)(bid_asset)(bid_fee)(bid_dapp_id)(price)(amount)(deal)(witness))
// FC_REFLECT_DERIVED(order_cancel_settlement_operation, (base_operation),  (fee)(order_id)(creator)(asset_id)(amount))
//FC_REFLECT( order_create_operation,(fee)(creator)(side)(order_type)(market_name)(amount)(price) )
FC_REFLECT(order_cancel_operation, (fee)(creator)(market_name)(order_id))
FC_REFLECT_DERIVED(order_cancel2_operation, (order_cancel_operation), (money_id)(stock_id))


//FC_REFLECT( call_order_update_operation, (fee)(funding_account)(delta_collateral)(delta_debt)(extensions) )
//FC_REFLECT( fill_order_operation, (fee)(order_id)(account_id)(pays)(receives)(fill_price)(is_maker) )
//FC_REFLECT( bid_collateral_operation, (fee)(bidder)(additional_collateral)(debt_covered)(extensions) )
//FC_REFLECT( execute_bid_operation, (fee)(bidder)(debt)(collateral) )

//FC_REFLECT_DERIVED(order_cancel_operation, (base_operation), (fee)(creator)(market_name)(order_id))
