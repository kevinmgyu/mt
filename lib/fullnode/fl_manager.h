#pragma once
#include "block_database.hpp"


int init_fullnode();
int fini_fullnode();

void fl_add_block_callback(std::function< int (const signed_block& block) >);

block_num_type fl_get_block_num();
optional<string> fl_fetch_by_string(block_num_type num);
