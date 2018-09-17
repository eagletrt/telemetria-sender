# Cheat sheet

## Set-up
build a [Docker container](https://docker.com):

```bash
$ docker build . -t eagle-rpi
$ docker run -ti --rm -v$(pwd):/root/devel --name erpi eagle-rpi
root@4fedfaf5b5c7:~/devel#
```

The first command builds the container image. It takes time but you only have to do it once. It downloads about 500 Mb from the Internet, so you need a good downlink.

The switch `-v$(pwd):/root/devel` means that the current dir in the host machine is *mounted* on the `/root/devel` container. Consequently, you will see the workdir in the guiest container synchronized with your starting point in the host machine. In other words, you can use your editor on your PC/Mac, and run compilation commands within the container shell.

The latter will put you in the cross-compiling enviroment (CCE). You can close the CCE with the usual logout (CTRL-D). Any change within the CCE will be lost, due to the `--rm` switch (it means 'remove'). If you want to have a permanent CCE, remove the `--rm` switch, and only use once the command. **After closing it**, you can enter again with:

```bash
$ docker start erpi
$ docker attach erpi
root@4fedfaf5b5c7:~/devel#
```

## CMake configure
Work in a terminal on the project root within CCE:

```bash
$ cmake -Bxbuild -H. -DCMAKE_TOOLCHAIN_FILE=/root/Toolchain-rpi.cmake
```

This will prepare an out-of-source build with cmake in the `xbuild` folder using the `CMakeLists.txt` found in the `.` directory.

## Build
Work in a terminal on the project root:

```bash
$ make -C xbuild
```

## Test
Work in a terminal on the project root within the CCE:

```bash
$ qemu-arm xbuild/cross-template
```

## Easiest install on RPI
Assuming that you mounted the `/usr/local` dir of the raspberry to the `install` folder with the command (**to be executed on the host machine, not on the CCE!**):

```bash
$ sshfs user@rpi.local:/usr/local install
```

then you can build and install on the target system with one command from within the CCE:

```bash
$ make -Cxbuild install
# or:
$ cd xbuild
$ make install
```

