/*
    Copyright (C) 2026 lunarmeadow (she/her)

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdio.h>

#include "gamestate.h"
#include "main.h"
#include "draw.h"

#include "raylib.h"

uint8_t worldMap[WORLD_WIDTH][WORLD_HEIGHT] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,0,1,0,0,1,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
  {1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1},
  {1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1},
  {1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void(*loopfunc)(void);

int main()
{
    loopfunc = OnGameLoop;

    InitWindow(screenWidth, screenHeight, "Raycaster");
    SetTargetFPS(refreshRate);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        loopfunc();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}