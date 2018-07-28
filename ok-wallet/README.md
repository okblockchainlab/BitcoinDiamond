
### 编译

##### centos7的编译依赖项
```shell
sudo yum install cmake autoconf automake libtool curl make g++ unzip

```
项目中用到了c++11标准，但centos的源上的gcc一般版本都比较低不支持c++11，所以有可能需要下载gcc源码手工编译安装。

##### 编译步骤
- git clone https://github.com/okblockchainlab/BitcoinDiamond.git
- cd BitcoinDiamond
- export COIN_DEPS=\`pwd\`/depslib
- ./build.sh (only run this script if you first time build the project)
- ./runbuild.sh

### 注意项
- key是分网络的，在网络类型为main时的一个有效key，在testnet下就不是一个有效的key。
