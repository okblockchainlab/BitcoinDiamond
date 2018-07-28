#include "wallet.h"
#include "app_wrapper.h"

#include "key.h"
#include "base58.h"
#include "univalue/include/univalue.h"
#include "rpc/client.h"
#include "rpc/server.h"

#include <boost/algorithm/string.hpp>


static std::list<std::string> _invokeRpc(const std::string& args)
{
  std::list<std::string> resultList;
  UniValue result;
  std::vector<std::string> vArgs;

  try {
    boost::split(vArgs, args, boost::is_any_of(" \t"));

    const auto strMethod = vArgs[0];
    vArgs.erase(vArgs.begin());
    JSONRPCRequest request;
    request.strMethod = strMethod;
    request.params = RPCConvertValues(strMethod, vArgs);
    request.fHelp = false;

    if (nullptr == tableRPC[strMethod]) {
      resultList.push_back("Error");
      resultList.push_back("No such a Jni Api " + strMethod);
      return resultList;
    }

    std::string res;
    rpcfn_type method = tableRPC[strMethod]->actor;
    result = (*method)(request);

    result.feedStringList(resultList);
  }
  catch(const UniValue &objError) {
    result = objError;

    resultList.push_back("Error");
    resultList.push_back(find_value(result.get_obj(), "message").get_str());
  }
  catch(...) {
    resultList.push_back("Error");
    resultList.push_back("Unknown exception!");
  }

  return resultList;
}

void execute(const std::string& _nettype, const std::string& args, std::list<std::string>& result)
{
  std::string nettype;
  if ("main" == _nettype) {
    nettype = CBaseChainParams::MAIN;
  }
  else if ("testnet" == _nettype) {
    nettype = CBaseChainParams::TESTNET;
  }
  else {
    result = {"Error", "Unknown net type!"};
  }

  AppWrapper aw(nettype);
  result = _invokeRpc(args);
}
