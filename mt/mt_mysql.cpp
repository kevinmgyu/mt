#include <fc/io/json.hpp>

# include "mt_config.h"
# include "mt_mysql.h"


static MYSQL *mysql_conn;
static nw_job *job;
static dict_t *dict_sql;
static nw_timer timer;

enum {
	HISTORY_USER_ETH_KCASH,
	HISTORY_USER_DEPOSIT,
	HISTORY_USER_WITHDRAW,
	HISTORY_USER_MT_PLEDGE,
	HISTORY_USER_MT_PLEDGE_SUM,
	HISTORY_BLOCK_INFO
};

struct dict_sql_key {
	uint32_t type;
	uint32_t hash;
};

static uint32_t dict_sql_hash_function(const void *key)
{
	return dict_generic_hash_function(key, sizeof(struct dict_sql_key));
}

static void *dict_sql_key_dup(const void *key)
{
	struct dict_sql_key *obj = (dict_sql_key*)malloc(sizeof(struct dict_sql_key));
	memcpy(obj, key, sizeof(struct dict_sql_key));
	return obj;
}

static int dict_sql_key_compare(const void *key1, const void *key2)
{
	return memcmp(key1, key2, sizeof(struct dict_sql_key));
}

static void dict_sql_key_free(void *key)
{
	free(key);
}

static void *on_job_init(void)
{
	return mysql_connect(&mt_settings.db_history);
}

static void on_job(nw_job_entry *entry, void *privdata)
{
	MYSQL *conn = (MYSQL *)privdata;
	sds sql = (sds)entry->request;
	log_debug("exec sql: %s", sql);
	while (true) {
		//  string out(sql);
		//vector<string> tt = split(out, "__bytetrade_mysql_split__");
		//  vector<string> tt = split(out, "\t");
		// if( tt.size() > 1 ) {
		// log_trace("sql %lu raw %s",tt.size(), out.c_str());
		// }
		//for( size_t i = 0; i < tt.size();++i) {
		//  if( tt.size() > 1 ) {
		// log_trace("sql %s",tt[i].c_str());
		//  }
		int ret = mysql_real_query(conn, sql, sdslen(sql));
		//int ret = mysql_real_query(conn, tt[i].c_str(), tt[i].size());
		if (ret != 0 && mysql_errno(conn) != 1062) {
			log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(conn), mysql_error(conn));
			usleep(1000 * 1000);
			continue;
		}
		// }
		break;
	}
}

static void on_job_cleanup(nw_job_entry *entry)
{
	sdsfree((sds)entry->request);
}

static void on_job_release(void *privdata)
{
	mysql_close((MYSQL*)privdata);
}

static void on_timer(nw_timer *t, void *privdata) {
	size_t count = 0;
	dict_iterator *iter = dict_get_iterator(dict_sql);
	dict_entry *entry;
	while ((entry = dict_next(iter)) != NULL) {
		nw_job_add(job, 0, entry->val);
		dict_delete(dict_sql, entry->key);
		count++;
	}
	dict_release_iterator(iter);

	if (count) {
		log_debug("flush history count: %zu", count);
	}
}

int init_history(void) {
	mysql_conn = mysql_init(NULL);
	if (mysql_conn == NULL)
		return -__LINE__;
	if (mysql_options(mysql_conn, MYSQL_SET_CHARSET_NAME, mt_settings.db_history.charset) != 0)
		return -__LINE__;

	dict_types dt;
	memset(&dt, 0, sizeof(dt));
	dt.hash_function  = dict_sql_hash_function;
	dt.key_compare    = dict_sql_key_compare;
	dt.key_dup        = dict_sql_key_dup;
	dt.key_destructor = dict_sql_key_free;

	dict_sql = dict_create(&dt, 1024);
	if (dict_sql == 0) {
		return -__LINE__;
	}

	nw_job_type jt;
	memset(&jt, 0, sizeof(jt));
	jt.on_init    = on_job_init;
	jt.on_job     = on_job;
	jt.on_cleanup = on_job_cleanup;
	jt.on_release = on_job_release;

	job = nw_job_create(&jt, mt_settings.history_thread);
	if (job == NULL)
		return -__LINE__;

	nw_timer_set(&timer, 0.1, true, on_timer, NULL);
	nw_timer_start(&timer);

	return 0;
}

int fini_history(void) {
	log_info("fini_history start");
	on_timer(NULL, NULL);

	usleep(100 * 1000);
	nw_job_release(job);

	log_info("fini_history finished");
	return 0;
}

static sds sql_append_mpd(sds sql, mpd_t *val, bool comma) {
	char *str = mpd_to_sci(val, 0);
	sql = sdscatprintf(sql, "'%s'", str);
	if (comma) {
		sql = sdscatprintf(sql, ", ");
	}
	free(str);
	return sql;
}

static sds get_sql(struct dict_sql_key *key) {
	dict_entry *entry = dict_find(dict_sql, key);
	if (!entry) {
		sds val = sdsempty();
		entry = dict_add(dict_sql, key, val);
		if (entry == NULL) {
			sdsfree(val);
			return NULL;
		}
	}
	return (sds)entry->val;
}

static void set_sql(struct dict_sql_key *key, sds sql) {     
	dict_entry *entry = dict_find(dict_sql, key);
	if (entry) {
		entry->val = sql;
	}
}

int append_mt_pledge(double t, 
		uint32_t block_num, const char* transaction_id,
		const char* from_user_id, const char *to_user_id, mpd_t *amount, int assetid) {
	struct dict_sql_key key;
	//    key.hash = user_id % HISTORY_HASH_NUM;
	key.hash = 1;
	key.type = HISTORY_USER_MT_PLEDGE;
	sds sql = get_sql(&key);
	if (sql == NULL)
		return -__LINE__;

	if (sdslen(sql) == 0) {
		sql = sdscatprintf(sql, "INSERT INTO `user_mt_pledge` (`id`, `time`, `block_num`, `transaction_id`, `from_user_id`, `to_user_id`,  `asset_type`, `amount`) VALUES ");
	} else {
		sql = sdscatprintf(sql, ", ");
	}

	sql = sdscatprintf(sql, "(NULL, %f, %d, '%s', '%s',  '%s', %d, ", t, block_num, transaction_id , from_user_id, to_user_id,assetid);
	sql = sql_append_mpd(sql, amount, false);
	sql = sdscatprintf(sql, ")");

	set_sql(&key, sql);

	return 0;
}

int append_mt_pledge_sum(double t, const char* from_user_id, mpd_t *amount, int assetid) {
	struct dict_sql_key key;
	//    key.hash = user_id % HISTORY_HASH_NUM;
	key.hash = 1;
	key.type = HISTORY_USER_MT_PLEDGE_SUM;
	sds sql = get_sql(&key);
	if (sql == NULL)
		return -__LINE__;

	if (sdslen(sql) == 0) {
		sql = sdscatprintf(sql, "replace INTO `user_mt_pledge_sum` (`id`, `time`, `frome_user_id`, `asset_type`, `amount`) VALUES ");
	} else {
		sql = sdscatprintf(sql, ", ");
	}

	sql = sdscatprintf(sql, "(NULL, %f, '%s', %d, ", t, from_user_id, assetid);
	sql = sql_append_mpd(sql, amount, true);
	sql = sdscatprintf(sql, ")");

	set_sql(&key, sql);

	return 0;
}


int append_deposit(double t, 
		uint32_t block_num, const char* transaction_id,const char* proposal_transaction_id,
		const char* external_transaction_id, const char* sign,const char* user_id, 
		const char* from_external_address, int asset_type, const char* chain_contract_address,
		int chain_type,mpd_t *amount, int ret) {

	struct dict_sql_key key;
	//    key.hash = user_id % HISTORY_HASH_NUM;
	key.hash = 1;
	key.type = HISTORY_USER_DEPOSIT;
	sds sql = get_sql(&key);
	if (sql == NULL)
		return -__LINE__;

	if (sdslen(sql) == 0) {
		sql = sdscatprintf(sql, "INSERT INTO `deposit` (`id`, `time`, `block_num`, `transaction_id`, `proposal_transaction_id`, `external_transaction_id`,  `sign`,   `user_id`, `from_external_address`, `asset_type`, `chain_contract_address`, `chain_type`, `ret`, `amount` ) VALUES ");
	} else {
		sql = sdscatprintf(sql, ", ");
	}

	sql = sdscatprintf(sql, "(NULL, %f, %d, '%s', '%s', '%s', '%s','%s','%s', %d,'%s', %d, %d, ", 
			t, block_num, transaction_id , proposal_transaction_id, external_transaction_id,sign,user_id,from_external_address,asset_type,chain_contract_address,chain_type,ret);
	sql = sql_append_mpd(sql, amount, false);
	sql = sdscatprintf(sql, ")");

	set_sql(&key, sql);

	return 0;
}


int append_withdraw(double t, 
		uint32_t block_num, const char* transaction_id,const char* proposal_transaction_id,
		const char* external_transaction_id,
		const char* user_id, 
		const char* to_external_address, int asset_type, const char* chain_contract_address,
		int chain_type,mpd_t *amount, int ret, int score) {

	struct dict_sql_key key;
	//    key.hash = user_id % HISTORY_HASH_NUM;
	key.hash = 1;
	key.type = HISTORY_USER_WITHDRAW;
	sds sql = get_sql(&key);
	if (sql == NULL)
		return -__LINE__;

	if (sdslen(sql) == 0) {
		sql = sdscatprintf(sql, "INSERT INTO `withdraw` (`id`, `time`, `block_num`, `transaction_id`, `proposal_transaction_id`, `external_transaction_id`,  `user_id`, `to_external_address`, `asset_type`, `chain_contract_address`, `chain_type`, `ret`, `score`, `amount`  ) VALUES ");
	} else {
		sql = sdscatprintf(sql, ", ");
	}

	sql = sdscatprintf(sql, "(NULL, %f, %d, '%s', '%s', '%s', '%s','%s',%d,'%s', %d,%d,%d,", 
			t, block_num, transaction_id , proposal_transaction_id, external_transaction_id,
			user_id,to_external_address,asset_type,chain_contract_address,chain_type,ret,score);
	sql = sql_append_mpd(sql, amount, false);
	sql = sdscatprintf(sql, ")");

	set_sql(&key, sql);	

	return 0;
}

int append_block_info(double t, uint32_t block_num) {

	struct dict_sql_key key;
	//	  key.hash = user_id % HISTORY_HASH_NUM;
	key.hash = 1;
	key.type = HISTORY_BLOCK_INFO;
	sds sql = get_sql(&key);
	if (sql == NULL)
		return -__LINE__;

	if (sdslen(sql) == 0) {
		sql = sdscatprintf(sql, "REPLACE INTO `mt_block` (`id`, `time`, `num`) VALUES ");
	} else {
		sql = sdscatprintf(sql, ", ");
	}

	sql = sdscatprintf(sql, "(%d, %f,%d", 1,t, block_num);
	sql = sdscatprintf(sql, ")");

	set_sql(&key, sql); 

	return 0;
}

