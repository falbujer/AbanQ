#!/bin/bash

if [ "$1" == "voidptr" ]
then
  for i in *.i
  do
    CLASS_IFACE=${i/AQS/}
    CLASS_IFACE=${CLASS_IFACE/_p\.i/}
    if [ "@AQSVOIDPTR@" == "$(grep -o '@AQSVOIDPTR@' AQS${CLASS_IFACE}_p.h)" ]
    then
      ./pub_iface.sh $CLASS_IFACE
    fi
  done
else
  for i in *.i
  do
    CLASS_IFACE=${i/AQS/}
    CLASS_IFACE=${CLASS_IFACE/_p\.i/}
    if [ "@AQSVOIDPTR@" == "$(grep -o '@AQSVOIDPTR@' AQS${CLASS_IFACE}_p.h)" ]
    then
      continue
    fi
    ./pub_iface.sh $CLASS_IFACE
  done
fi

