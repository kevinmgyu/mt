#pragma once

#include "mt_mysql.h"

#include "fl_manager.h"
#include "fl_config.h"
#include "operations.hpp"
#include "threadsafe_queue.h"
int handle_mt_block(const signed_block& block);
