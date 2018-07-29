set(BITCOINDIAMOND_SOURCE
${PROJECT_SOURCE_DIR}/../src/test/test_bitcoin.cpp
${PROJECT_SOURCE_DIR}/../src/test/testutil.cpp
${PROJECT_SOURCE_DIR}/../src/script/sign.cpp
${PROJECT_SOURCE_DIR}/../src/keystore.cpp
${PROJECT_SOURCE_DIR}/../src/protocol.cpp
${PROJECT_SOURCE_DIR}/../src/amount.cpp
${PROJECT_SOURCE_DIR}/../src/netbase.cpp
${PROJECT_SOURCE_DIR}/../src/clientversion.cpp
${PROJECT_SOURCE_DIR}/../src/base58.cpp
${PROJECT_SOURCE_DIR}/../src/core_write.cpp
)

set(WALLET_LIB_SOURCE
  wallet.cpp
  app_wrapper.cpp
  com_okcoin_vault_jni_bitcoindiamond_BitcoinDiamondj.cpp
)

add_definitions(-DHAVE_CONFIG_H)

include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../src/config)
include_directories(${PROJECT_SOURCE_DIR}/../src/secp256k1)
include_directories(${PROJECT_SOURCE_DIR}/../src/leveldb/include)
include_directories(${PROJECT_SOURCE_DIR}/../src/univalue/include)

include_directories($ENV{COIN_DEPS}/boost/include)
include_directories($ENV{COIN_DEPS}/berkeley-db/include)

include_directories($ENV{JAVA_HOME}/include)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories($ENV{JAVA_HOME}/include/linux)
endif()

add_library(${OKLIBRARY_NAME} SHARED ${BITCOINDIAMOND_SOURCE} ${WALLET_LIB_SOURCE})
target_link_libraries(${OKLIBRARY_NAME}
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_common.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_server.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_util.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_wallet.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_consensus.a
  ${PROJECT_SOURCE_DIR}/../src/secp256k1/.libs/libsecp256k1.a
  ${PROJECT_SOURCE_DIR}/../src/univalue/.libs/libunivalue.a
  ${PROJECT_SOURCE_DIR}/../src/leveldb/libleveldb.a
  ${PROJECT_SOURCE_DIR}/../src/leveldb/libmemenv.a
  ${PROJECT_SOURCE_DIR}/../src/crypto/libbitcoin_crypto.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_cli.a
  $ENV{COIN_DEPS}/openssl/lib/libcrypto.a
  $ENV{COIN_DEPS}/openssl/lib/libssl.a
  $ENV{COIN_DEPS}/boost/lib/libboost_system.a
  $ENV{COIN_DEPS}/boost/lib/libboost_thread.a
  $ENV{COIN_DEPS}/boost/lib/libboost_filesystem.a
  $ENV{COIN_DEPS}/boost/lib/libboost_program_options.a
  $ENV{COIN_DEPS}/boost/lib/libboost_chrono.a
  $ENV{COIN_DEPS}/boost/lib/libboost_unit_test_framework.a
  $ENV{COIN_DEPS}/berkeley-db/lib/libdb_cxx.a
  $ENV{COIN_DEPS}/miniupnpc/lib/libminiupnpc.a
  $ENV{COIN_DEPS}/zeromq/lib/libzmq.a
)

################################################################################
ExternalProject_Add (proj_gtest
	PREFIX ${PROJECT_SOURCE_DIR}/test/deps/googletest
	SOURCE_DIR ${PROJECT_SOURCE_DIR}/test/deps/googletest
	INSTALL_DIR ${PROJECT_BINARY_DIR}/deps-install
	TMP_DIR ${PROJECT_BINARY_DIR}/deps-tmp/googletest
	STAMP_DIR ${PROJECT_BINARY_DIR}/deps-stamp/googletest
	BINARY_DIR ${PROJECT_BINARY_DIR}/deps-binary/googletest
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)
ExternalProject_Get_Property(proj_gtest source_dir)
ExternalProject_Get_Property(proj_gtest install_dir)
add_library(gtest STATIC IMPORTED)
set_target_properties(gtest PROPERTIES
	IMPORTED_LOCATION ${install_dir}/lib/libgmock_main.a
)
add_dependencies(gtest proj_gtest)

include_directories(${install_dir}/include)
include_directories(${install_dir}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

enable_testing()
include(add_test.cmake)
cxx_test(test_okwallet "" gtest)
