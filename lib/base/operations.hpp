#pragma once
#include "base_operation.hpp"
#include "create.hpp"
#include "market.hpp"
#include "transfer.hpp"
#include "balance.hpp"
#include "witness.hpp"
#include "vote_and_approve.hpp"
#include "deposit_withdraw.hpp"
#include "authority.hpp"
#include "account_op.hpp"

typedef fc::static_variant<
    transfer_operation,
    order_create_operation,
    order_cancel_operation,
    deposit_operation,
    withdraw_operation,
    create_asset_operation,
    create_market_operation,
    witness_create_operation,
    witness_update_operation,
    deal_operation,
    order_cancel_settlement_operation,
    witness_minning_rewards_operation,
    pledge_asset_operation,
    redeem_asset_operation,
    redeem_asset_settlement_operation,
    set_balance_operation,
    vote_operation,
    execute_operation,
    proposal_operation,
    withdraw_settlement_operation,
    super_deposit_operation,
    order_freeze_btt_fee_settlement_operation,
    account_create_operation,
    account_update_operation,
    account_reset_claim,
    btc_withdraw_operation,
    withdraw2_operation,
    order_create2_operation,
    transfer2_operation,
    deposit2_operation,
    vote2_operation,
    withdraw_settlement2_operation,
    order_create3_operation,
    order_cancel2_operation        
 
    //request_income_operation,
    //proposal_to_deposit_operation,
    //proposal_to_withdraw_operation,
    //proposal_to_withdraw_settlement_operation,
   // 
    /*         call_order_update_operation,
      fill_order_operation           // VIRTUAL
      account_create_operation,
      account_update_operation,
      account_whitelist_operation,
      account_upgrade_operation,
      account_transfer_operation,
      asset_create_operation,
      asset_update_operation,
      asset_update_bitasset_operation,
      asset_update_feed_producers_operation,
      asset_issue_operation,
      asset_reserve_operation,
      asset_fund_fee_pool_operation,
      asset_settle_operation,
      asset_global_settle_operation,
      asset_publish_feed_operation,
      
      proposal_create_operation,
      proposal_update_operation,
      proposal_delete_operation,
      withdraw_permission_create_operation,
      withdraw_permission_update_operation,
      withdraw_permission_claim_operation,
      withdraw_permission_delete_operation,
      committee_member_create_operation,
      committee_member_update_operation,
      committee_member_update_global_parameters_operation,
      vesting_balance_create_operation,
      vesting_balance_withdraw_operation,
      worker_create_operation,
      custom_operation,
      assert_operation,
      balance_claim_operation,
      override_transfer_operation,
      transfer_to_blind_operation,
      blind_transfer_operation,
      transfer_from_blind_operation,
      asset_settle_cancel_operation,  // VIRTUAL
      asset_claim_fees_operation,
      fba_distribute_operation,       // VIRTUAL
      bid_collateral_operation,
      execute_bid_operation           // VIRTUAL*/
    >
    operation;

    enum OperationIndex {
        OI_TRANSFER_OPERATION = operation::tag<transfer_operation>::value,
        OI_ORDER_CREATE_OPERATION = operation::tag<order_create_operation>::value,
        OI_ORDER_CANCEL_OPERATION = operation::tag<order_cancel_operation>::value,
        OI_DEPOSIT_OPERATION = operation::tag<deposit_operation>::value,
        OI_WITHDRAW_OPERATION = operation::tag<withdraw_operation>::value,
        OI_CREATE_ASSET_OPERATION = operation::tag<create_asset_operation>::value,
        OI_CREATE_MARKET_OPERATION = operation::tag<create_market_operation>::value,
        OI_WITNESS_CREATE_OPERATION = operation::tag<witness_create_operation>::value,
        OI_WITNESS_UPDATE_OPERATION = operation::tag<witness_update_operation>::value,
        OI_DEAL_OPERATION = operation::tag<deal_operation>::value,
        OI_ORDER_CANCEL_SETTLEMENT_OPERATION = operation::tag<order_cancel_settlement_operation>::value,
        OI_WITNESS_MINNING_REWARDS_OPERATION = operation::tag<witness_minning_rewards_operation>::value,
        OI_PLEDGE_ASSET_OPERATION = operation::tag<pledge_asset_operation>::value,
        OI_REDEEM_ASSET_OPERATION = operation::tag<redeem_asset_operation>::value,
        OI_REDEEM_ASSET_SETTLEMENT_OPERATION = operation::tag<redeem_asset_settlement_operation>::value,
        OI_SET_BALANCE_OPERATION = operation::tag<set_balance_operation>::value,
        OI_VOTE_OPERATION = operation::tag<vote_operation>::value,
        OI_EXECUTE_OPERATION = operation::tag<execute_operation>::value,
        OI_PROPOSAL_OPERATION = operation::tag<proposal_operation>::value,
        OI_WITHDRAW_SETTLEMENT_OPERATION = operation::tag<withdraw_settlement_operation>::value,
        OI_SUPER_DEPOSIT_OPERATION = operation::tag<super_deposit_operation>::value,
        OI_ORDER_FREEZE_BTT_FEE_SETTLEMENT_OPERATION = operation::tag<order_freeze_btt_fee_settlement_operation>::value,
        OI_ACCOUNT_CREATE_OPERATION = operation::tag<account_create_operation>::value,
        OI_ACCOUNT_UPDATE_OPERATION = operation::tag<account_update_operation>::value,
        OI_ACCOUNT_RESET_CLAIM = operation::tag<account_reset_claim>::value,
        OI_BTC_WITHDRAW_OPERATION = operation::tag<btc_withdraw_operation>::value,
        OI_WITHDRAW2_OPERATION = operation::tag<withdraw2_operation>::value,
        OI_ORDER_CREATE2_OPERATION = operation::tag<order_create2_operation>::value,
        OI_TRANSFER2_OPERATION = operation::tag<transfer2_operation>::value,
        OI_DEPOSIT2_OPERATION = operation::tag<deposit2_operation>::value,
        OI_VOTE2_OPERATION = operation::tag<vote2_operation>::value,
        OI_WITHDRAW_SETTLEMENT2_OPERATION = operation::tag<withdraw_settlement2_operation>::value,
        OI_ORDER_CREATE3_OPERATION = operation::tag<order_create3_operation>::value,
        OI_ORDER_CANCEL2_OPERATION = operation::tag<order_cancel2_operation>::value,

        
    };

/// @} // operations group

/**
    *  Appends required authorites to the result vector.  The authorities appended are not the
    *  same as those returned by get_required_auth 
    *
    *  @return a set of required authorities for @ref op
    */
void operation_get_required_authorities( const operation& op,
                                         flat_set<account_id_type>& active,
                                         flat_set<account_id_type>& owner,
                                         vector<authority>&  other );
// void operation_get_required_authorities(const operation &op,
//                                         flat_set<address> &active);

void operation_validate(const operation &op);

void operation_vertify_fee(const operation &op,
                           bool &res);
/**
    *  @brief necessary to support nested operations inside the proposal_create_operation
    */
struct op_wrapper {
  public:
    op_wrapper(const operation &op = operation()) : op(op) {}
    operation op;
};

FC_REFLECT_TYPENAME(operation)
FC_REFLECT(op_wrapper, (op))
