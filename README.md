# Yunik-GTC
Yunik Game Template for C++

## Overview
This is the template for the game using C++

## Prerequisites
- CMake
- Ninja
- Clang

## External Dependencies
- OpenGL

## Build

Currently linux is not supported because SoLoud does not support the CMake build with linux audio system.

If you are Windows10 user & want to build with clang, then run the following code first:
```powershell
cmd /c "`"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat`" x86_amd64 & powershell"
```

```bash
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=RELEASE
ninja
```

## License

- Every external libraries has their own license.

- This project is MIT license.

- Sample music is arranged version of 'Tenshi ni Fureta yo!' which I(yunik1004) made.
 You cannot use it for your business purpose.

## Contributions
* [**yunik1004**](https://github.com/yunik1004) - *Initial work*, sample music
