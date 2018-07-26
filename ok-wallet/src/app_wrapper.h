#pragma once

#include "test/test_bitcoin.h"

class AppWrapper
{
public:
  AppWrapper(const std::string& chainName);
  ~AppWrapper();

  bool valid() { return valid_;}

private:
  bool valid_ = false;

  BasicTestingSetup bs;

  CCoinsViewDB *pcoinsdbview;
  boost::filesystem::path pathTemp;
  boost::thread_group threadGroup;
  CConnman* connman;
};
