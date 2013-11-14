tar -zxvf rtmidi-2.0.1.tar.gz
cd rtmidi-2.0.1
./configure
make
cd tests
make
cd ../..
make
rm -rf rtmidi-2.0.1.tar.gz
make

