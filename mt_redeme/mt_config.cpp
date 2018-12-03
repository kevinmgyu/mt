/*
 * Description: 
 *     History: yang@haipo.me, 2017/03/16, create
 */

# include "mt_config.h"
# include "asset.cpp"

// string slice_path = "slice";
// bool g_is_test_mode = false;


std::map<std::string,int> assets_map;
std::map<string,std::map<int,int>>assets_adr;
std::map<int, simple_asset> assets;
struct mt_settings mt_settings;
// string slice_path = "slice";
// bool g_is_test_mode = false;

static int init_asset() {
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
		simple_asset a;
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

		assets[a.id] = a;

		assets_map[a.symbol] = a.id;
		std::map<int,int> tmp;
		tmp[a.chain_type]=a.id;	
		assets_adr[a.chain_contract_address]=tmp;
	}
	return 0;
}



static int read_config_from_json(json_t *root) {
	int ret;
	ret = read_cfg_bool(root, "debug", &mt_settings.debug, false, false);
	if (ret < 0) {
		printf("read debug config fail: %d\n", ret);
		return -__LINE__;
	}
	ret = load_cfg_process(root, "process", &mt_settings.process);
	if (ret < 0) {
		printf("load process config fail: %d\n", ret);
		return -__LINE__;
	}
	ret = load_cfg_log(root, "log", &mt_settings.log);
	if (ret < 0) {
		printf("load log config fail: %d\n", ret);
		return -__LINE__;
	}

	ret = read_cfg_str(root, "httpurl", &mt_settings.httpurl, NULL);
	if (ret < 0) {
		printf("load httpurl fail: %d\n", ret);
		return -__LINE__;
	}

	ret = read_cfg_str(root, "pledge_ad", &mt_settings.pledge_ad, NULL);
	if (ret < 0) {
		printf("load pledge_ad fail: %d\n", ret);
		return -__LINE__;
	}

	ret = read_cfg_str(root, "priv", &mt_settings.priv, NULL);
	if (ret < 0) {
		printf("load priv fail: %d\n", ret);
		return -__LINE__;
	}
	
	ret = read_cfg_uint64(root, "end_num", &mt_settings.end_num, false, 10);
	if (ret < 0) {
		printf("load history_thread fail: %d", ret);
		return -__LINE__;
	}

	ret = load_cfg_mysql(root, "db_history", &mt_settings.db_history);
	if (ret < 0) {
		printf("load history db config fail: %d\n", ret);
		return -__LINE__;
	}


	ret = read_cfg_int(root, "history_thread", &mt_settings.history_thread, false, 10);
	if (ret < 0) {
		printf("load history_thread fail: %d", ret);
		return -__LINE__;
	}



	ret = load_cfg_rpc_clt(root, "matchengine", &mt_settings.matchengine);
	if (ret < 0) {
		printf("load matchengine clt config fail: %d\n", ret);
		return -__LINE__;
	}

	set_btt_base_url(mt_settings.httpurl);
	ret = init_asset();
	if(ret != 0 ) {
		printf("init asset failed");
		return -__LINE__;
	}

	return 0;
}

int init_mt_config(const char *path) {

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


optional<simple_asset> get_asset_marketing(int a) {

	if( assets.find(a) == assets.end() ) {
		return optional<simple_asset>();
	}

	return optional<simple_asset>(assets[a]);
}

optional<simple_asset> get_asset_by_chaintype_and_contract(int chain_type,string contract) {

	for( auto sa : assets ) {
		simple_asset s = sa.second;
		if( s.chain_type == chain_type && s.chain_contract_address == contract ) {
			return optional<simple_asset>(s);
		}
	}

	return optional<simple_asset>();
}
