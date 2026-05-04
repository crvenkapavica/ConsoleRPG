# Building

## Windows clang++ C++26

Install LLVM/clang at `A:\LLVM` and make sure Ninja is available on `PATH`.
From a Developer PowerShell at the repository root, run:

```powershell
cmake -S . -B out/build/windows-clang-cpp26 -G Ninja -DCMAKE_CXX_COMPILER=A:/LLVM/bin/clang++.exe
cmake --build out/build/windows-clang-cpp26
.\out\build\windows-clang-cpp26\ConsoleRPG\ConsoleRPG.exe
```

The CMake configuration uses clang's `-std=c++2c` flag when the installed compiler supports it.
If an older clang does not support `-std=c++2c`, it falls back to `-std=c++2b`.

Run these commands from a Developer PowerShell or Developer Command Prompt if your LLVM install uses the MSVC/Windows SDK headers and libraries.
