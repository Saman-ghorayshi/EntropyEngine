#include <iostream>
#include <raylib.h>
#include "core/GameState.hpp"
#include "core/LevelGen.hpp"
#include "core/PhysicsEngine.hpp"
#include "ui/Renderer.hpp"
#include "ui/Dashboard.hpp" 
#include "ai/Solver.hpp"

using namespace std;

int main() {
    cout << "[SYSTEM] Booting Enterprise Engine..." << endl;
    // declare board
    GameState current_board = LevelGen::make_level(5, 2, 60);

    int screen_w = 1200; 
    int screen_h = 500;
    InitWindow(screen_w, screen_h, "Entropy Engine - Telemetry Mode");
    SetTargetFPS(60);

    int selected_flask = -1;
    
    // keep data to draw
    AIMetrics latest_ai_data; 

    while (!WindowShouldClose()) {
        
        // input
        if(IsKeyPressed(KEY_SPACE)) {
            // A* call big gun
            latest_ai_data = Solver::solve_astar(current_board);
            
            if(latest_ai_data.success && !latest_ai_data.path.empty()) {
                Move next_move = latest_ai_data.path[0];
                current_board = PhysicsEngine::do_pour(current_board, next_move.from, next_move.to);
            }
            selected_flask = -1; 
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int mx = GetMouseX();
            int my = GetMouseY();

            int start_x = 100;
            int y = 150;
            int spacing = 100;
            int clicked_idx = -1;

            for(int i = 0; i < current_board.board.size(); i++) {
                int fx = start_x + (i * spacing);
                if(mx >= fx && mx <= fx + 60 && my >= y && my <= y + 200) {
                    clicked_idx = i;
                    break;
                }
            }

            if(clicked_idx != -1) {
                if(selected_flask == -1) {
                    if(!current_board.board[clicked_idx].colors.empty()) {
                        selected_flask = clicked_idx;
                    }
                } else {
                    if(PhysicsEngine::can_pour(current_board, selected_flask, clicked_idx)) {
                        current_board = PhysicsEngine::do_pour(current_board, selected_flask, clicked_idx);
                        // manual move means the AI path is stale
                        latest_ai_data = AIMetrics{};
                    }
                    selected_flask = -1; 
                }
            } else {
                selected_flask = -1;
            }
        }

        // draw
        BeginDrawing();
        ClearBackground(GetColor(0x181818ff)); 

        DrawText("Left Click to Pour | Press SPACE to trigger AI Graph Search", 20, 20, 20, LIGHTGRAY);
        
        Renderer::draw_board(current_board, selected_flask);
        
        Dashboard::draw(screen_w, screen_h, latest_ai_data);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}