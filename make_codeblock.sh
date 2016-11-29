mkdir -p codeblocks_debug
cmake -Bcodeblocks_debug -H. -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cp -rf res codeblocks_debug

mkdir -p codeblocks_release
cmake -Bcodeblocks_release -H. -G"CodeBlocks - Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cp -rf res codeblocks_release
