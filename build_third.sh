#!/bin/bash

set -e
set -x

cd third/Tonic/
make $@
# we are not using ziplib yet
#cd ../ZipLib/
#make $@
cd ../lua-5.3.5/
make $@ posix MYCFLAGS=-fPIC
