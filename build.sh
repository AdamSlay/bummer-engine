#!/bin/bash

if [ ! -d "./vcpkg" ] ; then
  # shallow clone vcpkg repo
  git clone --depth 1 "https://github.com/microsoft/vcpkg.git"

  # bootstrap vcpkg install
  ./vcpkg/bootstrap-vcpkg.sh

  # remove vcpkg .git directory
  rm -rf ./vcpkg/.git

else
  # if the user already has vcpkg
  echo "vcpkg already exists"
  echo "using existing vcpkg"
fi


# install project dependencies from vcpkg.json file in project root
./vcpkg/vcpkg install

# make new build directory and cd into it
rm -rf build && mkdir build && cd build

# run cmake
cmake ..

# build executable
make