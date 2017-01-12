if [ ! -f sphinxbase-0.8.tar.gz ]; then
wget http://downloads.sourceforge.net/project/cmusphinx/sphinxbase/0.8/sphinxbase-0.8.tar.gz
fi
tar -zxvf sphinxbase-0.8.tar.gz
cd sphinxbase-0.8
./configure --enable-fixed
make -j2 #(2 core)
sudo make install -j2
cd ../

tput setaf 2; echo 'sphinx base installed successfully!'

if [ ! -f pocketsphinx-0.8.tar.gz ]; then
wget http://downloads.sourceforge.net/project/cmusphinx/pocketsphinx/0.8/pocketsphinx-0.8.tar.gz
fi
tar -zxvf pocketsphinx-0.8.tar.gz
cd pocketsphinx-0.8
./configure
make -j2 #(2 core)
sudo make install -j2
cd ../

tput setaf 2; echo 'pocketsphinx installed successfully!'

svn co https://svn.code.sf.net/p/cmusphinx/code/trunk/cmuclmtk/
cd cmuclmtk/
sudo ./autogen.sh && sudo make -j2 && sudo make install -j2
cd ../

tput setaf 2; echo 'Pocketsphinx installed successfully!'
