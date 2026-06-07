#pragma once
#include "core/GameState.hpp"
#include <raylib.h>

using namespace std;

class Renderer {
public:
    static Color get_color(int id) {
        switch(id) {
            case 1: return RED;
            case 2: return BLUE;
            case 3: return GREEN;
            case 4: return YELLOW;
            case 5: return PURPLE;
            case 6: return ORANGE;
            default: return DARKGRAY;
        }
    }

    // added is_selected param
    static void draw_flask(const Flask& f, int x, int y, bool is_selected) {
        int w = 60;   
        int h = 200;  
        int layer_h = h / f.max_cap; 

        if(is_selected) y -= 30;

        for(int i = 0; i < f.colors.size(); i++) {
            Color c = get_color(f.colors[i]);
            int rect_y = y + h - ((i + 1) * layer_h);
            DrawRectangle(x, rect_y, w, layer_h, c);
        }

        DrawRectangleLinesEx({(float)x, (float)y, (float)w, (float)h}, 4, RAYWHITE);
    }

    static void draw_board(const GameState& s, int selected_idx) {
        int start_x = 100;
        int y = 150;
        int spacing = 100; 

        for(int i = 0; i < s.board.size(); i++) {
            bool is_sel = (i == selected_idx);
            draw_flask(s.board[i], start_x + (i * spacing), y, is_sel);
        }
    }
};