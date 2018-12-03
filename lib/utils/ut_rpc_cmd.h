/*
 * Description: 
 *     History: yang@haipo.me, 2016/09/30, create
 */

# ifndef _UT_RPC_CMD_H_
# define _UT_RPC_CMD_H_

// balance
# define CMD_BALANCE_QUERY          101
//# define CMD_BALANCE_UPDATE         102
# define CMD_BALANCE_HISTORY        103
# define CMD_ASSET_LIST             104
# define CMD_ASSET_SUMMARY          105
# define CMD_BTC_UTXO_LIST          106
# define CMD_BTC_UTXO_STATUS        107
# define CMD_BALANCE_QUERY2         108
# define CMD_BALANCE_UPDATE         109

// trade
//# define CMD_ORDER_PUT_LIMIT        201
//# define CMD_ORDER_PUT_MARKET       202
# define CMD_ORDER_QUERY            203
//# define CMD_ORDER_CANCEL           204
# define CMD_ORDER_BOOK             205
# define CMD_ORDER_BOOK_DEPTH       206
# define CMD_ORDER_DETAIL           207
# define CMD_ORDER_HISTORY          208
# define CMD_ORDER_DEALS            209
# define CMD_ORDER_DETAIL_FINISHED  210

// market
# define CMD_MARKET_STATUS          301
# define CMD_MARKET_KLINE           302
# define CMD_MARKET_DEALS           303
# define CMD_MARKET_LAST            304
# define CMD_MARKET_STATUS_TODAY    305
# define CMD_MARKET_USER_DEALS      306
# define CMD_MARKET_LIST            307
# define CMD_MARKET_SUMMARY         308

//# define CMD_FULL_NODE_SIZE         401
# define CMD_FULL_NODE              401

# define CMD_BTC_FEERATE            501
# define CMD_BTC_TXID               502
# define CMD_BTC_WITHDRAW           503
# define CMD_BTC_PUSH_RAW_TX        504
# define CMD_BTC_SYNC_BLOCK         505

//# define CMD_CREATE_ASSET          401
//# define CMD_CREATE_MARKET         402

//# define CMD_DEPOSIT_ASSET         501
//# define CMD_WITHDRAW_ASSET        502
# define CMD_ACCOUNT_IS_REGISTERED   701

# define CMD_BLOCKCHAIN              801

# define CMD_SYNC_BLOCK              901
# define CMD_SYNC_TRANSACTION        902


#define CMD_DH_LIST  10000
#define CMD_DH_QUERY 10001
#define CMD_DH_NEWEST_TRANSACTION 10003

#define CMD_MARKETING_DAY  10100
#define CMD_MARKETING_USER_REALTIME 10101
#define CMD_GET_DPOSIT_STATUS_BY_ETH_TRAN_ID 10102
#define CMD_GET_WITHDRAW_STATUS_BY_BTT_TRAN_ID 10103
#define CMD_USER_REALTIME 10104
#define CMD_MARKETING_DAY_ALL  10105
#define CMD_DAILY_REDEME  10106
#define CMD_USER_PLEDGE  10107
#define CMD_PLEDGE_SUM  10108
#define CMD_USER_REDEME  10109
#define CMD_USER_SHARE  10110
#define CMD_REALTIME_INCOME  10111
#define CMD_USER_DIVIDEND_COMOUTE  10112
#define CMD_GET_PLEDGE_INFO  10113
#define CMD_GET_BLOCK_INFO  10114
#define CMD_USER_DIVIDEND  10115
#define CMD_USER_DIVIDEND_EXPECT 10116

//# define CMD_SYNC_DEPOSIT_WITHDRAW   903





# endif

