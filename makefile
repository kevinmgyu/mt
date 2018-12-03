all: dividend
.PHONY : all 

INCS = -I ../../../network -I ../../../utils -I ../../../base -I ../../../block -I ../../../httptools -I ../../../phxpaxos/src/logstorage/ -I ../../../phxpaxos/src/comm/ -I ../../../phxpaxos/include -I ../../../phxpaxos/src/utils
LIBS = -L ../../../httptools -L ../../../utils -L ../../../network -L ../../../base -L ../../../block -lhttptools -lblock -lbbtbase -lutils -lnetwork -lrestclient-cpp -L ../../../../protobuf-3.5.0/lib -L ../../../phxpaxos/lib -lphxpaxos -lleveldb -lrdkafka -lssl -lcrypto -levent -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lrestclient-cpp -lcurl -lmpdec -lmysqlclient -lz -lhiredis -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl
#-lleveldb  -lprotobuf -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lmpdec -lcurl -lz -lrdkafka -lssl -lcrypto -lhiredis -lmysqlclient -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl
LIBS2 = -L ../utils -L ../network -L ../base -L ../block -lblock -lbbtbase -lutils -lnetwork -L ../../protobuf-3.5.0/lib -L ../phxpaxos/lib -lphxpaxos -lleveldb  -lprotobuf -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lmpdec -lmysqlclient -lz -lrdkafka  -lcrypto -lhiredis -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl
LIBS3 = -L ../httptools -L ../utils -L ../network -L ../base -lhttptools -L ../lib -lbtc -lbitcoin -lbitcoin_rpcclient -lbitcoin_util -lbitcoin_crypto_base -lbbtbase  -levent -lunivalue -lutils -lnetwork -lrestclient-cpp -L ../../protobuf-3.5.0/lib -L ../phxpaxos/lib -lphxpaxos -lleveldb  -lprotobuf -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lmpdec -lmysqlclient -lcurl -lz -lrdkafka  -lcrypto -lhiredis -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl

dividend:
	g++ dividend.cpp -std=gnu++11 -g -o dividend.exe -fpermissive $(INCS) $(LIBS)

clean:
	rm -f dividend.exe
