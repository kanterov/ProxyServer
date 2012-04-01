#!/bin/bash
for ((i=0; $i<=10; i=$(($i+1)))); do
    http_proxy="http://localhost:8888" wget http://localhost/1.avi -O /dev/null &
done
