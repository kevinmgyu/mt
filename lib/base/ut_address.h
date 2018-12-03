#ifndef _UT_ADDRESS_H_
#define _UT_ADDRESS_H_

#include <fc/array.hpp>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/optional.hpp>

#include "ut_define.h"
#include "ut_base.hpp"

class address{
    public:
        address(); ///< constructs empty / null address
        explicit address( const std::string& base58str );   ///< converts to binary, validates checksum
        address( const fc::ecc::public_key& pub ); ///< converts to binary
        explicit address( const fc::ecc::public_key_data& pub ); ///< converts to binary
    //       address( const pts_address& pub ); ///< converts to binary
        address( const public_key_type& pubkey );

        static bool is_valid( const std::string& base58str, const std::string& prefix = BYTETRADE_ADDRESS_PREFIX );

        std::string str();
        const char* c_str();
        explicit operator std::string()const; ///< converts to base58 + checksum

        friend size_t hash_value( const address& v ) { 
            const void* tmp = static_cast<const void*>(v.addr._hash+2);

            const size_t* tmp2 = reinterpret_cast<const size_t*>(tmp);
            return *tmp2;
        }
        fc::ripemd160 addr;
};
inline bool operator == ( const address& a, const address& b ) { return a.addr == b.addr; }
inline bool operator != ( const address& a, const address& b ) { return a.addr != b.addr; }
inline bool operator <  ( const address& a, const address& b ) { return a.addr <  b.addr; }

std::string                        key_to_wif(const fc::sha256& private_secret );
std::string                        key_to_wif(const fc::ecc::private_key& key);
fc::optional<fc::ecc::private_key> wif_to_key( const std::string& wif_key );

address generate_address(fc::ecc::private_key& private_key, fc::ecc::public_key& public_key);

void to_variant( const address& var,  fc::variant& vo );
void from_variant( const fc::variant& var,  address& vo );

namespace std
{
   template<>
   struct hash<address>
   {
       public:
         size_t operator()(const address &a) const
         {
            return (uint64_t(a.addr._hash[0])<<32) | uint64_t( a.addr._hash[0] );
         }
   };
}

#include <fc/reflect/reflect.hpp>
FC_REFLECT( address, (addr) )


#endif

