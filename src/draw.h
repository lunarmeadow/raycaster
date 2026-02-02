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

#pragma once

extern int screenWidth;
extern int screenHeight;
extern int refreshRate;

typedef struct renderVars {
    // delta time stuff
    float time, oldtime;
    
    float camX;

    // DDA step, once intersects are found, we can step {s | -1, 1} from intersection to rapidly traverse grid.
    int stepX, stepY;

    float rayDirX, rayDirY;

    // delta of x and y distance
    float dxDist, dyDist;

    // length of ray perpendicular to hit (i recall this avoids fisheye)
    float perpendicularDist;

    // step to next side while traversing DDA.
    float sideDistX, sideDistY;

    // 0 = x, 1 = y
    int hitSide;
} render_t;