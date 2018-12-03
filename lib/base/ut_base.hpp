#pragma once

#include <fc/container/flat_fwd.hpp>
#include <fc/io/varint.hpp>
#include <fc/io/enum_type.hpp>
#include <fc/crypto/sha224.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/reflect/variant.hpp>
#include <fc/optional.hpp>
#include <fc/safe.hpp>
#include <fc/container/flat.hpp>
#include <fc/string.hpp>
#include <fc/io/raw.hpp>
#include <fc/static_variant.hpp>
#include <fc/smart_ref_fwd.hpp>

#include <memory>
#include <vector>
#include <deque>
#include <cstdint>

#include "ut_define.h"

namespace fc { namespace ecc {
    class public_key;
    typedef fc::array<char,33>  public_key_data;
} } // fc::ecc

struct public_key_type
{
    struct binary_key
    {
        binary_key() {}
        uint32_t                 check = 0;
        fc::ecc::public_key_data data;
    };
    fc::ecc::public_key_data key_data;
    public_key_type();
    public_key_type( const fc::ecc::public_key_data& data );
    public_key_type( const fc::ecc::public_key& pubkey );
    explicit public_key_type( const std::string& base58str );
    operator fc::ecc::public_key_data() const;
    operator fc::ecc::public_key() const;
    explicit operator std::string() const;
    friend bool operator == ( const public_key_type& p1, const fc::ecc::public_key& p2);
    friend bool operator == ( const public_key_type& p1, const public_key_type& p2);
    friend bool operator != ( const public_key_type& p1, const public_key_type& p2);
    friend bool operator < ( const public_key_type& p1, const public_key_type& p2);
    // TODO: This is temporary for testing
    bool is_valid_v1( const std::string& base58str );
};

struct extended_public_key_type
{
    struct binary_key
    {
        binary_key() {}
        uint32_t                   check = 0;
        fc::ecc::extended_key_data data;
    };
    
    fc::ecc::extended_key_data key_data;
    
    extended_public_key_type();
    extended_public_key_type( const fc::ecc::extended_key_data& data );
    extended_public_key_type( const fc::ecc::extended_public_key& extpubkey );
    explicit extended_public_key_type( const std::string& base58str );
    operator fc::ecc::extended_public_key() const;
    explicit operator std::string() const;
    friend bool operator == ( const extended_public_key_type& p1, const fc::ecc::extended_public_key& p2);
    friend bool operator == ( const extended_public_key_type& p1, const extended_public_key_type& p2);
    friend bool operator != ( const extended_public_key_type& p1, const extended_public_key_type& p2);
};

struct extended_private_key_type
{
    struct binary_key
    {
        binary_key() {}
        uint32_t                   check = 0;
        fc::ecc::extended_key_data data;
    };
    
    fc::ecc::extended_key_data key_data;
    
    extended_private_key_type();
    extended_private_key_type( const fc::ecc::extended_key_data& data );
    extended_private_key_type( const fc::ecc::extended_private_key& extprivkey );
    explicit extended_private_key_type( const std::string& base58str );
    operator fc::ecc::extended_private_key() const;
    explicit operator std::string() const;
    friend bool operator == ( const extended_private_key_type& p1, const fc::ecc::extended_private_key& p2);
    friend bool operator == ( const extended_private_key_type& p1, const extended_private_key_type& p2);
    friend bool operator != ( const extended_private_key_type& p1, const extended_private_key_type& p2);
};

void to_variant( const public_key_type& var,  fc::variant& vo );
void from_variant( const fc::variant& var,  public_key_type& vo );
void to_variant( const extended_public_key_type& var, fc::variant& vo );
void from_variant( const fc::variant& var, extended_public_key_type& vo );
void to_variant( const extended_private_key_type& var, fc::variant& vo );
void from_variant( const fc::variant& var, extended_private_key_type& vo );

std::string get_deal_id_by_order_ids(const char* id1,const char* id2);

FC_REFLECT( public_key_type, (key_data) )
FC_REFLECT( public_key_type::binary_key, (data)(check) )
FC_REFLECT( extended_public_key_type, (key_data) )
FC_REFLECT( extended_public_key_type::binary_key, (check)(data) )
FC_REFLECT( extended_private_key_type, (key_data) )
FC_REFLECT( extended_private_key_type::binary_key, (check)(data) )

