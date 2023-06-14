call C:\Users\remla\Desktop\emsdk\emsdk\emsdk_env.bat
mkdir bin

C:\Users\remla\Desktop\emsdk\emsdk\upstream\emscripten\em++ code\main.cpp ^
    -lc++ ^
    -o bin/game.js ^
    -std=c++20  ^
    -Wall -Wextra -Werror -Wno-error=unused-parameter ^
    -I libs/include ^
    libs/include/wasm/libminifb.a ^
    -fcolor-diagnostics ^
    -fansi-escape-codes ^
    -sEXPORT_NAME=my_game ^
    -sASYNCIFY ^
    --preload-file C:\Users\remla\Documents\VisualStudioProject\TileMap\assets@assets ^
    -sSTRICT=1 ^
    -sENVIRONMENT=web ^
    -sMODULARIZE=1 ^
    -sALLOW_MEMORY_GROWTH=1 ^
    -sALLOW_TABLE_GROWTH ^
    -sMALLOC=emmalloc ^
    -sEXPORT_ALL=1 ^
    -sEXPORTED_FUNCTIONS=["_malloc","_free","_main"] ^
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap ^
    -sASYNCIFY ^
    --no-entry ^
