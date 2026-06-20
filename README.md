# FileManagementSystem

Bare-bones C++ file manager prototype for Windows/NTFS.

## Build

```powershell
g++ -std=c++20 -Wall -Wextra -pedantic main.cpp File.cpp Directory.cpp FileSystem.cpp CommandParser.cpp Storage.cpp Utility.cpp -o fms.exe
```

Or with CMake:

```powershell
cmake -S . -B build
cmake --build build
```

## Current Commands

- `ls` / `dir` lists the current folder.
- `cd <path>` changes folder.
- `pwd` prints the current folder.
- `search <name>` recursively searches by filename from the current folder.
- `info <path>` prints basic file metadata.
- `help` shows commands.
- `exit` saves the last folder to `filesystem.dat`.

## Next Research Milestones

1. Replace recursive scanning with an index.
2. Use NTFS volume metadata to detect whether a drive supports the faster path.
3. Build an initial name index from NTFS MFT/USN enumeration.
4. Keep the index fresh by reading the NTFS USN Change Journal.
5. Store the index in SQLite or a compact custom database instead of `filesystem.dat`.
6. Add a minimal Windows UI layer after the search core is stable.

