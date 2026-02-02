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

#include "draw.h"
#include "cast.h"
#include "player.h"
#include "main.h"

#include <math.h>

#include "raylib.h"

// https://lodev.org/cgtutor/raycasting.html
void RayLoop(player_t* pobj, render_t* render)
{
    int mapX, mapY;

    // hit wall?
    bool hasIntersect = false;

    // casting to int truncates fractional value, no need to floor or divide etc
    // this is used to step along tilemap from players position
    mapX = (int)pobj->posX;
    mapY = (int)pobj->posY;

    // cast ray for each horizontal pixel of screen
    for(int x = 0; x < screenWidth; x++)
    {
        // -- BASIC PARAMETERS, GET POSITIONS AND ANGLES --

        render->camX = 2 * x / (float)screenWidth - 1;

        render->rayDirX = pobj->angX + render->planeX * render->camX;
        render->rayDirY = pobj->angY + render->planeY * render->camX;
        
        // slower, verbose way to do this, but I want to be explicit as this is a learning project.
        // deltaDistX = abs(1 / rayDirX) and deltaDistY = abs(1 / rayDirY) are faster
        render->dxDist = sqrt(1 + pow(render->rayDirY, 2) / pow(render->rayDirX, 2));
        render->dyDist = sqrt(1 + pow(render->rayDirX, 2) / pow(render->rayDirY, 2));

        // -- FIND STEP ALONG ANGLE --

        // HORIZONTAL
        if(render->rayDirX < 0) // ray going west
        {
            render->stepX = -1;
            render->sideDistX = (pobj->posX - mapX) * render->dxDist;
        }
        else // ray going east
        {
            render->stepX = 1;
            render->sideDistX = (mapX + 1.0f - pobj->posX) * render->dxDist;
        }

        // VERTICAL
        if(render->rayDirY < 0) // ray going south
        {
            render->stepY = -1;
            render->sideDistY = (pobj->posY - mapY) * render->dyDist;
        }
        else // ray going north
        {
            render->stepY = 1;
            render->sideDistY = (mapY + 1.0f - pobj->posY) * render->dyDist;
        }

        // -- DDA --

        while(!hasIntersect)
        {
            if(render->sideDistX < render->sideDistY) // x step
            {
                render->sideDistX += render->dxDist;
                mapX += render->stepX;
                render->hitSide = 0;
            }
            else // y step
            {
                render->sideDistY += render->dyDist;
                mapY += render->stepY;
                render->hitSide = 1;
            }

            // stepping tile along x and y, once ray hits something we know we have a hit
            if(worldMap[mapX][mapY] != 0)
                hasIntersect = true;
        }

        if(render->hitSide == 0)
        {
            render->perpendicularDist = render->sideDistX - render->dxDist;
        }
        else if(render->hitSide == 1)
        {
            render->perpendicularDist = render->sideDistY - render->dyDist;
        }

        // center line on screen column
        int lh = (int)(screenHeight / render->perpendicularDist);

        // draw start, end
        int ds = -lh / 2 + screenHeight / 2;
        int de = lh / 2 + screenHeight / 2;

        // cap low and high render positions
        if(ds < 0)
            ds = 0;
        if(de >= screenHeight)
            de = screenHeight - 1;

        DrawLine(x, ds, x, de, VIOLET);
    }
}