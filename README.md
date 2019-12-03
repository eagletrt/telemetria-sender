# Fenice Telemetria

## `Setup Can on Linux`

```bash
$ sudo apt-get update
$ sudo apt-get can-utils
```

```bash
$ modprobe vcan
$ sudo ip link add dev vcan0 type vcan
$ sudo ip link set up vcan0 
```

## `Publish and Subscribe MQTT`

```bash
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt-get install mosquitto mosquitto-clients
```

```bash
$ mosquitto_sub -h localhost -t dev/test
$ mosquitto_pub -t dev/test -m "provare per credere"
```

## `\docs`

Un paio di Documenti con materiale relativo a MQTT, sei libero di caricare cose che possano servire.
In più nella cartella config i procedimenti base per configurare il nostro raspberry. NodeJS e Mongo non sono ancora definitivi, bisogna trovare la versione adatta

# `\rpi-telemetry`

Contiene il progetto diviso come segue:

```bash
Telemetria
├─ src
│	├─ lib
│	├─ node_modules
│	│	└─ <..>
│	├─ javascript
	│	├─ utils
│		├─ generators
│	│	└─ index.js
│	
	├─ compile.sh
│	├─ cfg.json
│	├─ messages.json
│	├─ structure.json
│	├─ main.c
│	└─ pub
├─ test
├─ docs
├─ README.md
└─ .git
```

Nei prossimi punti si può vedere come configurare l'ambiente per poter testare la piattaforma
sia su Docker che in locale. 

## `Install prerequisites (Ubuntu)`

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

(single line)
```bash
$ sudo apt install build-essential cmake libmongoc-dev mosquitto-dev mosquitto mosquitto-clients libmosquitto-dev libmosquitto1 libreadline-dev
```

## `Install prerequisites (Arch/Manjaro)`
install the following from the package manager (or with pacman -S packagename):
- cmake
- mongo-c-driver
- mosquitto
- can-utils

for docker setup follow this [guide](https://linuxhint.com/arch-linux-docker-tutorial/)

## `Compilation for local testing between local consoles:`
Nella directory rpi-telemetry lanciare

```sh
cmake -Bbuild -H.
make -Cbuild
cd build
make install
```
Se tutto è andato per il verso giusto con il make genererai gli esegubili. Ora basterà lanciare il publisher e subscriber con argomento il file config .lua che è nella directory principale. Dalla directory rpi-telemetry lanciare:

```sh
build/sub cfg.lua
```
> in alternativa all ultimo comando si puó usare: install/bin/sub cfg.lua

Da un altro terminale sempre da rpi-telemetri:

```sh
build/pub cfg.lua
```
> in alternativa all ultimo comando si puó usare: install/bin/pub cfg.lua

Per terminare ovviamente basta un Ctrl+C sul terminale del pub in questo modo il subscriber resta in ascolto

## `Set-up`
build a [Docker container](https://docker.com):

```bash
$ docker build . -t eagle-rpi
$ docker run -ti --rm -v$(pwd):/root/devel --name erpi eagle-rpi
root@4fedfaf5b5c7:~/devel#
```
> se da VM: docker build . -t eagle-rpi --network=host 

The first command builds the container image. It takes time but you only have to do it once. It downloads about 500 Mb from the Internet, so you need a good downlink.

The switch `-v$(pwd):/root/devel` means that the current dir in the host machine is *mounted* on the `/root/devel` container. Consequently, you will see the workdir in the guiest container synchronized with your starting point in the host machine. In other words, you can use your editor on your PC/Mac, and run compilation commands within the container shell.

The latter will put you in the cross-compiling enviroment (CCE). You can close the CCE with the usual logout (CTRL-D). Any change within the CCE will be lost, due to the `--rm` switch (it means 'remove'). If you want to have a permanent CCE, remove the `--rm` switch, and only use once the command. **After closing it**, you can enter again with:

```bash
$ docker start erpi
$ docker attach erpi
root@4fedfaf5b5c7:~/devel#
```

## `CMake configure`
Work in a terminal on the project root within CCE:

```bash
$ cmake -Bxbuild -H. -DCMAKE_TOOLCHAIN_FILE=/root/Toolchain-rpi.cmake -DCROSS=ON
```
> we are cross compiling therefore we need to set the cmake to CROSS mode

This will prepare an out-of-source build with cmake in the `xbuild` folder using the `CMakeLists.txt` found in the `.` directory.

## `Build`
Work in a terminal on the project root:

```bash
$ make -C xbuild
```

## `Test`
Work in a terminal on the project root within the CCE:

```bash
$ qemu-arm xbuild/cross-template
```

## `Easiest install on RPI`
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

## `SHORT version`
`Cross compilation (run from **HOST** shell):`

```bash
$ ./cross cmake -Bxbuild -H. -DCROSS=ON
$ ./cross make -Cxbuild
$ .cross xbuild/cross-template
Version 0.0.1-3-gb0f4849§

`Testing Mosquitto`
MQTT: Cannot assign requested address

`Testing BSON`
Document:
{ "idx" : 1, "name" : "test" }
length: 30
```

`Compilation for local host:`

```bash
$ cmake -Bbuild -H.
$ make -Cbuild
$ build/cross-template
Version 0.0.1-3-gb0f4849§

`Testing Mosquitto`

Testing BSON
Document:
{ "idx" : 1, "name" : "test" }
length: 30
```

`Compilation for Dummies:`

```bash
$ cmake -Bbuild -H.
$ make -Cbuild
$ build/cross-template
$ build/sub cfg.lua
$ build/pub cfg.lua
```

## `Setup Raspberry Pi`

`in /boot/config.txt alla fine del file`

```bash
    dtparam=i2c_arm=on
    dtparam=i2c0=on
    dtparam=i2c1=on
    dtparam=i2s=on
    dtparam=spi=on

    dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25
    dtoverlay=spi1-1cs 
```
`in /home/pi/Desktop/setup-Can-Bus.sh`

```bash
    #!/bin/bash
    modprobe i2c-dev
    modprobe can_dev
    modprobe can
    modprobe can_raw
    ip link set can0 type can bitrate 1000000
    ifconfig can0 up
```
`Cambiare Password`

```bash
$ sudo passwd 
```
Inserire telemetrypi

`in /etc/wpa_supplicant/wpa_supplicant.conf`

```bash
    network={
            ssid="eagleTRT"
            psk="eaglepiTRT"
            key_mgmt=WPA-PSK
    }
```

