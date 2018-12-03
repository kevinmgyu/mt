#pragma once
#include "account.hpp"
#include "base_operation.hpp"
 
struct account_create_operation : public base_operation {
    
    struct fee_parameters_type {
        uint64_t basic_fee = 5 * GRAPHENE_BLOCKCHAIN_PRECISION;      ///< the cost to register the cheapest non-free account
        uint64_t premium_fee = 2000 * GRAPHENE_BLOCKCHAIN_PRECISION; ///< the cost to register the cheapest non-free account
        uint32_t price_per_kbyte = GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    
    account_object account;

  //  string id;
    account_id_type registrar;
    // authority owner;
    // authority active;
    // optional<fc::public_key> public_key;
    // optional<string> pass_seed;

    account_id_type fee_payer() const { return registrar; }
    void validate() const;
    share_type calculate_fee( ) const;
    share_type get_fee() const;

    void get_required_active_authorities(flat_set<account_id_type> &a) const {
        // registrar should be required anyway as it is the fee_payer(), but we insert it here just to be sure
        a.insert(registrar);
        // if (extensions.value.buyback_options.valid())
        //     a.insert(extensions.value.buyback_options->asset_to_buy_issuer);
    }

     operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};
 

struct account_update_operation : public base_operation {
    
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    /// The account to update
    account_id_type id;

    /// New owner authority. If set, this operation requires owner authority to execute.
    optional<authority> owner;
    /// New active authority. This can be updated by the current active authority.
    optional<authority> active;

    optional<fc::public_key> public_key;
    optional<string> pass_seed;

    /// New account options
    
    account_id_type fee_payer() const { return id; }
    void validate() const;
    share_type calculate_fee( ) const;
    share_type get_fee() const;

    bool is_owner_update() const { 
        if( !owner ) {
            return false;
        }
        return true;
        /*owner || extensions.value.owner_special_authority.valid();*/ }

    void get_required_owner_authorities(flat_set<account_id_type> &a) const {
        if (is_owner_update()) a.insert(id);
    }

    void get_required_active_authorities(flat_set<account_id_type> &a) const {
        if (!is_owner_update()) a.insert(id);
    }

     operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};

struct account_reset_claim : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 5 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };
     
    share_type fee;
    account_id_type id;
    string          seed_plain;
    /// New owner authority. If set, this operation requires owner authority to execute.
    optional<authority> owner;
    /// New active authority. This can be updated by the current active authority.
    optional<authority> active;

    optional<fc::public_key> public_key;
    optional<string> pass_seed;

    /// New account option
    account_id_type fee_payer() const { return id; }
    void validate() const;
    share_type calculate_fee( ) const;
    share_type get_fee() const;

     operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};
   
//FC_REFLECT(account_object,(membership_expiration_date)(registrar)(referrer)(lifetime_referrer)(network_fee_percentage)(lifetime_referrer_fee_percentage)(referrer_rewards_percentage)(name)(owner)(active)(options)(statistics)(whitelisting_accounts)(blacklisting_accounts)(whitelisted_accounts)(blacklisted_accounts)(cashback_vb)(owner_special_authority)(active_special_authority)(top_n_control_flags)(allowed_assets))
 
FC_REFLECT(account_create_operation, (fee)(account)(registrar))
FC_REFLECT(account_update_operation, (fee)(id)(owner)(active)(public_key)(pass_seed))
FC_REFLECT(account_reset_claim, (fee)(id)(seed_plain)(owner)(active)(public_key)(pass_seed))