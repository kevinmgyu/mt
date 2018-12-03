#include <iostream>
#include <iomanip>
//#include <gflags/gflags.h>

#include <fc/io/json.hpp>
//#include <fc/crypto/elliptic.hpp>
//#include <fc/exception/exception.hpp>
//#include <fc/crypto/hex.hpp>
//#include "boost/random.hpp"  

#include "types.hpp"

#include "genesis.hpp"
#include "witness.hpp"
#include <ctime>
#include "ut_log.h"
#include "ut_mysql.h"
#include "chain_types.hpp"

#include "asset.hpp"
#include "chain_api.h"

mpd_t* devide_int64_to_mpd(share_type a, share_type b,int prec) {
	mpd_t * mpda = decimal(to_string(a).c_str(),   prec);
	mpd_t * mpdb = decimal(to_string(b).c_str(),   prec);

	log_trace("a %s",to_string(a).c_str());
	log_trace("b %s",to_string(b).c_str());

	log_trace("a2 %s",mpd_to_sci(mpda,0));
	log_trace("b2 %s",mpd_to_sci(mpdb,0));

	mpd_t *result = mpd_new(&mpd_ctx);
	mpd_div(result,mpda  ,mpdb, &mpd_ctx);

	if (prec) {
		mpd_rescale(result, result, -prec, &mpd_ctx);
	}

	log_trace("b2 %s",mpd_to_sci(result,0));
	mpd_del(mpda);
	mpd_del(mpdb);

	return result;
}
# define ERR_RET_LN(x) do { \
	if ((x) < 0) { \
		return -__LINE__; \
	} \
} while (0)
struct mt_settings {
	mysql_cfg           db_history;
	char*               url;
	char*               from_id;
	char*               key;
	int  		asset_type;

};
mt_settings seting;


struct simple_asset1 {
	int id;
	std::string symbol;
	// int prec;
	//double price;
	//string market;
	std::string chain_contract_address;
	int chain_type;
	uint64_t base_precision;
	uint64_t min_transfer_amount;

	simple_asset1() {

	}

	simple_asset1(const simple_asset1& a) {
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


std::map<std::string,int> assets_map;
std::map<string,std::map<int,int>>assets_adr;
std::map<int, simple_asset1> assets1;
static int init_asset1() {
	Response r = get_asset_list();
	if( r.code != 200 ) {
		printf("init asset failed %d",r.code);
		return 1;
	}
	Object o;
	o.parse(r.body);
	cout << "init_asset" << " " << get_asset_list().body << endl;

	Array result = o.get<Array>("result");
	//  cout << result.json() << endl;
	for( size_t i = 0; i < result.size();++i ) {
		Object coin =  result.get<Object>(i); 
		//cout << coin.json() << endl;
		simple_asset1 a;
		a.id = coin.get<Number>("id");
		a.symbol = coin.get<String>("symbol");
		// a.prec = coin.get<Number>("prec");
		if ( coin.has<String>("chain_contract_address") ) {
			a.chain_contract_address = coin.get<String>("chain_contract_address");
		} else {
			a.chain_contract_address = "";
		}

		if ( coin.has<Number>("chain_type") ) {
			a.chain_type = coin.get<Number>("chain_type");
		} else {
			a.chain_type = 0;
		}

		a.base_precision = coin.get<Number>("base_precision");
		a.min_transfer_amount = coin.get<Number>("min_trade_amount");

		printf("%s\n",a.symbol.c_str());

		assets1[a.id] = a;

		assets_map[a.symbol] = a.id;
		std::map<int,int> tmp;
		tmp[a.chain_type]=a.id;	
		assets_adr[a.chain_contract_address]=tmp;
	}
	return 0;
}
mpd_t *decimal_to_mpd(double v, int prec) {

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
}

MYSQL *mysql_connect1(mysql_cfg *db)
{
	MYSQL *conn = mysql_init(NULL);
	if (conn == NULL)
		return NULL;

	my_bool reconnect = 1;
	if (mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect) != 0) {
		mysql_close(conn);
		return NULL;
	}
	if (mysql_options(conn, MYSQL_SET_CHARSET_NAME, db->charset) != 0) {
		mysql_close(conn);
		return NULL;
	}
	if (mysql_real_connect(conn, db->host, db->user, db->pass, db->name, db->port, NULL, 0) == NULL) {
		mysql_close(conn);
		return NULL;
	}

	return conn;
}

static int read_config_from_json(json_t *root) {
	int ret;
	ret = load_cfg_mysql(root, "db_history", &seting.db_history);
	if (ret < 0) {
		printf("load history db config fail: %d\n", ret);
		return -__LINE__;
	}

	ERR_RET_LN(read_cfg_int(root, "asset_type", &seting.asset_type, false, 0));

	ret = read_cfg_str(root, "url", &seting.url, NULL);
	if (ret < 0) {
		printf("url fail: %d\n", ret);
		return -__LINE__;
	} 
	ret = read_cfg_str(root, "from_id", &seting.from_id, NULL);
	if (ret < 0) {
		printf("from_id fail: %d\n", ret);
		return -__LINE__;
	} 
	ret = read_cfg_str(root, "key", &seting.key, NULL);
	if (ret < 0) {
		printf("key fail: %d\n", ret);
		return -__LINE__;
	} 

	return 0;
}

int init_read_mt_server_config(const char *path) {


	json_error_t error;
	json_t *root = json_load_file(path, 0, &error);
	if (root == NULL) {
		printf("json_load_file from: %s fail: %s in line: %d\n", path, error.text, error.line);
		return -__LINE__;
	}
	if (!json_is_object(root)) {
		json_decref(root);
		return -__LINE__;
	}

	int ret = read_config_from_json(root);
	if (ret < 0) {
		json_decref(root);
		return ret;
	}
	json_decref(root);

	return 0;
}
share_type mul_mpd_to_int64(mpd_t* a,uint64_t b,int prec) {

	mpd_t *base = int64(b,prec);
	mpd_t *result = mpd_new(&mpd_ctx);
	log_trace("1 %s",mpd_to_sci(base,0));
	log_trace("2 %s",mpd_to_sci(a,0));
	mpd_mul(result, a, base, &mpd_ctx);
	log_trace("3 %s",mpd_to_sci(result,0));
	if (prec) {
		mpd_rescale(result, result, -prec, &mpd_ctx);
	}
	log_trace("4 %s",mpd_to_sci(result,0));
	char *str = mpd_to_sci(result, 0);
	string t(str);
	share_type t1(t);
	//log_trace("5 %s",t.c_str());
	//    uint64_t res;
	//    sscanf(t.c_str(),"%lu",&res);
	//uint64_t res = boost::lexical_cast<uint64_t>(t);

	free(str);
	mpd_del(base);
	mpd_del(result);

	return t1;
}



int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("usage: %s config.json\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int ret1 = init_read_mt_server_config(argv[3]);
	if (ret1 < 0) {
		log_trace("load config fail: %d", ret1);
	}
	ret1 = init_mpd();
	if (ret1 < 0) {
		log_trace("init mpd fail: %d", ret1);
	}
	set_btt_base_url(seting.url);
	ret1 = init_asset1();
	if(ret1 != 0 ) {
		printf("init asset failed");
		return -__LINE__;
	}

	int start = atoi(argv[1]);
	int end = atoi(argv[2]);
	log_trace("%s,%s",argv[1],argv[2]);	
	string from_id = seting.from_id;
	string key = seting.key;
	fc::ecc::private_key priv = fc::ecc::private_key::regenerate(fc::sha256(key));
	int asset_type = seting.asset_type;

	MYSQL *mysql_conn;	
	mysql_conn = mysql_init(NULL);
	if (mysql_conn == NULL)
		return -__LINE__;
	if (mysql_options(mysql_conn, MYSQL_SET_CHARSET_NAME, seting.db_history.charset) != 0)
		return -__LINE__;
	MYSQL* mysql_con = (MYSQL*)mysql_connect1(&seting.db_history);
	if (NULL == mysql_con)
		return -__LINE__;

	sds sql = sdsempty();
	sql = sdscatprintf(sql, "SELECT user_id,amount from user_mt_dividend WHERE `time` >= '%d' and `time` < '%d'",start, end);
	log_trace("exec sql: %s", sql);
	int ret = mysql_real_query(mysql_con, sql, sdslen(sql));
	if (ret != 0) {
		log_fatal("exec sql: %s fail: %d %s", sql, mysql_errno(mysql_conn), mysql_error(mysql_conn));
		sdsfree(sql);
		return -__LINE__;
	}
	sdsfree(sql);

	MYSQL_RES *result = mysql_store_result(mysql_con);
	size_t num_rows = mysql_num_rows(result);
	if (num_rows == 0) {
		mysql_free_result(result);
		log_trace("num_rows is 0");
		return  1;
	}


	for (size_t i = 0; i < num_rows; ++i) {
		MYSQL_ROW row = mysql_fetch_row(result);
		string user_id = row[0];
		if(NULL == row[0])
		{
			mysql_free_result(result);
			return __LINE__;
		}
		/*	string a ="22";
			string b ="40";
			share_type c =share_type(a);
			mpd_t *mpt_amount = devide_int64_to_mpd(c, 1000000000000000000, 18);
			mpd_t *mpd_total_amount = devide_int64_to_mpd(b, 1000000000000000000, 18);
			mpd_t *result_share = mpd_new(&mpd_ctx);
			mpd_div(result_share,mpt_amount ,mpd_total_amount,&mpd_ctx);
			int prec = 10;
			if (prec) {
			mpd_rescale(result_share, result_share, -prec, &mpd_ctx);
			}
			log_trace("%s, %s,%s",mpd_to_sci(mpt_amount,0),mpd_to_sci(mpd_total_amount,0),mpd_to_sci(result_share,0));*/
		/*mpd_t *tap = decimal_to_mpd(amount, 10);
		  share_type transfer_amount1 = mul_mpd_to_int64(tap, assets1[asset_type].base_precision, 10);
		  char *amount_str = mpd_to_sci(tap, 0);
		  log_trace("%s",amount_str);*/
		/*amount= 100000000000;
		  double transfer_amount1 = 0.5 * 0.9;

		  mpd_t *tap1 = decimal_to_mpd(transfer_amount1, 10);
		  log_trace("decimal_to_mpd %lf %s", transfer_amount1, mpd_to_sci(tap1, 0));*/
		//mpd_t *tap = decimal_to_mpd(amount, 10);
		//share_type transfer_amount = mul_mpd_to_int64(tap, assets1[asset_type].base_precision, 10);
		//share_type transfer_amount = mul_mpd_to_int64(tap, 10, 10);
		//char *amount_str = mpd_to_sci(tap, 0);
		string amount = rstripzero(row[1]);
		mpd_t * mpt_amount = decimal(amount.c_str(),10);
		share_type transfer_amount = mul_mpd_to_int64(mpt_amount,assets1[asset_type].base_precision,10);

		log_trace("%s,%s,%lu",user_id.c_str(),to_string(transfer_amount).c_str(),assets1[asset_type].base_precision);
		RestClient::Response r;

		r = transfer(from_id,user_id, asset_type, transfer_amount,priv);
		mpd_del(mpt_amount);
		//free(amount_str);
		//std::cout << witness_key_pairs.kps[0].id.str() << std::endl;
	}
	mysql_free_result(result);
	return 0;
}
