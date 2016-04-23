#!/bin/bash

for site in $*
do
	echo $site:$(printf "GET / HTTP/1.0\r\n\r\n" | nc $site 80 | grep Content-Length | cut -d: -f2)
	sleep 2
done
