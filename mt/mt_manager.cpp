#include "mt_manager.h"
#include "mt_config.h"
#include "nw_svr.h"
#include "nw_clt.h"
#include "nw_job.h"
#include "nw_timer.h"
#include "nw_state.h"
#include "ut_log.h"
#include "ut_sds.h"
#include "ut_cli.h"
#include "ut_misc.h"
#include "ut_list.h"
#include "ut_kafka.h"
#include "ut_signal.h"
#include "ut_config.h"
#include "ut_decimal.h"
#include "ut_rpc_clt.h"
#include "ut_rpc_svr.h"
#include "ut_rpc_cmd.h"
#include "ut_http_svr.h"
#include "bmath.cpp"
#include <fc/io/json.hpp>
#include <sstream>
#include <iostream>
#include "chain_api.h"
//#include "jsonxx.h"
static int g_block_num = 0;

string double2string(double do_b)
{
	string str1;
	std::stringstream ss;
	ss<<do_b;
	ss>>str1;
	return str1;
}

int handle_mt_block(const signed_block& block) {
	// for test ,need to delete
	/*log_info("print block store block ");
	string tmp_date ;
	optional<simple_asset> asset2 = get_asset_marketing(1);
	if (!asset2) {
		log_error("not exists ask asset %d", 1);
		return  -__LINE__;
	}
	//share_type amount= uint64_t(100.00123 *asset2->base_precision); 
	//mpd_t *amount2 = devide_int64_to_mpd(amount, asset2->base_precision, 0);
	mpd_t *a = decimal("100.12345678", 0);
	append_mt_pledge(current_timestamp(),1, "1","Sagittarius", "Sagittarius",a,1);
	*/
	g_block_num = g_block_num +1;
	if(g_block_num > mt_settings.flush_num )
	{
		g_block_num = 0;
		append_block_info(block.timestamp.sec_since_epoch(),block.block_num);
		
	}
	for (size_t m = 0; m < block.transactions.size(); ++m) {
		processed_transaction trx = block.transactions[m];
		// processed_transaction ptrx(trx);
		// processed_transaction ptrx1 = block.transactions[m];
		string trx_id = trx.id().str();
		uint32_t block_time  = trx.timestamp.sec_since_epoch();

		for (size_t j = 0; j < trx.operations.size(); ++j) {


			int which = trx.operations[j].which();
			//	log_trace("which %d %d", which,trx.operation_results[j].reason);
			if((trx.operation_results[j].reason != ORR_OK) && (trx.operation_results[j].reason != ORR_PASS_TO_NEXT_CHAIN))					     	     {	
				continue;
			}

			if( which == 0 ) {
				transfer_operation op = trx.operations[j].get<transfer_operation>();
				if(strcmp(mt_settings.pledge_ad,op.to.c_str()))
				{
					continue;
				}
				optional<simple_asset> asset1 = get_asset_marketing(op.asset_type);
				if (!asset1) {
					log_error("not exists ask asset %d", op.asset_type);
					continue;
				}

				mpd_t *amount1 = devide_int64_to_mpd(op.amount, asset1->base_precision, 10);

				
				append_mt_pledge(block.timestamp.sec_since_epoch(),block.block_num, trx_id.c_str(),
						op.from.c_str(), op.to.c_str(),amount1,op.asset_type);
				mpd_del(amount1);


			}
		} 

	}	

	return 0;
}

