#include <iostream>
#include "core/GameState.hpp"
#include "core/PhysicsEngine.hpp"
#include "core/LevelGen.hpp"

using namespace std;

// quick draw in terminal
void print_board(const GameState& s) {
    cout << "\n=== CURRENT  BOARD ===" << endl;
    for(int i = 0; i < s.board.size();i++) {
        cout << "Flask " << i << ": [ ";
        for(int c : s.board[i].colors) {
            cout << c << " ";
        }
        cout << "]" << endl;
    }
    cout << "==========================\n" <<  endl;
}

int main() {
    cout << "[SYSTEM] Entropy Engine Booting..." <<endl;
    
    // lets make a level with 3 colors, 2 empty flasks, and scramble it 15 times
    cout << "Generating puzzle..." << endl;
    GameState s = LevelGen::make_level(3, 2, 15);
    
    print_board(s);

    // a player making a move
    cout << "Attempting to pour Flask 0 into Flask 1..." <<endl;
    if(PhysicsEngine::can_pour(s, 0, 1)) {
        s = PhysicsEngine::do_pour(s, 0, 1);
        cout << ">> Move succesful!" << endl;
    } else {
        cout << ">> Illegal move!" << endl;
    }

    print_board(s);

    return 0;
}