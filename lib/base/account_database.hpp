
#pragma once
#include <iostream>
#include <fstream>
#include "market.hpp"
#include <leveldb/db.h>   
#include <mutex>
#include "account.hpp"

class account_database {
public:
      
      int open( const fc::path& dbdir );
      bool is_open();
      void close();

      int save_or_update_account(const account_object& b);
      optional<account_object> get_account_by_id(const string& id) const;
     // optional<account_object> get_account_by_address(const address& addr);
      bool has_account_by_id(const string& id) const;

      optional<authority> get_active(const string& id) const;
      optional<authority> get_owner(const string& id) const;
      
private:
      bool _is_open = false;
      leveldb::DB* db;  

      leveldb::WriteOptions write_options;
      fc::path _index_filename;
  //    std::mutex mut;
};


