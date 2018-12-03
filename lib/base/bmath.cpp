
# include "bmath.hpp"
# include "ut_log.h"
# include "types.hpp"
// mpd_t* devide_int64_to_mpd(int64_t a,int64_t b,int prec) {
//     mpd_t* mpda = int64(a,prec);
//     mpd_t* mpdb = int64(b,prec);
   
//     mpd_t *result = mpd_new(&mpd_ctx);
//     mpd_div(result,mpda  ,mpdb, &mpd_ctx);

//     if (prec) {
//         mpd_rescale(result, result, -prec, &mpd_ctx);
//     }

//     mpd_del(mpda);
//     mpd_del(mpdb);

//     return result;
// }

mpd_t* devide_int64_to_mpd(share_type a, share_type b,int prec) {
    mpd_t * mpda = decimal(to_string(a).c_str(),   prec);
    mpd_t * mpdb = decimal(to_string(b).c_str(),   prec);

     //log_trace("a %s",to_string(a).c_str());
     //log_trace("b %s",to_string(a).c_str());

     //log_trace("a2 %s",mpd_to_sci(mpda,0));
     //log_trace("b2 %s",mpd_to_sci(mpdb,0));

    mpd_t *result = mpd_new(&mpd_ctx);
    mpd_div(result,mpda  ,mpdb, &mpd_ctx);

    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }

    mpd_del(mpda);
    mpd_del(mpdb);

    return result;
}
/*
uint64_t mul_mpd_to_int64(mpd_t* a,uint64_t b,int prec) {

    mpd_t *base = int64(b,prec);
    mpd_t *result = mpd_new(&mpd_ctx);
    // log_trace("1 %s",mpd_to_sci(base,0));
    // log_trace("2 %s",mpd_to_sci(a,0));
    mpd_mul(result, a, base, &mpd_ctx);
    //log_trace("3 %s",mpd_to_sci(result,0));
    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }
//log_trace("4 %s",mpd_to_sci(result,0));
   char *str = mpd_to_sci(result, 0);
   string t(str);
   //log_trace("5 %s",t.c_str());
   uint64_t res;
   sscanf(t.c_str(),"%lu",&res);
   //uint64_t res = boost::lexical_cast<uint64_t>(t);

   free(str);
   mpd_del(base);
   mpd_del(result);
   
   return res;
}*/

share_type mul_mpd_to_int64(mpd_t* a,uint64_t b,int prec) {

    mpd_t *base = int64(b,prec);
    mpd_t *result = mpd_new(&mpd_ctx);
    // log_trace("1 %s",mpd_to_sci(base,0));
    // log_trace("2 %s",mpd_to_sci(a,0));
    mpd_mul(result, a, base, &mpd_ctx);
    //log_trace("3 %s",mpd_to_sci(result,0));
    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }
//log_trace("4 %s",mpd_to_sci(result,0));
   char *str = mpd_to_sci(result, 0);
   string t(str);
   share_type t1(t);
   //log_trace("5 %s",t.c_str());
//    uint64_t res;
//    sscanf(t.c_str(),"%lu",&res);
   //uint64_t res = boost::lexical_cast<uint64_t>(t);

   free(str);
   mpd_del(base);
   mpd_del(result);
   
   return t1;
}

uint64_t  mpd_to_int64(mpd_t* a, int prec) {

   
   char *str = mpd_to_sci(a, 0);
   //string t(str);
   //log_trace("5 %s",t.c_str());
   uint64_t res;
   sscanf(str,"%lu",&res);
   //uint64_t res = boost::lexical_cast<uint64_t>(t);

   free(str);
  
   return res;
}

mpd_t *decimal_to_mpd(double v, int prec) {

    char str[50];
    memset(str,'\0',50);
    snprintf(str,50,"%lf",v);
    str[49]='\0';
   // log_trace("%s",str);
    
    mpd_t *result = mpd_new(&mpd_ctx);
    mpd_ctx.status = 0;
    mpd_set_string(result, str, &mpd_ctx);
    if (mpd_ctx.status == MPD_Conversion_syntax) {
        mpd_del(result);
        return NULL;
    }

   // log_trace("%s",mpd_to_sci(result,0));
    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }

  // log_trace("%s",mpd_to_sci(result,0));

    return result;
}

/*
mpd_context_t mpd_ctx;

mpd_t *mpd_one;
mpd_t *mpd_ten;
mpd_t *mpd_zero;
mpd_t *mpd_minus_zero;

int init_mpd(void)
{
    mpd_ieee_context(&mpd_ctx, MPD_DECIMAL128);
    mpd_ctx.round = MPD_ROUND_DOWN;

    mpd_one = mpd_new(&mpd_ctx);
    mpd_set_string(mpd_one, "1", &mpd_ctx);
    mpd_ten = mpd_new(&mpd_ctx);
    mpd_set_string(mpd_ten, "10", &mpd_ctx);
    mpd_zero = mpd_new(&mpd_ctx);
    mpd_set_string(mpd_zero, "0", &mpd_ctx);
	mpd_minus_zero = mpd_new(&mpd_ctx);
    mpd_set_string(mpd_minus_zero, "-1.0", &mpd_ctx);

    return 0;
}

mpd_t *decimal(const char *str, int prec)
{
    mpd_t *result = mpd_new(&mpd_ctx);
    mpd_ctx.status = 0;
    mpd_set_string(result, str, &mpd_ctx);
    if (mpd_ctx.status == MPD_Conversion_syntax) {
        mpd_del(result);
        return NULL;
    }

    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }

    return result;
}

mpd_t *int64(int64_t v, int prec)
{
    mpd_t *result = mpd_new(&mpd_ctx);
    mpd_ctx.status = 0;
    mpd_set_i64(result, v, &mpd_ctx);
    if (mpd_ctx.status == MPD_Conversion_syntax) {
        mpd_del(result);
        return NULL;
    }

    if (prec) {
        mpd_rescale(result, result, -prec, &mpd_ctx);
    }

    return result;
}

char *rstripzero(char *str)
{
    if (strchr(str, 'e'))
        return str;
    char *point = strchr(str, '.');
    if (point == NULL)
        return str;
    int len = strlen(str);
    for (int i = len - 1; i >= 0; --i) {
        if (str[i] == '0') {
            str[i] = '\0';
            --len;
        } else {
            break;
        }
    }
    if (str[len - 1] == '.') {
        str[len - 1] = '\0';
    }

    return str;
}

int json_object_set_new_mpd(json_t *obj, const char *key, mpd_t *value)
{
    char *str = mpd_to_sci(value, 0);
    int ret = json_object_set_new(obj, key, json_string(rstripzero(str)));
    free(str);
    return ret;
}

int json_array_append_new_mpd(json_t *obj, mpd_t *value)
{
    char *str = mpd_to_sci(value, 0);
    int ret = json_array_append_new(obj, json_string(rstripzero(str)));
    free(str);
    return ret;
}
*/
