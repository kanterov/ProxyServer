#!/bin/bash
rm -rf index.html localhost
http_proxy=http://localhost:8888 wget -r "http://localhost:7777/"
