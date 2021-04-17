#!/bin/bash

mysqladmin -u root -ptoor -h mysql ping

mysql -u root -ptoor -h mysql -e "SHOW DATABASES;"
mysql -u root -ptoor -h mysql -e "CREATE DATABASE qa;"
mysql -u root -ptoor -h mysql qa < /root/dump.sql"
mysql -u root -ptoor -h mysql -e "show tables;"

