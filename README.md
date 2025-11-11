<h1 align="center">🌄 ImageViewer</h1>

> **ImageViewer** — inspired by Google's [Picasa Image Viewer](https://en.wikipedia.org/wiki/Picasa).

(The name of the project may change in the future.)

## ⚙️ Tech Stack

- Language: C/C++
- Frameworks/Libs: [SDL3 3.2.26](https://github.com/libsdl-org/SDL), [SAIL 0.9.10](https://github.com/HappySeaFox/sail)
- Build System: CMake + Ninja + Clang/LLVM + vcpkg

## 🚧 Roadmap

- [x] Basic image loading and rendering
- [x] Semi-transparent background
- [x] Support for PNG, JPEG, WebP etc. (WIP)
<!--- [ ] Image zoom&move around on screen
- [ ] Arrow image navigation (inside app)
- [ ] Cool UI?
- [ ] Settings
- [ ] Image rotation
- [ ] Smooth animations-->
- [ ] ...

## ⌨️ Current shortcuts

- `ESC` or `Q` - Close Window
- `D` - Toggle Debug info
<!--- `SPACE` - Reset image position
- `R` - Reset image position and zoom-->
- ...

## 📦 Build

Make sure you have installed: CMake, Ninja, vcpkg, Clang/LLVM

My personal recommendation for windows users, use [scoop](https://scoop.sh), it will setup PATH values and etc for you.

```sh
# faster download via scoop
scoop install aria2
# tools
scoop install main/cmake main/ninja main/vcpkg main/llvm
```

You have 4 options:

- Debug
- Release
- RelWithDebInfo
- MinSizeRel

Generate make files

```sh
cmake --preset Debug
```

Build project

```sh
cmake --build --preset Debug
```
