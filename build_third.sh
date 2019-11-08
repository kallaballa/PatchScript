#!/bin/bash

set -e

cd third/Tonic/
make $@
cd ../lua-5.3.5/
make $@ posix MYCFLAGS=-fPIC
