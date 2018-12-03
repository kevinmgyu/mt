#pragma once
#include "asset.hpp"
#include "authority.hpp"
#include "base_operation.hpp"
#include "fc/crypto/pke.hpp"
#include "types.hpp"

bool is_valid_id(string id);

class account_object {
  public:
    account_id_type id;
    authority owner;
    authority active;

    optional<fc::public_key> public_key;

    optional<string> pass_seed;

    account_object() {
      
    }

    account_object(const account_object& ao) {
      id = ao.id;
      owner = ao.owner;
      active = ao.active;
      public_key = ao.public_key;
      pass_seed = ao.pass_seed;
    }
};

//FC_REFLECT(account_object,(membership_expiration_date)(registrar)(referrer)(lifetime_referrer)(network_fee_percentage)(lifetime_referrer_fee_percentage)(referrer_rewards_percentage)(name)(owner)(active)(options)(statistics)(whitelisting_accounts)(blacklisting_accounts)(whitelisted_accounts)(blacklisted_accounts)(cashback_vb)(owner_special_authority)(active_special_authority)(top_n_control_flags)(allowed_assets))
FC_REFLECT(account_object, (id)(owner)(active))
//(public_key)(pass_seed)
