mkdir bin
clang++ code/main.cpp ^
    -o bin/game.exe ^
    -std=c++20 ^
    -Wall -Wextra ^
    -I libs/include ^
    libs/win-x64/minifb.lib ^
    -Xlinker /NODEFAULTLIB ^
    -lkernel32 -luser32 -lshell32 -ldxgi -ld3d11 -lole32 -lgdi32 -lwinmm -lopengl32 -lmsvcrt -lucrt -lmsvcprt -lvcruntime
bin\game.exe
