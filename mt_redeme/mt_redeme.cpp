#include "mt_redeme.h"
#include "mt_send_request.h"

static MYSQL *mysql_conn;
static nw_timer mt_redeme_timer;
static bool has_mt_redeme_process = false;


void on_mt_redeme_timer(nw_timer *timer, void *privdata) {
	if (has_mt_redeme_process)
	{
		return;
	}
	has_mt_redeme_process = true;
	MYSQL *conn = (MYSQL *)privdata;
	int flag =1;
	int ret;

	sds sql = sdsempty();
	double cur_time = current_timestamp();
	
	// for test ,need to delete
	cur_time = 1542804528;
	sql = sdscatprintf(sql, "SELECT id,user_id,amount,from_id,asset_type from user_mt_redeme WHERE `flag` = '%d' and `exe_time` <= '%f'",flag,cur_time);


	log_trace("exec sql: %s", sql);
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		has_mt_redeme_process = false;
		return;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		log_trace("num_rows is 0");
		has_mt_redeme_process = false;
		return ;
	}
	for (size_t i = 0; i < num_rows; ++i) {
		MYSQL_ROW row = mysql_fetch_row(result);
		int id = strtoul(row[0], NULL, 0);
		string user = row[1];
		string amount = rstripzero(row[2]);
		mpd_t *tap = decimal(amount.c_str(), 10);
		
		string from = row[3];
		int asset_type = strtoul(row[4], NULL, 0);

		sql = sdsempty();
		sql = sdscatprintf(sql, "UPDATE `user_mt_redeme` SET `flag`= %d WHERE `id`= %d",0,id);

		log_trace("exe sql : %s",sql);
		while (true) {
			ret = mysql_real_query(conn, sql, sdslen(sql));

			if (ret != 0 && mysql_errno(conn) != 1062) {
				log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
				usleep(1000 * 1000);
				continue;
			}
			break;
		}
		
		sdsfree(sql);
		// transfer to operation to chain 
		optional<simple_asset> asset1 = get_asset_marketing(asset_type);
		if (!asset1) {
			log_error("not exists asset %d", asset_type);
			mpd_del(tap);
			continue;
		}

		share_type input_amount = mul_mpd_to_int64(tap, asset1->base_precision,10);;
		fc::ecc::private_key priv_input = fc::ecc::private_key::regenerate(fc::sha256(mt_settings.priv));
		signed_transaction trx;
		transfer_operation t;
		t.fee = 30 * core_asset_fee_precision;
		t.from = from;
		t.to = user;
		t.asset_type = asset_type;
		t.amount = input_amount;


		trx.operations.push_back(t);

		trx.timestamp = fc::time_point::now();
		//trx.sign(priv2,get_chain_id());
		trx.sign(priv_input, 1);

		bm_send_request2 req;
		req.method = CMD_BLOCKCHAIN;
		req.o = fc::json::to_string(trx);
		if (send_request(req) == 0)
			log_info("sent transfer_operation: %s", trx.id().str().c_str());

		//RestClient::Response r2 = transfer(from,user,asset_type,input_amount,priv_input);
		mpd_del(tap);
		log_trace("transfer from %s %s asset %d to %s",from.c_str(),to_string(input_amount).c_str(),asset_type,user.c_str());

	}
	mysql_free_result(result);
	has_mt_redeme_process = false;

}

int init_mt_redeme() {
	has_mt_redeme_process = false;

	mysql_conn = mysql_init(NULL);
	if (mysql_conn == NULL)
		return -__LINE__;
	if (mysql_options(mysql_conn, MYSQL_SET_CHARSET_NAME, mt_settings.db_history.charset) != 0)
		return -__LINE__;
	void* mysql_con = mysql_connect(&mt_settings.db_history);
	if (NULL == mysql_con)
		return -__LINE__;


	nw_timer_set(&mt_redeme_timer, 10, true, on_mt_redeme_timer, mysql_con);
	nw_timer_start(&mt_redeme_timer);

	return 0;
}
