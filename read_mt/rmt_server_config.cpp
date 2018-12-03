
# include "rmt_server_config.h"

struct mt_rdg_settings mt_rdg_settings;
// string slice_path = "slice";
// bool g_is_test_mode = false;

static int read_config_from_json(json_t *root) {
	int ret;
	ret = read_cfg_bool(root, "debug", &mt_rdg_settings.debug, false, false);
	if (ret < 0) {
		printf("read debug config fail: %d\n", ret);
		return -__LINE__;
	}
	ret = load_cfg_process(root, "process", &mt_rdg_settings.process);
	if (ret < 0) {
		printf("load process config fail: %d\n", ret);
		return -__LINE__;
	}
	ret = load_cfg_log(root, "log", &mt_rdg_settings.log);
	if (ret < 0) {
		printf("load log config fail: %d\n", ret);
		return -__LINE__;
	}
	// ret = load_cfg_alert(root, "alert", &fl_server_settings.alert);
	// if (ret < 0) {
	//     printf("load alert config fail: %d\n", ret);
	//     return -__LINE__;
	// }
	ret = load_cfg_rpc_svr(root, "svr", &mt_rdg_settings.svr);
	if (ret < 0) {
		printf("load svr config fail: %d\n", ret);
		return -__LINE__;
	}

	ret = load_cfg_mysql(root, "db_history", &mt_rdg_settings.db_history);
	if (ret < 0) {
		printf("load history db config fail: %d\n", ret);
		return -__LINE__;
	}

	ERR_RET_LN(read_cfg_int(root, "worker_num", &mt_rdg_settings.worker_num, false, 10));
	ERR_RET_LN(read_cfg_int(root, "asset_type", &mt_rdg_settings.asset_type, false, 0));

	ret = read_cfg_str(root, "pledge_ad", &mt_rdg_settings.pledge_ad, NULL);
	if (ret < 0) {
		printf("pledge_ad fail: %d\n", ret);
		return -__LINE__;
	} 
	ret = read_cfg_uint64(root, "base_precision", &mt_rdg_settings.base_precision, false, 10);
	if (ret < 0) {
		printf("load base_precision fail: %d", ret);
		return -__LINE__;
	}
	ret = read_cfg_uint64(root, "save_precision", &mt_rdg_settings.save_precision, false, 10);
	if (ret < 0) {
		printf("load save_precision fail: %d", ret);
		return -__LINE__;
	}
	ERR_RET_LN(read_cfg_int(root, "start_num", &mt_rdg_settings.start_num, false, 0));
	ERR_RET_LN(read_cfg_int(root, "end_num", &mt_rdg_settings.end_num, false, 0));
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

