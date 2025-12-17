# FIRAClient RAS UFAL

## Requirements
 * g++ (used v9.3.0)
 * Qt (used version v5.12.8)
 * Qt OpenGL
 * Google protocol buffers (used protoc v3.6.1)
 
## Clonning

```shell
git clone --recurse-submodules https://github.com/PedroGilo12/VSSSClient-RAS-UFAL.git
git submodule update --init
```

## Compilation
Create an folder named `build`, open it and run the command `qmake ..`  
So, after this, run the command `make` and if everything goes ok, the binary will be at the folder `bin` (at the main folder).  

```shell
mkdir build
cd build
cmake ..
make
```