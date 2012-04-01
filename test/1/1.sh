#!/bin/bash
for ((i=0; $i<=10; i=$(($i+1)))); do
    echo -e "foobarbaz\r\n\r\n" | nc localhost 8888 &
done
