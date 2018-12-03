#pragma once
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
# include "threadsafe_queue.h"

int mt_send_request(void);

struct bm_send_request2 {
    std::string o;
    int method;

    bm_send_request2() {

    }

    bm_send_request2(const bm_send_request2& s) {
        o = s.o;
        method = s.method;
    }
};

extern threadsafe_queue<bm_send_request2> g_requests;

int send_request(bm_send_request2 req);



