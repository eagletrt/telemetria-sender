FROM ubuntu:latest
LABEL maintainer="paolo.bosetti@unitn.it"
LABEL name=eagle-rpi

RUN mkdir /root/devel
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y libstdc++6:i386 libgcc1:i386 zlib1g:i386 \
                       build-essential cmake git rsync wget file

ENV CROSS_TRIPLE arm-linux-gnueabihf
ENV CROSS_ROOT /usr/${CROSS_TRIPLE}
ENV WORKDIR /root/devel
WORKDIR ${WORKDIR}
ENV AS=/usr/bin/${CROSS_TRIPLE}-as \
    AR=/usr/bin/${CROSS_TRIPLE}-ar \
    CC=/usr/bin/${CROSS_TRIPLE}-gcc \
    CPP=/usr/bin/${CROSS_TRIPLE}-cpp \
    CXX=/usr/bin/${CROSS_TRIPLE}-g++ \
    LD=/usr/bin/${CROSS_TRIPLE}-ld

# Instead of cloning the whole repo (>1GB at the of writing this), we want to do a so-called "sparse checkout" with "shallow cloning":
# https://stackoverflow.com/questions/600079/is-there-any-way-to-clone-a-git-repositorys-sub-directory-only/13738951#13738951
RUN mkdir rpi_tools && cd rpi_tools && git init && \
    git remote add -f origin https://github.com/raspberrypi/tools &&\
    git config core.sparseCheckout true &&\
    echo "arm-bcm2708/gcc-linaro-${CROSS_TRIPLE}-raspbian" >> .git/info/sparse-checkout && \
    git pull --depth=1 origin master
RUN cd rpi_tools && \
    rsync -av arm-bcm2708/gcc-linaro-${CROSS_TRIPLE}-raspbian/ /usr/ && \
    rm -rf ../rpi_tools

COPY Toolchain-rpi.cmake ${WORKDIR}/..

# Allow dynamically linked executables to run with qemu-arm
RUN apt-get install -y qemu-user qemu-user-static
ENV QEMU_LD_PREFIX ${CROSS_ROOT}/libc
ENV QEMU_SET_ENV "LD_LIBRARY_PATH=${CROSS_ROOT}/lib:${CROSS_ROOT}/libc/lib/${CROSS_TRIPLE}/"

# OpenSSL
# Needed by mosquitto and mongodb
RUN cd ${WORKDIR} && wget https://launchpad.net/openssl-cmake/1.0.1e/1.0.1e-1/+download/openssl-cmake-1.0.1e-src.tar.gz && \
    tar xf openssl-cmake-1.0.1e-src.tar.gz && \
    cd openssl-cmake-1.0.1e-src && \
    cmake -Bxbuild -H. -DCMAKE_TOOLCHAIN_FILE=/root/Toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=${CROSS_ROOT} && \
    make -Cxbuild CFLAGS=-D_POSIX_C_SOURCE=1 install

# libmongo
# See https://mongoc.org
RUN cd ${WORKDIR} && git clone https://github.com/mongodb/mongo-c-driver.git && \
    cd mongo-c-driver && \
    git checkout r1.12  && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_TOOLCHAIN_FILE=/root/Toolchain-rpi.cmake -DCMAKE_INSTALL_PREFIX=${CROSS_ROOT} -Bxbuild -H. && \
    make -Cxbuild CFLAGS=-D_POSIX_C_SOURCE=1 install
  
# libmosquitto
# See https://mosquitto.org/api/files/mosquitto-h.html
ENV MQTT_VERSION 1.5.1
RUN cd ${WORKDIR} && wget https://github.com/eclipse/mosquitto/archive/v${MQTT_VERSION}.tar.gz && tar xvf v${MQTT_VERSION}.tar.gz && cd mosquitto-${MQTT_VERSION} && \
    cmake -DCMAKE_TOOLCHAIN_FILE=/root/Toolchain-rpi.cmake -DDOCUMENTATION=OFF -DWITH_STATIC_LIBRARIES=ON -DWITH_PIC=ON -DCMAKE_INSTALL_PREFIX=${CROSS_ROOT} -Bxbuild -H. && \
    make -Cxbuild CFLAGS=-D_POSIX_C_SOURCE=1 install

RUN cd ${WORKDIR} && wget https://www.lua.org/ftp/lua-5.3.5.tar.gz && \
    tar xf lua-5.3.5.tar.gz && cd lua-5.3.5  && \ 
    MYCFLAGS=-I${CROSS_ROOT}/include make -Csrc liblua.a CC=$CC AR="${AR} rcu" && \
    cd src && install -m 0755 liblua.a ${CROSS_ROOT}/lib && \
    install -m 0644 lua.h luaconf.h lualib.h lauxlib.h lua.hpp ${CROSS_ROOT}/include

# Cleanup build dir
RUN rm -rf ${WORKDIR}/*

VOLUME ${WORKDIR}
