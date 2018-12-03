#pragma once

# include "ut_decimal.h"
# include "types.hpp"

//mpd_t* devide_int64_to_mpd(int64_t a,int64_t b,int prec);
mpd_t* devide_int64_to_mpd(share_type a, share_type b,int prec);
share_type mul_mpd_to_int64(mpd_t* a,uint64_t b,int prec);
//uint64_t mpd_to_int64(mpd_t* a, int prec);

mpd_t *decimal_to_mpd(double v, int prec);