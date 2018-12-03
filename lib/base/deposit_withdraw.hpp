#pragma once
#include "asset.hpp"
#include "base_operation.hpp"
#include "memo.hpp"
#include "vote_and_approve.hpp"

using namespace fc;

struct deposit_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;
    /// Account to transfer asset from
    account_id_type sign;
    account_id_type to;

    string from_external_address;
    string external_transaction_id;
    //proposal_id_type proposal_id;

    //asset_id_type asset_type;
    string chain_contract_address;
    uint32_t chain_type;
    string amount;

    account_id_type fee_payer() const { return sign; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        optional<transaction_id_type> p_transaction_id,
        string tid,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        vector<string> utxos
        );
    int proposal_update(bool isreal, string tid, int cmd,const char *status);
    //      share_type      calculate_fee(const fee_parameters_type& k)const;
};

struct deposit2_operation : public deposit_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    std::string omni_amount;
    vector<string> utxos;

    operation_result evaluate2(
        block_num_type block_num,
        block_chain_type chain,
        optional<transaction_id_type> p_transaction_id,
        string tid,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
    //int proposal_update(bool isreal, string tid, int cmd,const char *status);
    //      share_type      calculate_fee(const fee_parameters_type& k)const;
};


struct super_deposit_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;
    /// Account to transfer asset from
    account_id_type sign;

    account_id_type to;

    string from_external_address;

    asset_id_type asset_type;
    share_type amount;

    account_id_type fee_payer() const { return sign; }
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
    //      share_type      calculate_fee(const fee_parameters_type& k)const;
};

struct withdraw_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;
    /// Account to transfer asset from
    account_id_type from;

    string to_external_address;
    /// The amount of asset to transfer from @ref from to @ref to
    asset_id_type asset_type;
    share_type amount;


    account_id_type fee_payer() const { return from; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    //bool is_btc_withdraw() const;
    
    OperationResultRet isvalid() const;
    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        optional<transaction_id_type> p_transaction_id,
        string dapp_id,
        bool isreal,
        bool ismain,
        optional<share_type> asset_fee
        );
    int proposal_update(string id, bool isreal,int cmd, const char *status);
    //      share_type      calculate_fee(const fee_parameters_type& k)const;
};

struct withdraw2_operation : public withdraw_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    optional<share_type> asset_fee;

    withdraw2_operation& operator=(const withdraw_operation& rhs) {
        this->payer = rhs.payer;
        this->fee = rhs.fee;
        this->from = rhs.from;
        this->to_external_address = rhs.to_external_address;
        this->asset_type = rhs.asset_type;
        this->amount = rhs.amount;
        
        return *this;
    }

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        transaction_id_type transaction_id,
        string dapp_id,
        bool isreal,
        bool ismain
        );
};

//struct btc_withdraw_request {
//    uint32_t chain_type;
//    std::string raw_trans;
//    std::string amount;
//    std::string to_external_addr;
//    std::string contract_addr;
//    transaction_id_type transaction_id;
//    std::vector<std::string> utxo_list;
//};

struct btc_withdraw_operation : public base_operation {
    const static uint64_t OMNI_BTC_FEE = 546;
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;
    account_id_type witness;
   //bool freeze_amount;
    //int64_t sequence;
    /// Account to transfer asset from
    account_id_type from;

    string to_external_address;
    /// The amount of asset to transfer from @ref from to @ref to
    asset_id_type asset_type;
    share_type amount;
    share_type btc_total;
    share_type btc_fee;
    //int64_t btc_feerate;
    std::vector<std::string> utxo_list;
    transaction_id_type  withdraw2_id;

    // btc_withdraw_operation& operator=(const btc_withdraw_operation& right);
    account_id_type fee_payer() const { return from; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    void get_msg_and_amount(std::vector<std::string>& msgs, uint64_t& btc_amount);
    OperationResultRet isvalid();
    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
       // string tid,
         
        string dapp_id,
        bool isreal,
        bool ismain);
    int proposal_update(string id, bool isreal,int cmd, const char *status);
    //      share_type      calculate_fee(const fee_parameters_type& k)const;
};

struct btc_withdraw_request {
    uint32_t chain_type;
    std::string raw_trans;
    std::string contract_addr;
    transaction_id_type transaction_id;
    
    btc_withdraw_operation btc_withdraw;
};

struct withdraw_settlement_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    share_type fee;

    account_id_type sign;
    transaction_id_type withdraw_transaction_id;
    string chain_contract_address;
    uint32_t chain_type;
    string to_external_address;
    string amount;
    string external_transaction_id;
    
    account_id_type fee_payer() const { return sign; }
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
        vector<string> utxos
        );
    int proposal_update(bool isreal, string tid, int cmd,const char *status);
};

struct withdraw_settlement2_operation : public withdraw_settlement_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
    };

    vector<string> utxos;

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

struct eth_withdraw_request {

    string request_id;
    account_id_type from;
    fc::time_point_sec timestamp;
    string to_external_address;
    string chain_contract_address;
    uint32_t chain_type;
    share_type amount;

    vector<string> approved_address;
    vector<string> r;
    vector<string> s;
    vector<uint8_t> v;
};


FC_REFLECT(deposit_operation::fee_parameters_type, (fee)(price_per_kbyte))
//FC_REFLECT(proposal_to_deposit_operation::fee_parameters_type, (fee))

FC_REFLECT(withdraw_operation::fee_parameters_type, (fee)(price_per_kbyte))
FC_REFLECT(withdraw_settlement_operation::fee_parameters_type, (fee)(price_per_kbyte))
//FC_REFLECT(proposal_to_withdraw_operation::fee_parameters_type, (fee))

FC_REFLECT(deposit_operation, (fee)(sign)(to)(from_external_address)(external_transaction_id)(chain_contract_address)(chain_type)(amount))
FC_REFLECT_DERIVED(deposit2_operation, (deposit_operation), (omni_amount)(utxos))

FC_REFLECT(super_deposit_operation, (fee)(sign)(to)(from_external_address)(asset_type)(amount))

//FC_REFLECT_DERIVED(deposit_operation, (base_operation),  (fee)(sign)(to)(from_external_address)(external_transaction_id)(chain_contract_address)(chain_type)(amount))
//FC_REFLECT_DERIVED(super_deposit_operation, (base_operation), (fee)(sign)(to)(from_external_address)(asset_type)(amount))

//FC_REFLECT(proposal_to_deposit_operation, (fee)(sign)(to)(from_external_address)(external_transaction_id)(chain_contract_address)(chain_type)(amount))

FC_REFLECT(withdraw_operation, (fee)(from)(to_external_address)(asset_type)(amount))
FC_REFLECT_DERIVED(withdraw2_operation, (withdraw_operation), (asset_fee))
//FC_REFLECT(btc_withdraw_operation, (fee)(witness)(freeze_amount)(sequence)(from)(to_external_address)(asset_type)(amount)(btc_total)(btc_fee)(btc_feerate)(utxo_list))
//FC_REFLECT(btc_withdraw_operation, (fee)(witness)(from)(to_external_address)(asset_type)(amount)(btc_total)(btc_fee)(btc_feerate)(utxo_list)(withdraw2_id))
FC_REFLECT(btc_withdraw_operation, (fee)(witness)(from)(to_external_address)(asset_type)(amount)(btc_total)(btc_fee)(utxo_list)(withdraw2_id))
FC_REFLECT(withdraw_settlement_operation, (fee)(sign)(withdraw_transaction_id)(chain_contract_address)(chain_type)(to_external_address)(amount)(external_transaction_id))
FC_REFLECT_DERIVED(withdraw_settlement2_operation, (withdraw_settlement_operation), (utxos))
//FC_REFLECT(proposal_to_withdraw_operation, (fee)(sign)(from)(external_transaction_id)(to_external_address)(asset_type)(amount))
//FC_REFLECT_DERIVED(withdraw_operation, (base_operation),  (fee)(from)(to_external_address)(asset_type)(amount) )
//FC_REFLECT_DERIVED(withdraw_settlement_operation, (base_operation), (fee)(sign)(withdraw_transaction_id)(chain_contract_address)(chain_type)(to_external_address)(amount)(external_transaction_id))

//FC_REFLECT(eth_withdraw_request, (request_id)(from)(timestamp)(to_external_address)(chain_contract_address)(chain_type)(amount)(approved_address)(r)(s)(v))

FC_REFLECT(eth_withdraw_request,(request_id)(from)(timestamp)(to_external_address)(chain_contract_address)(chain_type)(amount)(approved_address)(r)(s)(v) )

FC_REFLECT(btc_withdraw_request,(chain_type)(raw_trans)(contract_addr)(transaction_id)(btc_withdraw))