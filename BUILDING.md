# Building

## Windows clang++ C++26

Install LLVM/clang at `A:\LLVM` and make sure Ninja is available on `PATH`.
From a Developer PowerShell at the repository root, run:

```powershell
$build = 'out/build/windows-clang-cpp26'
cmake -S . -B $build -G Ninja `
  -DCMAKE_CXX_COMPILER=A:/LLVM/bin/clang++.exe
cmake --build $build
.\out\build\windows-clang-cpp26\ConsoleRPG\ConsoleRPG.exe
```

The CMake configuration uses clang's `-std=c++2c` flag when supported.
Older clang versions fall back to `-std=c++2b`.

Run these commands from a Developer PowerShell or Developer Command Prompt
if your LLVM install uses the MSVC/Windows SDK headers and libraries.
