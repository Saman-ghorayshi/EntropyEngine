#pragma once
#pragma once
#include "core/GameState.hpp"
#include <raylib.h>

using namespace std;


class Renderer {
public:
    // lazy way to map our int ids to actual colors
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

    // draw a single bottle at a specific x,y coordinate
    static void draw_flask(const Flask& f, int x, int y) {
        int w = 60;   //  bottle width
        int h = 200;  //   bottle height
        int layer_h = h / f.max_cap; // height of one liquid block

        // draw fluids from bottom to top
        for(int i = 0; i < f.colors.size(); i++) {
            Color c = get_color(f.colors[i]);
            // math to stack them from the bottom of the bottle
            int rect_y =  y +h -  ((i + 1 )  *  layer_h);
            DrawRectangle(x,rect_y, w , layer_h, c );
        }

        // draw the glass outline 
        DrawRectangleLinesEx({(float)x, (float)y, (float)w, (float)h}, 4, RAYWHITE);
    }

    // loops through the matrix and draws everything side by side
    static void draw_board(const GameState& s) {
        int start_x = 100;
        int y = 150;
        int spacing = 100; // space between bottles

        for(int i = 0; i < s.board.size(); i++) {
            draw_flask(s.board[i], start_x + (i * spacing), y);
        }
    }
};