#pragma once
#include "asset.hpp"
#include "base_operation.hpp"
#include "memo.hpp"
#include "vote_and_approve.hpp"

using namespace fc;

struct transfer_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;
    account_id_type from;
    account_id_type to;
    asset_id_type asset_type;
    share_type amount;
    
    optional<memo_data> memo;
    extensions_type extensions;

    account_id_type fee_payer() const { 
        if( payer ) {
            return *payer;
        } 

        return from; 
    }

    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        optional<string> message
        );
};

struct transfer2_operation : public transfer_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    //  transfer2_operation& operator=(const transfer_operation& rhs) {
    //     this->payer = rhs.payer;
    //     this->fee = rhs.fee;
    //     this->from = rhs.from;
    //     this->to = rhs.to;
    //     this->asset_type = rhs.asset_type;
    //     this->amount = rhs.amount;
        
    //     return *this;
    // }

    optional<string> message;
    //share_type calculate_fee() const;

    operation_result evaluate2(
        block_num_type block_num,
        block_chain_type chain,
        string tid,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
};

struct transfer_info {
    optional<account_id_type> from;
    optional<account_id_type> to;
    optional<string> message;
};
 
FC_REFLECT(transfer_operation::fee_parameters_type, (fee)(price_per_kbyte))
//FC_REFLECT_DERIVED(transfer_operation, (base_operation), (fee)(from)(to)(asset_type)(amount))
FC_REFLECT(transfer_operation, (fee)(from)(to)(asset_type)(amount))
FC_REFLECT_DERIVED(transfer2_operation, (transfer_operation), (message))
FC_REFLECT(transfer_info,  (from)(to)(message))
 