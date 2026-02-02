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

#include "draw.h"
#include "cast.h"
#include "player.h"
#include "main.h"
#include "math.h"

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

void InitializeStructures(player_t* p)
{
    p->posX = p->posY = 3;
    p->angX = -1;
    p->angY = 0;

    p->planeX = 0;
    p->planeY = 0.66;
}

void PlayerMovement(player_t* pobj)
{
    float moveSpeed = 5 * GetFrameTime();
    float rotSpeed = 2.5 * GetFrameTime();

    int testX, testY;

    if(IsKeyDown(KEY_LEFT_SHIFT))
    {
        moveSpeed *= 1.5;
        rotSpeed *= 1.5;
    }

    // FORWARDS/BACKWARDS
    if (IsKeyDown(KEY_W))
    {
        testX = (int)((pobj->posX) + (pobj->angX * moveSpeed));
        testY = (int)((pobj->posY) + (pobj->angY * moveSpeed));

        if(worldMap[testX][testY] == 0) 
        {
            pobj->posX += (pobj->angX * moveSpeed);
            pobj->posY += (pobj->angY * moveSpeed);
        }
    }
    if (IsKeyDown(KEY_S))
    {
        testX = (int)((pobj->posX) - (pobj->angX * moveSpeed));
        testY = (int)((pobj->posY) - (pobj->angY * moveSpeed));

        if(worldMap[testX][testY] == 0) 
        {
            pobj->posX -= (pobj->angX * moveSpeed);
            pobj->posY -= (pobj->angY * moveSpeed);
        }
    }

    // STRAFE
    // FORWARDS/BACKWARDS
    if (IsKeyDown(KEY_A))
    {
        testX = (int)((pobj->posX) - (pobj->planeX * moveSpeed));
        testY = (int)((pobj->posY) - (pobj->planeY * moveSpeed));

        if(worldMap[testX][testY] == 0) 
        {
            pobj->posX -= (pobj->planeX * moveSpeed);
            pobj->posY -= (pobj->planeY * moveSpeed);
        }
    }
    if (IsKeyDown(KEY_D))
    {
        testX = (int)((pobj->posX) + (pobj->planeX * moveSpeed));
        testY = (int)((pobj->posY) + (pobj->planeY * moveSpeed));

        if(worldMap[testX][testY] == 0) 
        {
            pobj->posX += (pobj->planeX * moveSpeed);
            pobj->posY += (pobj->planeY * moveSpeed);
        }
    }

    // TURN
    if (IsKeyDown(KEY_J))
    {        
        // preserve angles to always calculate relative to original angle
        pobj->oldAngX = pobj->angX;
        pobj->oldPlaneX = pobj->planeX;

        // set angles
        pobj->angX = pobj->angX * cos(rotSpeed) - pobj->angY * sin(rotSpeed);
        pobj->angY = pobj->oldAngX * sin(rotSpeed) + pobj->angY * cos(rotSpeed);

        // set projection planes
        pobj->planeX = pobj->planeX * cos(rotSpeed) - pobj->planeY * sin(rotSpeed);
        pobj->planeY = pobj->oldPlaneX * sin(rotSpeed) + pobj->planeY * cos(rotSpeed);
    }
    if (IsKeyDown(KEY_L))
    {
        // preserve angles to always calculate relative to original angle
        pobj->oldAngX = pobj->angX;
        pobj->oldPlaneX = pobj->planeX;

        // set angles
        pobj->angX = pobj->angX * cos(-rotSpeed) - pobj->angY * sin(-rotSpeed);
        pobj->angY = pobj->oldAngX * sin(-rotSpeed) + pobj->angY * cos(-rotSpeed);

        // set projection planes
        pobj->planeX = pobj->planeX * cos(-rotSpeed) - pobj->planeY * sin(-rotSpeed);
        pobj->planeY = pobj->oldPlaneX * sin(-rotSpeed) + pobj->planeY * cos(-rotSpeed);
    }
    
    // VERTICAL AIM
    if(IsKeyDown(KEY_I))   
    {
        if(pobj->angZ < 0 + (screenHeight / 2))
            pobj->angZ += rotSpeed * screenHeight / 2;
    }
    if(IsKeyDown(KEY_K))
    {
        if(pobj->angZ > 0 - (screenHeight / 2))
            pobj->angZ -= rotSpeed * screenHeight / 2;
    }
}

int main()
{
    // use dependency injection to avoid ugly globals when sharing data.
    player_t pobj;
    render_t rVars;

    rVars.time = 0;

    InitializeStructures(&pobj);

    InitWindow(screenWidth, screenHeight, "Test Window");
    SetTargetFPS(refreshRate);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        PlayerMovement(&pobj);
        RayLoop(&pobj, &rVars);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}