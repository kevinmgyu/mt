#pragma once
#include "transaction.hpp"
#include "memo.hpp"

struct block_meta_info {
    block_num_type num;
    block_chain_type     type;
};

struct block_header
{
    digest_type                   digest()const;
    block_id_type                 previous;
    //uint32_t                      block_num()const { return num_from_id(previous) + 1; }
    block_num_type                block_num;
    fc::time_point_sec            timestamp;
    witness_id_type               witness;
    checksum_type                 transaction_merkle_root;
    extensions_type               extensions;
    block_chain_type              block_type = 0;
    vector<memo_data>             memos;
    vector<block_meta_info>       solved_blocks;

    static uint32_t num_from_id(const block_id_type& id);
};

struct signed_block_header : public block_header
{
    block_id_type               id()const;
    vector<fc::ecc::public_key> signee()const;
    void                        sign( const fc::ecc::private_key& signer );
    bool                        add_signature( const fc::ecc::public_key& signer, const signature_type& signature );
    signature_type              get_signature( const fc::ecc::private_key& signer );
    bool                        validate_signee( const fc::ecc::public_key& expected_signee )const;

    vector<signature_type>      witness_signature;
};

struct signed_block : public signed_block_header
{
    checksum_type calculate_merkle_root()const;
    vector<processed_transaction> transactions;

    friend bool operator < (signed_block a, signed_block b)  
    {  
        return a.block_num > b.block_num;
    } 

    friend bool operator == (signed_block a, signed_block b)  
    {  
        return a.block_num == b.block_num;
    } 
};


//FC_REFLECT( graphene::chain::block_header, (previous)(timestamp)(witness)(transaction_merkle_root)(extensions) )
FC_REFLECT( block_meta_info, (num)(type) )
FC_REFLECT( block_header, (previous)(block_num)(timestamp)(witness)(transaction_merkle_root)(block_type)(memos)(solved_blocks) )
FC_REFLECT_DERIVED( signed_block_header, (block_header), (witness_signature) )
FC_REFLECT_DERIVED( signed_block, (signed_block_header), (transactions) )

