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
# include "base_operation.hpp"


struct pledge { 
    string                        id;
    // address                       from;
    // address                       to;
    account_id_type               from;
    account_id_type               to;
    share_type                    amount;
    fc::time_point_sec            timestamp;
    optional<block_num_type>        start_term;
    optional<block_num_type>        end_term;
    bool                          wait_to_reedm;

    pledge() {

    }
    
    pledge(const pledge& p) {
        id = p.id;
        from = p.from;
        to = p.to;
        amount = p.amount;
        timestamp = p.timestamp;
        start_term = p.start_term;
        end_term = p.end_term;
        wait_to_reedm = p.wait_to_reedm;
    }
};

struct pledge_slice { 
    std::map<witness_id_type,share_type> m_witness_group;

    pledge_slice() {

    }

    pledge_slice(const pledge_slice& p) {

        for(std::map<witness_id_type,share_type>::const_iterator it = p.m_witness_group.begin(); it != p.m_witness_group.end(); ++it ) {
            m_witness_group.insert(*it);
        }
    }
};

struct pledge_group { 
    std::map<pledge_id_type,pledge > _pledges_;
    std::map<witness_id_type,std::set<pledge_id_type> > _pledges_by_witness;
    std::vector<pledge_slice> _pledge_slice;
    std::map<witness_id_type,share_type> _witness_group;
    fee_distribution _fee_distribution;

    pledge_group() {

    }

    pledge_group(const pledge_group& p) {

        for(std::map<pledge_id_type,pledge>::const_iterator it = p._pledges_.begin(); it != p._pledges_.end(); ++it ) {
            _pledges_.insert(*it);
        }

        for(std::map<witness_id_type,share_type>::const_iterator it = p._witness_group.begin(); it != p._witness_group.end(); ++it ) {
            _witness_group.insert(*it);
        }

        for(std::map<witness_id_type,std::set<pledge_id_type>>::const_iterator it = p._pledges_by_witness.begin(); 
                it != p._pledges_by_witness.end(); ++it ) {
            std::set<pledge_id_type> ns;
            for(std::set<pledge_id_type>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 ) {
                ns.insert(*it2);
            }
            
            _pledges_by_witness[it->first] = ns;
        }

        for(size_t i = 0; i < p._pledge_slice.size(); ++i ) {
            _pledge_slice.push_back(p._pledge_slice[i]);
        }

        _fee_distribution.clear();
    }
    
    share_type get_total_amount();
    share_type get_amount_by_id(const witness_id_type& id);
    void       init();
    void       next_term();
    vector<pledge>  clear_redeem();
    void       snapshot();
    void       add_one_block_fees(const account_id_type add,  const asset_id_type asset_type, const share_type amount);


    int       add_one_pledge(pledge p);
    int       redeem_one_pledge(pledge_id_type pledge_id);
    optional<pledge> get_pledge_by_id(pledge_id_type pledge_id);
};
 

FC_REFLECT( pledge, (id)(from)(to)(amount)(timestamp)(start_term)(end_term)(wait_to_reedm))
FC_REFLECT( pledge_slice, (m_witness_group))
FC_REFLECT( pledge_group, (_witness_group)(_pledges_)(_pledges_by_witness)(_pledge_slice) )
