#pragma once

#include "types.hpp"
#include "vector_ref.h"
#include <array>
#include <boost/multiprecision/cpp_int.hpp>
#include <cassert>
#include <cstdint>
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/hex.hpp>
#include <fc/exception/exception.hpp>
#include <fc/io/json.hpp>
#include <fc/io/raw.hpp>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <sstream>
#include <string>
#include <type_traits>
#include "types.hpp"
using namespace std;

namespace eth {
// Binary data types.
using byte = uint8_t;
using bytes = std::vector<byte>;
using bytesRef = vector_ref<byte>;
using bytesConstRef = vector_ref<byte const>;

template <class T, class Out>
inline void toBigEndian(T _val, Out &o_out);
template <class T, class In>
inline T fromBigEndian(In const &_bytes);

/// Converts a string to a byte array containing the string's (byte) data.
inline bytes asBytes(std::string const &_b) {
    return bytes((byte const *) _b.data(), (byte const *) (_b.data() + _b.size()));
}

 
template <class _T>
std::string asHex(_T const &_data, int _w = 2) {
    std::ostringstream ret;
    for (auto i : _data)
        ret << std::hex << std::setfill('0') << std::setw(_w) << (int) (typename std::make_unsigned<decltype(i)>::type) i;
    return ret.str();
}

int fromHex(char _i);
bytes fromUserHex(std::string const &_s);
bytes toHex(std::string const &_s);

bytes idto_eth_signbytes(string s);
bytes stringto_eth_signbytes(string s);
bytes tokento_eth_signbytes(string s);
bytes numberto_eth_signbytes(uint64_t s);
bytes numberto_eth_signbytes(share_type s);


std::string sha3(string input);
std::string sha3(bytes input);
bytes sha3bytes(bytes _input2);

string get_eth_address_from_private_key(string private_key);
string get_eth_address_from_private_key(fc::ecc::private_key priv3);
string get_eth_address_from_public_key(fc::ecc::public_key priv3);
string get_eth_address_from_public_key_data(fc::ecc::public_key_point_data kd);



}

eth::bytes operator+(eth::bytes a, eth::bytes b);
