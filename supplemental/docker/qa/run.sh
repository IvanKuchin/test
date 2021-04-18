#!/bin/bash

mysqladmin -u root -ptoor -h mysql ping && mysql -u root -ptoor -h mysql -e "CREATE DATABASE db;" && mysql -u root -ptoor -h mysql db < ./dump.sql && mysql -u root -ptoor -h mysql -e "show tables;" db
