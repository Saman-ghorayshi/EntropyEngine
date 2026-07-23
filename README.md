# Entropy Engine

A water-sort puzzle solver and visualizer written in C++20 with raylib. A
randomly generated level of N bottles (each capped at 4 units, each unit one
of N colors) is shuffled by reverse-pouring. The player pours top-of-stack
color from one flask onto another if the colors match (or the destination is
empty). Hitting SPACE runs A* over the board state and advances one move per
key press; a side panel shows nodes explored, compute time, and the length of
the full solve path the search returned.

The puzzle itself is a nice small state-space-search problem. The interesting
part is the state-dedup trick (flasks are unordered in the puzzle, so two
boards with the same flasks in different slots are the same state) and the
admissibility question on the heuristic (it overcounts color boundaries, so
A* finds a solution but not provably the shortest one).

## Layout

```
 +----------------------------------------------------+
 | Left Click to Pour | Press SPACE to AI Search      |
 |                                                    |
 |  [R]  [ ]  [G]  [Y]  [P]  [B]  [GB]    +----------+|
 |  [R]  [ ]  [G]  [Y]  [P]  [B]  [G ]   |AI TELEMETRY||
 |  [R]  [ ]  [B]  [Y]  [P]  [B]  [ ]    |A* Heuristic||
 |  [B]  [ ]  [ ]  [Y]  [P]  [G]  [ ]    |IDLE        ||
 |                                        |nodes: 0   ||
 |  ^ selection lifts the flask           |time: 0 ms ||
 |                                        +----------+|
 +----------------------------------------------------+
```

Each flask is a stack of color IDs (1=red, 2=blue, 3=green, 4=yellow,
5=purple, 6=orange). The bottom of the stack is index 0; the top is what
`back()` returns and what gets poured. Click a non-empty flask to lift it,
click a second flask to pour (or another empty flask to put the first back
without pouring).

## How it works

**Level generation.** `LevelGen::make_level(num_colors, num_empty, mix_steps)`
builds N solved flasks (4 of same color each), adds E empty flasks, then
randomly reverse-pours `mix_steps` times. Reverse-pours are guaranteed legal
(A can pour into B if either B is empty or B's top matches A's top and B has
room), so the shuffling always produces a legal, solvable start state.

**Pours.** `PhysicsEngine::can_pour` checks: not same flask, source has
liquid, destination has room, and either destination is empty or its top
color matches the source's top. `do_pour` then pops the entire contiguous
block of the same color off the source stack and pushes it onto the destination
until either the source runs out of that color or the destination is full.

**Search.** `Solver::solve_astar` is a standard A* over the state graph.
`g = moves so far`, `h = 2 * number of color transitions inside any flask`
(every place two adjacent units differ in a flask). `h` is *not* admissible:
a flask with two color blocks needs at least one pour but our heuristic
charges it as two, so A* here returns a valid solve, not the shortest one.
The visited set uses `GameState::operator==` and `std::hash<GameState>` which
both canonicalize by sorting each flask's key string first -- two boards that
differ only by flask order compare equal and hash the same, so the solver
does not re-expand permutation-equivalent states. This is the difference
between the solver exploring 50 nodes vs 5000 on a 5-color board.

**Telemetry.** Each SPACE press runs A* from the current board, saves the
returned path, and plays only path[0]. Subsequent SPACE presses re-solve from
the new (one-move-advanced) state. The dashboard shows `Full Solve Length`
(the path length the most recent solve found) and not "moves remaining",
because the engine recomputes from scratch each press and the displayed path
goes stale the moment a move is applied. A manual click-pour clears the
telemetry because the path is no longer valid for a board the player changed
by hand.

## Build

```
cmake -B build
cmake --build build --config Debug
```

The GUI target (`EntropyEngine`) links raylib, fetched via CMake
`FetchContent`, which builds its own GLFW -- you need an OpenGL 3.3 driver on
the build machine. On Windows the executable lands in `build/Debug/`. On
Linux you also need the X11 / GL dev headers that raylib's GLFW expects
(`libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`,
`libxi-dev`, `libgl-dev`, ...).

If you only want the engine tests (no GUI, no raylib, no GL):

```
cmake -B build -DENTROPY_BUILD_APP=OFF -DENTROPY_BUILD_TESTS=ON
cmake --build build
cd build && ctest --output-on-failure
```

## Tests

`tests/test_engine.cpp` is a single `main` with `CHECK(...)` asserts, no
framework. It covers:
- `Flask::is_done` edge cases (empty, partial, partial-but-mixed, full-same)
- `PhysicsEngine::can_pour` (self-pour, empty source, full dest, empty dest,
  matching top, mismatched top)
- `PhysicsEngine::do_pour` (block moves, stop at cap leaving remainder behind)
- `GameState` canonicalization: permutation-equivalent boards compare equal
  and hash equal; non-equivalent boards do not
- A* on a minimal scrambled level: succeeds, returns a non-empty path, and
  replaying the path yields a won state
- A* on an already-won board: succeeds with empty path and at most 1 node
  explored
- A* on a real 3-color, 4-unit, 2-empty layout: solves in under 1 second
- `LevelGen` determinism: 5 boards generated from `srand(42)` all solve

Run with `ctest --output-on-failure` from the build dir.

## Known limitations

- **Heuristic is not admissible.** `h = 2 * transitions` overcounts a flask
  that has two adjacent differing color blocks: that flask needs at least one
  pour to fix but the heuristic charges it as two. A* still finds a *valid*
  solve because the search is complete, but the path is not guaranteed
  shortest. To get optimal paths, replace the heuristic with one counting
  flasks-not-yet-done (or the count of distinct color blocks minus number of
  completed flasks -- that is admissible).
- **A* recomputes from scratch every SPACE press.** The engine plays one move
  per press and throws the rest of the path away; the next press re-solves
  from the new state. This is fine on small boards (compute time is logged in
  the panel) but on a 7-color or larger board it visibly stalls the UI for
  the duration of the solve. The lazy fix is one CMake flag toggling
  "auto-play the whole path at 1 move per N ms" -- not implemented because
  the per-move telemetry reading is more interesting to watch than a solved
  animation.
- **`LevelGen` uses `srand(time(NULL))`.** Two levels generated within the
  same wall-clock second come out identical. Not a concern for human play,
  but it means the `LevelGen::make_level` test seeds `srand` manually before
  it runs so the 5-level solver loop is reproducible.
- **No undo, restart, or level-regen button.** If the board is stuck, close
  the window and re-launch. The default board is 5 colors / 2 empties / 60
  mix steps, hardened into `main.cpp` for now.
- **raylib is pinned to `master` via FetchContent.** A future raylib release
  could break the build; pin to a release tag (e.g. `5.5`) for reproducible
  CI.

## Repo layout

```
CMakeLists.txt          build setup, raylib FetchContent, test target
LICENSE                 MIT
include/
  ai/Solver.hpp         A* solver, heuristic, AIMetrics
  core/GameState.hpp     Flask, GameState, hash, canonical form
  core/LevelGen.hpp      random shuffler (reverse-pours)
  core/PhysicsEngine.hpp can_pour / do_pour
  ui/Dashboard.hpp       raylib telemetry side panel
  ui/Renderer.hpp       raylib flask drawer
src/main.cpp            window, input loop, SPACE -> solve_astar
tests/test_engine.cpp   assert-based engine self-checks
```

## License

MIT, see `LICENSE`.
