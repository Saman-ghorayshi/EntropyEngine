#pragma once
#include "../core/GameState.hpp"
#include "../core/PhysicsEngine.hpp"
#include <queue>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <chrono> //sub mili
#include <string>

using namespace std;

struct Move {
    int from;
    int to;
};

// new struct
struct AIMetrics {
    vector<Move> path;
    int nodes_explored = 0;
    double compute_time_ms = 0.0;
    bool success = false;
};

class Solver {
public:
    static AIMetrics solve_bfs(const GameState& start_state) {
        AIMetrics metrics;
        // clock accurate
        auto start_time = chrono::high_resolution_clock::now();

        queue<pair<GameState, vector<Move>>> q;
        unordered_set<GameState> visited;

        q.push({start_state, {}});
        visited.insert(start_state);

        while(!q.empty()) {
            auto curr = q.front();
            GameState s = curr.first;
            vector<Move> path = curr.second;
            q.pop();

            metrics.nodes_explored++;

            bool win = true;
            for(const auto& f : s.board) {
                if(!f.is_done()) { win = false; break; }
            }

            if(win) {
                // stop  clock
                auto end_time = chrono::high_resolution_clock::now();
                metrics.compute_time_ms = chrono::duration<double, milli>(end_time - start_time).count();
                metrics.path = path;
                metrics.success = true;
                return metrics;
            }

            for(int i = 0; i < s.board.size(); i++) {
                for(int j = 0; j < s.board.size(); j++) {
                    if(PhysicsEngine::can_pour(s, i, j)) {
                        GameState next_s = PhysicsEngine::do_pour(s, i, j);
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

        // if we fail just record the time anyway
        auto end_time = chrono::high_resolution_clock::now();
        metrics.compute_time_ms = chrono::duration<double, milli>(end_time - start_time).count();
        return metrics;
    }
};