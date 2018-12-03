all: dividend
.PHONY : all 

INCS = -I ../lib/network -I ../lib/utils -I ../lib/base -I ../lib/httptools 
LIBS = -L ../lib/httptools -L ../lib/utils -L ../lib/network -L ../lib/base -lhttptools -lbbtbase -lutils -lnetwork -lrestclient-cpp -lleveldb -lrdkafka -lssl -lcrypto -levent -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lrestclient-cpp -lcurl -lmpdec -lmysqlclient -lz -lhiredis -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl
#-lleveldb  -lprotobuf -Wl,-Bstatic -lfc -lsecp256k1 -lev -ljansson -lmpdec -lcurl -lz -lrdkafka -lssl -lcrypto -lhiredis -lmysqlclient -lboost_system -lboost_thread -lboost_chrono -lboost_filesystem -lboost_date_time -lboost_context -lboost_coroutine -lboost_iostreams -Wl,-Bdynamic -lm -lpthread -ldl

dividend:
	g++ dividend.cpp -std=gnu++11 -g -o dividend.exe -fpermissive $(INCS) $(LIBS)

clean:
	rm -f dividend.exe
