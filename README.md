## Building

### Clone the repo

```
git clone --recursive https://github.com/TheGearShift/Carousel-Editor.git
```

### Initialize vcpkg

Windows:

```
./vcpkg/bootstrap-vcpkg.bat
```

Unix:

```
./vcpkg/bootstrap-vcpkg.sh
```

### Generate cmake build system

```
mkdir build &&
cd build &&
cmake ..
```

### Build binaries

```
cmake --build .
```
