
#pragma once
#include "asset.hpp"
#include "base_operation.hpp"

struct asset_options {
    /// The maximum supply of this asset which may exist at any given time. This can be as large as
    /// GRAPHENE_MAX_SHARE_SUPPLY
    share_type max_supply = GRAPHENE_MAX_SHARE_SUPPLY;
    /// When this asset is traded on the markets, this percentage of the total traded will be exacted and paid
    /// to the issuer. This is a fixed point value, representing hundredths of a percent, i.e. a value of 100
    /// in this field means a 1% fee is charged on market trades of this asset.
    uint16_t market_fee_percent = 0;
    /// Market fees calculated as @ref market_fee_percent of the traded volume are capped to this value
    share_type max_market_fee = GRAPHENE_MAX_SHARE_SUPPLY;

    /// The flags which the issuer has permission to update. See @ref asset_issuer_permission_flags
   // uint16_t issuer_permissions = UIA_ASSET_ISSUER_PERMISSION_MASK;
    /// The currently active flags on this permission. See @ref asset_issuer_permission_flags
    uint16_t flags = 0;

    /// When a non-core asset is used to pay a fee, the blockchain must convert that asset to core asset in
    /// order to accept the fee. If this asset's fee pool is funded, the chain will automatically deposite fees
    /// in this asset to its accumulated fees, and withdraw from the fee pool the same amount as converted at
    /// the core exchange rate.
    //      price core_exchange_rate;

    /// A set of accounts which maintain whitelists to consult for this asset. If whitelist_authorities
    /// is non-empty, then only accounts in whitelist_authorities are allowed to hold, use, or transfer the asset.
    //flat_set<account_id_type> whitelist_authorities;
    /// A set of accounts which maintain blacklists to consult for this asset. If flags & white_list is set,
    /// an account may only send, receive, trade, etc. in this asset if none of these accounts appears in
    /// its account_object::blacklisting_accounts field. If the account is blacklisted, it may not transact in
    /// this asset even if it is also whitelisted.
    //flat_set<account_id_type> blacklist_authorities;

    /** defines the assets that this asset may be traded against in the market */
    //flat_set<asset_id_type>   whitelist_markets;
    /** defines the assets that this asset may not be traded against in the market, must not overlap whitelist */
    //flat_set<asset_id_type>   blacklist_markets;

    /**
       * data that describes the meaning/purpose of this asset, fee will be charged proportional to
       * size of description.
       */
    string description;
    extensions_type extensions;

    /// Perform internal consistency checks.
    /// @throws fc::exception if any check fails
    void validate() const;
};

/**
    * @ingroup operations
    */
struct create_asset_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t symbol3 = 500000 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint64_t symbol4 = 300000 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint64_t long_symbol = 5000 * GRAPHENE_BLOCKCHAIN_PRECISION;
        uint32_t price_per_kbyte = 10; /// only required for large memos.
    };

    share_type fee;
    account_id_type creator;
    /*
      /// This account must sign and pay the fee for this operation. Later, this account may update the asset
      
      /// The ticker symbol of this asset
      string                  symbol;
      string                  name;
      /// Number of digits to the right of decimal point, must be less than or equal to 12
      uint8_t                 show_precision = 0;
      uint8_t                 save_precision = 0;

      optional<string>        eth_contract_address = "";*/
    asset create_asset;
    signature_type  signature;

    OperationResultRet isvalid();
    account_id_type fee_payer() const { 
        if(payer) {return *payer;}
        return creator; 
    }
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
    //share_type      calculate_fee( const fee_parameters_type& k )const;
};

struct create_market_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 5 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    account_id_type creator;
    string name;
    asset_id_type stock;
    asset_id_type money;
    uint8_t stock_precision = 0;
    uint8_t money_precision = 0;
    uint8_t fee_precision = 0;

    //  time_point_sec expiration = time_point_sec::maximum();

    /// If this flag is set the entire order must be filled or the operation is rejected
    //  bool fill_or_kill = false;
    //  extensions_type   extensions;

    //  pair<asset_id_type,asset_id_type> get_market()const
    //  {
    //     return amount_to_sell.asset_id < min_to_receive.asset_id ?
    //           std::make_pair(amount_to_sell.asset_id, min_to_receive.asset_id) :
    //           std::make_pair(min_to_receive.asset_id, amount_to_sell.asset_id);
    //  }
    account_id_type fee_payer() const { 
        if( payer) {return *payer;}
        return creator; 
    }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    OperationResultRet isvalid();
    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
    //  price           get_price()const { return amount_to_sell / min_to_receive; }
};

FC_REFLECT(create_asset_operation::fee_parameters_type, (symbol3)(symbol4)(long_symbol)(price_per_kbyte))
FC_REFLECT(create_market_operation::fee_parameters_type, (fee))
//FC_REFLECT( call_order_update_operation::fee_parameters_type, (fee) )
//FC_REFLECT( bid_collateral_operation::fee_parameters_type, (fee) )
//FC_REFLECT( fill_order_operation::fee_parameters_type,  ) // VIRTUAL
//FC_REFLECT( execute_bid_operation::fee_parameters_type,  ) // VIRTUAL

//FC_REFLECT( create_asset_operation, (fee)(issuer)(symbol)(name)(show_precision)(save_precision)(eth_contract_address))
FC_REFLECT(create_asset_operation, (fee)(creator)(create_asset)(signature))
FC_REFLECT(create_market_operation, (fee)(creator)(name)(stock)(money)(stock_precision)(money_precision)(fee_precision))

//FC_REFLECT_DERIVED(create_asset_operation, (base_operation),(fee)(creator)(create_asset) )
//FC_REFLECT_DERIVED(create_market_operation, (base_operation), (fee)(creator)(name)(stock)(money)(stock_precision)(money_precision)(fee_precision) )

//FC_REFLECT( call_order_update_operation, (fee)(funding_account)(delta_collateral)(delta_debt)(extensions) )
//FC_REFLECT( fill_order_operation, (fee)(order_id)(account_id)(pays)(receives)(fill_price)(is_maker) )
//FC_REFLECT( bid_collateral_operation, (fee)(bidder)(additional_collateral)(debt_covered)(extensions) )
//FC_REFLECT( execute_bid_operation, (fee)(bidder)(debt)(collateral) )
