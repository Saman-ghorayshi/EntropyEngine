#pragma once
#include "core/GameState.hpp"

using namespace std;

class PhysicsEngine {
public:
    // checks if moving top liquid from f1 to f2 is legal
    static bool can_pour(const GameState& state, int from_idx, int to_idx) {
        // cant pour into itself
        if(from_idx == to_idx) return false; 
        
        const Flask& f1 = state.board[from_idx];
        const Flask& f2 = state.board[to_idx];

        if(f1.colors.empty()) return false; // nothing to pour
        if((int)f2.colors.size() >= f2.max_cap) return false; // dest is full

        // if dest is empty, we can always pour into it
        if(f2.colors.empty()) return true;

        // colors must match to stack them
        int top1 = f1.colors.back();
        int top2 = f2.colors.back();
        
        return top1 == top2;
    }
    // actualy moves the liquid and returns a new state copy for the ai  graph
    static GameState do_pour(GameState state, int from_idx, int to_idx) {
        // double check just in case
        if(!can_pour(state, from_idx, to_idx)) return state; 

        int moving_color = state.board[from_idx].colors.back();

        // keep pouring the same color block as long as there is space
        while(!state.board[from_idx].colors.empty() && 
              state.board[from_idx].colors.back() == moving_color &&
              (int)state.board[to_idx].colors.size() < state.board[to_idx].max_cap) {
            
            state.board[from_idx].colors.pop_back();
            state.board[to_idx].colors.push_back(moving_color);
        }
        
        return state;
    }
};