# include "mt_config.h"
#include "mt_send_request.h"

threadsafe_queue<bm_send_request2> g_requests;

static rpc_clt *matchengine;
static bool is_in_mt_request = false;
static double last_send_time = 0;
static nw_timer send_reuqest_timer;

static void on_backend_connect(nw_ses *ses, bool result) {
    rpc_clt *clt = (rpc_clt *)ses->privdata;
    if (result) {
        log_info("connect %s:%s success", clt->name, nw_sock_human_addr(&ses->peer_addr));
    } else {
        is_in_mt_request = false;
        log_info("connect %s:%s fail", clt->name, nw_sock_human_addr(&ses->peer_addr));
    }
}

static void on_backend_recv_pkg(nw_ses *ses, rpc_pkg *pkg) {
   
    log_info("recv pkg from: %s, cmd: %u, sequence: %u",
            nw_sock_human_addr(&ses->peer_addr), pkg->command, pkg->sequence);
     
    json_t *body = json_loadb((char*)pkg->body, pkg->body_size, 0, NULL);
    json_t *result = json_object_get(body,"result");

    const char *status = json_string_value(json_object_get(result, "status"));
   
    int id = json_integer_value(json_object_get(body, "id"));
    //log_info("send_request status %s %d",status,id);
    if( status != NULL ){
        if( strcmp(status,"success") == 0 ) {
            g_requests.pop();
        
            log_info("send_request [BLOCK_WS_RECEIVE] succed %d",id);
        } else {
            log_info("send_request [BLOCK_WS_RECEIVE] failed %s",status);
        }
    } else {
        log_info("send_request [BLOCK_WS_RECEIVE] failed status == null");
    }
   
    json_decref(body);
    

    is_in_mt_request = false;
}

//int send_ws_block(const signed_block ws_push_block) {
int send_request(bm_send_request2 req) {

    if (!rpc_clt_connected( matchengine ) ) {
         is_in_mt_request = false;
         log_error("send_request connected failed");
         return -2;
    }

    double now = current_timestamp();
    if (is_in_mt_request && (now - last_send_time) > matchengine->heartbeat_timeout * 2) {
         is_in_mt_request = false;
         last_send_time = now;
         log_error("send_request timeout local failed %lf %lf %lf",now,matchengine->last_heartbeat,matchengine->heartbeat_timeout);
         return -3;
    } else {
       // log_info("send_ws_block timeout local continue %lf %lf %lf",now,matchengine->last_heartbeat,matchengine->heartbeat_timeout);
    }

    if (matchengine == NULL)
        return -__LINE__;

    log_info("start send");        

   // uint64_t id = ws_push_block.block_num;
    
   // log_trace("send_ws_block %s",body1.c_str());

    std::string body = req.o;

    char* cc = (char*)malloc(sizeof(char)*(body.length()+1));
    strncpy(cc,body.c_str(),body.length());
    cc[body.length()] = '\0';
    
    rpc_pkg pkg;
    memset(&pkg, 0, sizeof(pkg));
    pkg.pkg_type  = RPC_PKG_TYPE_REQUEST;
    pkg.command   = req.method;
    pkg.sequence  = time(NULL);
    pkg.req_id    = time(NULL);
    pkg.body      = cc;
    // pkg.body      = (void*)body.c_str();
    pkg.body_size = body.length();

    rpc_clt_send(matchengine, &pkg);
    last_send_time = current_timestamp();
    log_info("send request to %s, cmd: %u, sequence: %u",
            nw_sock_human_addr(rpc_clt_peer_addr(matchengine)), pkg.command, pkg.sequence);
        

    free(cc);
    
    return 0;
}

void on_send_request_timer(nw_timer *timer, void *privdata) {
    log_trace("start");

    if ( g_requests.size() == 0 || is_in_mt_request ) {
        log_trace("on_send_request_timer %s g_requests.size() %lu",  is_in_mt_request ? "true" : "false",g_requests.size());
        return;
    }
    is_in_mt_request = true;

    bm_send_request2 bm;
    if( !g_requests.front(bm) ) {
        is_in_mt_request = false;
        return;
    }

    send_request(bm);
}


int mt_send_request() {
    
    is_in_mt_request = false;

    rpc_clt_type ct;
    memset(&ct, 0, sizeof(ct));
    ct.on_connect = on_backend_connect;
    ct.on_recv_pkg = on_backend_recv_pkg;
    matchengine = rpc_clt_create(&mt_settings.matchengine, &ct);
    if (matchengine == NULL)
        return -__LINE__;
    if (rpc_clt_start(matchengine) < 0) {
        return 0;    
    }

    /*nw_timer_set(&send_reuqest_timer, 0.2, true, on_send_request_timer, NULL);
    nw_timer_start(&send_reuqest_timer);*/

    return 0;
}
