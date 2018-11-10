# Fenice Telemetria

I contenuti di questa repository sono provvisori, ma ti serviranno per interagire
con le tecnologie che stiamo utilizzando.

## docs

Un paio di Documenti con materiale relativo a MQTT, sei libero di caricare cose che possano servire.
In più nella cartella config i procedimenti base per configurare il nostro raspberry. NodeJS e Mongo non sono ancora definitivi, bisogna trovare la versione adatta

## linux-can utils

https://github.com/linux-can/can-utils

## Server Node and Dashboard 

https://www.npmjs.com/package/iot-dashboard

Questo è un esempio di come si può lavorare con JS per fare il frontend del nostro sito

https://github.com/dennisdegreef/mqtt-mongo-recorder

Questa è la repository più vicina a quello che stiamo cercando di sviluppare penso possa essere un buon aiuto all'inizio. 

## iot-dashboard

https://www.npmjs.com/package/iot-dashboard

Questo è un esempio di come si può lavorare con JS per fare il frontend del nostro sito

## Setup Can on Linux

```bash
$ sudo apt-get update
$ sudo apt-get can-utils
```

```bash
$ modprobe vcan
$ sudo ip link add dev vcan0 type vcan
$ sudo ip link set up vcan0 
```

## Setup mqtt-mongo-server

```bash
$ npm install
$ npm install bson
$ node server.js
```

http://localhost:8080/

## Publish and Subscribe MQTT

```bash
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt-get install mosquitto mosquitto-clients
```

```bash
$ mosquitto_sub -h localhost -t dev/test
$ mosquitto_pub -t dev/test -m "provare per credere"
```

## rpi-telemetry

Contiene il progetto diviso come segue:

```bash
├── architecture.txt
├── cfg.lua
├── CMakeLists.txt
├── cross
├── Dockerfile
├── fsm.dot
├── fsm.pdf
├── install
├── Lua.cmake
├── src
│   ├── app
│   │   ├── pub.c
│   │   └── sub.c
│   ├── linux
│   │   ├── can_netlink.h
│   │   ├── libsocketcan.c
│   │   └── libsocketcan.h
│   ├── plugin
│   │   ├── canbus.c
│   │   └── dummy.c
│   ├── plugin.h
│   ├── timer.c
│   ├── ultils.c
│   ├── utils.h
│   └── version.h.in
└── Toolchain-rpi.cmake
```

Nei prossimi punti si può vedere come configurare l'ambiente per poter testare la piattaforma
sia su Docker che in locale. 

## Install prerequisites (Ubuntu)

```bash
$ sudo apt install build-essential
$ sudo apt install cmake
$ sudo apt install libmongoc-dev 
$ sudo apt install mosquitto-dev
$ sudo apt install mosquitto
$ sudo apt install mosquitto-clients 
$ sudo apt install libmosquitto-dev
$ sudo apt install libmosquitto1
$ sudo apt-get install libreadline-dev
```

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

## SHORT version
Cross compilation (run from **HOST** shell):

```bash
$ ./cross cmake -Bxbuild -H. -DCROSS=ON
$ ./cross make -Cxbuild
$ .cross xbuild/cross-template
Version 0.0.1-3-gb0f4849§

Testing Mosquitto
MQTT: Cannot assign requested address

Testing BSON
Document:
{ "idx" : 1, "name" : "test" }
length: 30
```

Compilation for local host:

```bash
$ cmake -Bbuild -H.
$ make -Cbuild
$ build/cross-template
Version 0.0.1-3-gb0f4849§

Testing Mosquitto

Testing BSON
Document:
{ "idx" : 1, "name" : "test" }
length: 30
```

Compilation for Dummies:

```bash
$ cmake -Bbuild -H.
$ make -Cbuild
$ build/cross-template
$ build/sub cfg.lua
$ build/pub cfg.lua
```


