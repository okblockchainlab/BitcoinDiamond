#include "gtest/gtest.h"
#include "wallet.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <list>


typedef void (*execute_t)(const std::string& _nettype, const std::string& args, std::list<std::string>& result);

class OKWalletTest : public ::testing::Test {
public:
  static void SetUpTestCase() {
    const auto& cwd = _get_current_dir();
    ASSERT_FALSE(cwd.empty());
#ifdef __APPLE__
    const auto& mod_path = cwd + "/libbitcoindiamond.dylib";
#elif __linux__
    const auto& mod_path = cwd + "/libbitcoindiamond.so";
#endif

    wlt_mod = dlopen(mod_path.c_str(), RTLD_LAZY);
    execute = (execute_t)dlsym(wlt_mod, "execute");
  }

  static void TearDownTestCase() {
    if (NULL != wlt_mod) {
      dlclose(wlt_mod);
      wlt_mod = NULL;
    }
  }

  static std::string _get_current_dir()
  {
    auto* buf = getcwd(NULL, 0);
    if (nullptr == buf) {
      return "";
    }

    std::string res(buf);
    free(buf);
    return res;
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  static void* wlt_mod;
  static execute_t execute;

  static const char* strSecret;
  static const char* addr;
  static const char* strSecretC;
  static const char* addrC;
};
void* OKWalletTest::wlt_mod = NULL;
execute_t OKWalletTest::execute = nullptr;

const char* OKWalletTest::strSecret = "92K9Np2PyMjEkdpp6dRKJzSpfkctKpphQNY1qbmJ3jh3emYh8dA";
const char* OKWalletTest::addr = "mhqcpXKWDfBpH1EbTFKmvF88gbgYf18hqV"; //CBitcoinAddress
const char* OKWalletTest::strSecretC = "cW6cwtiSD1xbAqQ82YUmN5xC41ZxMosAKRq3dKSDyajiSmrmF7dv";
const char* OKWalletTest::addrC = "n289PW2xzp2x63YjjA3TaU4StF4vigwPN6";

TEST_F(OKWalletTest, getaddressbyprivatekey) {
  std::list<std::string> result;

  auto cmd = std::string("getaddressbyprivatekey ") + strSecret;
  execute("testnet", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(addr, result.back().c_str());

  cmd = std::string("getaddressbyprivatekey ") + strSecretC;
  execute("testnet", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(addrC, result.back().c_str());
}

TEST_F(OKWalletTest, createrawtransaction) {
  const auto& cmd = std::string("createrawtransaction [{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\",\"vout\":1"
        ",\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\",\"redeemScript\""
        ":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}] ") +
        std::string("{\"") + addrC + std::string("\":11}");

  std::list<std::string> result;
  execute("testnet", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(
    "0c00000043497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea33090000000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb40100000000ffffffff0180778e06000000001976a914e20a55c7930322b0f730365618c7e59b9c99959d88ac00000000",
    result.back().c_str()
  );
}

TEST_F(OKWalletTest, signrawtransaction) {
  //const auto& cmd = std::string("signrawtransaction "
    //"0c00000043497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea33090000000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb40100000000ffffffff0180778e06000000001976a914e20a55c7930322b0f730365618c7e59b9c99959d88ac00000000 "
    //"[{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\",\"vout\":1,\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\",\"redeemScript\":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}] ") +
    //"null ") +
    //std::string("[\"") + strSecret + std::string("\"]");
  const auto& cmd = "signrawtransaction "
                    "0200000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb40100000000ffffffff0100ab90410000000017a914b10c9df5f7edf436c697f02f1efdba4cf39961518700000000 "
                    "[{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\",\"vout\":1,\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\""
                    ",\"redeemScript\":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}]"
                    " [\"KzsXybp9jX64P5ekX1KUxRQ79Jht9uzW7LorgwE65i5rWACL6LQe\",\"Kyhdf5LuKTRx4ge69ybABsiUAWjVRK4XGxAKk2FQLp2HjGMy87Z4\"]";


  std::list<std::string> result;
  execute("main", cmd, result);

  ASSERT_EQ((size_t)4, result.size());
  const auto& hex = *std::next(result.begin());
  const auto& complete = *result.rbegin();
  ASSERT_STREQ("0200000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb4010000009200483045022100aa60e88fd655415eea4a10947f84e844127dda8433e38dec2736bab906897b41022027ab233c232c4cb0258204e0ae6e9200da0b60319cb99e0029a6159fbd889d0d0147512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052aeffffffff0100ab90410000000017a914b10c9df5f7edf436c697f02f1efdba4cf39961518700000000",
            hex.c_str());
  ASSERT_STREQ("true", complete.c_str());
}
