#ifndef _SRC_PARAMS_H_
#define _SRC_PARAMS_H_

constexpr int MAP_WIDTH=60;
constexpr int MAP_HEIGHT=40;
constexpr int PERCENT_FLOOR=45.0f;
constexpr int NUM_FLOOR=(MAP_WIDTH*MAP_HEIGHT)*(PERCENT_FLOOR/100.0f);

extern char map[MAP_HEIGHT][MAP_WIDTH];

void clearMap(char c);

#endif// _SRC_PARAMS_H_
