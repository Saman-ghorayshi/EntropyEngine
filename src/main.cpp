#include <iostream>
#include "core/GameState.hpp"

using namespace std;

int main() {
    cout << "[SYSTEM] initilizing Entropy Engine..." << endl;

    // quick check make sure structs compiles 
    Flask f1;
    f1.colors = {1, 1, 1, 1}; // pretend 1 is the color red

    Flask f2;
    f2.colors = {2, 2}; // half full of color 2

    if(f1.is_done()) {
        cout << ">> Test pass: Flask 1 recognizes it is sorted." << endl;
    } else {
        cout << ">> Test fail: Flask 1 logic is broken." << endl;
    }

    if(!f2.is_done()) {
        cout << ">> Test pass: Flask 2 knows it is NOT sorted." << endl;
    }

    return 0;
}