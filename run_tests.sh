#!/bin/bash

echo "Build and run all the tests"
echo

result=0

#for testcode in `find test -name *.pro`
for testcode in `find test | grep \.pro`
do
    dir=$(dirname $testcode)
    file=$(basename $testcode .pro)

    echo "#############################################################"
    echo "### "$testcode $dir $file

    pushd $dir || exit 10
    if [ -f Makefile ]
    then
        make distclean
    fi

    qmake || exit 11
    make -j`getconf _NPROCESSORS_ONLN` || exit 12
    ./$file 
    if [ ! "$?" == "0" ]
    then 
        result=1
        echo ""
        echo "*************************************************************"
        echo "*** FAIL $PWD"
        echo "*************************************************************"
        echo ""
        echo ""
    fi
    make distclean
    popd
    echo
done

exit $result
