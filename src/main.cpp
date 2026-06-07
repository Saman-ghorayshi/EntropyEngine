#include <iostream>
#include <raylib.h>
#include "core/GameState.hpp"
#include "core/LevelGen.hpp"
#include "core/PhysicsEngine.hpp" //for pour 
#include "ui/Renderer.hpp"
#include "ai/Solver.hpp"   

using namespace std;

int main() {
    cout << "[SYSTEM] Booting interactive AI mode..." << endl;

    // lowered mix steps slightly so the BFS solver doesnt take 5 sec to think
    GameState current_board = LevelGen::make_level(5, 2, 20);

    int screen_w = 900;
    int screen_h = 500;
    InitWindow(screen_w, screen_h, "Entropy Engine - AI Assisted");
    SetTargetFPS(60);

    int selected_flask = -1;

    while (!WindowShouldClose()) {
        
       
        //ai solver
        if(IsKeyPressed(KEY_SPACE)) {
            cout << ">> Spacebar pressed. Dispatching AI Solver..." << endl;
            vector<Move> best_path = Solver::solve_bfs(current_board);
            
            if(!best_path.empty()) {
                // excute first move of opt path
                Move next_move = best_path[0];
                current_board = PhysicsEngine::do_pour(current_board, next_move.from, next_move.to);
                cout << ">> AI executed move: " << next_move.from << " -> " << next_move.to << endl;
            } else {
                cout << ">> AI ERROR: Board is currently unsolvable." << endl;
            }
            // drop any held bottles
            selected_flask = -1; 
        }

        // mouse
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

        DrawText("Entropy Engine - Left Click to Pour | Press SPACE for AI assist", 20, 20, 20, LIGHTGRAY);
        
        Renderer::draw_board(current_board, selected_flask);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}