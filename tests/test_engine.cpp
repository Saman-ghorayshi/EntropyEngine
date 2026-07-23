#include <cassert>
#include <iostream>
#include "../include/core/GameState.hpp"
#include "../include/core/PhysicsEngine.hpp"
#include "../include/core/LevelGen.hpp"
#include "../include/ai/Solver.hpp"

using namespace std;

static int failed = 0;
#define CHECK(cond) do { \
    if(!(cond)) { cout << "FAIL line " << __LINE__ << ": " #cond << endl; failed++; } \
} while(0)

static GameState make_board(const vector<vector<int>>& flasks) {
    GameState s;
    for(const auto& c : flasks) {
        Flask f;
        f.colors = c;
        s.board.push_back(f);
    }
    return s;
}

static bool is_won(const GameState& s) {
    for(const auto& f : s.board) if(!f.is_done()) return false;
    return true;
}

static void test_flask_done() {
    CHECK(Flask{}.is_done());
    CHECK((Flask{{1,1,1,1}}).is_done());
    CHECK(!(Flask{{1,1,1}}).is_done());
    CHECK(!(Flask{{1,2,1,1}}).is_done());
    CHECK((Flask{{2,2,2,2}}).is_done());
}

static void test_can_pour_basic() {
    GameState s = make_board({{1,1,1,1},{2,2,2,2},{}});
    CHECK(!PhysicsEngine::can_pour(s, 0, 0));
    CHECK(!PhysicsEngine::can_pour(s, 2, 0));
    CHECK(!PhysicsEngine::can_pour(s, 0, 1));
    CHECK(PhysicsEngine::can_pour(s, 0, 2));
    CHECK(!PhysicsEngine::can_pour(s, 1, 0));
    CHECK(PhysicsEngine::can_pour(s, 1, 2));
}

static void test_can_pour_match_only() {
    // colors vec: back() is the top. {2,1} = bottom 2, top 1.
    GameState s = make_board({{2,1},{1}});
    CHECK(PhysicsEngine::can_pour(s, 0, 1));
    GameState s2 = make_board({{1,2},{1}});
    CHECK(!PhysicsEngine::can_pour(s2, 0, 1));
}

static void test_pour_moves_block() {
    GameState s = make_board({{1,1,1,1},{2,2,2,2},{}});
    GameState n = PhysicsEngine::do_pour(s, 0, 2);
    CHECK(n.board[0].colors.empty());
    vector<int> expected = {1,1,1,1};
    CHECK(n.board[2].colors == expected);
    CHECK(n.board[1].colors == s.board[1].colors);
}

static void test_pour_stops_at_cap() {
    // from has 5 ones, to has cap 4 with 1 already in it. pour stops
    // when to fills (4 ones in), leaving 2 ones behind in from.
    GameState s = make_board({{1,1,1,1,1},{1}});
    s.board[0].max_cap = 5;
    GameState n = PhysicsEngine::do_pour(s, 0, 1);
    CHECK((int)n.board[0].colors.size() == 2);
    CHECK((int)n.board[1].colors.size() == 4);
}

static void test_canonical_eq() {
    GameState a = make_board({{1,1},{2,2},{}});
    GameState b = make_board({{},{1,1},{2,2}});
    GameState c = make_board({{2,2},{1,1},{}});
    CHECK(a == b);
    CHECK(a == c);
    CHECK(b == c);
    hash<GameState> h;
    CHECK(h(a) == h(b));
    CHECK(h(a) == h(c));
}

static void test_canonical_neq() {
    GameState a = make_board({{1,2},{},{}});
    GameState b = make_board({{2,1},{},{}});
    CHECK(!(a == b));
}

static void test_solver_wins_solvable() {
    // a minimal scrambled level: swap top of two solved flasks
    GameState s = make_board({{1,1,1,2},{2,2,2,1},{},{}});
    auto m = Solver::solve_astar(s);
    CHECK(m.success);
    CHECK(!m.path.empty());
    // replay path, must reach a won state
    GameState cur = s;
    for(const auto& mv : m.path)
        cur = PhysicsEngine::do_pour(cur, mv.from, mv.to);
    CHECK(is_won(cur));
}

static void test_solver_already_won() {
    GameState s = make_board({{1,1,1,1},{2,2,2,2},{3,3,3,3},{}});
    auto m = Solver::solve_astar(s);
    CHECK(m.success);
    CHECK(m.path.empty());
    CHECK(m.nodes_explored <= 1);
}

static void test_levelgen_solvable() {
    // generated levels must be solvable: they come from valid pours
    // backward, so the solver should always find a path.
    srand(42);
    for(int i = 0; i < 5; i++) {
        GameState s = LevelGen::make_level(4, 2, 40);
        auto m = Solver::solve_astar(s);
        CHECK(m.success);
    }
}

static void test_solver_fast_won_small() {
    // 3 colors, 4 units each, 2 empties: a real solvable layout.
    GameState s = make_board({{1,2,3,1},{3,1,2,3},{2,3,1,2},{},{}});
    auto m = Solver::solve_astar(s);
    CHECK(m.success);
    CHECK(m.compute_time_ms < 1000.0);
}

int main() {
    test_flask_done();
    test_can_pour_basic();
    test_can_pour_match_only();
    test_pour_moves_block();
    test_pour_stops_at_cap();
    test_canonical_eq();
    test_canonical_neq();
    test_solver_wins_solvable();
    test_solver_already_won();
    test_levelgen_solvable();
    test_solver_fast_won_small();

    if(failed) {
        cout << failed << " test(s) failed\n";
        return 1;
    }
    cout << "all tests passed\n";
    return 0;
}
