
#pragma once
#include "types.hpp"
#include "ut_log.h"
#include "witness.hpp"

enum ProposalResultRet {
    PRR_NO_PROPOSAL = -1,
    PRR_NO_RESULT = 0,
    PRR_PASSED = 1,
    PRR_REJECTED = 2
};

struct witness_info {
    std::string pub_key;
    witness_id_type witness_id;
    
    witness_info(const std::string& key, const witness_id_type& id) : pub_key(key), witness_id(id) {}
};

struct vote {
    vote_id_type id;
    witness_id_type witness;
    string public_address;
    bool result;
    block_num_type block_num;
    byte_array signature;

    vote() {
    }

    vote(const vote_id_type &vote_id, const witness_id_type &wid, const string &pa,
         const bool res, const block_num_type b,
         const byte_array &s) {
        id = vote_id;
        witness = wid;
        public_address = pa;
        result = res;
        block_num = b;
        signature = s;
    }

    vote(const vote &v) {
        id = v.id;
        witness = v.witness;
        public_address = v.public_address;
        result = v.result;
        block_num = v.block_num;
        signature = v.signature;
    }
};

struct proposal {
    string external_id;
    proposal_id_type id;
    transaction_id_type tran_id;
    uint32_t op_seq;
    share_type total;
    std::map<witness_id_type, witness> g_witness;
    std::map<witness_id_type, share_type> g_witness_weight;
    std::map<witness_id_type, vote> votes;
    block_num_type block_num;
    bool is_finished;

    proposal() {
        is_finished = false;
    }

    proposal(
            const string& eid,
            const proposal_id_type &proposal_id,
             const transaction_id_type &tid, const uint32_t &o,
             const std::map<witness_id_type, witness> witness,
             std::map<witness_id_type, share_type> witness_weight,
             const block_num_type b) {
        external_id = eid;
        id = proposal_id;
        tran_id = tid;
        op_seq = o;

        g_witness_weight.clear();
        total = 0;

        g_witness.clear();
        for (auto w : witness) {
            g_witness[w.first] = w.second;
        }

        for (auto w : witness_weight) {
            witness_id_type aa = w.first;
            g_witness_weight[aa] = w.second;
            //log_trace("[CMD_SYNC_DEPOSIT_WITHDRAW] proposal %s %lu", aa.c_str(), w.second);
            total += w.second;
        }
        //log_trace("[CMD_SYNC_DEPOSIT_WITHDRAW] proposal total %lu", total.value);

        is_finished = false;
        votes.clear();
        block_num = b;
    }

    proposal(const proposal &p) {
        external_id = p.external_id;
        id = p.id;
        tran_id = p.tran_id;
        op_seq = p.op_seq;

        g_witness.clear();
        for (auto w : p.g_witness) {
            g_witness[w.first] = w.second;
        }

        g_witness_weight.clear();
        for (auto w : p.g_witness_weight) {
            g_witness_weight[w.first] = w.second;
        }
        total = p.total;

        votes.clear();
        for (const std::pair<witness_id_type, vote>& w : p.votes) {
            votes[w.first] = w.second;
        }
        is_finished = p.is_finished;
    }

    bool vote_proposal(const vote_id_type &vote_id,
                       const witness_id_type &witness,
                       const string& public_address,
                       const bool result,
                       const block_num_type block_num,
                       const byte_array &s) {
        if (g_witness_weight.find(witness) == g_witness_weight.end()) {
            return false;
        }

        if (votes.find(witness) != votes.end()) {
            return false;
        }

        //string public_key = g_witness[witness].pub_key.key_data;
        fc::ecc::public_key pc = g_witness[witness].pub_key;
        vote v(vote_id, witness, public_address, result, block_num, s);
        votes[witness] = v;

        return true;
    }

    bool can_vote(const witness_id_type &witness) {
        if (g_witness_weight.find(witness) == g_witness_weight.end()) {
            return false;
        }

        if (votes.find(witness) != votes.end()) {
            return false;
        }

        if (is_finished) {
            return false;
        }

        return true;
    }

    ProposalResultRet get_proposal_result() {
        share_type pass = 0;
        share_type reject = 0;
        for (const std::pair<witness_id_type, vote>& v : votes) {
            if (v.second.result) {
                pass = pass + g_witness_weight[v.first];
            } else {
                reject = reject + g_witness_weight[v.first];
            }
        }

        if (pass > total / 2) {
            return PRR_PASSED;
        }

        if (reject > total / 2) {
            return PRR_REJECTED;
        }

        return PRR_NO_RESULT;
    }
};

FC_REFLECT(vote, (id)(witness)(result)(block_num)(signature))
FC_REFLECT(proposal, (external_id)(id)(tran_id)(op_seq)(total)(g_witness_weight)(votes)(block_num)(is_finished))

