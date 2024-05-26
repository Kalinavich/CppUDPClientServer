#!/bin/bash

src="./src"
tools="$src/tools"
serverDir="$src/server"
bin="./bin"

mkdir $bin
cp $serverDir/resources.txt $bin

g++ $tools/Socket.cpp ./src/server/main2.cpp -o ./bin/server
g++ $tools/Socket.cpp $tools/Address.cpp ./src/client/main.cpp -o ./bin/client