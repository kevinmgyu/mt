# ifndef MT_MYSQL_H
# define MT_MYSQL_H
#include "mt_config.h"

int init_history(void);
int fini_history(void);

int append_block_info(double t, uint32_t block_num);
int append_mt_pledge(double t, uint32_t block_num, const char* transaction_id,const char* from_user_id, const char *to_user_id, mpd_t *amount, int assetid);
int append_mt_pledge_sum(double t, const char* from_user_id, mpd_t *amount, int assetid);
int append_deposit(double t, 
    uint32_t block_num, const char* transaction_id,const char* proposal_transaction_id,
    const char* external_transaction_id, const char* sign,const char* user_id, 
    const char* from_external_address, int asset_type, const char* chain_contract_address,
    int chain_type,mpd_t *amount,int ret);

int append_withdraw(double t, 
    uint32_t block_num, const char* transaction_id,const char* proposal_transaction_id,
    const char* external_transaction_id,
    const char* user_id, 
    const char* to_external_address, int asset_type, const char* chain_contract_address,
    int chain_type,mpd_t *amount, int ret, int code);

# endif

