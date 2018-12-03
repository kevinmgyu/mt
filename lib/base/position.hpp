#pragma once
#include "types.hpp"

/*
order_book_id	str	合约代码
quantity	int	当前持仓股数
pnl	float	持仓累计盈亏
sellable	int	该仓位可卖出股数。T＋1的市场中sellable = 所有持仓-今日买入的仓位
market_value	float	获得该持仓的实时市场价值
value_percent	float	获得该持仓的实时市场价值在总投资组合价值中所占比例，取值范围[0, 1]
avg_price	float	平均建仓成本
*/

struct position {
    asset_id_type 		 id;

    share_type		     quantity;
    double 				 pnl;
    share_type           sellable;
    share_type           market_value;
    double               value_percent;
    
    position() {
    }

    position(const position& a) {
        
    }
     
};
 
FC_REFLECT( position,  (id)(symbol)(name)(show_precision)(save_precision)(base_precision)(is_native)(chain_type)(chain_contract_address)(creator)(min_trade_amount)(min_transfer_amount)(total_supply))
 
