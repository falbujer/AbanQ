#!/bin/bash

USER_DIR=$1

if [ "$USER_DIR" == "" ]
then
  USER_DIR="$HOME/user/src/scripts"
fi

CUR_DIR=$PWD
cd ../../
source setenv.profile
cd $CUR_DIR

qsac aqapplication.qs.user -qso aqapplication.qs
mv -vf aqapplication.qs $USER_DIR
qsac -human -nopseudo aqapplication.qs.user -qso aqapplication.qs.src
mv -vf aqapplication.qs.src $USER_DIR


qsac sys.qs.user -qso sys.qs
mv -vf sys.qs $USER_DIR
qsac -human -nopseudo sys.qs.user -qso sys.qs.src
mv -vf sys.qs.src $USER_DIR
