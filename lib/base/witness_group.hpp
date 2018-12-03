#pragma once

# include <vector>
# include <map>
# include <string>
# include "ut_log.h"
# include "ut_decimal.h"
# include "types.hpp"
# include "ut_base.hpp"
# include "block.hpp"
# include "witness.hpp"
# include "pledge.hpp"
# include "genesis.hpp"
# include <mutex>  

struct witness_group_slice { 
    witness_id_type             leader;
    vector<witness_id_type>     witness;
    vector<share_type>          weight;
    share_type                  total;
    int                         chain_type;
    block_num_type              start_block;
    int                         duration;
 
    witness_group_slice() {

    }

    witness_group_slice(const witness_group_slice& p) {
       for( size_t i = 0; i < p.witness.size(); ++i ) {
            witness.push_back(p.witness[i]);
        }

        for( size_t i = 0; i < p.weight.size(); ++i ) {
            weight.push_back(p.weight[i]);
        }
 
        leader = p.leader;
        total = p.total;
        chain_type = p.chain_type;
        start_block = p.start_block;
        duration = p.duration;
    }
};

struct witness_group { 
    int                  chain_type;
    std::map<witness_id_type,witness> g_witness;
    std::map<witness_id_type,share_type> g_witness_weight;
    std::vector<witness_id_type> g_witness_ids;

    witness_id_type      g_witness_now;
    share_type           g_amount;
    block_num_type       g_start_term;
    int                  g_duration;
    bool                 is_init;
    std::set<witness_id_type> g_suspended;

    witness_group() {

    }

    witness_group(const witness_group& p) {
        chain_type = p.chain_type;
        g_witness_now = p.g_witness_now;
        g_amount = p.g_amount;
        g_start_term = p.g_start_term;
        g_duration = p.g_duration;
        is_init = p.is_init;

        g_witness.clear();
        for(std::map<witness_id_type,witness>::const_iterator it = p.g_witness.begin(); it != p.g_witness.end(); ++it ) {
            g_witness.insert(*it);
        }

        g_witness_weight.clear();
        for(std::map<witness_id_type,share_type>::const_iterator it = p.g_witness_weight.begin(); it != p.g_witness_weight.end(); ++it ) {
            g_witness_weight.insert(*it);
        }

        g_witness_ids.clear();
        for(size_t i = 0; i < p.g_witness_ids.size(); ++i ) {
            g_witness_ids.push_back(p.g_witness_ids[i]);
        }

        g_suspended.clear();
        for(std::set<witness_id_type>::const_iterator it = p.g_suspended.begin(); it != p.g_suspended.end(); ++it ) {
            g_suspended.insert(*it);
        }
    }

    int init_witness(const int chain_type);
    //int init_witness(const int chain_type);
    int add_witness(const witness& w );
    memo_data  elect_a_witness( optional<block_id_type> block_hash, 
                        pledge_group& pg,int start_block,int duration);
    memo_data  get_memo();
    
    int update_witness(const witness& w);
    int remove_witness(const witness_id_type& id);
    void set_slice(const witness_group_slice& slice);
private:
   // void sign_block(  key_pairs& kps, signed_block& block);
   // void vertify_block(  signed_block& block);
    //void sign_transaction(   key_pairs& kps, signed_transaction& transaction);
   // void sign_transaction_by_weight(   key_pairs& kps, signed_transaction& transaction);
   // void vertify_transaction(signed_transaction& transaction);
    
    witness_group_slice get_slice();
    //std::mutex mut;  
};

double get_point(const ripemd160& block_id_hash);

FC_REFLECT( witness_group_slice, (witness)(weight)(total)(chain_type)(start_block)(duration)(leader) )
FC_REFLECT( witness_group, (chain_type)(g_witness)(g_witness_weight)(g_witness_ids)(g_witness_now)(g_amount)(g_start_term)(g_duration)(is_init)(g_suspended) )