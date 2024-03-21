# CrossTool_NG
## Introduction
Crosstool-NG aims at building toolchains. Toolchains are an essential component in a software development project. It will compile, assemble and link the code that is being developed. Some pieces of the toolchain will eventually end up in the resulting binaries: static libraries are but an example.

## 1- Installing CrossTool_NG
```
git clone https://github.com/crosstool-ng/crosstool-ng.git

cd crosstool-ng/
```
## 2- install missing dependencies
* it will ask you for the sudo password , Enter the passord :D
```
sudo apt-get install -y gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 xz-utils unzip \
patch libstdc++6 rsync
```
## 3- run bootstrap binary to setup the enviroment
`./bootstrap`
## 4- run configure binary to check all dependencies
`./configure --enable-local`
## 5- now run the command `make` to generate MakeFile for the tool
`make`
## 6- run this command to list all samples of pre-configured tool configuration 
  
`./ct-ng list-samples`
* you can use `|grep sample_name ` to get specific result
* the full command would be `./ct-ng list-samples | grep sample_name`

## 7- choose the which config you want to start with
`./ct-ng MicroController_name`
* in our case it would be arm-cortexa9
* the full command would be `./ct-ng arm-cortexa9_neon-linux-gnueabihf`

## 8- start configuring using the simple GUI of the Tool

`./ct-ng menuconfig`

### our configuration
* C-library ---> C library(glibc) ---> musl
* Companion tools --> make
* Debug facilities --> strace

## 9- Time To build our own Compiler
`./ct-ng build`
  * it is going to take some time so stay patient (~ hour)

## 10- Now you have got yourself a custom compiler 
you can find it under `cd ~/x-tools/`