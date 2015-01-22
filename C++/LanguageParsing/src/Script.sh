#!/bin/bash
PWD=`pwd`

#you should probably run this script in a temporary dir

#stops script on error.
set -e

#aquire groupname
read -p "Group name from repo URL (group-XXXX): " group

#get user and pass
read -p "Enter x500 username: " user


#clone repo into temp dir + check success
git clone https://${user}@github.umn.edu/umn-csci-3081S14/${group}.git

cd ${group}/project/src
#git checkout tags/iter1

#check for junk
set +e
echo "Object files inappropriately committed: `ls -l *.o | wc -l` "
set -e

#check for decent dependencies in makefile.
make clean
rm -f *.o

make regex.o
rm -f regex.o
make regex_tests.cpp
rm -f regex_tests.cpp 
make regex_tests

make scanner.o
rm -f scanner.o
make scanner_tests.cpp
rm -f scanner_tests.cpp
make scanner_tests

make i1_assessment_tests

#check that it runs without errors
./regex_tests	
./scanner_tests
./i1_assessment_tests


echo "Congratulations, your project is ready for evaluation"