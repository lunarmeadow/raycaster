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

#include <miniz.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "miniz_common.h"
#include "miniz_zip.h"
#include "raylib.h"

#include "archive.h"

// RESOURCES

const char* mpkName = "data.mpk";
char mpkPath[512];

lump* entries;

mz_zip_archive mpkArc;

uint32_t NUM_LUMPS = 0;

static bool _isLoaded = false;
static bool _isMounted = false;

// -- MPK MANAGER --

bool MountPackFile()
{
    mz_zip_archive_file_stat stat;
    mz_bool status = MZ_FALSE;

    for(int i = 0; i < NUM_LUMPS; i++)
    {
        if(mz_zip_reader_is_file_a_directory(&mpkArc, i))
            continue;

        status = mz_zip_reader_file_stat(&mpkArc, i, &stat);

        if(!status)
        {
            printf("%s: no data for idx %d!\n", __func__, i);
            return false;
        }

        snprintf(entries[i].name, 512, "%s", stat.m_filename);
        entries[i].data = malloc(stat.m_uncomp_size);
        entries[i].size = stat.m_uncomp_size;

        status = mz_zip_reader_extract_to_mem(&mpkArc, i, entries[i].data, stat.m_uncomp_size, 0);

        if(!status)
        {
            printf("%s: couldn't decomp file %i into buffer!\n", __func__, i);
            return false;
        }
    }

    _isMounted = true;
}

bool LoadPackFile()
{
    mz_bool status = MZ_FALSE;
    mz_zip_error err;

    strncat(mpkPath, GetApplicationDirectory(), 512);
    strncat(mpkPath, mpkName, 512);

    if(mz_zip_validate_file_archive(mpkPath, 0, &err))
        status = mz_zip_reader_init_file(&mpkArc, mpkPath, 0);

    if(!status)
    {
        printf("%s: couldn't initialize MPK file %s - %s!\n", __func__, mpkPath, mz_zip_get_error_string(err));
        _isLoaded = false;
        return false;
    }

    NUM_LUMPS = mz_zip_reader_get_num_files(&mpkArc);

    if(!NUM_LUMPS)
    {
        printf("%s: no lumps, err %s\n", __func__, mz_zip_get_error_string(err));
        _isLoaded = false;
        return false;
    }

    entries = calloc(NUM_LUMPS, sizeof(lump));

    if(!MountPackFile())
    {
        _isLoaded = false;
        return false;
    }

    // we loaded alright
    printf("Added %s with %d lumps\n", mpkPath, NUM_LUMPS);
    _isLoaded = true;

    // close and free miniz resources
    mz_zip_reader_end(&mpkArc);

    return true;
}

void ClosePackFile()
{
    if (entries)
    {
        for (uint32_t i = 0; i < NUM_LUMPS; i++)
        {
            // check if data is actually allocated, free if not.
            // this should avoid freeing on
            if (entries[i].data)
            {
                free(entries[i].data);
            }
        }
        free(entries);
    }

    // reset state variables
    _isLoaded = _isMounted = false;
}

// -- FILE OPERATIONS --

// indexing

// returns a pointer to data, if you need size use GetLengthForLump
void* MPK_GetLumpForName(const char* name)
{
    for(int i = 0; i < NUM_LUMPS; i++)
    {
        if(strcmp(name, entries[i].name) == 0)
        {
            return entries[i].data;
        }
    }

    // nothing found
    return nullptr;
}

void* MPK_GetLumpForNum(int lumpnum)
{
    if(lumpnum >= 0 && lumpnum < NUM_LUMPS)
        return entries[lumpnum].data;
    else
        return nullptr;

    return nullptr;
}

// length

int MPK_GetLengthForLump(const char* name)
{
    for(int i = 0; i < NUM_LUMPS; i++)
    {
        if(strcmp(name, entries[i].name) == 0)
        {
            return entries[i].size;
        }
    }

    // nothing found
    return -1;
}

int MPK_GetLengthForNum(int lumpnum)
{
    if(lumpnum >= 0 && lumpnum < NUM_LUMPS)
        return entries[lumpnum].size;
    else
        return -1;

    // nothing found
    return -1;
}
