#########################################################################
# File Name: find_get_pkg_depends.sh
# Description: find and get pkg depends
# Author: iczelion
# Email: 
# Created: 2017.04.30 11:44:05
#########################################################################
#!/bin/bash

if [ ! $# == 1 ]; then
	echo -e "\033[31mUsage: $0 search_package_name\033[0m"
	exit
fi

echo -e "\033[31m[*] Search $1 package's depends package list.\033[0m"
apt-cache depends $1 
echo

echo -e "\033[31m[*] Generate pkg_depends_raw_list.\033[0m"
if [ -f pkg_depends_raw_list ]; then
	rm pkg_depends_raw_list
fi
apt-cache depends $1 | grep "Depends" | cut -c 12-50 > pkg_depends_raw_list
sed 's/$/&*/g' pkg_depends_raw_list > pkg_depends_raw_list.txt 
if [ -f pkg_depends_raw_list ]; then
	rm pkg_depends_raw_list
fi
cat pkg_depends_raw_list.txt
echo

echo -e "\033[31m[*] Generate pkg_depends_name_list.\033[0m"
if [ -f pkg_depends_name_list.txt ]; then
	rm pkg_depends_name_list.txt
fi
for line in `cat pkg_depends_raw_list.txt`
do
	find /var/cache/apt/archives/ | grep "${line}" >> pkg_depends_name_list.txt
done
cat pkg_depends_name_list.txt
echo

echo -e "\033[31m[*] Copy pkg's depends file to the current directory.\033[0m"
rm -rf pkg_depends
mkdir pkg_depends
for line in `cat pkg_depends_name_list.txt`
do
	cp ${line} pkg_depends/
done
ls pkg_depends/
rm pkg_depends_raw_list.txt
echo -e "\033[31mAll done!\033[0m"
echo
