#!/bin/bash

LAST_FILE=`ls -Art | tail -n 1`
PATH='~/Desktop/'

LAST_PATH="$PATH""$LAST_FILE"

#Pruebas Variables
#echo "${LAST_PATH}"
#cat $LAST_PATH

#Pruebas SFTP
#sftp username@ipaddress
#cd testDirectory 
#put $LAST_PATH
#exit