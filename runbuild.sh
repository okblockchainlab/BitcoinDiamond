#!/usr/bin/env bash

if [ -z "$JAVA_HOME" ]; then
	printf "No JAVA_HOME detected! "
	printf "Setup JAVA_HOME before build: export JAVA_HOME=/path/to/java\\n"
	exit 1
fi

if [ -z "$COIN_DEPS" ]; then
  printf "NO COIN_DEPS detected!"
	printf "Setup COIN_DEPS before build: export COIN_DEPS=path/to/depslib\\n"
	exit 1
fi


EXT=so
TARGET_OS=`uname -s`
case "$TARGET_OS" in
    Darwin)
        EXT=dylib
        ;;
    Linux)
        EXT=so
        ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac


./autogen.sh
./configure --disable-gui-tests --with-gui=no --with-boost=$COIN_DEPS/boost BDB_CFLAGS="-I$COIN_DEPS/berkeley-db/include" BDB_LIBS="-L$COIN_DEPS/berkeley-db/lib -ldb -ldb_cxx" SSL_CFLAGS="-I$COIN_DEPS/openssl/include" SSL_LIBS="-L$COIN_DEPS/openssl/lib -lssl" CRYPTO_CFLAGS="-I$COIN_DEPS/openssl/include" CRYPTO_LIBS="-L$COIN_DEPS/openssl/lib -lcrypto" PROTOBUF_CFLAGS="-I$COIN_DEPS/protobuf/include" PROTOBUF_LIBS="-L$COIN_DEPS/protobuf/lib -lprotobuf" EVENT_CFLAGS="-I$COIN_DEPS/libevent/include" EVENT_LIBS="-L$COIN_DEPS/libevent/lib -levent" EVENT_PTHREADS_CFLAGS="-I$COIN_DEPS/libevent/include" EVENT_PTHREADS_LIBS="-L$COIN_DEPS/libevent/lib -levent_pthreads"
#./configure --enable-shared=yes --disable-pie CXXFLAGS="-fPIC"  CFLAGS="-fPIC" libpq_LIBS="-L${COIN_DEPS}/libpq/lib -lpq" libpq_CFLAGS="-I${COIN_DEPS}/libpq/include"
make -j 2
[ $? -ne 0 ] && exit 1


PROJECT_NAME=BitcoinDiamond
#mkdir ok-build
#cd ok-build

#cmake -DCMAKE_BUILD_TYPE=Release -DOKLIBRARY_NAME=${PROJECT_NAME} ../ok-wallet
#[ $? -ne 0 ] && exit 1

#make -j 2
#[ $? -ne 0 ] && exit 1

#cp ./lib${PROJECT_NAME}.${EXT} ../

#nm lib${PROJECT_NAME}.${EXT} |grep "[ _]Java_com_okcoin"
