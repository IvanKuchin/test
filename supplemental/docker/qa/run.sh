#!/bin/bash

mysqladmin -u root -ptoor -h mysql ping

mysql -u root -ptoor -h mysql -e "SHOW DATABASES;"
mysql -u root -ptoor -h mysql db < /root/dump.sql
mysql -u root -ptoor -h mysql db -e "show tables;"

