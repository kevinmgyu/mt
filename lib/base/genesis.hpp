#pragma once
#include "account.hpp"
#include "asset.hpp"
#include "base_operation.hpp"
#include "types.hpp"
#include "witness.hpp"
#include "transaction.hpp"

struct genesis {

    witness start_witness;
    // witness         matchengin_start_ewitness;

    asset core_asset;
    vector<witness> witnesses;
    vector<account_object> accounts;
    vector<processed_transaction> transactions;
    // vector<witness> matchengin_witnesses;

    genesis() {
    }

    genesis(const genesis &a) {
        start_witness = a.start_witness;
        // matchengin_start_ewitness = a.matchengin_start_ewitness;
        //witnesses = a.witnesses;

        witnesses.clear();
        for (size_t i = 0; i < a.witnesses.size(); ++i) {
            witnesses.push_back(a.witnesses[i]);
        }

        accounts.clear();
        for (size_t i = 0; i < a.accounts.size(); ++i) {
            accounts.push_back(a.accounts[i]);
        }

        transactions.clear();
        for (auto t : a.transactions) {
            transactions.push_back(t);
        }
        // matchengin_witnesses.clear();
        // for( size_t i = 0; i < a.matchengin_witnesses.size(); ++i ) {
        //     matchengin_witnesses.push_back(a.matchengin_witnesses[i]);
        // }

        core_asset = a.core_asset;
    }
};

struct second_genesis {
    vector<asset> ethereum_assets;

    second_genesis() {
    }

    second_genesis(const second_genesis &a) {

        ethereum_assets.clear();
        for (size_t i = 0; i < a.ethereum_assets.size(); ++i) {
            ethereum_assets.push_back(a.ethereum_assets[i]);
        }
    }
};

/*
struct btc_genesis {
    vector<asset> btc_assets;

    btc_genesis() {
    }

    btc_genesis(const btc_genesis &a) {
        btc_assets.clear();
        for (size_t i = 0; i < a.btc_assets.size(); ++i) {
            btc_assets.push_back(a.btc_assets[i]);
        }
    }
};
*/
struct key_pairs {

    vector<key_pair> kps;

    key_pairs() {
    }

    key_pairs(const key_pairs &a) {
        kps.clear();
        for (size_t i = 0; i < a.kps.size(); ++i) {
            kps.push_back(a.kps[i]);
        }
        //kps = a.kps;
    }

    int add_key_pair(const key_pair kp) {
        for (size_t i = 0; i < kps.size(); ++i) {
            if (kps[i].id == kp.id) {
                return -__LINE__;
            }
        }

        kps.push_back(kp);
        return 0;
    }

    optional<key_pair> get_key_pair(const string &id) {
        for (size_t i = 0; i < kps.size(); ++i) {
            if (kps[i].id == id) {
                return optional<key_pair>(kps[i]);
            }
        }

        return optional<key_pair>();
    }
};

//FC_REFLECT( genesis, (mainchain_start_witness)(matchengin_start_ewitness)(core_asset)(mainchain_witnesses)(matchengin_witnesses) )
FC_REFLECT(genesis, (start_witness)(witnesses)(core_asset)(accounts)(transactions))
FC_REFLECT(second_genesis, (ethereum_assets))
//FC_REFLECT(btc_genesis, (btc_assets))
FC_REFLECT(key_pairs, (kps))