#include <iostream>
#include <raylib.h>
#include "core/GameState.hpp"
#include "core/LevelGen.hpp"
#include "core/PhysicsEngine.hpp" //for pour
#include "ui/Renderer.hpp"

using namespace std;

int main() {
    cout << "[SYSTEM] Booting interactive mode..." << endl;

    GameState current_board = LevelGen::make_level(5, 2, 30);

    int screen_w = 900;
    int screen_h = 500;
    InitWindow(screen_w, screen_h, "Entropy Engine - Interactive");
    SetTargetFPS(60);

    // state variable to remember which bottle we clicked first
    int selected_flask = -1;

    while (!WindowShouldClose()) {
        
        // --- INPUT LOGIC ---
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int mx = GetMouseX();
            int my = GetMouseY();

            // hardcoded layout math from our renderer
            int start_x = 100;
            int y = 150;
            int spacing = 100;
            int clicked_idx = -1;

            // check if mouse is inside any of the bottle hitboxes
            for(int i = 0; i < current_board.board.size(); i++) {
                int fx = start_x + (i * spacing);
                if(mx >= fx && mx <= fx + 60 && my >= y && my <= y + 200) {
                    clicked_idx = i;
                    break;
                }
            }

            if(clicked_idx != -1) {
                if(selected_flask == -1) {
                    // pick up the bottle (only if it has liquid)
                    if(!current_board.board[clicked_idx].colors.empty()) {
                        selected_flask = clicked_idx;
                    }
                } else {
                    // try to pour it
                    if(PhysicsEngine::can_pour(current_board, selected_flask, clicked_idx)) {
                        current_board = PhysicsEngine::do_pour(current_board, selected_flask, clicked_idx);
                    }
                    // drop the bottle whether it worked or not
                    selected_flask = -1; 
                }
            } else {
                // clicked empty background, drop bottle
                selected_flask = -1;
            }
        }

        // --- DRAW LOGIC ---
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff)); 

        DrawText("Entropy Engine - Click to Pour", 20, 20, 20, LIGHTGRAY);
        
        // pass the selected_flask to the renderer so it knows what to pop up
        Renderer::draw_board(current_board, selected_flask);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}