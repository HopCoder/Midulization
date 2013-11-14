echo extracting libraries
tar -zxvf rtmidi-2.0.1.tar.gz > /dev/null
cd rtmidi-2.0.1 > /dev/null
echo configuring libraries
./configure > /dev/null
echo building libraries
make > /dev/null
cd tests > /dev/null
echo building included .o files
make > /dev/null
cd ../.. > /dev/null
echo building project
make build
echo removing static libraries
rm -rf rtmidi-2.0.1 > /dev/null
echo build successfull

