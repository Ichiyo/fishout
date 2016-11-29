mkdir -p build
cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Debug
make -C build
cp -rf res build
