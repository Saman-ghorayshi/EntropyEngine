#pragma once
#include "core/GameState.hpp"
#include <cstdlib>
#include <ctime>

using namespace std;

class LevelGen {
public:
    static GameState make_level(int num_colors, int empty_flasks, int mix_steps) {
        GameState s;
        
        for(int c = 1; c <= num_colors; c++) {
            Flask f;
            for(int i = 0; i < f.max_cap; i++) {
                f.colors.push_back(c);
            }
            s.board.push_back(f);
        }
        
        for(int i = 0; i < empty_flasks; i++) {
            Flask f;
            s.board.push_back(f);
        }

        srand((unsigned int)time(NULL)); 
        
        int steps_done = 0;
        int failsafe = 0; 
        
        // fix: entropy scattering
        while(steps_done < mix_steps && failsafe < 10000) {
            int f1 = rand() % s.board.size();
            int f2 = rand() % s.board.size();
            
            // matching colors dont matter
            // grab one block ..
            if(f1 != f2 && !s.board[f1].colors.empty() && s.board[f2].colors.size() < s.board[f2].max_cap) {
                int moving_color = s.board[f1].colors.back();
                s.board[f1].colors.pop_back();
                s.board[f2].colors.push_back(moving_color);
                steps_done++;
            }
            failsafe++;
        }
        
        return s;
    }
};