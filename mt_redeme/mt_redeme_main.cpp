#include <stdio.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/wait.h>
#include "mt_config.h"
#include "mt_redeme.h"
#include "mt_send_request.h"
//# include "me_operlog.h"

#include "fc/crypto/openssl.hpp"

const char *__process__ = "mt_redeme";
const char *__version__ = "0.1.0";

nw_timer cron_timer;

static void on_cron_check(nw_timer *timer, void *data) {
	dlog_check_all();
	if (signal_exit) {
		nw_loop_break();
		signal_exit = 0;
	}
}

static int init_process(void) {
	if (mt_settings.process.file_limit) {
		if (set_file_limit(mt_settings.process.file_limit) < 0) {
			return -__LINE__;
		}
	}
	if (mt_settings.process.core_limit) {
		if (set_core_limit(mt_settings.process.core_limit) < 0) {
			return -__LINE__;
		}
	}

	return 0;
}

static int init_log(void) {
	default_dlog = dlog_init(mt_settings.log.path, mt_settings.log.shift, mt_settings.log.max, mt_settings.log.num, mt_settings.log.keep);
	if (default_dlog == NULL)
		return -__LINE__;
	default_dlog_flag = dlog_read_flag(mt_settings.log.flag);
	// if (alert_init(&settings.alert) < 0)
	//     return -__LINE__;

	return 0;
}
void sig_chld_handler(int sig) {   
	pid_t pid;   
	if (sig == SIGCHLD) {   
		pid = wait(NULL);   
	}   
} 


int main(int argc, char *argv[]) {
	//fc::init_openssl();

	printf("process: %s version: %s, compile date: %s %s\n", __process__, __version__, __DATE__, __TIME__);

	if (argc < 2) {
		printf("usage: %s config.json\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// if (process_exist(__process__) != 0) {
	//     printf("process: %s exist\n", __process__);
	//     exit(EXIT_FAILURE);
	// }

	signal(SIGCHLD, sig_chld_handler);
	int ret;
	ret = init_mpd();
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "init mpd fail: %d", ret);
	}
	ret = init_mt_config(argv[1]);
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "load config fail: %d", ret);
	}
	ret = init_process();
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "init process fail: %d", ret);
	}
	ret = init_log();
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "init log fail: %d", ret);
	}

	ret = daemon(1, 1);
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "daemon fail: %d", ret);
	}


	ret = mt_send_request();
	if (ret < 0) {
		error(EXIT_FAILURE, errno, "init send_request fail: %d", ret);
	}

	ret = init_mt_redeme();
	if (ret < 0) {
		error(EXIT_FAILURE, errno, " init_mt_redeme fail: %d", ret);
	}


	nw_timer_set(&cron_timer, 0.5, true, on_cron_check, NULL);
	nw_timer_start(&cron_timer);

	log_vip("server start");
	log_stderr("server start");
	nw_loop_run();


	log_vip("server stop");

	return 0;
}
