# ImageViewer

A lightweight desktop image viewer inspired by Google Picasa. ImageViewer focuses on a fast, familiar, and simple workflow for viewing photos and supports a wide range of image formats.

## Build requirements

- Windows 10 or 11
- Visual Studio 2022 or newer (Desktop development with C++) / or just MS Build tools without VS
- CMake >= 3.25
- Git
- vcpkg

## Build

1. Clone the repository:

```powershell
git clone git@github.com:fr0zert/ImageViewer.git
cd ImageViewer
```

2. Open "Developer PowerShell for Visual Studio" (or a command prompt with the Visual Studio environment set).

3. Configure the project using a CMake preset:

```powershell
cmake --preset windows-release
# or
cmake --preset windows-debug
```

4. Build the project:

```powershell
cmake --build --preset windows-release
# or
cmake --build --preset windows-debug
```

## Run

After a successful build the executable is available in the output folder (`out\Release` or `out\Debug`). Run it from the terminal:

```powershell
.\out\Release\ImageViewer.exe "D:\test.webp"
# or
.\out\Debug\ImageViewer.exe "D:\test.webp"
```

## Contributing

Contributions, issues, and feature requests are welcome.

## TODOs

- [ ] Load animation for big pictures
- [ ] Scroll images with arrow keys
- [ ] Linux support
- [ ] ...
