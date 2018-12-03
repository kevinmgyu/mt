#pragma once
#include "types.hpp"

  /**
    *  @brief defines the keys used to derive the shared secret
    *
    *  Because account authorities and keys can change at any time, each memo must
    *  capture the specific keys used to derive the shared secret.  In order to read
    *  the cipher message you will need one of the two private keys.
    *
    *  If @ref from == @ref to and @ref from == 0 then no encryption is used, the memo is public.
    *  If @ref from == @ref to and @ref from != 0 then invalid memo data
    *
    */
   struct memo_data
   {
      public_key_type from;
      public_key_type to;
      /**
       * 64 bit nonce format:
       * [  8 bits | 56 bits   ]
       * [ entropy | timestamp ]
       * Timestamp is number of microseconds since the epoch
       * Entropy is a byte taken from the hash of a new private key
       *
       * This format is not mandated or verified; it is chosen to ensure uniqueness of key-IV pairs only. This should
       * be unique with high probability as long as the generating host has a high-resolution clock OR a strong source
       * of entropy for generating private keys.
       */
      uint64_t nonce = 0;

      uint32_t data_type = 0;
      /**
       * This field contains the AES encrypted packed @ref memo_message
       */
     // vector<char> message;
     string message;

      /// @note custom_nonce is for debugging only; do not set to a nonzero value in production
      void        set_message(const fc::ecc::private_key& priv,
                              const fc::ecc::public_key& pub, const string& msg, uint64_t custom_nonce = 0);

//      std::string get_message(const fc::ecc::private_key& priv,
  //                            const fc::ecc::public_key& pub)const;

      void set_public_message(const int data_type, const string& msg);
   };

   /**
    * @brief defines a message and checksum to enable validation of successful decryption
    *
    * When encrypting/decrypting a checksum is required to determine whether or not
    * decryption was successful.
    */
    /*
   struct memo_message
   {
      memo_message(){}
      memo_message( uint32_t checksum, const std::string& text )
      :checksum(checksum),text(text){}

      uint32_t    checksum = 0;
      std::string text;

      string serialize() const;
      static memo_message deserialize(const string& serial);
   };*/

//FC_REFLECT( memo_message, (checksum)(text) )
//FC_REFLECT( memo_data, (from)(to)(nonce)(message)(data_type) )
FC_REFLECT( memo_data,  (message)(data_type) )

