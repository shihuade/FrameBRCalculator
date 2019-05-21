#!/bin/bash
#**********************************************************
# brief: format the source code via astyle tool
#        refer to :
#          http://astyle.sourceforge.net/astyle.html
#
#**********************************************************

runCheck()
{
    Version=`astyle --version | awk '{print $4}' | awk 'BEGIN {FS="."} {print $1}'`
    if [ $? -ne 0 ]; then
        echo -e "\033[31m ************************************************ \033[0m"
        echo -e "\033[31m   astyle not found! please install !!!           \033[0m"
        echo -e "\033[31m   refer to:                                      \033[0m"
        echo -e "\033[31m     http://astyle.sourceforge.net/astyle.html    \033[0m"
        echo -e "\033[31m ************************************************ \033[0m"
        exit 1
    fi

    if [ ${Version} -lt 3 ]; then
        echo -e "\033[33m ***************************************************** \033[0m"
        echo -e "\033[33m   warning: version less than 3.0                      \033[0m"
        echo -e "\033[33m             3.0 above is prefered                     \033[0m"
        echo -e "\033[33m ***************************************************** \033[0m"
    fi
}

runMain()
{
    if [ ! -f ${File} ]; then
        echo "can not find file ${File}, please double check!"
        exit 1
    fi

    CfgFile="astyle.cfg"

    runCheck
    astyle --options=${CfgFile} ${File} --suffix=none
    if [ $? -ne 0 ]; then
        echo "astyle failed!"
    fi
}

#**************************************************
if [ $# -ne 1 ]; then
    echo " Usage: $0 \$SourceFile"
    exit 1
fi

File=$1
runMain
#**************************************************
