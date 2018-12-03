#ifndef _RMT_SEVER_CONFIG_H_
#define _RMT_SEVER_CONFIG_H_

#include <math.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <map>
#include <inttypes.h>

#include "nw_svr.h"
#include "nw_clt.h"
#include "nw_job.h"
#include "nw_timer.h"

#include "ut_sds.h"
#include "ut_cli.h"
#include "ut_misc.h"
#include "ut_list.h"
#include "ut_mysql.h"
#include "ut_signal.h"
#include "ut_define.h"
#include "ut_config.h"
#include "ut_decimal.h"
#include "ut_rpc_clt.h"
#include "ut_rpc_svr.h"
#include "ut_rpc_cmd.h"
#include "ut_skiplist.h"
#include <set>


using std::string;

struct mt_rdg_settings {
	bool                debug;
	process_cfg         process;
	log_cfg             log;
	//alert_cfg           alert;
	rpc_svr_cfg         svr;
	mysql_cfg           db_history;
	int                 worker_num;
	int  		asset_type;
	char*              pledge_ad;
	uint64_t       base_precision;
	uint64_t       save_precision;
	int            start_num;
	int 	       end_num;	

};

extern struct mt_rdg_settings mt_rdg_settings;


int init_read_mt_server_config(const char *path);

#endif
