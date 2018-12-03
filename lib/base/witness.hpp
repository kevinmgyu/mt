#pragma once
#include "types.hpp"
//#include "asset.hpp"
#include "base_operation.hpp"

struct key_pair {
    fc::ecc::private_key priv;
    public_key_type pub;
    string id;
    authority owner;
    authority active;

    key_pair() {
    }

    key_pair(const key_pair &a) {
        priv = a.priv;
        pub = a.pub;
        id = a.id;
        owner = a.owner;
        active = a.active;
    }

    friend bool operator<(const key_pair &a, const key_pair &b) {
        return a.id < b.id;
    }

    friend bool operator==(const key_pair &a, const key_pair &b) {
        return a.id == b.id;
    }

    friend bool operator!=(const key_pair &a, const key_pair &b) {
        return !(a == b);
    }
};

struct witness {

    public_key_type pub_key;
    witness_id_type id;
    //   share_type          weight;
    string url;
    int chain_type;

    witness() {
    }

    witness(  key_pair &a, string u, int c) {
        //witness(const key_pair& a,string u) {
        id = a.id;

        pub_key = a.pub;
        //weight = w;
        url = u;
        chain_type = c;
    }

    witness(const witness &a) {
        id = a.id;
        //  weight = a.weight;
        url = a.url;
        pub_key = a.pub_key;
        chain_type = a.chain_type;
    }

    friend bool operator<(const witness &a, const witness &b) {
        //FC_ASSERT( a.asset_id == b.asset_id );
        return a.id < b.id;
    }

    friend bool operator==(const witness &a, const witness &b) {
        return a.id == b.id;
    }

    friend bool operator!=(const witness &a, const witness &b) {
        return !(a == b);
    }
};

struct witness_create_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 5000 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    /// The account which owns the witness. This account pays the fee for this operation.
    account_id_type witness_account;
    string url;
    //  share_type            weight;
    public_key_type block_signing_key;
    int chain_type;

    account_id_type fee_payer() const { return witness_account; }
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

/**
    * @brief Update a witness object's URL and block signing key.
    * @ingroup operations
    */
struct witness_update_operation : public base_operation {
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    /// The witness object to update.
    //  witness_id_type   witness;
    /// The account which owns the witness. This account pays the fee for this operation.
    account_id_type witness_account;
    /// The new URL.
    optional<string> new_url;

    /// The new block signing key.
    //optional< public_key_type > new_signing_key;

    account_id_type fee_payer() const { return witness_account; }
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

struct witness_minning_rewards_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;

    vector<account_id_type> to;
    vector<asset_id_type> asset_type;
    vector<share_type> amount;

    account_id_type fee_payer() const { return to[0]; }
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

/**
    * @brief Update a witness object's URL and block signing key.
    * @ingroup operations
    */
struct pledge_asset_operation : public base_operation {
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    /// The witness object to update.
    //  witness_id_type   witness;
    /// The account which owns the witness. This account pays the fee for this operation.
    account_id_type creator;

    account_id_type witness_account;
    /// The new URL.
    share_type amount;
    /// The new block signing key.
    //optional< public_key_type > new_signing_key;
    optional<int> start_term;
    optional<int> end_term;

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

struct redeem_asset_operation : public base_operation {
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;

    account_id_type creator;

    pledge_id_type pledge_id;
    /// The witness object to update.
    //  witness_id_type   witness;
    /// The account which owns the witness. This account pays the fee for this operation.
    //  account_id_type       witness_account;
    /// The new URL.
    //  share_type            amount;

    /// The new block signing key.
    //optional< public_key_type > new_signing_key;

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

struct redeem_asset_settlement_operation : public base_operation {
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;

    account_id_type creator;

    pledge_id_type pledge_id;

    share_type amount;

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

struct request_income_operation : public base_operation {
    struct fee_parameters_type {
        share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    account_id_type creator;
    account_id_type witness_account; 
    vector<share_type> assets;
    vector<share_type> amount;

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

/*
   struct witness_claim_operation : public base_operation {
      struct fee_parameters_type
      {
         share_type fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
      };

      share_type            fee;

      vector<account_id_type>     witness;
      vector<share_type>          weight;
      share_type                  total;
      int                         chain_type;
      term_id_type                start_block;
      int                         duration;

     
      account_id_type fee_payer()const { return GRAPHENE_COMMITTEE_ACCOUNT; }
      void            validate()const;
      share_type  calculate_fee()const;
      share_type  get_fee()const;
   };
*/

//FC_REFLECT( witness, (id)(weight)(url)(pub_key) )
FC_REFLECT(witness, (id)(url)(pub_key)(chain_type))
//FC_REFLECT( witness, (id)(weight)(url) )
FC_REFLECT(key_pair, (priv)(pub)(id)(owner)(active))

FC_REFLECT(witness_create_operation::fee_parameters_type, (fee))
//FC_REFLECT( witness_create_operation, (fee)(witness_account)(url)(weight)(block_signing_key)(chain_type) )
FC_REFLECT(witness_create_operation, (fee)(witness_account)(url)(block_signing_key)(chain_type))
//FC_REFLECT_DERIVED(witness_create_operation, (base_operation), (fee)(witness_account)(url)(block_signing_key)(chain_type))

FC_REFLECT(witness_update_operation::fee_parameters_type, (fee))
FC_REFLECT(witness_update_operation, (fee)(witness_account)(new_url))
//FC_REFLECT_DERIVED(witness_update_operation, (base_operation), (fee)(witness_account)(new_url))

FC_REFLECT(witness_minning_rewards_operation, (fee)(to)(asset_type)(amount))
//FC_REFLECT( witness_claim_operation, (fee)(witness)(weight)(total)(chain_type)(start_block)(duration) )
//FC_REFLECT_DERIVED(witness_minning_rewards_operation, (base_operation),(fee)(to)(asset_type)(amount))

FC_REFLECT(pledge_asset_operation, (fee)(creator)(witness_account)(amount)(start_term)(end_term))
FC_REFLECT(redeem_asset_operation, (fee)(creator)(pledge_id))
FC_REFLECT(redeem_asset_settlement_operation, (fee)(creator)(pledge_id)(amount))
FC_REFLECT(request_income_operation, (fee)(creator)(witness_account)(assets)(amount))

// FC_REFLECT_DERIVED(pledge_asset_operation, (base_operation), (fee)(creator)(witness_account)(amount)(start_term)(end_term))
// FC_REFLECT_DERIVED(redeem_asset_operation, (base_operation), (fee)(creator)(pledge_id))
// FC_REFLECT_DERIVED(redeem_asset_settlement_operation, (base_operation),(fee)(creator)(pledge_id)(amount))
// FC_REFLECT_DERIVED(request_income_operation, (base_operation), (fee)(creator)(witness_account)(assets)(amount))