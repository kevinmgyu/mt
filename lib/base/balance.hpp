#pragma once
#include "types.hpp"
#include "base_operation.hpp"
#include "memo.hpp"
#include "asset.hpp"

using namespace fc;

struct balance {
    asset_id_type 		 asset_type;
    uint8_t              balance_type;
    string 				 name;
    share_type		     value;
    account_id_type		 owner;
    
    balance() {
    }

    balance(const balance& a) {
        asset_type = a.asset_type;
        balance_type = a.balance_type;
        name = a.name;
        value = a.value;
        owner = a.owner;
    }
};

struct set_balance_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee       = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type       fee;
    
    balance          amount;
    
    account_id_type  setter;
    
    optional<memo_data> memo;
   
    account_id_type fee_payer()const { 
        if( payer ) {
            return *payer;
        } 
        return setter; 
    }

    void            validate()const;
    share_type calculate_fee() const ;
    share_type  get_fee()const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id, 
        string dapp_id,
        bool isreal,
        bool ismain
    );
};

 
FC_REFLECT( balance,  (asset_type)(balance_type)(name)(value)(owner))
FC_REFLECT( set_balance_operation::fee_parameters_type, (fee)(price_per_kbyte))
//FC_REFLECT_DERIVED(set_balance_operation, (base_operation),(fee)(amount)(setter)(memo) )
FC_REFLECT(set_balance_operation,  (fee)(amount)(setter)(memo) )
