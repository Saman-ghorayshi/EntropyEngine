#include <iostream>
#include "core/GameState.hpp"
#include "core/PhysicsEngine.hpp" 

using namespace std;

int main() {
    cout << "[SYSTEM] running physics tests  ..." << endl;

    GameState s;
    Flask f1; f1.colors = {1, 2, 2}; 
    Flask f2; f2.colors = {1};       
    Flask f3; f3.colors = {2};       

    s.board.push_back(f1); 
    s.board.push_back(f2); 
    s.board.push_back(f3); 

    // test 1: illegal move (pouring 2 onto 1)
    if(PhysicsEngine::can_pour(s, 0, 1)) {
        cout << ">> ERR: allowed an illegal pour." << endl;
    } else {
        cout << ">> pass: blocked illegal pour." << endl;
    }

    // test 2: legal move (pouring the two 2s onto the other 2)
    GameState next_s = PhysicsEngine::do_pour(s, 0, 2);
    
    // f1 should now only have one item (the 1)  f3 should have three items (2, 2, 2)
    if(next_s.board[0].colors.size() == 1 && next_s.board[2].colors.size() == 3) {
        cout << ">> pass: liquids transfered correctly." << endl;
    } else {
        cout << ">> ERR: liquid math is broken." << endl;
    }

    return 0;
}