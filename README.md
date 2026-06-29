# vey

GCC/CLANG
```bash
cd vey
mkdir -p build
cd build
cmake ..
cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE="-O3" -DCMAKE_EXE_LINKER_FLAGS="-s --static"
cmake --build .
```

MSVC
```bash
cd vey
mkdir build
cd build
cmake ..
cmake . -DCMAKE_CXX_FLAGS_RELEASE="/O2 /MT"
cmake --build . --config Release
```
