/*
 * Description: 
 *     History: yang@haipo.me, 2017/03/16, create
 */

# ifndef _MT_CONFIG_H_
# define _MT_CONFIG_H_

# include <math.h>
# include <stdio.h>
# include <error.h>
# include <errno.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <assert.h>
# include <map>
# include <inttypes.h>
# include <string>

# include "nw_svr.h"
# include "nw_clt.h"
# include "nw_job.h"
# include "nw_timer.h"


# include "ut_sds.h"
# include "ut_cli.h"
# include "ut_misc.h"
# include "ut_list.h"
# include "ut_mysql.h"
# include "ut_signal.h"
# include "ut_define.h"
# include "ut_config.h"
# include "ut_decimal.h"
# include "ut_rpc_clt.h"
# include "ut_rpc_svr.h"
# include "ut_rpc_cmd.h"
# include "ut_skiplist.h"
#include <fc/io/json.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/exception/exception.hpp>
#include <fc/crypto/hex.hpp> 
#include <fc/optional.hpp>

using namespace std;
using namespace fc;

struct mt_settings {
	bool                debug;
	process_cfg         process;
	log_cfg             log;
	//alert_cfg           alert;
	//rpc_svr_cfg         svr;
	//char                *block_db_path;

	//	int chain_type;
	//	rpc_clt_cfg         fullnode_server;
	//	int start_num; 
	char                *httpurl;
	char                *pledge_ad;
	uint64_t            end_num;
	int                  flush_num;
	mysql_cfg db_history;
	int history_thread;
};

struct simple_asset {
	int id;
	std::string symbol;
	// int prec;
	//double price;
	//string market;
	std::string chain_contract_address;
	int chain_type;
	uint64_t base_precision;
	uint64_t min_transfer_amount;

	simple_asset() {

	}

	simple_asset(const simple_asset& a) {
		id = a.id;
		symbol = a.symbol;
		// prec = a.prec;
		//price = a.price;
		//market = a.market;
		base_precision = a.base_precision;
		min_transfer_amount = a.min_transfer_amount;
		chain_contract_address = a.chain_contract_address;
		chain_type = a.chain_type;
	}
};
extern struct mt_settings mt_settings;
extern std::map<int, simple_asset> assets;
extern std::map<std::string,int> assets_map;
extern std::map<std::string,std::map<int,int>> assets_adr;

optional<simple_asset> get_asset_marketing(int a);
optional<simple_asset> get_asset_by_chaintype_and_contract(int chain_type,string contract);

int init_mt_config(const char *path);

# endif
