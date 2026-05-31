#include <iostream>
#include <raylib.h>
#include "core/GameState.hpp"
#include "core/LevelGen.hpp"
#include "ui/Renderer.hpp" 

using namespace std;

int main() {
    cout << "[SYSTEM] Booting graphical mode..." << endl;

    // generate a puzzle 5 color, 2 empty,30 mix
    GameState current_board = LevelGen::make_level(5,2,30);

    // start window
    int screen_w = 900;
    int screen_h = 500;
    InitWindow(screen_w, screen_h, "Entropy Engine - Visual Test");
    SetTargetFPS(60);

    // standard game loop
    while (!WindowShouldClose()) {
        
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff)); // dark backgroudn

        // draw text
        DrawText("Entropy Engine - Core Rendering Test", 20, 20, 20, LIGHTGRAY);
        Renderer::draw_board(current_board);

        EndDrawing();
    }

    // clean up memo when press x
    CloseWindow();
    return 0;
}