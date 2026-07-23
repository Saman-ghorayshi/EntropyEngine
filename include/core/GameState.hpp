#pragma once
#include <vector>
#include <string>
#include <algorithm>
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

    // canonical key so two flasks holding the same stack in the same order
    // compare equal regardless of which slot they sit in. flasks have no
    // identity in the puzzle, so [R,B]==[R,B] even if one is slot 0 and the
    // other is slot 3. used by GameState::operator== and hash.
    string key() const {
        string k;
        k.reserve(colors.size() + 1);
        for(int c : colors) k.push_back((char)c);
        return k;
    }
};

struct GameState {
    vector<Flask> board;

    // flasks are unordered in the puzzle, so sort their keys before
    // comparing. otherwise the solver's visited set misses
    // permutation-equivalent states and re-explores them all.
    // ponytail: O(n log n) per compare, n = flask count. fine for
    // board sizes this engine targets (<= ~12 flasks).
    bool operator==(const GameState& other) const {
        if(board.size() != other.board.size()) return false;
        vector<string> a, b;
        a.reserve(board.size());
        b.reserve(other.board.size());
        for(const auto& f : board)        a.push_back(f.key());
        for(const auto& f : other.board) b.push_back(f.key());
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        return a == b;
    }

    vector<string> sorted_keys() const {
        vector<string> ks;
        ks.reserve(board.size());
        for(const auto& f : board) ks.push_back(f.key());
        sort(ks.begin(), ks.end());
        return ks;
    }
};

// stackoverflow said i need this custom hash so game state works with unordered_set well hope they it works
namespace std {
    template <>
    struct hash<GameState> {
        size_t operator()(const GameState& s) const {
            size_t res = 0;
            // hash over sorted flask keys so permutation-equivalent
            // boards share a bucket. matches operator== above.
            auto ks = s.sorted_keys();
            for(const auto& k : ks) {
                for(unsigned char c : k) {
                    res ^= hash<int>()(c) + 0x9e3779b9 + (res << 6) + (res >> 2);
                }
                // one separator byte per flask so [A,B] != [AB]
                res ^= 0xFF + 0x9e3779b9 + (res << 6) + (res >> 2);
            }
            return res;
        }
    };
}
