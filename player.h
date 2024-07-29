#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    float x, y; // player position
    float a;    // player view direction
    float fov;  // field of view

    Player(float x, float y, float a, float fov) : x(x), y(y), a(a), fov(fov) {}
};

#endif // PLAYER_H