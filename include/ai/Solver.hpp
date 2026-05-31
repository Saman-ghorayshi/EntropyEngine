#pragma once
#include "../core/GameState.hpp"
#include "../core/PhysicsEngine.hpp"
#include <queue>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;

//  struct to hold a move
struct Move {
    int from;
    int to;
};

class Solver {
public:
    // returns a list of moves to win/ empty list if impossible
    static vector<Move> solve_bfs(const GameState& start_state) {
        // queue holds the current board & the history of moves to get there
        queue<pair<GameState, vector<Move>>> q;
        unordered_set<GameState> visited;

        q.push({start_state, {}});
        visited.insert(start_state);

        int nodes_checked = 0; // metrics

        while(!q.empty()) {
            auto curr = q.front();
            GameState s = curr.first;
            vector<Move> path = curr.second;
            q.pop();

            nodes_checked++;

            // check if we won this branch
            bool win = true;
            for(const auto& f : s.board) {
                if(!f.is_done()) {
                    win = false;
                    break;
                }
            }

            if(win) {
                cout << ">> AI solved it! Explored " << nodes_checked << " unique states." << endl;
                return path;
            }

            // brute force every possible move on this board
            for(int i = 0; i < s.board.size(); i++) {
                for(int j = 0; j < s.board.size(); j++) {
                    
                    if(PhysicsEngine::can_pour(s, i, j)) {
                        GameState next_s = PhysicsEngine::do_pour(s, i, j);
                        
                        // if we havent seen this exact board before, add to queue
                        if(visited.find(next_s) == visited.end()) {
                            visited.insert(next_s);
                            
                            vector<Move> new_path = path;
                            new_path.push_back({i, j});
                            q.push({next_s, new_path});
                        }
                    }
                }
            }
        }

        cout << ">> AI failed. Level trapped in unsolveable state." << endl;
        return {};
    }
};