#!/bin/bash

gather_deb_by_name()
{
	echo -e "\033[31m[*] Start to gather $1 depends package ...\033[0m"

	#echo -e "\033[31m[*] Search $1 package's depends package list.\033[0m"
	#apt-cache depends $1 
	#echo

	#echo -e "\033[31m[*] Generate $1_depends_raw_list.\033[0m"
	if [ -f $1_depends_raw_list ]; then
		rm $1_depends_raw_list
	fi

	touch $1_depends_raw_list
	echo $1 >> $1_depends_raw_list
	apt-cache depends $1 | grep "Depends" | cut -c 12-80 >> $1_depends_raw_list
	sed 's/$/&*/g' $1_depends_raw_list > $1_depends_raw_list.txt 
	if [ -f $1_depends_raw_list ]; then
		rm $1_depends_raw_list
	fi
	cat $1_depends_raw_list.txt
	echo

	#echo -e "\033[31m[*] Generate $1_depends_name_list.\033[0m"
	if [ -f $1_depends_name_list.txt ]; then
		rm $1_depends_name_list.txt
	fi

	find /var/cache/apt/archives/ | grep "$1" >> $1_depends_name_list.txt
	for line in `cat $1_depends_raw_list.txt` 
	do
		find /var/cache/apt/archives/ | grep "${line}" >> $1_depends_name_list.txt
	done
	cat $1_depends_name_list.txt
	echo

	#echo -e "\033[31m[*] Copy $1's depends file to the current directory.\033[0m"
	rm -rf $1_depends
	mkdir $1_depends
	for line in `cat $1_depends_name_list.txt`
	do
		cp ${line} $1_depends/
	done
	echo -e "\033[31m[*] $1 depends package list: \033[0m"
	ls $1_depends/
	rm $1_depends_raw_list.txt
	rm $1_depends_name_list.txt
	echo
}

if [ ! -f cmd_list.txt ]; then
	echo "cmd_list.txt does not exist!"
else
	rm -rf all_deb
	mkdir all_deb
	cp cmd_list.txt all_deb
	cd all_deb
	for cmd_line in `cat cmd_list.txt`
	do
		apt-get -f install ${cmd_line}
		gather_deb_by_name ${cmd_line} 
	done
fi
tree ../all_deb/

for cmd in `cat cmd_list.txt`
do
	echo -e "\033[31m[*] Confirm ${cmd} has been installed or not.\033[0m"
	echo -e "\033[31m[*] Before ${cmd} deleted ...\033[0m"
	${cmd} --version
	apt-get -f remove ${cmd}
	echo -e "\033[31m[*] After ${cmd} deleted ...\033[0m"
	${cmd} --version
	pwd
	cd "${cmd}_depends"
	dpkg --force-all -i *.deb
	cd .. 
	echo -e "\033[31m[*] After ${cmd} installed ...\033[0m"
	${cmd} --version
done

for cmd in `cat cmd_list.txt`
do
	echo -e "\033[31m[*] After ${cmd} reinstalled ...\033[0m"
	${cmd} --version
done

echo -e "\033[31mAll done!\033[0m"
