# EntropyEngine: Procedural State-Space & Autonomous AI Solving Framework

EntropyEngine is a high-performance, hardware-accelerated puzzle engine and AI benchmarking tool written in modern C++20. It features a custom procedural level generator and a built-in telemetry dashboard to monitor algorithmic efficiency in real-time.

## Core Architecture

* **Procedural Entropy Generation:** Instead of randomized configurations that risk unsolvable states, the engine utilizes a reverse-entropy scattering algorithm. It begins with a mathematically solved matrix and applies randomized, valid reverse-moves to guarantee 100% solvability for every generated state space.
* **A* Heuristic Graph Search:** The core AI bypasses brute-force Breadth-First Search (BFS) by implementing an A* pathfinding algorithm. It evaluates game states using a custom heuristic function that penalizes localized color fragmentation, allowing the engine to find optimal sorting paths in sub-milliseconds.
* **Hardware-Accelerated Rendering:** The visualizer and interactive game loop are built natively using Raylib, ensuring a strict 60FPS execution environment completely decoupled from the AI logic layer.
* **Real-Time Telemetry:** A custom-built dashboard tracks and displays AI compute times, state-nodes explored, and memory efficiency dynamically during execution.

## Tech Stack
* **Language:** C++20
* **Graphics/UI:** Raylib (Hardware Accelerated)
* **Build System:** CMake (FetchContent enabled for automatic dependency resolution)

## Compilation & Execution
This project utilizes modern CMake. To build and run the engine locally:

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Debug/EntropyEngine.exe
```
