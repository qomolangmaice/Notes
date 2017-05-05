#########################################################################
# File Name: git-config.sh
# Description: 
# Author: iczelion
# Email: qomolangmaice@163.com
# Created: 2017.05.05 21:33:13
#########################################################################
#!/bin/bash

sudo apt-get install git
git config --global user.name "qomolangmaice"
git config --global user.email "qomolangmaice@163.com"

ssh-keygen -t rsa -C "qomolangmaice@163.com"
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
cat ~/.ssh/id_rsa
