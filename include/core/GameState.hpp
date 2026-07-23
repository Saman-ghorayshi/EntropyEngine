#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

// one bottle in the puzzle
struct Flask {
    vector<int> colors; // 0 empty, 1..N are fluid colors
    int max_cap = 4;

    bool is_done() const {
        if(colors.empty()) return true;
        if(colors.size() < max_cap) return false;
        int c = colors[0];
        for(int i = 1; i < (int)colors.size(); i++)
            if(colors[i] != c) return false;
        return true;
    }

    // flasks have no identity in the puzzle, so [R,B] in slot 0 and
    // [R,B] in slot 3 are the same state. key() lets us sort and
    // compare boards independent of flask order.
    string key() const {
        string k;
        k.reserve(colors.size());
        for(int c : colors) k.push_back((char)c);
        return k;
    }
};

struct GameState {
    vector<Flask> board;

    bool operator==(const GameState& other) const {
        if(board.size() != other.board.size()) return false;
        vector<string> a = sorted_keys(), b = other.sorted_keys();
        return a == b;
    }

    vector<string> sorted_keys() const {
        vector<string> ks;
        for(const auto& f : board) ks.push_back(f.key());
        sort(ks.begin(), ks.end());
        return ks;
    }
};

// unordered_set needs a hash. sort the flask keys first so
// permutation-equivalent boards hash to the same bucket.
namespace std {
    template <>
    struct hash<GameState> {
        size_t operator()(const GameState& s) const {
            size_t res = 0;
            for(const auto& k : s.sorted_keys()) {
                for(unsigned char c : k)
                    res ^= hash<int>()(c) + 0x9e3779b9 + (res << 6) + (res >> 2);
                // separator byte so [A,B] and [AB] differ
                res ^= 0xFF + 0x9e3779b9 + (res << 6) + (res >> 2);
            }
            return res;
        }
    };
}
