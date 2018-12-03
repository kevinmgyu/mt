
#pragma once
#include "market.hpp"
#include <fstream>
#include <iostream>
#include <leveldb/db.h>
#include <mutex>

class update_database {
  public:
    int open(const fc::path &dbdir);
    bool is_open();
    void close();

    int SyncLastApprovedBlockNum(const uint64_t block_num);
    int getLastApprovedBlockNum(uint64_t &block_num);
   // int get_last_slice(uint64_t &llCheckpointInstanceID);
  //  int SyncCheckpointInstanceID(const uint64_t llInstanceID);

    int SyncLastProposeValue(const string value);
    int getLastProposeValue(string &value);

    int SyncLastProposeTimestamp(const double value);
    int getLastProposeTimestamp(double &value);
    
    int syncInt64(const std::string& key, const int64_t num);
    int getInt64(const std::string& key, int64_t &num);
    
  private:
    bool _is_open = false;
    leveldb::DB *db;

    leveldb::WriteOptions write_options;
    fc::path _index_filename;
    std::mutex mut;
};
