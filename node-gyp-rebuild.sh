#!/bin/bash

version=`node --version`
echo $version
node-gyp rebuild --target=$version
