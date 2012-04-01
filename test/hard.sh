#!/bin/bash
for ((i=0; $i<=10; i=$(($i+1)))); do
    echo "foobarbaz" | nc localhost 8888 &
done
