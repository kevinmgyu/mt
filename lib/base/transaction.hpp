/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include "operations.hpp"
#include "types.hpp"
#include "account_database.hpp"

#include <numeric>

/**
    *  @brief groups operations that should be applied atomically
    */
struct transaction {
    /**
       * Least significant 16 bits from the reference block number. If @ref relative_expiration is zero, this field
       * must be zero as well.
       */
    uint16_t ref_block_num = 0;
    /**
       * The first non-block-number 32-bits of the reference block ID. Recall that block IDs have 32 bits of block
       * number followed by the actual block hash, so this field should be set using the second 32 bits in the
       * @ref block_id_type
       */
    uint32_t ref_block_prefix = 0;

    /**
       * This field specifies the absolute expiration for this transaction.
       */
    optional<fc::time_point_sec> expiration;
    fc::time_point_sec timestamp;

    vector<operation> operations;
    extensions_type extensions;
    optional<account_id_type> dapp;

   // optional<transaction_id_type> old_id;

    /// Calculate the digest for a transaction
    digest_type digest() const;

    //transaction_id_type id(bool refresh) const;
    transaction_id_type id() const;
    void validate() const;
    /// Calculate the digest used for signature validation
    //digest_type         sig_digest( const chain_id_type& chain_id )const;
    digest_type sig_digest(const block_chain_type &chain_id) const;
    uint8_t validate_type = 0;
    optional<transaction_id_type> proposal_transaction_id;

    void set_expiration(fc::time_point_sec expiration_time);
    void set_reference_block(const block_id_type &reference_block);

    /// visit all operations
    template <typename Visitor>
    vector<typename Visitor::result_type> visit(Visitor &&visitor) {
        vector<typename Visitor::result_type> results;
        for (auto &op : operations)
            results.push_back(op.visit(std::forward<Visitor>(visitor)));
        return results;
    }
    template <typename Visitor>
    vector<typename Visitor::result_type> visit(Visitor &&visitor) const {
        vector<typename Visitor::result_type> results;
        for (auto &op : operations)
            results.push_back(op.visit(std::forward<Visitor>(visitor)));
        return results;
    }

   // void get_required_authorities(flat_set<account_id_type> &active) const;
   void get_required_authorities( 
        flat_set<account_id_type>& active, flat_set<account_id_type>& owner, vector<authority>& other )const;
};


struct signed_checker {
    //std::function<const authority *(account_id_type)> get_active;
    flat_set<address> address_sigs;
    const account_database* g_account_database;

    signed_checker(
       // const std::function<const authority *(account_id_type)> &ga,
         const account_database* ga, flat_set<address> as
    ) {
        address_sigs = as;
        //get_active = ga;
        g_account_database = ga;
    }

    bool check_authority(
        // const flat_set<fc::ecc::public_key>& public_key_sigs,
        optional<authority> auth, uint32_t depth = 0) {
        if (!auth) {
            return false;
        }

        if (depth == max_recursion) {
            return false;
        }

      //  const authority &auth = *au;

        uint32_t total_weight = 0;
        // for (const auto &k : auth.key_auths)
        //     if (signed_by(k.first)) {
        //         total_weight += k.second;
        //         if (total_weight >= auth.weight_threshold)
        //             return true;
        //     }

        for (const auto &k : auth->address_auths)
            // if (signed_by(k.first)) {
            if (address_sigs.find(k.first) != address_sigs.end()) {
                total_weight += k.second;
                if (total_weight >= auth->weight_threshold)
                    return true;
            }

        for (const auto &a : auth->account_auths) {
            // if (approved_by.find(a.first) == approved_by.end()) {
            if (check_authority(g_account_database->get_active(a.first), depth + 1)) {
                total_weight += a.second;
            }
            // } else {
            //     total_weight += a.second;
            //     if (total_weight >= auth.weight_threshold)
            //         return true;
            // }
        }
        return total_weight >= auth->weight_threshold;
    }
};

/**
    *  @brief adds a signature to a transaction
    */
struct signed_transaction : public transaction {
    signed_transaction(const transaction &trx = transaction())
        : transaction(trx) {}

    /** signs and appends to signatures */
    const signature_type &sign(const private_key_type &key, const block_chain_type &chain_id);

    /** returns signature but does not append */
    signature_type sign(const private_key_type &key, const block_chain_type &chain_id) const;

    /**
       *  The purpose of this method is to identify some subset of
       *  @ref available_keys that will produce sufficient signatures
       *  for a transaction.  The result is not always a minimal set of
       *  signatures, but any non-minimal result will still pass
       *  validation.
       */
    set<public_key_type> get_required_signatures(
        const block_chain_type &chain_id,
        const flat_set<public_key_type> &available_keys,
        const std::function<const authority *(account_id_type)> &get_active,
        const std::function<const authority *(account_id_type)> &get_owner,
        uint32_t max_recursion = GRAPHENE_MAX_SIG_CHECK_DEPTH) const;

   //uint32_t max_recursion = GRAPHENE_MAX_SIG_CHECK_DEPTH
      bool verify_authority(
           block_chain_type chain_id,
             account_database* g_account_database);
        // const std::function<const authority*(account_id_type)>& get_active,
        // const std::function<const authority*(account_id_type)>& get_owner ) ;

    //bool verify_authority(const block_chain_type &chain_id) const;
    bool contain_signature(const block_chain_type &chain_id, const address &a) const;

    /**
       * This is a slower replacement for get_required_signatures()
       * which returns a minimal set in all cases, including
       * some cases where get_required_signatures() returns a
       * non-minimal set.
       */

    set<public_key_type> minimize_required_signatures(
        const block_chain_type &chain_id,
        const flat_set<public_key_type> &available_keys,
        const std::function<const authority *(account_id_type)> &get_active,
        const std::function<const authority *(account_id_type)> &get_owner,
        uint32_t max_recursion = GRAPHENE_MAX_SIG_CHECK_DEPTH) const;

    flat_set<public_key_type> get_signature_keys(const block_chain_type &chain_id) const;

    vector<signature_type> signatures;

    /// Removes all operations and signatures
    void clear() {
        operations.clear();
        signatures.clear();
    }

    bool verify_fee();
};

void verify_authority(const vector<operation> &ops, const flat_set<public_key_type> &sigs,
                      const std::function<const authority *(account_id_type)> &get_active,
                      const std::function<const authority *(account_id_type)> &get_owner,
                      uint32_t max_recursion = GRAPHENE_MAX_SIG_CHECK_DEPTH,
                      bool allow_committe = false,
                      const flat_set<account_id_type> &active_aprovals = flat_set<account_id_type>(),
                      const flat_set<account_id_type> &owner_approvals = flat_set<account_id_type>());

/**
    *  @brief captures the result of evaluating the operations contained in the transaction
    *
    *  When processing a transaction some operations generate
    *  new object IDs and these IDs cannot be known until the
    *  transaction is actually included into a block.  When a
    *  block is produced these new ids are captured and included
    *  with every transaction.  The index in operation_results should
    *  correspond to the same index in operations.
    *
    *  If an operation did not create any new object IDs then 0
    *  should be returned.
    */
struct processed_transaction : public signed_transaction {
    processed_transaction(const signed_transaction &trx = signed_transaction())
        : signed_transaction(trx) {}

    vector<operation_result> operation_results;
    //vector<share_type> cost_fees;

    digest_type merkle_digest() const;
};

/// @} transactions group

//fc::sha256 get_chain_id(string de = "");

//operation_id_make_slicetype get_operation_id(const transaction_id_type &tran_id, int op_id);
operation_id_type get_operation_id(const string &tran_id, int op_id);

// bool is_proposal_transaction(const signed_transaction& trx);
// string get_transaction_proposal_id(const signed_transaction& trx);
//string get_operation_id(const transaction& tran, int op_id);

vector<signed_transaction> sort_transactions(const vector<signed_transaction> &t);

//template<typename T>

FC_REFLECT(transaction, (timestamp)(expiration)(operations)(validate_type)(dapp)(proposal_transaction_id))
//FC_REFLECT( transaction,   )
FC_REFLECT_DERIVED(signed_transaction, (transaction), (signatures))
FC_REFLECT_DERIVED(processed_transaction, (signed_transaction), (operation_results))
