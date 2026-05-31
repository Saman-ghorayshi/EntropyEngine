#include <iostream>
#include "core/GameState.hpp"
#include "core/PhysicsEngine.hpp"
#include "core/LevelGen.hpp"
#include "ai/Solver.hpp" 

using namespace std;

void print_board(const GameState& s) {
    cout << "\n=== CURRENT BOARD ===" << endl;
    for(int i = 0; i < s.board.size(); i++) {
        cout << "Flask " << i << ": [ ";
        for(int c : s.board[i].colors) {
            cout << c << " ";
        }
        cout << "]" <<endl;
    }
    cout << "=========================\n"<< endl;
}

int main() {
    cout << "[SYSTEM] Entropy Engine AI Mode Booting..."  <<  endl;
    
    // making a bit smaller level so the basic BFS doesnt take 10 minutes 
    cout << "Generating puzzle..." << endl;
    GameState s = LevelGen::make_level(4, 2,20); 
    
    print_board(s);

    cout << "[AI] Dispatching BFS Graph Solver..." <<endl;
    vector<Move> win_path = Solver::solve_bfs(s);

    if(!win_path.empty()) {
        cout << ">> Optimal solution found in " << win_path.size() <<" moves:" << endl;
        for(int i = 0; i < win_path.size(); i++) {
            cout << "   Step " << i+1 << ": Pour Flask " << win_path[i].from << " -> Flask " << win_path[i].to << endl;
        }
    }

    return 0;
    
}