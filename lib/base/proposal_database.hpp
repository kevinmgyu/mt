
#pragma once
#include <iostream>
#include <fstream>
#include "proposal.hpp"
#include <leveldb/db.h>   
#include <mutex>

class proposal_database {
public:
      
      int open( const fc::path& dbdir );
      bool is_open();
      void close();

      int save_or_update_proposal(const proposal& b);
      int save_external_id(const string& external_id);
      optional<proposal> get_proposal(const proposal_id_type& proposal_id);
      bool has_proposal(const proposal_id_type& proposal_id);
      bool has_external_id(const string& external_id);
      

private:
      bool _is_open = false;
      leveldb::DB* db;  

      leveldb::WriteOptions write_options;
      fc::path _index_filename;
      std::mutex mut;
};
