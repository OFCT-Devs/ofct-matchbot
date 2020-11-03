# `ircbot`: An irc bot written in C used for [osu!](https://osu.ppy.sh/home) match

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
MIT LICENSE

Copyright 2020 OFCT-Devs TEAM

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.