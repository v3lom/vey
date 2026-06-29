# vey
IP Stresser

## usage
```
usage:
        vey.exe [ip] [port] [TCP/UDP] [THREADS=5] [PACKET_SIZE=1024]
exmaple:
        vey.exe 192.168.0.1 152 TCP 5 2048
```

## build
### GCC/CLANG
```bash
cd vey
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3" -DCMAKE_EXE_LINKER_FLAGS="-s --static"
cmake --build .
```

### MSVC
```bash
cd vey
mkdir build
cd build
cmake .. -DCMAKE_CXX_FLAGS_RELEASE="/O2 /MT"
cmake --build . --config Release
```
