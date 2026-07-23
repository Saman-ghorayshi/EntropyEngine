#pragma once
#include <raylib.h>
#include "../ai/Solver.hpp"
#include <string>

using namespace std;

class Dashboard {
public:
    static void draw(int screen_w, int screen_h, const AIMetrics& metrics) {
        int panel_w = 300;
        int panel_x = screen_w - panel_w;

        // darw dark bg panel
        DrawRectangle(panel_x, 0, panel_w, screen_h, GetColor(0x222222FF));
        DrawLine(panel_x, 0, panel_x, screen_h, DARKGRAY); // border line

        int pad = 20;
        DrawText("AI TELEMETRY", panel_x + pad, 20, 20, RAYWHITE);
        DrawLine(panel_x + pad, 45, panel_x + panel_w - pad, 45, GRAY);

        DrawText("Search Algorithm: A* Heuristic", panel_x + pad, 60, 10, LIGHTGRAY);

        string status = metrics.success ? "Status: SOLVED" : "Status: IDLE / PLAYING";
        Color status_color = metrics.success ? GREEN : ORANGE;
        DrawText(status.c_str(), panel_x + pad, 100, 10, status_color);

        string nodes = "State Nodes Explored: " + to_string(metrics.nodes_explored);
        DrawText(nodes.c_str(), panel_x + pad, 130, 10, LIGHTGRAY);

        // format the float so it doesnt run off the screen
        string time_str = "Compute Time: " + to_string(metrics.compute_time_ms).substr(0, 6) + " ms";
        DrawText(time_str.c_str(), panel_x + pad, 160, 10, LIGHTGRAY);

        // showing the path length when the engine only plays one move
        // per SPACE press is misleading, so label it as the full solve
        string depth = "Full Solve Length: " + to_string(metrics.path.size()) + " moves";
        DrawText(depth.c_str(), panel_x + pad, 190, 10, LIGHTGRAY);

        string hint = metrics.success
            ? "SPACE: advance one AI move"
            : "click flasks to pour, SPACE to solve";
        DrawText(hint.c_str(), panel_x + pad, 220, 10, GRAY);
    }
};