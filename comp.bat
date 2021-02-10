DEL game.exe
g++ .\main.cpp -o game.exe  -O1 -Wall -Wextra -std=c++11 -Wno-missing-braces -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
