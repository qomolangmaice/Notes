# 文件中的内容搜索
# find words in files
find * -type f -print | xargs grep “words”
egrep -r “words”

# 查找/etc目录下所有的ip地址
find /etc -exec grep ‘[0-9][0-9]*[.][0-9][0-9]*[.][0-9][0-9]*[.][0-9][0-9]*’ {} \;

# 查找当前文件夹及其子目录下所有*.lsp的文件并删除所有*.lsp文件
find . -name “*.lsp” -type f -print -exec rm -rf {} \;
# . 表示当前目录下;
# -name “*.lsp” 表示查找所有后缀为lsp的文件;
# -type f 表示文件类型为一般正规文件;
# -print 表示将查询结果打印到屏幕上;
# -exec command ： command为其他命令，-exec后可再接其他命令来处理查找到的结果，
# 上式中，{}表示由find查找到的结果放置到{}位置，-exec一直到”\;”关键字，
# 表示find额外命令的开始(-exec)到结束(\;), 这中间的就是find命令的额外命令rm -rf

# 运行某个目录先多个脚本文件
find /tmp/test -name “*.sh” -exec {} \;

# 修改当前目录下所有脚本的执行权限
find . -name “*.sh” -exec {} \;

# 从 ‘/’ 开始进入根文件系统搜索文件和目录
find / -name file1

# 搜索属于用户 ‘user1′ 的文件和目录
find / -user user1

# 在目录 ‘/ home/user1′ 中搜索带有’.bin’ 结尾的文件
find /home/user1 -name \*.bin

# 搜索在过去100天内未被使用过的执行文件
find /usr/bin -type f -atime +100

# 搜索在10天内被创建或者修改过的文件
find /usr/bin -type f -mtime -10

# 搜索以 ‘.rpm’ 结尾的文件并定义其权限
find / -name \*.rpm -exec chmod 755 ‘{}’ \;

# 搜索以 ‘.rpm’ 结尾的文件，忽略光驱、捷盘等可移动设备
find / -xdev -name \*.rpm

# search software in software center
apt-cache search dmidecode

# 查找本机所有硬件各项信息
sudo dmidecode

# 生成wifi热点二维码
qrencode -s 7 -o qr-wifi.png “WIFI:S:$(zenity –entry –text=”Network name (SSID)” –title=”Create WiFi QR”);T:WPA;P:$(zenity –password –title=”Wifi Password”);;”

# 寻找以 ‘.ps’ 结尾的文件 – 先运行 ‘updatedb’ 命令
locate \*.ps

# 显示一个二进制文件、源码或man的位置
whereis halt

# 显示一个二进制文件或可执行文件的完整路径
which halt

# 删除进程名为’name’的进程
pkill -9 ‘name’

# sftp上传文件到远程服务器以及从远程机器下载文件到本地
# 1.登陆
sftp root@http://www.xyz.com

# 2.从远程机器下载文件到本地,将/var/www/fuyatao/目录下的index.php下载到本地/home/fuyatao/目录下。
sftp> get /var/www/fuyatao/index.php /home/fuyatao/

# 3.上传文件到远程服务器 将把本地/home/fuyatao/downloads/目录下的linuxgl.pdf文件上传至远程主机/var/www/fuyatao/ 目录下
sftp> put /home/fuyatao/downloads/Linuxgl.pdf /var/www/fuyatao/
# 说明：
# 查询本机当前工作目录 lpwd. 改变路径可以用cd ，改变本机路径可以用 lcd;
# ls rm rmdir mkdir 这些命令都可以使用。同理调用本机都是加 l , 即 lls lrm.
# 要离开sftp，用exit 或quit、 bye 均可。

# 压缩解压缩
#打包后，以 gzip 压缩
tar -zcvf /tmp/etc.tar.gz /etc
# 打包后，以 gzip 压缩

#打包后，以 bzip2 压缩
tar -jcvf /tmp/etc.tar.bz2 /etc

# 解压.tar.gz文件
tar -zxvf /tmp/etc.tar.gz

# 解压.tar.bz2文件
tar -jxvf /tmp/etc.tar.bz2

# 假装自己很忙的样子:)
cat /dev/urandom | hexdump -C | grep “ca fe”

# 提取网页中的链接
sudo apt-get install -y lynx
lynx -dump -listonly http://web.mit.edu

# 捕获无线网卡wlp3s0的http请求
sudo tcpdump -i wlp3s0 -n -s 0 -w – | grep -a -o -E “Host\: .*|GET \/.*”

# Linux中如何查看显卡硬件信息
# 在 Linux 中目前有几个命令可以用来获取硬件信息，其中 lspic 命令行工具便可用来获取显卡的硬件信息。
# 大家可使用如下命令来获取系统上的显卡信息：
lspci -vnn | grep VGA -A 12
# 第一行输出便有硬件厂商、型号名称/序列号和 PCI ID。

# 此外，大家还可以使用 lshw 命令来获取上述信息：
lshw -C display

# 查看当前使用的显卡驱动
# 要查看当前 Linux 系统上所使用的显卡驱动名称，同样可以使用 lshw 命令：
sudo lshw -c video | grep configuration
# 可以看到输出的显卡驱动名称有两条，其中一条为 driver=vmwgfx，我们则可以使用如下命令来检查显卡驱动的详情(另外一条操作类似)：
sudo modinfo vmwgfx

# 检查硬件加速
# 启用基于硬件的 3D 加速可以在绘制 3D 图形时直接使用硬件进行处理，这大大加快了 3D 渲染的速度。要使用该功能，必需显卡支持硬件加速并安装了正确的驱动。
# 我们可以使用 glxinfo 命令来获查看得 OpenGL 的详细信息：
glxinfo | grep OpenGL

# diff和patch生成补丁
# 1、使用diff生成补丁；
# diff是Linux下的文件比较命令，不仅可以比较文件，也可以比较两个目录，并且可以将不同之处生成补丁文件，其实就是一种打补丁的命令。使用方法如下：
diff -rNu a b > diff.patch
# 其中，a是旧的目录或文件，b为最近修改后的目录或文件，生成的补丁文件为patch。

# 2、使用patch打补丁；
# patch命令的参数详见man手册，使用patch给另外一个a目录打补丁时如下：
$ patch -p0 < diff.patch

# 用zip命令对文件加密压缩和解压

# 压缩：

# 方法1： zip -re filename.zip filename 回车，输入2次密码

# 方法2： zip -rP passwork filename.zip filename passwork是要输入的密码

# 解压：

# 方法1： unzip filename.zip 按提示输入密码

# 方法2： unzip -P password filename.zip

password是要解压的密码，这个不会有提示输入密码的操作

# 系统信息

# 显示机器的处理器架构
arch

# 显示机器的处理器架构
uname -m

# 显示正在使用的内核版本
uname -r

# 显示硬件系统部件 – (SMBIOS / DMI)
dmidecode -q

# 罗列一个磁盘的架构特性
hdparm -i /dev/hda

# 在磁盘上执行测试性读取操作
hdparm -tT /dev/sda

# 显示CPU info的信息
cat /proc/cpuinfo

# 显示中断
cat /proc/interrupts

# 校验内存使用
cat /proc/meminfo

# 显示哪些swap被使用
cat /proc/swaps

# 显示内核的版本
cat /proc/version

# 显示网络适配器及统计
cat /proc/net/dev

# 显示已加载的文件系统
cat /proc/mounts

# 罗列 PCI 设备
lspci -tv

# 显示 USB 设备
lsusb -tv

# 显示系统日期
date

# 显示2007年的日历表
cal 2007

# 设置日期和时间 – 月日时分年.秒
date 041217002007.00

# 将时间修改保存到 BIOS
clock -w

# 关机 (系统的关机、重启以及登出 )

# 关闭系统(1)
shutdown -h now

# 关闭系统(2)
init 0

# 关闭系统(3)
telinit 0

# 按预定时间关闭系统
shutdown -h hours:minutes &

# 取消按预定时间关闭系统
shutdown -c

# 重启(1)
shutdown -r now

# 重启(2)
reboot

# 注销
logout

# 文件和目录

# 进入 ‘/ home’ 目录
cd /home

# 返回上一级目录
cd ..

# 返回上两级目录
cd ../..

# 进入个人的主目录
cd

# 进入个人的主目录
cd ~user1

# 返回上次所在的目录
cd –

# 显示工作路径
pwd

# 查看目录中的文件
ls

# 查看目录中的文件
ls -F

# 显示文件和目录的详细资料
ls -l

# 显示隐藏文件
ls -a

# 显示包含数字的文件名和目录名

ls *[0-9]*

# 显示文件和目录由根目录开始的树形结构(1)
tree

# 创建一个叫做 ‘dir1′ 的目录
mkdir dir1

# 同时创建两个目录
mkdir dir1 dir2

# 创建一个目录树
mkdir -p /tmp/dir1/dir2

# 删除一个叫做 ‘file1′ 的文件’
rm -f file1

# 删除一个叫做 ‘dir1′ 的目录’
rmdir dir1

# 删除一个叫做 ‘dir1′ 的目录并同时删除其内容
rm -rf dir1

# 同时删除两个目录及它们的内容

rm -rf dir1 dir2

# 重命名/移动 一个目录
mv dir1 new_dir

# 复制一个文件
cp file1 file2

# 复制一个目录下的所有文件到当前工作目录
cp dir/* .

# 复制一个目录到当前工作目录
cp -a /tmp/dir1 .

# 复制一个目录
cp -a dir1 dir2

# 创建一个指向文件或目录的软链接
ln -s file1 lnk1

# 创建一个指向文件或目录的物理链接
ln file1 lnk1

# 修改一个文件或目录的时间戳 – (YYMMDDhhmm)
touch -t 0712250000 file1

# 列出已知的编码
iconv -l

# creates a new from the given input file by assuming it is encoded in fromEncoding and converting it to toEncoding.
iconv -f fromEncoding -t toEncoding inputFile > outputFile

# batch resize files in the current directory and send them to a thumbnails directory (requires convert from Imagemagick)
find . -maxdepth 1 -name *.jpg -print -exec convert “{}” -resize 80×60 “thumbs/{}” \;

# 挂载一个文件系统
# 挂载一个叫做hda2的盘 – 确定目录 ‘/ mnt/hda2′ 已经存在
mount /dev/hda2 /mnt/hda2

# 卸载一个叫做hda2的盘 – 先从挂载点 ‘/ mnt/hda2′ 退出
umount /dev/hda2

# 当设备繁忙时强制卸载
fuser -km /mnt/hda2

# 运行卸载操作而不写入 /etc/mtab 文件- 当文件为只读或当磁盘写满时非常有用
umount -n /mnt/hda2

# 挂载一个软盘
mount /dev/fd0 /mnt/floppy

# 挂载一个cdrom或dvdrom
mount /dev/hdc /mnt/cdrecorder

# 挂载一个文件或ISO镜像文件
mount -o loop file.iso /mnt/cdrom

# 挂载一个Windows FAT32文件系统
mount -t vfat /dev/hda5 /mnt/hda5

# 挂载一个usb 捷盘或闪存设备
mount /dev/sda1 /mnt/usbdisk

# 挂载一个windows网络共享
mount -t smbfs -o username=user,password=pass //WinClient/share /mnt/share

# 磁盘空间
# 显示已经挂载的分区列表
df -h

# 以尺寸大小排列文件和目录
ls -lSr |more

# 估算目录 ‘dir1′ 已经使用的磁盘空间
du -sh dir1

# 以容量大小为依据依次显示文件和目录的大小
du -sk * | sort -rn

# 以大小为依据依次显示已安装的rpm包所使用的空间 (fedora, redhat类系统)
rpm -q -a –qf ‘%10{SIZE}t%{NAME}n’ | sort -k1,1n

# 以大小为依据显示已安装的deb包所使用的空间 (ubuntu, debian类系统)
dpkg-query -W -f=’${Installed-Size;10}t${Package}n’ | sort -k1,1n

# 用户和群组
# 创建一个新用户组
groupadd group_name

# 删除一个用户组
groupdel group_name

# 重命名一个用户组
groupmod -n new_group_name old_group_nam

# 创建一个属于 “admin” 用户组的用户
useradd -c “Name Surname ” -g admin -d /home/user1 -s /bin/bash user1

# 创建一个新用户
useradd user1

# 删除一个用户 ( ‘-r’ 排除主目录)
userdel -r user1

# 修改用户属性
usermod -c “User FTP” -g system -d /ftp/user1 -s /bin/nologin user1

# 修改口令
passwd

# 修改一个用户的口令 (只允许root执行)
passwd user1

# 设置用户口令的失效期限
chage -E 2005-12-31 user1

# 检查 ‘/etc/passwd’ 的文件格式和语法修正以及存在的用户
pwck

# 检查 ‘/etc/passwd’ 的文件格式和语法修正以及存在的群组
grpck

# 登陆进一个新的群组以改变新创建文件的预设群组
newgrp group_name

# 文件的权限 – 使用 “+” 设置权限，使用 “-” 用于取消
# 显示权限
ls -lh

# 将终端划分成5栏显示
ls /tmp | pr -T5 -W$COLUMNS

# 设置目录的所有人(u)、群组(g)以及其他人(o)以读（r ）、写(w)和执行(x)的权限
chmod ugo+rwx directory1

# 删除群组(g)与其他人(o)对目录的读写执行权限
chmod go-rwx directory1

# 改变一个文件的所有人属性
chown user1 file1

# 改变一个目录的所有人属性并同时改变改目录下所有文件的属性
chown -R user1 directory1

# 改变文件的群组
chgrp group1 file1

# 改变一个文件的所有人和群组属性
chown user1:group1 file1

# 罗列一个系统中所有使用了SUID控制的文件
find / -perm -u+s

# 设置一个二进制文件的 SUID 位 – 运行该文件的用户也被赋予和所有者同样的权限
chmod u+s /bin/file1

# 禁用一个二进制文件的 SUID位
chmod u-s /bin/file1

# 设置一个目录的SGID 位 – 类似SUID ，不过这是针对目录的
chmod g+s /home/public

# 禁用一个目录的 SGID 位
chmod g-s /home/public 禁用一个目录的 SGID 位

# 设置一个文件的 STIKY 位 – 只允许合法所有人删除文件
chmod o+t /home/public

# 文件的特殊属性 – 使用 “+” 设置权限，使用 “-” 用于取消
# 只允许以追加方式读写文件
chattr +a file1

# 允许这个文件能被内核自动压缩/解压
chattr +c file1 允许这个文件能被内核自动压缩/解压

# 在进行文件系统备份时，dump程序将忽略这个文件
chattr +d file1

# 设置成不可变的文件，不能被删除、修改、重命名或者链接
chattr +i file1

# 允许一个文件被安全地删除
chattr +s file1

# 一旦应用程序对这个文件执行了写操作，使系统立刻把修改的结果写到磁盘
chattr +S file1

# 若文件被删除，系统会允许你在以后恢复这个被删除的文件
chattr +u file1

# 显示特殊的属性
lsattr

# 打包和压缩文件
# 解压一个叫做 ‘file1.bz2’的文件
bunzip2 file1.bz2

# 压缩一个叫做 ‘file1′ 的文件
bzip2 file1

# 解压一个叫做 ‘file1.gz’的文件
gunzip file1.gz

# 压缩一个叫做 ‘file1’的文件
gzip file1

# 最大程度压缩
gzip -9 file1

# 创建一个叫做 ‘file1.rar’ 的包
rar a file1.rar test_file

# 同时压缩 ‘file1′, ‘file2′ 以及目录 ‘dir1′
rar a file1.rar file1 file2 dir1

# 解压rar包
rar x file1.rar

# 解压rar包
unrar x file1.rar

# 创建一个非压缩的 tarball
tar -cvf archive.tar file1

# 创建一个包含了 ‘file1′, ‘file2′ 以及 ‘dir1’的档案文件
tar -cvf archive.tar file1 file2 dir1

# 显示一个包中的内容
tar -tf archive.tar

# 释放一个包
tar -xvf archive.tar

# 将压缩包释放到 /tmp目录下
tar -xvf archive.tar -C /tmp

# 创建一个bzip2格式的压缩包
tar -cvfj archive.tar.bz2 dir1

# 解压一个bzip2格式的压缩包
tar -xvfj archive.tar.bz2

# 创建一个gzip格式的压缩包
tar -cvfz archive.tar.gz dir1

# 解压一个gzip格式的压缩包
tar -xvfz archive.tar.gz

# 创建一个zip格式的压缩包
zip file1.zip file1

# 将几个文件和目录同时压缩成一个zip格式的压缩包
zip -r file1.zip file1 file2 dir1

# 解压一个zip格式压缩包
unzip file1.zip

# RPM 包 – （Fedora, Redhat及类似系统）
# 安装一个rpm包
rpm -ivh package.rpm

# 安装一个rpm包而忽略依赖关系警告
rpm -ivh –nodeeps package.rpm

# 更新一个rpm包但不改变其配置文件
rpm -U package.rpm 更新一个rpm包但不改变其配置文件

# 更新一个确定已经安装的rpm包
rpm -F package.rpm

# 删除一个rpm包
rpm -e package_name.rpm

# 显示系统中所有已经安装的rpm包
rpm -qa

# 显示所有名称中包含 “httpd” 字样的rpm包
rpm -qa | grep httpd 显示所有名称中包含 “httpd” 字样的rpm包

# 获取一个已安装包的特殊信息
rpm -qi package_name

# 显示一个组件的rpm包
rpm -qg “System Environment/Daemons”

# 显示一个已经安装的rpm包提供的文件列表
rpm -ql package_name

# 显示一个已经安装的rpm包提供的配置文件列表
rpm -qc package_name

# 显示与一个rpm包存在依赖关系的列表
rpm -q package_name –whatrequires

# 显示一个rpm包所占的体积
rpm -q package_name –whatprovides

# 显示在安装/删除期间所执行的脚本l
rpm -q package_name –scripts

# 显示一个rpm包的修改历史
rpm -q package_name –changelog

# 确认所给的文件由哪个rpm包所提供
rpm -qf /etc/httpd/conf/httpd.conf

# 显示由一个尚未安装的rpm包提供的文件列表
rpm -qp package.rpm -l

# 导入公钥数字证书
rpm –import /media/cdrom/RPM-GPG-KEY

# 确认一个rpm包的完整性
rpm –checksig package.rpm

# 确认已安装的所有rpm包的完整性
rpm -qa gpg-pubkey

# 检查文件尺寸、 许可、类型、所有者、群组、MD5检查以及最后修改时间
rpm -V package_name

# 检查系统中所有已安装的rpm包- 小心使用
rpm -Va

# 确认一个rpm包还未安装
rpm -Vp package.rpm

# 从一个rpm包运行可执行文件
rpm2cpio package.rpm | cpio –extract –make-directories *bin*

# 从一个rpm源码安装一个构建好的包
rpm -ivh /usr/src/redhat/RPMS/`arch`/package.rpm

# 从一个rpm源码构建一个 rpm 包
rpmbuild –rebuild package_name.src.rpm

# YUM 软件包升级器 – （Fedora, RedHat及类似系统）
# 下载并安装一个rpm包
yum install package_name

# 将安装一个rpm包，使用你自己的软件仓库为你解决所有依赖关系
yum localinstall package_name.rpm

# 更新当前系统中所有安装的rpm包
yum update package_name.rpm

# 更新一个rpm包
yum update package_name

# 删除一个rpm包
yum remove package_name

# 列出当前系统中安装的所有包
yum list

# 在rpm仓库中搜寻软件包
yum search package_name

# 清理rpm缓存删除下载的包
yum clean packages

# 删除所有头文件
yum clean headers

# 删除所有缓存的包和头文件
yum clean all

# DEB 包 (Debian, Ubuntu 以及类似系统)
# 安装/更新一个 deb 包
dpkg -i package.deb 安装/更新一个 deb 包

# 从系统删除一个 deb 包
dpkg -r package_name

# 显示系统中所有已经安装的 deb 包
dpkg -l

# 显示所有名称中包含 “httpd” 字样的deb包
dpkg -l | grep httpd

# 获得已经安装在系统中一个特殊包的信息
dpkg -s package_name

# 显示系统中已经安装的一个deb包所提供的文件列表
dpkg -L package_name

# 显示尚未安装的一个包所提供的文件列表
dpkg –contents package.deb

# 确认所给的文件由哪个deb包提供
dpkg -S /bin/ping

# APT 软件工具 (Debian, Ubuntu 以及类似系统)
# 安装/更新一个 deb 包
apt-get install package_name 安装/更新一个 deb 包

# 从光盘安装/更新一个 deb 包
apt-cdrom install package_name 从光盘安装/更新一个 deb 包

# 升级列表中的软件包
apt-get update 升级列表中的软件包

# 升级所有已安装的软件
apt-get upgrade 升级所有已安装的软件

# 从系统删除一个deb包
apt-get remove package_name 从系统删除一个deb包

# 确认依赖的软件仓库正确
apt-get check 确认依赖的软件仓库正确

# 从下载的软件包中清理缓存
apt-get clean 从下载的软件包中清理缓存

# 返回包含所要搜索字符串的软件包名称
apt-cache search searched-package 返回包含所要搜索字符串的软件包名称

# 查看文件内容
# 从第一个字节开始正向查看文件的内容
cat file1

# 从最后一行开始反向查看一个文件的内容
tac file1

# 查看一个长文件的内容
more file1 查看一个长文件的内容

# 类似于 ‘more’ 命令，但是它允许在文件中和正向操作一样的反向操作
less file1

# 查看一个文件的前两行
head -2 file1

# 查看一个文件的最后两行
tail -2 file1

# 实时查看被添加到一个文件中的内容
tail -f /var/log/messages

# 文本处理
# general syntax for text manipulation using PIPE, STDIN and STDOUT
cat file1 file2 … | command <> file1_in.txt_or_file1_out.txt

# 合并一个文件的详细说明文本，并将简介写入一个新文件中
cat file1 | command( sed, grep, awk, grep, etc…) > result.txt

# 合并一个文件的详细说明文本，并将简介写入一个已有的文件中
cat file1 | command( sed, grep, awk, grep, etc…) >>

# 在文件 ‘/var/log/messages’中查找关键词”Aug”
grep Aug /var/log/messages

# 在文件 ‘/var/log/messages’中查找以”Aug”开始的词汇
grep ^Aug /var/log/messages

# 选择 ‘/var/log/messages’ 文件中所有包含数字的行
grep [0-9] /var/log/messages

# 在目录 ‘/var/log’ 及随后的目录中搜索字符串”Aug”
grep Aug -R /var/log/*

# 将example.txt文件中的 “string1″ 替换成 “string2″
sed ‘s/stringa1/stringa2/g’ example.txt

# 从example.txt文件中删除所有空白行
sed ‘/^$/d’ example.txt

# 从example.txt文件中删除所有注释和空白行
sed ‘/ *#/d; /^$/d’ example.txt

# 合并上下单元格内容
echo ‘esempio’ | tr ‘[:lower:]’ ‘[:upper:]’

# 从文件example.txt 中排除第一行
sed -e ‘1d’ result.txt

# 查看只包含词汇 “string1″的行
sed -n ‘/stringa1/p’

# 删除每一行最后的空白字符
sed -e ‘s/ *$//’ example.txt

# 从文档中只删除词汇 “string1″ 并保留剩余全部
sed -e ‘s/stringa1//g’ example.txt

# 查看从第一行到第5行内容
sed -n ‘1,5p;5q’ example.txt

# 查看第5行
sed -n ‘5p;5q’ example.txt

# 用单个零替换多个零
sed -e ‘s/00*/0/g’ example.txt

# 标示文件的行数
cat -n file1

# 删除example.txt文件中的所有偶数行
cat example.txt | awk ‘NR%2==1′

# 查看一行第一栏
echo a b c | awk ‘{print $1}’

# 查看一行的第一和第三栏
echo a b c | awk ‘{print $1,$3}’

# 合并两个文件或两栏的内容
paste file1 file2

# 合并两个文件或两栏的内容，中间用”+”区分
paste -d ‘+’ file1 file2

# 排序两个文件的内容
sort file1 file2

# 取出两个文件的并集(重复的行只保留一份)
sort file1 file2 | uniq

# 删除交集，留下其他的行
sort file1 file2 | uniq -u

# 取出两个文件的交集(只留下同时存在于两个文件中的文件)
sort file1 file2 | uniq -d

# 比较两个文件的内容只删除 ‘file1′ 所包含的内容
comm -1 file1 file2

# 比较两个文件的内容只删除 ‘file2′ 所包含的内容
comm -2 file1 file2

# 比较两个文件的内容只删除两个文件共有的部分
comm -3 file1 file2

# 字符设置和文件格式转换
# 将一个文本文件的格式从MSDOS转换成UNIX
dos2unix filedos.txt fileunix.txt

# 将一个文本文件的格式从UNIX转换成MSDOS
unix2dos fileunix.txt filedos.txt

# 将一个文本文件转换成html
recode ..HTML < page.txt > page.html

# 显示所有允许的转换格式
recode -l | more 显示所有允许的转换格式

# 文件系统分析
# 检查磁盘hda1上的坏磁块
badblocks -v /dev/hda1

#修复/检查hda1磁盘上linux文件系统的完整性
fsck /dev/hda1

# 修复/检查hda1磁盘上ext2文件系统的完整性
fsck.ext2 /dev/hda1

# 修复/检查hda1磁盘上ext2文件系统的完整性
e2fsck /dev/hda1

# 修复/检查hda1磁盘上ext3文件系统的完整性
e2fsck -j /dev/hda1

# 修复/检查hda1磁盘上ext3文件系统的完整性
fsck.ext3 /dev/hda1

# 修复/检查hda1磁盘上fat文件系统的完整性
fsck.vfat /dev/hda1

# 修复/检查hda1磁盘上dos文件系统的完整性
fsck.msdos /dev/hda1

# 修复/检查hda1磁盘上dos文件系统的完整性
dosfsck /dev/hda1

# 初始化一个文件系统
# 在hda1分区创建一个文件系统
mkfs /dev/hda1

# 在hda1分区创建一个linux ext2的文件系统
mke2fs /dev/hda1

# 在hda1分区创建一个linux ext3(日志型)的文件系统
mke2fs -j /dev/hda1

# 创建一个 FAT32 文件系统
mkfs -t vfat 32 -F /dev/hda1

# 格式化一个软盘
fdformat -n /dev/fd0

# 创建一个swap文件系统
mkswap /dev/hda3

# SWAP文件系统
# 创建一个swap文件系统
mkswap /dev/hda3

# 启用一个新的swap文件系统
swapon /dev/hda3

# 启用两个swap分区
swapon /dev/hda2 /dev/hdb3 启用两个swap分区

# 备份
# 创建远程机器的备份
ssh user@host “tar -cf – /path/to/dir” | gzip > dir.tar.gz

# 目录的完整备份
dump -0aj -f /tmp/home0.bak /home

#目录的交互式备份
dump -1aj -f /tmp/home0.bak /home

# 还原一个交互式备份
restore -if /tmp/home0.bak

# 同步两边的目录
rsync -rogpav –delete /home /tmp

# 通过SSH通道rsync
rsync -rogpav -e ssh –delete /home ip_address:/tmp

# 通过ssh和压缩将一个远程目录同步到本地目录
rsync -az -e ssh –delete ip_addr:/home/public /home/local

# 通过ssh和压缩将本地目录同步到远程目录
rsync -az -e ssh –delete /home/local ip_addr:/home/public

# 通过ssh在远程主机上执行一次备份本地磁盘的操作
dd bs=1M if=/dev/hda | gzip | ssh user@ip_addr ‘dd of=hda.gz’

# 备份磁盘内容到一个文件
dd if=/dev/sda of=/tmp/file1

# 执行一次对 ‘/home/user’ 目录的交互式备份操作
tar -Puf backup.tar /home/user

# 通过ssh在远程目录中复制一个目录内容
( cd /tmp/local/ && tar c . ) | ssh -C user@ip_addr ‘cd /home/share/ && tar x -p’

# 通过ssh在远程目录中复制一个本地目录
( tar c /home ) | ssh -C user@ip_addr ‘cd /home/backup-home && tar x -p’

# 本地将一个目录复制到另一个地方，保留原有权限及链接
tar cf – . | (cd /tmp/backup ; tar xf – )

# 从一个目录查找并复制所有以 ‘.txt’ 结尾的文件到另一个目录
find /home/user1 -name ‘*.txt’ | xargs cp -av –target-directory=/home/backup/ –parents

# 查找所有以 ‘.log’ 结尾的文件并做成一个bzip包
find /var/log -name ‘*.log’ | tar cv –files-from=- | bzip2 > log.tar.bz2

# 做一个将 MBR (Master Boot Record)内容复制到软盘的动作
dd if=/dev/fd0 of=/dev/hda bs=512 count=1

# 清空一个可复写的光盘内容
cdrecord -v gracetime=2 dev=/dev/cdrom -eject blank=fast -force

# 在磁盘上创建一个光盘的iso镜像文件
mkisofs /dev/cdrom > cd.iso

# 在磁盘上创建一个压缩了的光盘iso镜像文件
mkisofs /dev/cdrom | gzip > cd_iso.gz

# 创建一个目录的iso镜像文件
mkisofs -J -allow-leading-dots -R -V “Label CD” -iso-level 4 -o ./cd.iso data_cd

# 刻录一个ISO镜像文件
cdrecord -v dev=/dev/cdrom cd.iso

# 刻录一个压缩了的ISO镜像文件
gzip -dc cd_iso.gz | cdrecord dev=/dev/cdrom

# 挂载一个ISO镜像文件
mount -o loop cd.iso /mnt/iso

# 从一个CD光盘转录音轨到 wav 文件中
cd-paranoia -B

# 从一个CD光盘转录音轨到 wav 文件中（参数-3）
cd-paranoia — “-3″

# 扫描总线以识别scsi通道
cdrecord –scanbus

# 校验一个设备的md5sum编码，例如一张 CD
dd if=/dev/hdc | md5sum

# 网络 – （以太网和WIFI无线）
# 显示一个以太网卡的配置
ifconfig eth0

# 启用一个 ‘eth0′ 网络设备
ifup eth0

# 禁用一个 ‘eth0′ 网络设备
ifdown eth0

# 控制IP地址
ifconfig eth0 192.168.1.1 netmask 255.255.255.0

# 设置 ‘eth0′ 成混杂模式以嗅探数据包 (sniffing)
ifconfig eth0 promisc

# 以dhcp模式启用 ‘eth0′
dhclient eth0

# show routing table
route -n

# configura default gateway
route add -net 0/0 gw IP_Gateway

# configure static route to reach network ‘192.168.0.0/16′
route add -net 192.168.0.0 netmask 255.255.0.0 gw 192.168.1.1

# remove static route
route del 0/0 gw IP_gateway

# activate ip routing
echo “1” > /proc/sys/net/ipv4/ip_forward

# show hostname of system
hostname

# lookup hostname to resolve name to ip address and viceversa(1)
host www.example.com

# lookup hostname to resolve name to ip address and viceversa(2)
nslookup www.example.com

# show link status of all interfaces
ip link show

# show link status of ‘eth0′
mii-tool eth0

# show statistics of network card ‘eth0′
ethtool eth0

# show all active network connections and their PID
netstat -tup

# show all network services listening on the system and their PID
netstat -tupl

# show all HTTP traffic
tcpdump tcp port 80

# how wireless networks
iwlist scan

# show configuration of a wireless network card
iwconfig eth1

# show hostname
hostname

# lookup hostname to resolve name to ip address and viceversa
host www.example.com

# lookup hostname to resolve name to ip address and viceversa
nslookup www.example.com

# lookup on Whois database
whois www.example.com

# Microsoft Windows networks (SAMBA)
# netbios name resolution
nbtscan ip_addr

# netbios name resolution
nmblookup -A ip_addr

# show remote shares of a windows host
smbclient -L ip_addr/hostname

# like wget can download files from a host windows via smb
smbget -Rr smb://ip_addr/share

# mount a windows network share
mount -t smbfs -o username=user,password=pass //WinClient/share /mnt/share
