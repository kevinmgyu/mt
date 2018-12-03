# include "rmt_server_config.h"
# include "rmt_reader.h"
# include "ut_decimal.h"
#include "bmath.cpp"
#include <fc/io/json.hpp>
#include <sstream>
#include <iostream>
#include "chain_api.h"
/*
   static mpd_t* devide_int64_to_mpd(share_type a, share_type b,int prec) {
   mpd_t * mpda = decimal(to_string(a).c_str(),   prec);
   mpd_t * mpdb = decimal(to_string(b).c_str(),   prec);

// log_trace("a %s",to_string(a).c_str());
// log_trace("b %s",to_string(a).c_str());

// log_trace("a2 %s",mpd_to_sci(mpda,0));
// log_trace("b2 %s",mpd_to_sci(mpdb,0));

mpd_t *result = mpd_new(&mpd_ctx);
mpd_div(result,mpda  ,mpdb, &mpd_ctx);

if (prec) {
mpd_rescale(result, result, -prec, &mpd_ctx);
}

mpd_del(mpda);
mpd_del(mpdb);

return result;
}

static mpd_t *decimal_to_mpd(double v, int prec) {

char str[50];
memset(str,'\0',50);
snprintf(str,50,"%lf",v);
str[49]='\0';
// log_trace("%s",str);

mpd_t *result = mpd_new(&mpd_ctx);
mpd_ctx.status = 0;
mpd_set_string(result, str, &mpd_ctx);
if (mpd_ctx.status == MPD_Conversion_syntax) {
mpd_del(result);
return NULL;
}

// log_trace("%s",mpd_to_sci(result,0));
if (prec) {
mpd_rescale(result, result, -prec, &mpd_ctx);
}

// log_trace("%s",mpd_to_sci(result,0));

return result;
}*/
static sds sql_append_mpd(sds sql, mpd_t *val, bool comma) {
	char *str = mpd_to_sci(val, 0);
	sql = sdscatprintf(sql, "'%s'", str);
	if (comma) {
		sql = sdscatprintf(sql, ", ");
	}
	free(str);
	return sql;
}

json_t *get_user_pledge(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT from_user_id,sum(amount) from user_mt_pledge WHERE `from_user_id` = '%s' and `time` >= '%d' and `time` < '%d' group by from_user_id",user,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	json_object_set_new(record, "amount", json_string(rstripzero(row[1])));

	mysql_free_result(result);

	return record;
}

json_t *get_user_redeme(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT user_id,sum(amount) from user_mt_redeme WHERE `user_id` = '%s' and `flag` = '%d' and `time` >= '%d' and `time` < '%d' group by user_id",user,1,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	json_object_set_new(record, "amount", json_string(rstripzero(row[1])));

	mysql_free_result(result);

	return record;
}

json_t *get_pledge_sum(MYSQL *conn, int32_t start, int32_t end) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT sum(amount) from user_mt_pledge WHERE `time` >= '%d' and `time` < '%d'",start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "amount", json_string(rstripzero(row[0])));

	mysql_free_result(result);

	return record;
}



json_t *get_user_share(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT from_user_id,sum(amount) from user_mt_pledge WHERE `from_user_id` = '%s' and `time` >= '%d' and `time` < '%d' group by from_user_id",user,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	string amount = rstripzero(row[1]);
	json_object_set_new(record, "amount", json_string(rstripzero(row[1])));

	mysql_free_result(result);


	sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT sum(amount) from user_mt_pledge WHERE `time` >= '%d' and `time` < '%d'",start,end);

	log_trace("exec sql: %s", sql);
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	result = mysql_store_result(conn);
	num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	row = mysql_fetch_row(result);

	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	string total_amount = rstripzero(row[0]);
	mpd_t * mpt_amount = decimal(amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t * mpd_total_amount = decimal(total_amount.c_str(),mt_rdg_settings.save_precision);

	mpd_t *result_share = mpd_new(&mpd_ctx);
	mpd_div(result_share,mpt_amount ,mpd_total_amount,&mpd_ctx);
	int prec = mt_rdg_settings.save_precision;
	if (prec) {
		mpd_rescale(result_share, result_share, -prec, &mpd_ctx);
	}

	char *result_str = mpd_to_sci(result_share, 0);
	//log_trace("%s, %s,%s",mpd_to_sci(mpt_amount,0),mpd_to_sci(mpd_total_amount,0),mpd_to_sci(result_share,0));	
	json_object_set_new(record, "total_amount", json_string(total_amount.c_str()));
	json_object_set_new(record, "share", json_string(result_str));

	mysql_free_result(result);
	mpd_del(mpt_amount);
	mpd_del(mpd_total_amount);
	mpd_del(result_share);
	free(result_str);

	return record;
}

json_t *get_dividend_compute(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT from_user_id,sum(amount) from user_mt_pledge WHERE `from_user_id` = '%s' and `time` >= '%d' and `time` < '%d' group by from_user_id",user,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	string amount = rstripzero(row[1]);
	json_object_set_new(record, "pledge_amount", json_string(amount.c_str()));

	mysql_free_result(result);


	sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT sum(amount) from user_mt_pledge WHERE `time` >= '%d' and `time` < '%d'",start,end);

	log_trace("exec sql: %s", sql);
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	result = mysql_store_result(conn);
	num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	row = mysql_fetch_row(result);

	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}

	string total_amount = rstripzero(row[0]);

	mpd_t * mpt_amount = decimal(amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t * mpd_total_amount = decimal(total_amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t *share_result = mpd_new(&mpd_ctx);
	mpd_div(share_result,mpt_amount ,mpd_total_amount,&mpd_ctx);

	char *result_str = mpd_to_sci(share_result, 0);

	json_object_set_new(record, "total_amount", json_string(total_amount.c_str()));
	json_object_set_new(record, "share", json_string(result_str));


	mysql_free_result(result);

	sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT sum(amount) from user_mt_income WHERE `time` >= '%d' and `time` < '%d'",start,end);

	log_trace("exec sql: %s", sql);
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	result = mysql_store_result(conn);
	num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	row = mysql_fetch_row(result);

	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	string income_total_amount =rstripzero(row[0]);
	mpd_t * mpt_income_total_amount = decimal(income_total_amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t *result_out = mpd_new(&mpd_ctx);

	mpd_mul(result_out, share_result, mpt_income_total_amount, &mpd_ctx);
	/*int prec = mt_rdg_settings.save_precision;
	  if (prec) {
	  mpd_rescale(result_out, result_out, -prec, &mpd_ctx);
	  }*/
	//log_trace("%s %s %s",mpd_to_sci(share_result, 0),mpd_to_sci(mpt_income_total_amount, 0),mpd_to_sci(result_out, 0));
	char *result_out_str = mpd_to_sci(result_out, 0);

	json_object_set_new(record, "total_income", json_string(income_total_amount.c_str()));
	//json_object_set_new(record, "income", json_string(result_out_str));
	json_object_set_new(record, "income", json_string(result_out_str));
	json_object_set_new(record, "result", json_integer(0));
	json_object_set_new(record, "message", json_string("sucess"));

	mysql_free_result(result);

	mpd_del(mpt_amount);
	mpd_del(mpd_total_amount);
	mpd_del(share_result);
	free(result_str);

	mpd_del(mpt_income_total_amount);
	mpd_del(result_out);
	free(result_out_str);


	return record;
}


json_t *set_realtime_income(MYSQL *conn, int32_t start, string amount)

{

	sds sql = sdsempty();
	json_t *record = json_object();
	sql = sdscatprintf(sql, "INSERT INTO `user_mt_income` (`id`, `time`, `amount`) VALUES ");	
	sql = sdscatprintf(sql, "(NULL, %f, ", double(start));
	mpd_t *tap = devide_int64_to_mpd(amount, mt_rdg_settings.base_precision, mt_rdg_settings.save_precision);
	sql = sql_append_mpd(sql, tap, false);
	sql = sdscatprintf(sql, ")");;
	log_trace("exec sql: %s", sql);
	int cnt =0;
	while (true) {
		cnt =cnt+1;
		if (cnt > 10)
		{
			json_object_set_new(record, "result", json_integer(1));
			json_object_set_new(record, "message", json_string("mysql insert user_mt_income fail"));
			return record;
		}
		int ret = mysql_real_query(conn, sql, sdslen(sql));
		if (ret != 0 && mysql_errno(conn) != 1062) {
			log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
			//usleep(1000 * 1000);
			continue;
		}
		// }
		break;
}
mpd_del(tap);
sdsfree(sql);

json_object_set_new(record, "result", json_integer(0));
json_object_set_new(record, "message", json_string("sucess"));


return record;
}


json_t *get_daily_redeme(MYSQL *conn, int32_t start, const char* user, string amount)

{

	sds sql = sdsempty();
	sql = sdscatprintf(sql, "SELECT from_user_id,sum(amount) as cur from user_mt_pledge WHERE `from_user_id`='%s' group by from_user_id",user);


	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}

	MYSQL_ROW row = mysql_fetch_row(result);


	log_info("mysql result %zu",num_rows);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));

	double amount_sum = strtod(row[1], NULL);
	string str_amount_sum = rstripzero(row[1]);
	mpd_t * mpt_amount_sum = decimal(str_amount_sum.c_str(),10);

	mpd_t *amount1 = devide_int64_to_mpd(amount, mt_rdg_settings.base_precision, mt_rdg_settings.save_precision);
	json_object_set_new(record, "amount", json_string(amount.c_str()));
	if(mpt_amount_sum == NULL || amount1 == NULL)   
	{
		json_object_set_new(record, "result", json_integer(6));
		json_object_set_new(record, "message", json_string("internal error"));
		mysql_free_result(result);
		mpd_del(mpt_amount_sum);
		mpd_del(amount1);
		return record;
	}
	if (mpd_cmp(mpt_amount_sum, amount1, &mpd_ctx) < 0)
	{
		log_info("apply redeme amount is %s,but pledge amount sum is %f", amount.c_str(), amount_sum);
		json_object_set_new(record, "result", json_integer(1));
		json_object_set_new(record, "message", json_string("amount is not enough"));
		mysql_free_result(result);
		mpd_del(mpt_amount_sum);
		mpd_del(amount1);
		return record;
	}
	mysql_free_result(result);

	mpd_t *mpd_tmp = decimal_to_mpd(-1,10);
	mpd_t *tap = mpd_new(&mpd_ctx);
	mpd_mul(tap, mpd_tmp, amount1, &mpd_ctx);


	sql = sdsempty();
	sql = sdscatprintf(sql, "INSERT INTO `user_mt_pledge` (`id`, `time`, `block_num`, `transaction_id`, `from_user_id`, `to_user_id`,  `asset_type`, `amount`) VALUES ");	
	sql = sdscatprintf(sql, "(NULL, %f, %d, '%s', '%s',  '%s', %d, ", double(start), 1, "1" , user, mt_rdg_settings.pledge_ad,mt_rdg_settings.asset_type);

	sql = sql_append_mpd(sql, tap, false);
	sql = sdscatprintf(sql, ")");;
	log_trace("exec sql: %s", sql);
	int cnt =0;
	while (true) {
		cnt =cnt+1;
		if (cnt > 10)
		{
			json_object_set_new(record, "result", json_integer(2));
			json_object_set_new(record, "message", json_string("mysql insert user_mt_pledge fail"));
			return record;
		}
		int ret = mysql_real_query(conn, sql, sdslen(sql));
		if (ret != 0 && mysql_errno(conn) != 1062) {
			log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
			//usleep(1000 * 1000);
			continue;
		}
		// }
		break;
}

cnt =0;
log_trace(" %s", mt_rdg_settings.pledge_ad);
log_trace(" %d", mt_rdg_settings.asset_type);
// afer one day exe, flag = 1 undo redeme flag;
double exe_time = start + 86400;
sql = sdsempty();
sql = sdscatprintf(sql, "INSERT INTO `user_mt_redeme` (`id`, `time`, `user_id`,`from_id`, `exe_time`, `asset_type`,`flag`, `amount`) VALUES ");	
sql = sdscatprintf(sql, "(NULL,%f,'%s','%s',%f,%d,%d,", double(start),user,mt_rdg_settings.pledge_ad,exe_time,mt_rdg_settings.asset_type,1);
sql = sql_append_mpd(sql, amount1, false);
sql = sdscatprintf(sql, ")");;
log_trace("exec sql: %s", sql);
while (true) {

	cnt =cnt+1;
	if (cnt > 10)
	{
		json_object_set_new(record, "result", json_integer(3));
		json_object_set_new(record, "message", json_string("mysql insert user_mt_redeme fail"));
		return record;
	}
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0 && mysql_errno(conn) != 1062) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		//usleep(1000 * 1000);
		continue;
	}
	// }
break;
}
mpd_del(mpd_tmp);
mpd_del(tap);
mpd_del(mpt_amount_sum);
mpd_del(amount1);


sdsfree(sql);

json_object_set_new(record, "result", json_integer(0));
json_object_set_new(record, "message", json_string("sucess"));


return record;
}




json_t *get_pledge_info(MYSQL *conn) 
{

	json_t *record = json_object();

	json_object_set_new(record, "pledge_ad", json_string(mt_rdg_settings.pledge_ad));
	json_object_set_new(record, "asset_type", json_integer(mt_rdg_settings.asset_type));
	json_object_set_new(record, "start_num", json_integer(mt_rdg_settings.start_num));
	json_object_set_new(record, "end_num", json_integer(mt_rdg_settings.end_num));


	return record;
}

json_t *get_block_info(MYSQL *conn) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT num  from mt_block");

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "block_num", json_integer(atoi(row[0])));


	mysql_free_result(result);

	return record;
}


json_t *get_user_dividend(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT user_id,sum(amount) from user_mt_dividend WHERE `user_id` = '%s' and `time` >= '%d' and `time` < '%d' group by user_id",user,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	json_object_set_new(record, "amount", json_string(rstripzero(row[1])));

	mysql_free_result(result);

	return record;
}


json_t *get_dividend_expect(MYSQL *conn, int32_t start, int32_t end, const char* user) 
{

	sds sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT from_user_id,sum(amount) from user_mt_pledge WHERE `from_user_id` = '%s' and `time` >= '%d' and `time` < '%d' group by from_user_id",user,start,end);

	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(conn);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	MYSQL_ROW row = mysql_fetch_row(result);
	json_t *record = json_object();
	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}
	json_object_set_new(record, "user_id", json_string(row[0]));
	string amount = rstripzero(row[1]);
	json_object_set_new(record, "pledge_amount", json_string(amount.c_str()));

	mysql_free_result(result);


	sql = sdsempty();

	sql = sdscatprintf(sql, "SELECT sum(pledge_amount),sum(dividend_amount) from mt_pledge_dividend");

	log_trace("exec sql: %s", sql);
	ret = mysql_real_query(conn, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
		sdsfree(sql);
		return NULL;
	}
	sdsfree(sql);

	result = mysql_store_result(conn);
	num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		return json_null();
	}
	row = mysql_fetch_row(result);

	if(NULL == row[0])
	{
		mysql_free_result(result);
		return json_null();
	}

	string pledge_amount = rstripzero(row[0]);
	string dividend_amount = rstripzero(row[1]);

	mpd_t * mpt_pledge_amount = decimal(pledge_amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t * mpd_dividend_amount = decimal(dividend_amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t *share_result = mpd_new(&mpd_ctx);
	mpd_div(share_result,mpt_pledge_amount ,mpd_dividend_amount,&mpd_ctx);

	char *result_str = mpd_to_sci(share_result, 0);

	json_object_set_new(record, "history_yield", json_string(result_str));


	mysql_free_result(result);

	
	mpd_t * mpt_amount = decimal(amount.c_str(),mt_rdg_settings.save_precision);
	mpd_t *result_out = mpd_new(&mpd_ctx);

	mpd_mul(result_out, share_result, mpt_amount, &mpd_ctx);
	/*int prec = mt_rdg_settings.save_precision;
	  if (prec) {
	  mpd_rescale(result_out, result_out, -prec, &mpd_ctx);
	  }*/
	//log_trace("%s %s %s",mpd_to_sci(share_result, 0),mpd_to_sci(mpt_amount, 0),mpd_to_sci(result_out, 0));
	char *result_out_str = mpd_to_sci(result_out, 0);


	json_object_set_new(record, "dividend_expect", json_string(result_out_str));
	json_object_set_new(record, "result", json_integer(0));
	json_object_set_new(record, "message", json_string("sucess"));

	mpd_del(mpt_pledge_amount);
	mpd_del(mpd_dividend_amount);
	mpd_del(share_result);
	free(result_str);

	mpd_del(mpt_amount);
	mpd_del(result_out);
	free(result_out_str);


	return record;
}


