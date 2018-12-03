# ifndef _ME_CONFIG_H_
# define _ME_CONFIG_H_

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
 
struct fullnode_settings {
    // bool                debug;
    // process_cfg         process;
    // log_cfg             log;
    //alert_cfg           alert;
    //rpc_svr_cfg         svr;
    char                *block_db_path;

    int chain_type;
    rpc_clt_cfg         fullnode_server;
    int start_num; 
};

extern struct fullnode_settings fl_settings;


int init_fl_config(const char *path);

# endif
