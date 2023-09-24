#! /bin/bash

_HOME2_=$(dirname $0)
export _HOME2_
_HOME_=$(cd $_HOME2_;pwd)
export _HOME_

echo $_HOME_
cd $_HOME_

if [ "$1""x" == "buildx" ]; then
    docker build -f Dockerfile_ub22 -t toxcore_ready_ub22_004 .
    exit 0
fi

build_for='
ubuntu:22.04
'

for system_to_build_for in $build_for ; do

    system_to_build_for_orig="$system_to_build_for"
    system_to_build_for=$(echo "$system_to_build_for_orig" 2>/dev/null|tr ':' '_' 2>/dev/null)

    cd $_HOME_/
    mkdir -p $_HOME_/"$system_to_build_for"/

    mkdir -p $_HOME_/"$system_to_build_for"/artefacts
    mkdir -p $_HOME_/"$system_to_build_for"/script
    mkdir -p $_HOME_/"$system_to_build_for"/workspace/build/

    ls -al $_HOME_/"$system_to_build_for"/

    rsync -a ../tox_generic_bot.c --exclude=.localrun $_HOME_/"$system_to_build_for"/workspace/build/
    chmod a+rwx -R $_HOME_/"$system_to_build_for"/workspace/build >/dev/null 2>/dev/null

    echo '#! /bin/bash

export DEBIAN_FRONTEND=noninteractive


os_release=$(cat /etc/os-release 2>/dev/null|grep "PRETTY_NAME=" 2>/dev/null|cut -d"=" -f2)
echo "# using /etc/os-release"
system__=$(cat /etc/os-release 2>/dev/null|grep "^NAME=" 2>/dev/null|cut -d"=" -f2|tr -d "\""|sed -e "s#\s##g")
version__=$(cat /etc/os-release 2>/dev/null|grep "^VERSION_ID=" 2>/dev/null|cut -d"=" -f2|tr -d "\""|sed -e "s#\s##g")

echo "# compiling on: $system__ $version__"

#------------------------

cp -a /workspace2/build/* /workspace/build/

export PKG_CONFIG_PATH=/workspace/build/inst_ct/lib/pkgconfig/
pkg-config --cflags --libs libsodium libtoxcore

echo "*** compile ***"


cd /workspace/build/
ls -al /workspace/build/inst_ct/lib/
gcc --version

rm -f tox_generic_bot
rm -f tox_generic_bot.log

gcc -O3 -g -fPIC \
    -DUSE_TOKTOK_TOXCORE \
    -fstack-protector-all \
    -fno-omit-frame-pointer -fsanitize=address \
    -fsanitize=leak \
    -fsanitize-address-use-after-scope \
    -static-libasan \
    tox_generic_bot.c \
    -Wl,-Bstatic $(pkg-config --cflags --libs libtoxcore libsodium) -Wl,-Bdynamic -pthread \
    -o tox_generic_bot

cd /workspace/build/
ldd tox_generic_bot
ls -hal tox_generic_bot

./tox_generic_bot &
sleep 1
tail -f tox_generic_bot.log &
sleep 60

cp -v tox_generic_bot /artefacts/
chmod a+rw /artefacts/*

' > $_HOME_/"$system_to_build_for"/script/run.sh

    mkdir -p $_HOME_/"$system_to_build_for"/workspace/build/c-toxcore/

    docker run -ti --rm \
      -v $_HOME_/"$system_to_build_for"/artefacts:/artefacts \
      -v $_HOME_/"$system_to_build_for"/script:/script \
      -v $_HOME_/"$system_to_build_for"/workspace:/workspace \
      --net=host \
     "toxcore_ready_ub22_004" \
     /bin/sh -c "apk add bash >/dev/null 2>/dev/null; /bin/bash /script/run.sh"
     if [ $? -ne 0 ]; then
        echo "** ERROR **:$system_to_build_for_orig"
        exit 1
     else
        echo "--SUCCESS--:$system_to_build_for_orig"
     fi

done


