#! /bin/sh
PWD=`pwd`
LIBDOWNLOAD=$PWD/libdownload

if [ -z "$COIN_DEPS" ]; then
	printf "No COIN_DEPS detected!\\n"
	printf "Setup COIN_DEPS before build: export COIN_DEPS=`pwd`/depslib \\n"
	exit 1
fi

if [ ! -d "$LIBDOWNLOAD" ];then
	mkdir $LIBDOWNLOAD
fi

if [ ! -d "$COIN_DEPS" ];then
	mkdir $COIN_DEPS
fi

cd $COIN_DEPS

if [ ! -d ./boost ];then
	mkdir boost
fi

if [ ! -d ./openssl ];then
	mkdir openssl
fi

if [ ! -d ./berkeley-db ]; then
	mkdir berkeley-db
fi

if [ ! -d ./miniupnpc ]; then
	mkdir miniupnpc
fi

if [ ! -d ./protobuf ]; then
	mkdir protobuf
fi

if [ ! -d ./libevent ]; then
	mkdir libevent
fi

###install boost
cd $LIBDOWNLOAD

if [ ! -f ./boost_1_66_0.tar.gz ];then
	wget https://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download boost_1_66_0.tar.gz" && exit 1)
fi

if [ ! -d ./boost_1_66_0 ];then
	tar xf boost_1_66_0.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot tar xf boost_1_66_0.tar.gz" && exit 1)
fi

cd boost_1_66_0
GCCJAM=`find . -name gcc.jam`
sed -i -e "s/if \$(link) = shared/if \$(link) = shared \|\| \$(link) = static/g" $GCCJAM
./bootstrap.sh
./b2 --prefix=$COIN_DEPS/boost --build-dir=boost.build link=static runtime-link=static variant=release install
[ $? -ne 0 ] &&  exit 1


###intstall openssl
cd $LIBDOWNLOAD

if [ ! -f ./openssl-1.0.2k.tar.gz ];then
	wget https://www.openssl.org/source/openssl-1.0.2k.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download openssl-1.0.2k.tar.gz" && exit 1)
fi

if [ ! -d ./openssl-1.0.2k ];then
	tar xf openssl-1.0.2k.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot tar xf openssl-1.0.2k.tar.gz" && exit 1)
fi

cd openssl-1.0.2k
TARGET_OS=`uname -s`
MHD_NAME=`uname -m`
if [ "$TARGET_OS" == "Darwin" && "$MHD_NAME" == "x86_64" ]; then
	./Configure  no-shared enable-ec enable-ecdh enable-ecdsa -fPIC --prefix=$COIN_DEPS/openssl darwin64-x86_64-cc 
else
	./config --prefix=$COIN_DEPS/openssl no-shared enable-ec enable-ecdh enable-ecdsa -fPIC
fi

make && make install
[ $? -ne 0 ] &&  exit 1


###intstall libevent-2.0.21-stable
cd $LIBDOWNLOAD

if [ ! -f ./libevent-2.1.8-stable.tar.gz ];then
	wget https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download libevent-2.1.8-stable.tar.gz" && exit 1)
fi

if [ ! -d ./libevent-2.1.8-stable ];then
	tar xf libevent-2.1.8-stable.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot tar xf libevent-2.1.8-stable.tar.gz" && exit 1)
fi

cd libevent-2.1.8-stable
./autogen.sh
./configure --enable-shared=no --enable-cxx --prefix=$COIN_DEPS/libevent CPPFLAGS="-I$COIN_DEPS/openssl/include -fPIC" CFLAGS="-I$COIN_DEPS/openssl/include -fPIC"
make && make install
[ $? -ne 0 ] &&  exit 1


cd $LIBDOWNLOAD

if [ ! -f ./db-4.8.30.NC.tar.gz ]; then
	wget http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download db-4.8.30.NC.tar.gz" && exit 1)
fi

if [ ! -d db-4.8.30.NC ]; then
	tar xf db-4.8.30.NC.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot tar xf db-4.8.30.NCtar.gz" && exit 1)
fi

cd db-4.8.30.NC/build_unix
sed -i -e "s/__atomic_compare_exchange/__atomic_compare_exchange_db/g" ../dbinc/atomic.h
sed -i -e "s/atomic_init/_atomic_init/g" ../dbinc/atomic.h
sed -i -e "s/atomic_init/_atomic_init/g" ../mp/mp_fget.c
sed -i -e "s/atomic_init/_atomic_init/g" ../mp/mp_mvcc.c
sed -i -e "s/atomic_init/_atomic_init/g" ../mp/mp_region.c
sed -i -e "s/atomic_init/_atomic_init/g" ../mutex/mut_method.c
sed -i -e "s/atomic_init/_atomic_init/g" ../mutex/mut_tas.c
../dist/configure --prefix=$COIN_DEPS/berkeley-db --enable-cxx --disable-shared --with-pic
make && make install
[ $? -ne 0 ] &&  exit 1


cd $LIBDOWNLOAD

if [ ! -f ./miniupnpc-2.1.tar.gz ]; then
	wget http://miniupnp.free.fr/files/miniupnpc-2.1.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download miniupnpc-2.1.tar.gz" && exit 1)
fi

if [ ! -d miniupnpc-2.1 ]; then
	tar xf miniupnpc-2.1.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot xf miniupnpc-2.1.tar.gz" && exit 1)
fi

cd miniupnpc-2.1
if [ -d build ]; then
	rm -rf build
fi
mkdir build && cd build
cmake -DUPNPC_BUILD_SHARED=FALSE -DUPNPC_BUILD_TESTS=FALSE -DUPNPC_BUILD_SAMPLE=FALSE -DCMAKE_INSTALL_PREFIX=$COIN_DEPS/miniupnpc  ..
make && make install
[ $? -ne 0 ] &&  exit 1


cd $LIBDOWNLOAD

if [ ! -f ./protobuf-cpp-3.6.0.tar.gz ]; then
	wget https://github.com/google/protobuf/releases/download/v3.6.0/protobuf-cpp-3.6.0.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot download protobuf-cpp-3.6.0.tar.gz" && exit 1)
fi

if [ ! -d protobuf-3.6.0 ]; then
	tar xf protobuf-cpp-3.6.0.tar.gz
	[ $? -ne 0 ] && (echo "Error cannot xf protobuf-cpp-3.6.0.tar.gz" && exit 1)
fi

cd protobuf-3.6.0
./configure --prefix=$COIN_DEPS/protobuf --disable-shared
make && make install
[ $? -ne 0 ] &&  exit 1
