#pragma once
#include "GameState.hpp"
#include "PhysicsEngine.hpp"
#include <cstdlib>
#include <ctime>

using namespace std;

class LevelGen {
public:
    // generates a garenteed solvable level
    static GameState make_level(int num_colors, int empty_flasks, int mix_steps) {
        GameState s;
        
        // 1) solved state
        for(int c = 1; c <= num_colors; c++) {
            Flask f;
            for(int i = 0; i < f.max_cap; i++) {
                f.colors.push_back(c);
            }
            s.board.push_back(f);
        }
        
        // 2) adding teh empty bottles
        for(int i = 0; i < empty_flasks; i++) {
            Flask f;
            s.board.push_back(f);
        }

        // random seed (stack overflow said use time)
        srand(time(NULL)); 
        
        // 3) scramble it by play random legal moves
        int steps_done = 0;
        int failsafe = 0; // stop infinite loops if it stucked
        
        while(steps_done < mix_steps && failsafe < 10000) {
            int f1 = rand() % s.board.size();
            int f2 = rand() % s.board.size();
            
            if(PhysicsEngine::can_pour(s, f1, f2)) {
                s = PhysicsEngine::do_pour(s, f1, f2);
                steps_done++;
            }
            failsafe++;
        }
        
        return s;
    }
};