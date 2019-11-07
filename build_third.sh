#!/bin/bash

set -e

cd third/Tonic/
make clean
make $@
cd ../lua-5.3.5/
make clean
make $@ posix MYCFLAGS=-fPIC
