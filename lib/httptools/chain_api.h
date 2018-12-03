#ifndef __CHAIN_API_H__
#define __CHAIN_API_H__


#include "restclient-cpp/restclient.h"

#include <fc/io/json.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/exception/exception.hpp>
#include <fc/crypto/hex.hpp>

#include "types.hpp"
#include "transaction.hpp"
#include "chain_types.hpp"

#include <ctime>

using namespace std;

using namespace RestClient;




void set_btt_base_url(const string& url);

Response get_asset_list();

template<class T>
Response put_method(string method,T params);

Response get_balance(string userid,vector<string> assets=vector<string>());

Response get_depth(String market,int limit,string interval);

Response put_deposit_transaction(account_id_type sign,account_id_type to,string from_external_address,
								 asset_id_type asset_type,share_type amount,fc::ecc::private_key priv );

Response put_deposit_proposal_transaction(account_id_type sign, account_id_type to, string from_external_address,
        string ex_trans_id, uint32_t chain_type, string chain_contract_addr, share_type amount, fc::ecc::private_key priv);

Response put_withdraw_proposal_transaction(account_id_type singer, account_id_type from, string to_external_address, 
        asset_id_type asset_id, share_type amount, fc::ecc::private_key priv);

Response put_withdraw2_proposal_transaction(account_id_type singer, account_id_type from, string to_external_address, 
        asset_id_type asset_id, share_type amount, share_type feerate, fc::ecc::private_key priv);

Response put_withdraw2_transaction(account_id_type singer, account_id_type from, string to_external_address, 
        asset_id_type asset_id, share_type amount, share_type feerate, fc::ecc::private_key priv);

Response put_asset_create_transaction(account_id_type creator, const asset& _asset, fc::ecc::private_key priv);

Response put_limit_order(account_id_type creator, uint8_t side,
						 string market_name, share_type amount, 
                         share_type price, fc::ecc::private_key priv );

Response put_market_order(account_id_type creator, uint8_t side,
						  string market_name, share_type amount,
                          fc::ecc::private_key priv );

Response cancel_order(account_id_type creator,string market_name,order_id_type order_id,fc::ecc::private_key priv );

Response transfer(account_id_type from,account_id_type to,
                  asset_id_type asset_type,share_type amount,fc::ecc::private_key priv );

Response transfer2(account_id_type from, account_id_type to,
        asset_id_type asset_type, share_type amount, optional<string> message, fc::ecc::private_key priv);

Response get_market_deals(string market);

int get_local_balance(string id,const vector<string>& assets,vector<string>& res);

Response get_running_order(string userid, string market, int offset, int limit);

int get_running_order(string userid, const vector<string>& markets, int offset, int limit,vector<running_order>& res);

#endif
