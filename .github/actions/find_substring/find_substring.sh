#!/usr/bin/env bash

grep -R "einsteiners.us" ./*

if [ $? == 0 ]
  then
    exit 1
fi
