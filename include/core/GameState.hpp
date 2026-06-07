#pragma once
#include <vector>
#include <iostream>

using namespace std;

// its a single bottle in  game
struct Flask {
    vector<int> colors; // 0 is empty & 1-N are the fluid colors
    int max_cap = 4;

    bool is_done() const {
        if(colors.empty()) return true;
        if(colors.size() < max_cap) return false;
        
        int c = colors[0];
        for(int i = 1; i < colors.size(); i++){
            if(colors[i] != c) return false;
        }
        return true;
    }
};

struct GameState {
    vector<Flask> board;

    // need this to compare board states in the solver
    bool operator==(const GameState& other) const {
        if(board.size() != other.board.size()) return false;
        for(int i = 0; i < board.size(); i++){
            if(board[i].colors != other.board[i].colors) return false;
        }
        return true;
    }
};

// stackoverflow said i need this custom hash so game state works with unordered_set well hope they it works
namespace std {
    template <>
    struct hash<GameState> {
        size_t operator()(const GameState& s) const {
            size_t res = 0;
            for(auto& f : s.board) {
                for(int c : f.colors) {
                    res ^= hash<int>()(c) + 0x9e3779b9 + (res << 6) + (res >> 2);
                }
            }
            return res;
        }
    };
}