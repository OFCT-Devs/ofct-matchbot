# `ircbot`: An irc bot written in C

## Prerequisites
The current project is known to be compiled with `gcc 9.3.0` under `Ubuntu 20.04`.
- `CMake` version required: `3.10.2`
- `OpenSSL` version known to compile: `1.1.1f-1ubuntu2 (31 Mar 2020)`
- `libircclient` version known to compile: `1.10`

To install `CMake` and `OpenSSL`, use the command
```shell script
sudo apt install cmake openssl
```
To install `libircclient`, get the latest version from
[sourceforge](https://sourceforge.net/projects/libircclient)
and use the command
```shell script
./configure --enable-openssl --enable-ipv6 --enable-shared
make
sudo make install
```

## LICENSE
MIT