#!/bin/bash

pwd
mysqladmin -u root -ptoor -h mysql ping

mysql -u root -ptoor -h mysql -e "SHOW DATABASES;"
mysql -u root -ptoor -h mysql db < ./supplemental/docker/qa/dump.sql
mysql -u root -ptoor -h mysql db -e "show tables;"

cd ./build && ctest --output-on-failure
