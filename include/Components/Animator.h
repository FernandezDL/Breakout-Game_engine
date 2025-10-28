#pragma once
#include <string>
#include <unordered_map>

struct AnimClip {
    int startFrame = 0;  // índice inicial en el sheet
    int length     = 1;  // cantidad de frames
    float fps      = 8.0f;
};

struct Animator {
    int frameWidth  = 32;
    int frameHeight = 32;
    int columns     = 8;   // columnas del sheet
    float timeAcc   = 0.0f;
    int current     = 0;   // índice de frame absoluto (0..N-1)
    std::string state = "idle_down";

    // mapas: "idle_down","walk_left","walk_right","walk_up","walk_down"...
    std::unordered_map<std::string, AnimClip> clips;
};
