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

#include "render/draw.h"

#include "raylib.h"

#define WORLD_WIDTH 16
#define WORLD_HEIGHT 16

uint8_t worldMap[WORLD_WIDTH][WORLD_HEIGHT]=
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

void InitializeStructures(player_t* p);
{
    p->posX = p->posY = 3;
    p->angX = -1;
    p->angY = 0;
}

int main()
{
    // use dependency injection to avoid ugly globals when sharing data.
    player_t* pobj;
    render_t* rVars;

    InitializeStructures(pobj);

    InitWindow(screenWidth, screenHeight, "Test Window");
    SetTargetFPS(refreshRate);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        
        ClearBackground(RAYWHITE);

        RayLoop(pobj, rVars);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}