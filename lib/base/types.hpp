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
#include <fc/container/flat_fwd.hpp>
#include <fc/io/enum_type.hpp>
#include <fc/io/varint.hpp>
//#include <fc/crypto/sha224.hpp>
#include <fc/container/flat.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/pke.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/io/raw.hpp>
#include <fc/optional.hpp>
#include <fc/reflect/reflect.hpp>
#include <fc/reflect/variant.hpp>
#include <fc/safe.hpp>
#include <fc/smart_ref_fwd.hpp>
#include <fc/static_variant.hpp>
#include <fc/string.hpp>
//#include <fc/uint128.hpp>
#include "uint128.hpp"

#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <deque>
#include <memory>
#include <vector>

#include "ut_address.h"
#include "ut_base.hpp"
#include "ut_define.h"

using std::map;
using std::vector;
using std::unordered_map;
using std::string;
using std::deque;
using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
using std::set;
using std::pair;
using std::enable_shared_from_this;
using std::tie;
using std::make_pair;

using fc::smart_ref;
using fc::variant_object;
using fc::variant;
using fc::enum_type;
using fc::optional;
using fc::unsigned_int;
using fc::signed_int;
using fc::time_point_sec;
using fc::time_point;
using fc::safe;
using fc::flat_map;
using fc::flat_set;
using fc::static_variant;
using fc::ecc::range_proof_type;
using fc::ecc::range_proof_info;
using fc::ecc::commitment_type;

#include <boost/multiprecision/cpp_int.hpp>

struct void_t {};

//typedef unsigned __int128 uint128_t;




// using u256 =  boost::multiprecision::number<boost::multiprecision::cpp_int_backend<256, 256, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>>;
// namespace fc
// {
//     void to_variant( const uint128_t& var,  fc::variant& vo );
//     void from_variant( const fc::variant& var,  uint128_t& vo );
// }

// namespace raw
// {
// template<typename Stream>
// inline void pack( Stream& s, const uint128_t& u ) { /*s.write( (char*)&u, sizeof(u) );*/ }
// template<typename Stream>
// inline void unpack( Stream& s, uint128_t& u ) { /*s.read( (char*)&u, sizeof(u) );*/ }
// }

typedef fc::ecc::private_key private_key_type;
typedef fc::sha256 chain_id_type;

typedef fc::array<char, GRAPHENE_MAX_ASSET_SYMBOL_LENGTH> symbol_type;
typedef fc::ripemd160 block_id_type;
typedef fc::ripemd160 checksum_type;
typedef fc::ripemd160 transaction_id_type;

typedef fc::sha256 digest_type;
typedef fc::ecc::compact_signature signature_type;
//typedef safe2<uint64_t> share_type;
typedef  bytetrade::uint128 share_type;
//typedef  fc::uint128 share_type;
//typedef uint128_t share_type;
typedef std::vector<char> byte_array;

typedef uint64_t block_num_type;
typedef uint64_t block_chain_type;
// typedef uint64_t                                        share_type;
typedef uint16_t weight_type;

typedef string account_id_type;
// typedef address                                              account_id_type;
typedef string witness_id_type;
//   typedef string witness_id_type;
typedef uint32_t asset_id_type;
typedef uint32_t market_id_type;
typedef fc::ripemd160 order_id_type;
typedef fc::ripemd160 deal_id_type;
//typedef uint32_t                             term_id_type;
typedef fc::ripemd160 operation_id_type;
typedef fc::ripemd160 proposal_id_type;
typedef fc::ripemd160 vote_id_type;

typedef string pledge_id_type;

//#define GRAPHENE_COMMITTEE_ACCOUNT address(0)
/// Represents the current witnesses
#define GRAPHENE_WITNESS_ACCOUNT 1
/// Represents the current committee members
#define GRAPHENE_RELAXED_COMMITTEE_ACCOUNT 2
/// Represents the canonical account with NO authority (nobody can access funds in null account)
#define GRAPHENE_NULL_ACCOUNT 3
/// Represents the canonical account with WILDCARD authority (anybody can access funds in temp account)
#define GRAPHENE_TEMP_ACCOUNT 4
/// Represents the canonical account for specifying you will vote directly (as opposed to a proxy)
#define GRAPHENE_PROXY_TO_SELF_ACCOUNT 5
/// Sentinel value used in the scheduler.
#define GRAPHENE_NULL_WITNESS 0

const int max_recursion = 3;

string to_string(const share_type itttt);


FC_REFLECT(void_t, )
