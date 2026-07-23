#pragma once
#include "../core/GameState.hpp"
#include "../core/PhysicsEngine.hpp"
#include <queue>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

struct Move { int from; int to; };

struct AIMetrics {
    vector<Move> path;
    int nodes_explored = 0;
    double compute_time_ms = 0.0;
    bool success = false;
};


struct AStarNode {
    GameState state;
    vector<Move> path;
    int g; // cost 
    int f; // g +

    // priority queue  lowest f cost first
    bool operator>(const AStarNode& other) const { return f > other.f; }
};

class Solver {
private:
    // messy
    static int get_heuristic(const GameState& s) {
        int h = 0;
        for(const auto& f : s.board) {
            if(f.colors.empty()) continue;
                for(int i = 1; i < (int)f.colors.size(); i++) {
                if(f.colors[i] != f.colors[i-1]) h += 2; 
            }
        }
        return h;
    }

public:
    static AIMetrics solve_astar(const GameState& start_state) {
        AIMetrics metrics;
        auto start_time = chrono::high_resolution_clock::now();

        //instead basic put priority_queue
        priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;
        unordered_set<GameState> visited;

        pq.push({start_state, {}, 0, get_heuristic(start_state)});
        
        while(!pq.empty()) {
            auto curr = pq.top();
            pq.pop();

            // found faster ? skip 
            if(visited.find(curr.state) != visited.end()) continue;
            visited.insert(curr.state);

            metrics.nodes_explored++;

            bool win = true;
            for(const auto& f : curr.state.board) {
                if(!f.is_done()) { win = false; break; }
            }

            if(win) {
                auto end_time = chrono::high_resolution_clock::now();
                metrics.compute_time_ms = chrono::duration<double, milli>(end_time - start_time).count();
                metrics.path = curr.path;
                metrics.success = true;
                return metrics;
            }

            // explore valid branches
            for(int i = 0; i < (int)curr.state.board.size(); i++) {
                for(int j = 0; j < (int)curr.state.board.size(); j++) {
                    if(PhysicsEngine::can_pour(curr.state, i, j)) {
                        GameState next_s = PhysicsEngine::do_pour(curr.state, i, j);
                        
                        if(visited.find(next_s) == visited.end()) {
                            vector<Move> new_path = curr.path;
                            new_path.push_back({i, j});
                            int new_g = curr.g + 1;
                            int new_f = new_g + get_heuristic(next_s);
                            pq.push({next_s, new_path, new_g, new_f});
                        }
                    }
                }
            }
        }
        
        auto end_time = chrono::high_resolution_clock::now();
        metrics.compute_time_ms = chrono::duration<double, milli>(end_time - start_time).count();
        return metrics;
    }
};