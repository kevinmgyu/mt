#pragma once

#include "types.hpp"
#include "asset.hpp"
#include "authority.hpp"


struct void_result {};
//   typedef fc::static_variant<void_result,object_id_type,asset> operation_result;
//typedef int64_t operation_result;

enum OperationResultRet {
    ORR_OK = 0,
    ORR_NOT_NEED_PROCCED = 1,
    ORR_PASS_TO_NEXT_CHAIN = 2,
    ORR_ERROR = 3,
    ORR_PROPOSAL_REJECT = 4,

    ORR_PROCCED_FEE_ERROR = 10,
    ORR_ASSET_NOT_EXISTS = 11,
    ORR_TRANSFER_SMALL_ZERO = 12,
    ORR_ACCOUNT_NOT_EXISTS = 13,
    ORR_ACCOUNT_NOT_AVAIBLE = 14,
    ORR_MARKET_NOT_EXISTS = 15,
    ORR_ORDER_NOT_EXISTS = 16,
    ORR_BALANCE_TYPE_ERROR = 17,
    ORR_WITNESS_NOT_EXISTS = 18,
    ORR_ASSET_CAN_NOT_ADD = 19,
    ORR_PLEDGE_NOT_EXISTS = 20,
    ORR_MARKET_PRICE_NOT_EXIST = 21,
    ORR_ACCOUNT_NOT_ENOUGH = 22,
    ORR_FREEZE_BTT_FEE_NOT_ENOUGH = 23,
    ORR_BTT_MARKET_NOT_FREEZE_BTT_AS_EXTERNAL_FEE = 24,
    ORR_FREEZE_BTT_FEE_MISS = 25,
    ORR_ACCOUNT_ALREADY_EXISTED = 26,
    ORR_NOT_CANCEL_SELF_ORDER = 27,
    ORR_ID_NOT_VALID = 28,
    ORR_PRECISION_TOO_HIGH = 29,
    ORR_VALUE_BIGGER_THAN_UINT64 = 30,

    ORR_STOCK_NOT_EXISTS = 31,
    ORR_MONEY_NOT_EXISTS = 32,
    ORR_STOCK_ID_SMALL_MONEY_ID = 33,
    ORR_MARKET_NAME_NOT_VALID = 34,
    ORR_MRAKET_HAD_EXISTED = 35,
    ORR_STOCK_PRECISION_ERROR_1 = 36,
    ORR_STOCK_PRECISION_ERROR_2 = 37,
    ORR_STOCK_PRECISION_ERROR_3 = 38,

    ORR_SIGNATUE_ERROR = 39,
    ORR_FEE_ERROR =40,
    ORR_ASSET_NAME_TOO_LONG =41,
    ORR_UTXOS_NOT_EXISTS =42,
    ORR_UTXOS_NOT_LEGAL =43,
    ORR_NOT_BTC_WITHDRAW = 44,
    ORR_NOT_ENOUGH_UTXO = 45,
    ORR_NOT_UTXO = 46,
    ORR_BTC_FEE_NOT_ENOUGH = 47,
    ORR_TRANSFER_MESSAGE_TOO_LONG = 48,
    ORR_CREATE_ASSET_NEED_WITNESS= 49,
    ORR_CREATE_ASSET_NEED_CHAIN_TYPE = 50
};

struct operation_cost_fee {
    asset_id_type asset_type;
    share_type amount;

    operation_cost_fee() {
        asset_type = 1;
        amount = 0;
    }

    operation_cost_fee(share_type a) {
        asset_type = 1;
        amount = a;
    }

    operation_cost_fee(asset_id_type ai, share_type a) {
        asset_type = ai;
        amount = a;
    }

    operation_cost_fee(const operation_cost_fee &o) {
        asset_type = o.asset_type;
        amount = o.amount;
    }
};

struct operation_cost_fees {
    map<asset_id_type, share_type> amounts;

    operation_cost_fees() {
        amounts.clear();
    }

    operation_cost_fees(const operation_cost_fees &o) {
        amounts.clear();
        for (auto oo : o.amounts) {
            amounts[oo.first] = oo.second;
        }
    }

    void push_back(const asset_id_type asset_type, const share_type amount) {
        if (amounts.find(asset_type) == amounts.end()) {
            amounts[asset_type] = 0;
        }

        amounts[asset_type] += amount;
    }

    void push_back(const operation_cost_fee &cost_fee) {
        if (amounts.find(cost_fee.asset_type) == amounts.end()) {
            amounts[cost_fee.asset_type] = 0;
        }

        amounts[cost_fee.asset_type] += cost_fee.amount;
    }

    void push_back(const operation_cost_fees &cost_fees) {
        for (auto key : cost_fees.amounts) {
            push_back(key.first, key.second);
        }
    }

    // void split(double percent, operation_cost_fees &out_a, operation_cost_fees &out_b) {
    //     for (auto key : amounts) {
    //         share_type a = key.second * percent;
    //         share_type b = key.second - a;
    //         out_a.push_back(key.first, a);
    //         out_b.push_back(key.first, b);
    //     }
    // }

    void clear() {
        amounts.clear();
    }
};

struct fee_distribution {
    map<account_id_type, operation_cost_fees> fees;

    fee_distribution() {
        fees.clear();
    }

    void push_back(const account_id_type add, const operation_cost_fees &cost_fees) {
        fees[add].push_back(cost_fees);
    }

    void push_back(const account_id_type add, const asset_id_type asset_type, const share_type amount) {
        fees[add].push_back(asset_type, amount);
    }

    void push_back(const fee_distribution &fd) {
        for (auto a : fd.fees) {
            fees[a.first].push_back(a.second);
        }
    }

    fee_distribution(const fee_distribution &fd) {
        fees.clear();
        for (auto a : fd.fees) {
            fees[a.first] = a.second;
        }
    }

    void clear() {
        fees.clear();
    }
};

struct operation_result {
    //operation_cost_fees cost_fees;
    share_type cost_fee;
    int reason;
    optional<string> error_string;

    operation_result() {
        // cost_fees.clear();
        cost_fee = 0;
        reason = ORR_OK;
    }

    operation_result(share_type fee) {
        //cost_fees.push_back(cost_fee);
        cost_fee = fee;
        reason = ORR_OK;
    }

    operation_result(share_type fee, int re) {
        //cost_fees.push_back(cost_fee);
        cost_fee = fee;
        reason = re;
    }

    operation_result(share_type fee, int re, string es) {
        //cost_fees.push_back(cost_fee);
        cost_fee = fee;
        reason = re;
        error_string = es;
    }

    operation_result(const operation_result &o) {
        cost_fee = o.cost_fee;
        reason = o.reason;
        error_string = o.error_string;
    }

    // void push_back(share_type cost_fee) {
    //     cost_fees.push_back(cost_fee);
    // }

    // void push_back(asset_id_type asset_type, share_type cost_fee) {
    //     operation_cost_fee ocf(asset_type,cost_fee);
    //     cost_fees.push_back(ocf);
    // }
};

struct base_operation {
    //    template<typename T>
    // share_type calculate_fee(const T& params)const
    // {
    //    return params.fee;
    // }
    //struct fee_parameters_type { uint64_t fee = 5 * core_asset_precision; };

    optional<account_id_type> payer;
    share_type calculate_fee() const;

    void get_required_authorities(vector<authority> &) const {}
    void get_required_active_authorities( flat_set<account_id_type>& )const{}
    void get_required_owner_authorities( flat_set<account_id_type>& )const{}
    void validate() const {}
    account_id_type fee_payer()const;
    // share_type      get_fee() const;

    static uint64_t calculate_data_fee(uint64_t bytes, uint64_t price_per_kbyte);

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};

/**
    *  For future expansion many structus include a single member of type
    *  extensions_type that can be changed when updating a protocol.  You can
    *  always add new types to a static_variant without breaking backward
    *  compatibility.   
    */
typedef static_variant<void_t> future_extensions;

/**
    *  A flat_set is used to make sure that only one extension of
    *  each type is added and that they are added in order.  
    *  
    *  @note static_variant compares only the type tag and not the 
    *  content.
    */
typedef flat_set<future_extensions> extensions_type;

///@}

FC_REFLECT(operation_cost_fee, (asset_type)(amount))
FC_REFLECT(operation_cost_fees, (amounts))
FC_REFLECT(fee_distribution, (fees))
FC_REFLECT(operation_result, (cost_fee)(reason)(error_string))
//FC_REFLECT_TYPENAME( graphene::chain::operation_result )
FC_REFLECT_TYPENAME(future_extensions)
FC_REFLECT(void_result, )
//FC_REFLECT( base_operation, (payer) )
