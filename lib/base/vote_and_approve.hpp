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
#include "asset.hpp"
#include "base_operation.hpp"

/**
    * op_wrapper is used to get around the circular definition of operation and proposals that contain them.
    */
struct op_wrapper;

struct proposal_operation : public base_operation {
    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;

    account_id_type proposaler;

    vector<op_wrapper> proposed_ops;

    time_point_sec expiration_time;

    account_id_type fee_payer() const { return proposaler; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};

struct vote_operation : public base_operation {

    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    operation_id_type proposal_id;
    account_id_type voter;
    string          public_address;
    bool result;
    // signature_type signature;
    byte_array signature;

    account_id_type fee_payer() const { return voter; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};


struct vote2_operation : public vote_operation {

    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    byte_array      signature1;
    
    operation_result evaluate2(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);

};

struct execute_operation : public base_operation {

    struct fee_parameters_type {
        uint64_t fee = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
    };

    share_type fee;
    operation_id_type proposal_id;
    account_id_type executer;
    bool result;

    account_id_type fee_payer() const { return executer; }
    void validate() const;
    share_type calculate_fee() const;
    share_type get_fee() const;

    operation_result evaluate(
        block_num_type block_num,
        block_chain_type chain,
        string operation_id,
        string dapp_id,
        bool isreal,
        bool ismain);
};

FC_REFLECT(vote_operation::fee_parameters_type, (fee))
FC_REFLECT(vote_operation, (fee)(proposal_id)(voter)(public_address)(result)(signature))
FC_REFLECT_DERIVED(vote2_operation, (vote_operation),(signature1))
//FC_REFLECT_DERIVED(vote_operation, (base_operation), (fee)(proposal_id)(voter)(public_address)(result)(signature))

FC_REFLECT(execute_operation::fee_parameters_type, (fee))
FC_REFLECT(execute_operation, (fee)(proposal_id)(executer)(result))
//FC_REFLECT_DERIVED(execute_operation, (base_operation), (fee)(proposal_id)(executer)(result))


FC_REFLECT(proposal_operation::fee_parameters_type, (fee))
FC_REFLECT(proposal_operation, (fee)(proposaler)(expiration_time)(proposed_ops))
//FC_REFLECT_DERIVED(proposal_operation, (base_operation),(fee)(proposaler)(expiration_time)(proposed_ops))
