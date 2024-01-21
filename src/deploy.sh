#!/bin/bash

sudo scp -v -i /home/n/.ssh/id_rsa ./src/cat/s21_cat n@192.168.1.82:/usr/local/bin
sudo scp -v -i /home/n/.ssh/id_rsa ./src/grep/s21_grep n@192.168.1.82:/usr/local/bin
