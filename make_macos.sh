mkdir -p mac_build
cmake . -Bmac_build -GXcode -DUSER_BUILD_MAC_OS=true
