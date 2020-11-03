# `ofct-matchbot`: An irc bot written in C used for [osu!](https://osu.ppy.sh/home) match

## WARNING
This project is currently under construction, so it **DOES NOT WORK AS A BOT YET**. However feel free to discuss about the program by making new issues.

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

Add a file named `manage.cfg` in the `build` folder and write your irc account info as below.
```text
YOUR_NICKNAME
YOUR_IRC_PASSWORD
```

Now run the file by typing
```shell
./ofct-matchbot
```

## TODO
- Cross-platform: Make it run at Windows 10 too, at least.
- Implement `match()`
- Make a grammar for `manage.cfg`.