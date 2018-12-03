# include "rmt_server_config.h"
# include "rmt_server.h"
# include "rmt_reader.h"

# define MAX_PENDING_JOB 10

static nw_job *job;
static rpc_svr *svr;

struct job_request {
	nw_ses   *ses;
	rpc_pkg  pkg;
	uint64_t ses_id;
	uint32_t command;
	json_t   *params;
};

struct job_reply {
	int     code;
	sds     message;
	json_t  *result;
};

static int reply_json(nw_ses *ses, rpc_pkg *pkg, const json_t *json)
{
	char *message_data;

	message_data = json_dumps(json, 0);

	if (message_data == NULL)
		return -__LINE__;
	log_trace("connection: %s send: %s", nw_sock_human_addr(&ses->peer_addr), message_data);

	rpc_pkg reply;
	memcpy(&reply, pkg, sizeof(reply));
	reply.pkg_type = RPC_PKG_TYPE_REPLY;
	reply.body = message_data;
	reply.body_size = strlen(message_data);
	rpc_send(ses, &reply);
	free(message_data);

	return 0;
}

static int reply_error(nw_ses *ses, rpc_pkg *pkg, int code, const char *message)
{
	json_t *error = json_object();
	json_object_set_new(error, "code", json_integer(code));
	json_object_set_new(error, "message", json_string(message));

	json_t *reply = json_object();
	json_object_set_new(reply, "error", error);
	json_object_set_new(reply, "result", json_null());
	json_object_set_new(reply, "id", json_integer(pkg->req_id));

	int ret = reply_json(ses, pkg, reply);
	json_decref(reply);

	return ret;
}

static int reply_error_internal_error(nw_ses *ses, rpc_pkg *pkg)
{
	return reply_error(ses, pkg, 2, "internal error");
}

static int reply_error_service_unavailable(nw_ses *ses, rpc_pkg *pkg)
{
	return reply_error(ses, pkg, 3, "service unavailable");
}

static int reply_result(nw_ses *ses, rpc_pkg *pkg, json_t *result)
{
	json_t *reply = json_object();
	json_object_set_new(reply, "error", json_null());
	json_object_set    (reply, "result", result);
	json_object_set_new(reply, "id", json_integer(pkg->req_id));

	int ret = reply_json(ses, pkg, reply);
	json_decref(reply);

	return ret;
}

static void *on_job_init(void)
{
	return mysql_connect(&mt_rdg_settings.db_history);
}

static int on_cmd_daily_redeme(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	const char * user = NULL;
	string amount;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));


	if (!json_is_string(json_array_get(params, 1)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 1));

	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	amount = json_string_value(json_array_get(params, 2));
	
	if(strlen(user) < 3)
		goto invalid_argument;
	records = get_daily_redeme(conn, start, user, amount);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_user_pledge(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	int end = 0;
	const char * user = NULL;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));
	
	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));

	if(strlen(user) < 3)
		goto invalid_argument;
	
	records = get_user_pledge(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}


static int on_cmd_user_dividend(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	int end = 0;
	const char * user = NULL;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));
	
	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));

	if(strlen(user) < 3)
		goto invalid_argument;
	
	records = get_user_dividend(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}


static int on_cmd_user_redeme(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	int end = 0;
	const char * user = NULL;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));
	
	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));

	if(strlen(user) < 3)
		goto invalid_argument;
	
	records = get_user_redeme(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_user_share(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	int end = 0;
	const char * user = NULL;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));
	
	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));

	if(strlen(user) < 3)
		goto invalid_argument;
	
	records = get_user_share(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_pledge_sum(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	int start = 0;
	int end = 0;
	//char *market = NULL;

	//    uint32_t user_id = 0;


	if (json_array_size(params) != 2)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));
	records = get_pledge_sum(conn, start, end);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}



static int on_cmd_dividend_compute(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	const char *user = NULL;
	int start = 0;
	int end = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));

	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));
	
	
	records = get_dividend_compute(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_realtime_income(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	string amount;
	int start = 0;


	if (json_array_size(params) != 2)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_string(json_array_get(params, 1)))
		goto invalid_argument;
	amount = json_string_value(json_array_get(params, 1));
	
	records = set_realtime_income(conn, start, amount);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_pledge_info(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;CMD_GET_PLEDGE_INFO3

	if (json_array_size(params) != 0)
		goto invalid_argument;

	
	records = get_pledge_info(conn);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static int on_cmd_block_info(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;CMD_GET_PLEDGE_INFO3

	if (json_array_size(params) != 0)
		goto invalid_argument;

	
	records = get_block_info(conn);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}


static int on_cmd_dividend_expect(MYSQL *conn, json_t *params, struct job_reply *rsp)
{
	json_t *records = NULL;
	//json_t *result = NULL;
	const char *user = NULL;
	int start = 0;
	int end = 0;


	if (json_array_size(params) != 3)
		goto invalid_argument;

	if (!json_is_integer(json_array_get(params, 0)))
		goto invalid_argument;
	start = json_integer_value(json_array_get(params, 0));

	if (!json_is_integer(json_array_get(params, 1)))
		goto invalid_argument;
	end = json_integer_value(json_array_get(params, 1));

	if (!json_is_string(json_array_get(params, 2)))
		goto invalid_argument;
	user = json_string_value(json_array_get(params, 2));
	
	
	records = get_dividend_expect(conn, start, end, user);
	if (records == NULL) {
		rsp->code = 2;
		rsp->message = sdsnew("internal error");
	}

	rsp->result = records;

	return 0;

invalid_argument:
	rsp->code = 1;
	rsp->message = sdsnew("invalid argument");

	return 0;
}

static void on_job(nw_job_entry *entry, void *privdata)
{
	MYSQL *conn = (MYSQL *)privdata;
	struct job_request *req = (job_request *)entry->request;
	struct job_reply *rsp = (job_reply*)malloc(sizeof(struct job_reply));
	entry->reply = rsp;
	if (rsp == NULL) {
		return;
	}
	memset(rsp, 0, sizeof(struct job_reply));

	int ret;
	switch (req->command) {
		case CMD_DAILY_REDEME:
			ret = on_cmd_daily_redeme(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_daily_redeme fail: %d", ret);
			}
			break;

		case CMD_USER_PLEDGE:
			ret = on_cmd_user_pledge(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_user_pledge fail: %d", ret);
			}
			break;

		case CMD_PLEDGE_SUM:
			ret = on_cmd_pledge_sum(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_pledge_sum fail: %d", ret);
			}
			break;
			
		case CMD_USER_REDEME:
			ret = on_cmd_user_redeme(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_user_redeme fail: %d", ret);
			}
			break;
		case CMD_USER_SHARE:
			ret = on_cmd_user_share(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_user_share fail: %d", ret);
			}
			break;
			
		case CMD_REALTIME_INCOME:
			ret = on_cmd_realtime_income(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_realtime_income fail: %d", ret);
			}
			break;
		
		case CMD_USER_DIVIDEND_COMOUTE:
			ret = on_cmd_dividend_compute(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_dividend_compute fail: %d", ret);
			}
			break;

		case CMD_GET_PLEDGE_INFO:
			ret = on_cmd_pledge_info(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_pledge_info  fail: %d", ret);
			}
			break;
		case CMD_GET_BLOCK_INFO:
			ret = on_cmd_block_info(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_block_info fail: %d", ret);
			}
			break;
		
		case CMD_USER_DIVIDEND:
			ret = on_cmd_user_dividend(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_user_dividend fail: %d", ret);
			}
			break;
		case CMD_USER_DIVIDEND_EXPECT:
			ret = on_cmd_dividend_expect(conn, req->params, rsp);
			if (ret < 0) {
				log_error("on_cmd_dividend_compute fail: %d", ret);
			}
			break;
		default:
			log_error("unkown cmd: %u", req->command);
			break;
	}
}

static void on_job_finish(nw_job_entry *entry)
{
	struct job_request *req = (job_request *)entry->request;
	if (req->ses->id != req->ses_id)
		return;
	if (entry->reply == NULL) {
		reply_error_internal_error(req->ses, &req->pkg);
		return;
	}

	struct job_reply *rsp = (job_reply *)entry->reply;
	if (rsp->code != 0) {
		reply_error(req->ses, &req->pkg, rsp->code, rsp->message);
		return;
	}

	if (rsp->result) {
		reply_result(req->ses, &req->pkg, rsp->result);
	}
}

static void on_job_cleanup(nw_job_entry *entry)
{
	struct job_request *req = (job_request *)entry->request;
	json_decref(req->params);
	free(req);
	if (entry->reply) {
		struct job_reply *rsp = (job_reply *)entry->reply;
		if (rsp->message)
			sdsfree(rsp->message);
		if (rsp->result)
			json_decref(rsp->result);
		free(rsp);
	}
}

static void on_job_release(void *privdata)
{
	mysql_close((MYSQL *)privdata);
}

static void svr_on_recv_pkg(nw_ses *ses, rpc_pkg *pkg)
{
	json_t *params = json_loadb((const char*)pkg->body, pkg->body_size, 0, NULL);
	if (params == NULL || !json_is_array(params)) {
		sds hex = hexdump(pkg->body, pkg->body_size);
		log_error("connection: %s, cmd: %u decode params fail, params data: \n%s", \
				nw_sock_human_addr(&ses->peer_addr), pkg->command, hex);
		sdsfree(hex);
		rpc_svr_close_clt(svr, ses);
		return;
	}

	sds params_str = sdsnewlen(pkg->body, pkg->body_size);
	log_debug("from %s command: %u, params: %s", nw_sock_human_addr(&ses->peer_addr), pkg->command, params_str);
	sdsfree(params_str);

	if (job->request_count >= MAX_PENDING_JOB * mt_rdg_settings.worker_num) {
		log_error("pending job: %u, service unavailable", job->request_count);
		reply_error_service_unavailable(ses, pkg);
		json_decref(params);
		return;
	}

	struct job_request *req = (job_request *)malloc(sizeof(struct job_request));
	memset(req, 0, sizeof(struct job_request));
	memcpy(&req->pkg, pkg, sizeof(rpc_pkg));
	req->ses = ses;
	req->ses_id = ses->id;
	req->command = pkg->command;
	req->params = params;
	nw_job_add(job, 0, req);

	return;
}

static void svr_on_new_connection(nw_ses *ses)
{
	log_trace("new connection: %s", nw_sock_human_addr(&ses->peer_addr));
}

static void svr_on_connection_close(nw_ses *ses)
{
	log_trace("connection: %s close", nw_sock_human_addr(&ses->peer_addr));
}

int init_rdg_server(void)
{
	rpc_svr_type st;
	memset(&st, 0, sizeof(st));
	st.on_recv_pkg = svr_on_recv_pkg;
	st.on_new_connection = svr_on_new_connection;
	st.on_connection_close = svr_on_connection_close;

	svr = rpc_svr_create(&mt_rdg_settings.svr, &st);
	if (svr == NULL)
		return -__LINE__;
	if (rpc_svr_start(svr) < 0)
		return -__LINE__;

	nw_job_type jt;
	memset(&jt, 0, sizeof(jt));
	jt.on_init    = on_job_init;
	jt.on_job     = on_job;
	jt.on_finish  = on_job_finish;
	jt.on_cleanup = on_job_cleanup;
	jt.on_release = on_job_release;

	//log_trace("%d",settings.worker_num);
	job = nw_job_create(&jt, mt_rdg_settings.worker_num);
	if (job == NULL)
		return -__LINE__;

	return 0;
}

