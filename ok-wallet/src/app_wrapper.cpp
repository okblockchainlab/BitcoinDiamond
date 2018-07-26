#include "app_wrapper.h"
#include "chainparams.h"
#include "rpc/server.h"
#include "rpc/register.h"
#include "test/testutil.h"
#include "validation.h"
#include "consensus/validation.h"
#include "net.h"
#include "net_processing.h"
#include "wallet/rpcwallet.h"

extern std::unique_ptr<CConnman> g_connman;

AppWrapper::AppWrapper(const std::string& chainName): bs(chainName)
{
  //copy from TestingSetup::TestingSetup

  const CChainParams& chainparams = Params();
  // Ideally we'd move all the RPC tests to the functional testing framework
  // instead of unit tests, but for now we need these here.

  static bool tablerpc_init = false;
  if (!tablerpc_init) {
    RegisterAllCoreRPCCommands(tableRPC);
#ifdef ENABLE_WALLET
    RegisterWalletRPCCommands(tableRPC);
#endif
  }
  ClearDatadirCache();
  pathTemp = GetTempPath() / strprintf("test_bitcoin_%lu_%i", (unsigned long)GetTime(), (int)(GetRand(100000)));
  boost::filesystem::create_directories(pathTemp);
  ForceSetArg("-datadir", pathTemp.string());
  mempool.setSanityCheck(1.0);
  pblocktree = new CBlockTreeDB(1 << 20, true);
  pcoinsdbview = new CCoinsViewDB(1 << 23, true);
  pcoinsTip = new CCoinsViewCache(pcoinsdbview);
  InitBlockIndex(chainparams);
  {
    CValidationState state;
    if (!ActivateBestChain(state, chainparams)) {
      return;
    }
  }
  nScriptCheckThreads = 3;
  for (int i=0; i < nScriptCheckThreads-1; i++)
  threadGroup.create_thread(&ThreadScriptCheck);
  g_connman = std::unique_ptr<CConnman>(new CConnman(0x1337, 0x1337)); // Deterministic randomness for tests.
  connman = g_connman.get();
  RegisterNodeSignals(GetNodeSignals());

  valid_ = true;
}
AppWrapper::~AppWrapper()
{
  UnregisterNodeSignals(GetNodeSignals());
  threadGroup.interrupt_all();
  threadGroup.join_all();
  UnloadBlockIndex();
  delete pcoinsTip;
  delete pcoinsdbview;
  delete pblocktree;
  boost::filesystem::remove_all(pathTemp);
}
