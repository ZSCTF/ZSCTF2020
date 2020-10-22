#!/bin/sh
echo "$FLAG">/home/ctf/flag
/etc/init.d/xinetd start;
sleep infinity;
