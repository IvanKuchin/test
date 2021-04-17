#!/bin/bash

mysqladmin -u root -ptoor -h mysql ping && mysql -u root -ptoor -h mysql db < ./supplemental/docker/qa/dump.sql
cd ./build && ctest --output-on-failure
