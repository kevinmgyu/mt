# ifndef _RMT_READER_H_
# define _RMT_READER_H_

# include "rmt_server_config.h"

json_t *get_daily_redeme(MYSQL *conn, int32_t start, const char* user, string amount);
json_t *get_user_pledge(MYSQL *conn, int32_t start, int32_t end, const char* user) ;
json_t *get_user_redeme(MYSQL *conn, int32_t start, int32_t end, const char* user) ;
json_t *get_pledge_sum(MYSQL *conn, int32_t start, int32_t end) ;
json_t *get_user_share(MYSQL *conn, int32_t start, int32_t end, const char* user) ;
json_t *set_realtime_income(MYSQL *conn, int32_t start, string amount);
json_t *get_dividend_compute(MYSQL *conn, int32_t start, int32_t end, const char* user);
json_t *get_pledge_info(MYSQL *conn);
json_t *get_block_info(MYSQL *conn);
json_t *get_user_dividend(MYSQL *conn, int32_t start, int32_t end, const char* user);
json_t *get_dividend_expect(MYSQL *conn, int32_t start, int32_t end, const char* user);


# endif

