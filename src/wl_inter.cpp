//
// Copyright (C) 1991-1992  id Software
// Copyright (C) 2007-2011  Moritz Kroll
// Copyright (C) 2013  Ioan Chera
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

// WL_INTER.C

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "foreign.h"
#include "wl_def.h"
#include "wl_agent.h"
#include "wl_game.h"
#include "wl_menu.h"
#include "wl_text.h"
#pragma hdrstop
#include "ioan_bot.h"	// IOANCH 27.05.2012
#include "ioan_bas.h"
// IOANCH 20130302: unification
LRstruct LevelRatios[LRpack_wl6 > LRpack_sod ? LRpack_wl6 : LRpack_sod];
int32_t lastBreathTime = 0;



void Write (int x, int y, const char *string);

//==========================================================================

/*
==================
=
= CLearSplitVWB
=
==================
*/

void
ClearSplitVWB (void)
{
    WindowX = 0;
    WindowY = 0;
    WindowW = 320;
    WindowH = 160;
}


//==========================================================================


// IOANCH 20130301: unification culling
////////////////////////////////////////////////////////
//
// End of Spear of Destiny
//
////////////////////////////////////////////////////////

void
EndScreen (int palette, int screen)
{
    SDL_Color pal[256];
    CA_CacheScreen (screen);
    VW_UpdateScreen ();
    CA_CacheGrChunk (palette);
    VL_ConvertPalette(grsegs[palette], pal, 256);
    VL_FadeIn (0, 255, pal, 30);
    UNCACHEGRCHUNK (palette);
    IN_ClearKeysDown ();
    IN_Ack ();
    VW_FadeOut ();
}


void EndSpear (void)
{
    SDL_Color pal[256];

    EndScreen (gfxvmap[END1PALETTE][SPEAR()], gfxvmap[ENDSCREEN11PIC][SPEAR()]);

    CA_CacheScreen (gfxvmap[ENDSCREEN3PIC][SPEAR()]);
    VW_UpdateScreen ();
    CA_CacheGrChunk (gfxvmap[END3PALETTE][SPEAR()]);
    VL_ConvertPalette(grsegs[gfxvmap[END3PALETTE][SPEAR()]], pal, 256);
    VL_FadeIn (0, 255, pal, 30);
    UNCACHEGRCHUNK (gfxvmap[END3PALETTE][SPEAR()]);
    fontnumber = 0;
    fontcolor = 0xd0;
    WindowX = 0;
    WindowW = 320;
    PrintX = 0;
    PrintY = 180;
    US_CPrint (STR_ENDGAME1 "\n");
    US_CPrint (STR_ENDGAME2);
    VW_UpdateScreen ();
    IN_UserInput(700);

    PrintX = 0;
    PrintY = 180;
    VWB_Bar (0, 180, 320, 20, 0);
    US_CPrint (STR_ENDGAME3 "\n");
    US_CPrint (STR_ENDGAME4);
    VW_UpdateScreen ();
    IN_UserInput(700);

    VW_FadeOut ();

    EndScreen (gfxvmap[END4PALETTE][SPEAR()], gfxvmap[ENDSCREEN4PIC][SPEAR()]);
    EndScreen (gfxvmap[END5PALETTE][SPEAR()], gfxvmap[ENDSCREEN5PIC][SPEAR()]);
    EndScreen (gfxvmap[END6PALETTE][SPEAR()], gfxvmap[ENDSCREEN6PIC][SPEAR()]);
    EndScreen (gfxvmap[END7PALETTE][SPEAR()], gfxvmap[ENDSCREEN7PIC][SPEAR()]);
    EndScreen (gfxvmap[END8PALETTE][SPEAR()], gfxvmap[ENDSCREEN8PIC][SPEAR()]);
    EndScreen (gfxvmap[END9PALETTE][SPEAR()], gfxvmap[ENDSCREEN9PIC][SPEAR()]);

    EndScreen (gfxvmap[END2PALETTE][SPEAR()], gfxvmap[ENDSCREEN12PIC][SPEAR()]);

    MainMenu[savegame].active = 0;
}



//==========================================================================

/*
==================
=
= Victory
=
==================
*/

void
Victory (void)
{
// IOANCH 20130301: unification culling
    int32_t sec;
    int i, min, kr, sr, tr, x;
    char tempstr[8];

#define RATIOX  6
#define RATIOY  14
#define TIMEX   14
#define TIMEY   8

    // IOANCH 20130302: unification
    if(SPEAR())
    {
        // IOANCH 20130301: unification music
        StartCPMusic (XTHEEND_MUS_sod);

        // IOANCH 20130302: unification
        CA_CacheGrChunk (gfxvmap[BJCOLLAPSE1PIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[BJCOLLAPSE2PIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[BJCOLLAPSE3PIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[BJCOLLAPSE4PIC][SPEAR()]);

        VWB_Bar (0, 0, 320, 200, VIEWCOLOR);
        VWB_DrawPic (124, 44, gfxvmap[BJCOLLAPSE1PIC][SPEAR()]);
        VW_UpdateScreen ();
        VW_FadeIn ();
        VW_WaitVBL (2 * 70);
        VWB_DrawPic (124, 44, gfxvmap[BJCOLLAPSE2PIC][SPEAR()]);
        VW_UpdateScreen ();
        VW_WaitVBL (105);
        VWB_DrawPic (124, 44, gfxvmap[BJCOLLAPSE3PIC][SPEAR()]);
        VW_UpdateScreen ();
        VW_WaitVBL (105);
        VWB_DrawPic (124, 44, gfxvmap[BJCOLLAPSE4PIC][SPEAR()]);
        VW_UpdateScreen ();
        VW_WaitVBL (3 * 70);

        UNCACHEGRCHUNK (gfxvmap[BJCOLLAPSE1PIC][SPEAR()]);
        UNCACHEGRCHUNK (gfxvmap[BJCOLLAPSE2PIC][SPEAR()]);
        UNCACHEGRCHUNK (gfxvmap[BJCOLLAPSE3PIC][SPEAR()]);
        UNCACHEGRCHUNK (gfxvmap[BJCOLLAPSE4PIC][SPEAR()]);
        VL_FadeOut (0, 255, 0, 17, 17, 5);
    }
            // IOANCH 20130301: unification music
    StartCPMusic (IMPALE((int)URAHERO_MUS));
    ClearSplitVWB ();
    CacheLump (gfxvmap[LEVELEND_LUMP_START][SPEAR()], gfxvmap[LEVELEND_LUMP_END][SPEAR()]);
    CA_CacheGrChunk (gfxvmap[STARTFONT][SPEAR()]);

    if(!SPEAR())
        CA_CacheGrChunk (gfxvmap[C_TIMECODEPIC][SPEAR()]);

    VWB_Bar (0, 0, 320, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);
    if (bordercol != VIEWCOLOR)
        DrawStatusBorder (VIEWCOLOR);
// IOANCH 20130301: unification culling
    Write (18, 2, STR_YOUWIN);

    Write (TIMEX, TIMEY - 2, STR_TOTALTIME);

    Write (12, RATIOY - 2, "averages");

#ifdef SPANISH
    Write (RATIOX + 2, RATIOY, STR_RATKILL);
    Write (RATIOX + 2, RATIOY + 2, STR_RATSECRET);
    Write (RATIOX + 2, RATIOY + 4, STR_RATTREASURE);
#else
    Write (RATIOX + 8, RATIOY, STR_RATKILL);
    Write (RATIOX + 4, RATIOY + 2, STR_RATSECRET);
    Write (RATIOX, RATIOY + 4, STR_RATTREASURE);
#endif



    VWB_DrawPic (8, 4, gfxvmap[L_BJWINSPIC][SPEAR()]);


    for (kr = sr = tr = sec = i = 0; i < LRpack; i++)
    {
        sec += LevelRatios[i].time;
        kr += LevelRatios[i].kill;
        sr += LevelRatios[i].secret;
        tr += LevelRatios[i].treasure;
    }

    if(!SPEAR())
    {
        kr /= LRpack;
        sr /= LRpack;
        tr /= LRpack;
    }
    else
    {
        kr /= 14;
        sr /= 14;
        tr /= 14;
    }

    min = sec / 60;
    sec %= 60;

    if (min > 99)
        min = sec = 99;

    i = TIMEX * 8 + 1;
    VWB_DrawPic (i, TIMEY * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (min / 10));
    i += 2 * 8;
    VWB_DrawPic (i, TIMEY * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (min % 10));
    i += 2 * 8;
    Write (i / 8, TIMEY, ":");
    i += 1 * 8;
    VWB_DrawPic (i, TIMEY * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (sec / 10));
    i += 2 * 8;
    VWB_DrawPic (i, TIMEY * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (sec % 10));
    VW_UpdateScreen ();

    itoanoreturn (kr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write (x, RATIOY, tempstr);

    itoanoreturn (sr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write (x, RATIOY + 2, tempstr);

    itoanoreturn (tr, tempstr, 10);
    x = RATIOX + 24 - (int) strlen(tempstr) * 2;
    Write (x, RATIOY + 4, tempstr);

#ifndef SPANISH
    // IOANCH 20130301: unification culling


    if(!SPEAR())
    {
        //
        // TOTAL TIME VERIFICATION CODE
        //
        if (gamestate.difficulty >= gd_medium)
        {
            VWB_DrawPic (30 * 8, TIMEY * 8, gfxvmap[C_TIMECODEPIC][SPEAR()]);
            fontnumber = 0;
            fontcolor = READHCOLOR;
            PrintX = 30 * 8 - 3;
            PrintY = TIMEY * 8 + 8;
            PrintX += 4;
            tempstr[0] = (((min / 10) ^ (min % 10)) ^ 0xa) + 'A';
            tempstr[1] = (int) ((((sec / 10) ^ (sec % 10)) ^ 0xa) + 'A');
            tempstr[2] = (tempstr[0] ^ tempstr[1]) + 'A';
            tempstr[3] = 0;
            US_Print (tempstr);
        }
    }


#endif

    fontnumber = 1;

    VW_UpdateScreen ();
    VW_FadeIn ();

    IN_Ack ();

    VW_FadeOut ();
    if(screenHeight % 200 != 0)
        VL_ClearScreen(0);

    if(!SPEAR())
        UNCACHEGRCHUNK (gfxvmap[C_TIMECODEPIC][SPEAR()]);
    UnCacheLump (gfxvmap[LEVELEND_LUMP_START][SPEAR()], gfxvmap[LEVELEND_LUMP_END][SPEAR()]);


    if(!SPEAR())
        EndText ();
    else
        EndSpear ();



}


//==========================================================================
// IOANCH 20130301: unification culling

/*
==================
=
= PG13
=
==================
*/

void
PG13 (void)
{
    VW_FadeOut ();
    VWB_Bar (0, 0, 320, 200, 0x82);     // background

    CA_CacheGrChunk (gfxvmap[PG13PIC][SPEAR()]);
    VWB_DrawPic (216, 110, gfxvmap[PG13PIC][SPEAR()]);
    VW_UpdateScreen ();

    UNCACHEGRCHUNK (gfxvmap[PG13PIC][SPEAR()]);

    VW_FadeIn ();
    IN_UserInput (TickBase * 7);

    VW_FadeOut ();
}



//==========================================================================

void
Write (int x, int y, const char *string)
{
    static const unsigned int alpha[] = { gfxvmap[L_NUM0PIC][SPEAR()],
        gfxvmap[L_NUM1PIC][SPEAR()], gfxvmap[L_NUM2PIC][SPEAR()], 
        gfxvmap[L_NUM3PIC][SPEAR()], gfxvmap[L_NUM4PIC][SPEAR()], 
        gfxvmap[L_NUM5PIC][SPEAR()], gfxvmap[L_NUM6PIC][SPEAR()], 
        gfxvmap[L_NUM7PIC][SPEAR()], gfxvmap[L_NUM8PIC][SPEAR()], 
        gfxvmap[L_NUM9PIC][SPEAR()], gfxvmap[L_COLONPIC][SPEAR()], 0, 0, 0, 0, 0, 0, 
        gfxvmap[L_APIC][SPEAR()], gfxvmap[L_BPIC][SPEAR()], gfxvmap[L_CPIC][SPEAR()], 
        gfxvmap[L_DPIC][SPEAR()], gfxvmap[L_EPIC][SPEAR()], gfxvmap[L_FPIC][SPEAR()], 
        gfxvmap[L_GPIC][SPEAR()], gfxvmap[L_HPIC][SPEAR()], gfxvmap[L_IPIC][SPEAR()],
        gfxvmap[L_JPIC][SPEAR()], gfxvmap[L_KPIC][SPEAR()], gfxvmap[L_LPIC][SPEAR()], 
        gfxvmap[L_MPIC][SPEAR()], gfxvmap[L_NPIC][SPEAR()], gfxvmap[L_OPIC][SPEAR()], 
        gfxvmap[L_PPIC][SPEAR()], gfxvmap[L_QPIC][SPEAR()], gfxvmap[L_RPIC][SPEAR()], 
        gfxvmap[L_SPIC][SPEAR()], gfxvmap[L_TPIC][SPEAR()], gfxvmap[L_UPIC][SPEAR()], 
        gfxvmap[L_VPIC][SPEAR()], gfxvmap[L_WPIC][SPEAR()], gfxvmap[L_XPIC][SPEAR()], 
        gfxvmap[L_YPIC][SPEAR()], gfxvmap[L_ZPIC][SPEAR()]
    };

    int i, ox, nx, ny, len = (int) strlen(string);
    char ch;

    ox = nx = x * 8;
    ny = y * 8;
    for (i = 0; i < len; i++)
    {
        if (string[i] == '\n')
        {
            nx = ox;
            ny += 16;
        }
        else
        {
            ch = string[i];
            if (ch >= 'a')
                ch -= ('a' - 'A');
            ch -= '0';

            switch (string[i])
            {
                case '!':
                    VWB_DrawPic (nx, ny, gfxvmap[L_EXPOINTPIC][SPEAR()]);
                    nx += 8;
                    continue;
// IOANCH 20130301: unification culling

                case '\'':
                    VWB_DrawPic (nx, ny, gfxvmap[L_APOSTROPHEPIC][SPEAR()]);
                    nx += 8;
                    continue;


                case ' ':
                    break;

                case 0x3a:     // ':'
                    VWB_DrawPic (nx, ny, gfxvmap[L_COLONPIC][SPEAR()]);
                    nx += 8;
                    continue;

                case '%':
                    VWB_DrawPic (nx, ny, gfxvmap[L_PERCENTPIC][SPEAR()]);
                    break;

                default:
                    VWB_DrawPic (nx, ny, alpha[ch]);
            }
            nx += 16;
        }
    }
}


//
// Breathe Mr. BJ!!!
//
void
BJ_Breathe (void)
{
    static int which = 0, max = 10;
    int pics[2] = { static_cast<int>(gfxvmap[L_GUYPIC][SPEAR()]), static_cast<int>(gfxvmap[L_GUY2PIC][SPEAR()]) };

    SDL_Delay(5);

    if ((int32_t) GetTimeCount () - lastBreathTime > max)
    {
        which ^= 1;
        VWB_DrawPic (0, 16, pics[which]);
        VW_UpdateScreen ();
        lastBreathTime = GetTimeCount();
        max = 35;
    }
}



/*
==================
=
= LevelCompleted
=
= Entered with the screen faded out
= Still in split screen mode with the status bar
=
= Exit with the screen faded out
=
==================
*/

void
LevelCompleted (void)
{
#define VBLWAIT 30
#define PAR_AMOUNT      500
#define PERCENT100AMT   10000
    typedef struct
    {
        float time;
        char timestr[6];
    } times;

    int x, i, min, sec, ratio, kr, sr, tr;
    char tempstr[10];
    int32_t bonus, timeleft = 0;
    // IOANCH 20130302: unification
    times parTimes_wl6[] = {
        //
        // Episode One Par Times
        //
        {1.5, "01:30"},
        {2, "02:00"},
        {2, "02:00"},
        {3.5, "03:30"},
        {3, "03:00"},
        {3, "03:00"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {0, "??:??"},           // Boss level
        {0, "??:??"},           // Secret level

        //
        // Episode Two Par Times
        //
        {1.5, "01:30"},
        {3.5, "03:30"},
        {3, "03:00"},
        {2, "02:00"},
        {4, "04:00"},
        {6, "06:00"},
        {1, "01:00"},
        {3, "03:00"},
        {0, "??:??"},
        {0, "??:??"},

        //
        // Episode Three Par Times
        //
        {1.5, "01:30"},
        {1.5, "01:30"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {3.5, "03:30"},
        {2.5, "02:30"},
        {2, "02:00"},
        {6, "06:00"},
        {0, "??:??"},
        {0, "??:??"},

        //
        // Episode Four Par Times
        //
        {2, "02:00"},
        {2, "02:00"},
        {1.5, "01:30"},
        {1, "01:00"},
        {4.5, "04:30"},
        {3.5, "03:30"},
        {2, "02:00"},
        {4.5, "04:30"},
        {0, "??:??"},
        {0, "??:??"},

        //
        // Episode Five Par Times
        //
        {2.5, "02:30"},
        {1.5, "01:30"},
        {2.5, "02:30"},
        {2.5, "02:30"},
        {4, "04:00"},
        {3, "03:00"},
        {4.5, "04:30"},
        {3.5, "03:30"},
        {0, "??:??"},
        {0, "??:??"},

        //
        // Episode Six Par Times
        //
        {6.5, "06:30"},
        {4, "04:00"},
        {4.5, "04:30"},
        {6, "06:00"},
        {5, "05:00"},
        {5.5, "05:30"},
        {5.5, "05:30"},
        {8.5, "08:30"},
        {0, "??:??"},
        {0, "??:??"}
    };
    times parTimes_sod[] = {

        //
        // SPEAR() OF DESTINY TIMES
        //
        {1.5, "01:30"},
        {3.5, "03:30"},
        {2.75, "02:45"},
        {3.5, "03:30"},
        {0, "??:??"},           // Boss 1
        {4.5, "04:30"},
        {3.25, "03:15"},
        {2.75, "02:45"},
        {4.75, "04:45"},
        {0, "??:??"},           // Boss 2
        {6.5, "06:30"},
        {4.5, "04:30"},
        {2.75, "02:45"},
        {4.5, "04:30"},
        {6, "06:00"},
        {0, "??:??"},           // Boss 3
        {6, "06:00"},
        {0, "??:??"},           // Boss 4
        {0, "??:??"},           // Secret level 1
        {0, "??:??"},           // Secret level 2
    };
    times *parTimes = IMPALE(parTimes);

    CacheLump (gfxvmap[LEVELEND_LUMP_START][SPEAR()], gfxvmap[LEVELEND_LUMP_END][SPEAR()]);
    ClearSplitVWB ();           // set up for double buffering in split screen
    VWB_Bar (0, 0, 320, screenHeight / scaleFactor - STATUSLINES + 1, VIEWCOLOR);

    if (bordercol != VIEWCOLOR)
        DrawStatusBorder (VIEWCOLOR);
            // IOANCH 20130301: unification music
    StartCPMusic (IMPALE((int)ENDLEVEL_MUS));

//
// do the intermission
//
    IN_ClearKeysDown ();
    IN_StartAck ();
// IOANCH 20130301: unification culling
    VWB_DrawPic (0, 16, gfxvmap[L_GUYPIC][SPEAR()]);

    if ((!SPEAR() && mapon < 8) || (SPEAR() && mapon != 4 && mapon != 9 && mapon != 15 && mapon < 17))
    {
        // IOANCH 20130301: unification culling
#ifdef SPANISH
        Write (14, 2, "piso\ncompletado");
#else
        Write (14, 2, "floor\ncompleted");
#endif

        Write (14, 7, STR_BONUS "     0");
        Write (16, 10, STR_TIME);
        Write (16, 12, STR_PAR);

#ifdef SPANISH
        Write (11, 14, STR_RAT2KILL);
        Write (11, 16, STR_RAT2SECRET);
        Write (11, 18, STR_RAT2TREASURE);
#else
        Write (9, 14, STR_RAT2KILL);
        Write (5, 16, STR_RAT2SECRET);
        Write (1, 18, STR_RAT2TREASURE);
#endif

        Write (26, 2, itoa (gamestate.mapon + 1, tempstr, 10));


#ifdef SPANISH
        Write (30, 12, parTimes[gamestate.episode * 10 + mapon].timestr);
#else
        Write (26, 12, parTimes[gamestate.episode * 10 + mapon].timestr);
#endif

        //
        // PRINT TIME
        //
        sec = gamestate.TimeCount / 70;

        if (sec > 99 * 60)      // 99 minutes max
            sec = 99 * 60;

        if (gamestate.TimeCount < parTimes[gamestate.episode * 10 + mapon].time * 4200)
            timeleft = (int32_t) ((parTimes[gamestate.episode * 10 + mapon].time * 4200) / 70 - sec);

        min = sec / 60;
        sec %= 60;

#ifdef SPANISH
        i = 30 * 8;
#else
        i = 26 * 8;
#endif
        VWB_DrawPic (i, 10 * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (min / 10));
        i += 2 * 8;
        VWB_DrawPic (i, 10 * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (min % 10));
        i += 2 * 8;
        Write (i / 8, 10, ":");
        i += 1 * 8;
        VWB_DrawPic (i, 10 * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (sec / 10));
        i += 2 * 8;
        VWB_DrawPic (i, 10 * 8, gfxvmap[L_NUM0PIC][SPEAR()] + (sec % 10));

        VW_UpdateScreen ();
        VW_FadeIn ();


        //
        // FIGURE RATIOS OUT BEFOREHAND
        //
        kr = sr = tr = 0;
        if (gamestate.killtotal)
            kr = (gamestate.killcount * 100) / gamestate.killtotal;
        if (gamestate.secrettotal)
            sr = (gamestate.secretcount * 100) / gamestate.secrettotal;
        if (gamestate.treasuretotal)
            tr = (gamestate.treasurecount * 100) / gamestate.treasuretotal;


        //
        // PRINT TIME BONUS
        //
        bonus = timeleft * PAR_AMOUNT;
        if (bonus)
        {
            for (i = 0; i <= timeleft; i++)
            {
                ltoanoreturn ((long int)((int32_t) i * PAR_AMOUNT), tempstr, 10);
                x = 36 - (int) strlen(tempstr) * 2;
                Write (x, 7, tempstr);
                if (!(i % (PAR_AMOUNT / 10)))
                    SD_PlaySound (ENDBONUS1SND);
                if(!usedoublebuffering || !(i % (PAR_AMOUNT / 50))) VW_UpdateScreen ();
                while(SD_SoundPlaying ())
                    BJ_Breathe ();
                if (IN_CheckAck ())
                    goto done;
            }

            VW_UpdateScreen ();

            SD_PlaySound (ENDBONUS2SND);
            while (SD_SoundPlaying ())
                BJ_Breathe ();
        }


#ifdef SPANISH
#define RATIOXX                33
#else
#define RATIOXX                37
#endif
        //
        // KILL RATIO
        //
        ratio = kr;
        for (i = 0; i <= ratio; i++)
        {
            itoanoreturn (i, tempstr, 10);
            x = RATIOXX - (int) strlen(tempstr) * 2;
            Write (x, 14, tempstr);
            if (!(i % 10))
                SD_PlaySound (ENDBONUS1SND);
            if(!usedoublebuffering || !(i & 1)) VW_UpdateScreen ();
            while (SD_SoundPlaying ())
                BJ_Breathe ();

            if (IN_CheckAck ())
                goto done;
        }
        if (ratio >= 100)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            bonus += PERCENT100AMT;
            ltoanoreturn ((long int)bonus, tempstr, 10);
            x = (RATIOXX - 1) - (int) strlen(tempstr) * 2;
            Write (x, 7, tempstr);
            VW_UpdateScreen ();
            SD_PlaySound (PERCENT100SND);
        }
        else if (!ratio)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            SD_PlaySound (NOBONUSSND);
        }
        else
            SD_PlaySound (ENDBONUS2SND);

        VW_UpdateScreen ();
        while (SD_SoundPlaying ())
            BJ_Breathe ();

        //
        // SECRET RATIO
        //
        ratio = sr;
        for (i = 0; i <= ratio; i++)
        {
            itoanoreturn (i, tempstr, 10);
            x = RATIOXX - (int) strlen(tempstr) * 2;
            Write (x, 16, tempstr);
            if (!(i % 10))
                SD_PlaySound (ENDBONUS1SND);
            if(!usedoublebuffering || !(i & 1)) VW_UpdateScreen ();
            while (SD_SoundPlaying ())
                BJ_Breathe ();

            if (IN_CheckAck ())
                goto done;
        }
        if (ratio >= 100)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            bonus += PERCENT100AMT;
            ltoanoreturn ((long int)bonus, tempstr, 10);
            x = (RATIOXX - 1) - (int) strlen(tempstr) * 2;
            Write (x, 7, tempstr);
            VW_UpdateScreen ();
            SD_PlaySound (PERCENT100SND);
        }
        else if (!ratio)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            SD_PlaySound (NOBONUSSND);
        }
        else
            SD_PlaySound (ENDBONUS2SND);
        VW_UpdateScreen ();
        while (SD_SoundPlaying ())
            BJ_Breathe ();

        //
        // TREASURE RATIO
        //
        ratio = tr;
        for (i = 0; i <= ratio; i++)
        {
            itoanoreturn (i, tempstr, 10);
            x = RATIOXX - (int) strlen(tempstr) * 2;
            Write (x, 18, tempstr);
            if (!(i % 10))
                SD_PlaySound (ENDBONUS1SND);
            if(!usedoublebuffering || !(i & 1)) VW_UpdateScreen ();
            while (SD_SoundPlaying ())
                BJ_Breathe ();
            if (IN_CheckAck ())
                goto done;
        }
        if (ratio >= 100)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            bonus += PERCENT100AMT;
            ltoanoreturn ((long int)bonus, tempstr, 10);
            x = (RATIOXX - 1) - (int) strlen(tempstr) * 2;
            Write (x, 7, tempstr);
            VW_UpdateScreen ();
            SD_PlaySound (PERCENT100SND);
        }
        else if (!ratio)
        {
            VW_WaitVBL (VBLWAIT);
            SD_StopSound ();
            SD_PlaySound (NOBONUSSND);
        }
        else
            SD_PlaySound (ENDBONUS2SND);
        VW_UpdateScreen ();
        while (SD_SoundPlaying ())
            BJ_Breathe ();


        //
        // JUMP STRAIGHT HERE IF KEY PRESSED
        //
done:   itoanoreturn (kr, tempstr, 10);
        x = RATIOXX - (int) strlen(tempstr) * 2;
        Write (x, 14, tempstr);

        itoanoreturn (sr, tempstr, 10);
        x = RATIOXX - (int) strlen(tempstr) * 2;
        Write (x, 16, tempstr);

        itoanoreturn (tr, tempstr, 10);
        x = RATIOXX - (int) strlen(tempstr) * 2;
        Write (x, 18, tempstr);

        bonus = (int32_t) timeleft *PAR_AMOUNT +
            (PERCENT100AMT * (kr >= 100)) +
            (PERCENT100AMT * (sr >= 100)) + (PERCENT100AMT * (tr >= 100));

        GivePoints (bonus);
        ltoanoreturn ((long int)bonus, tempstr, 10);
        x = 36 - (int) strlen(tempstr) * 2;
        Write (x, 7, tempstr);

        //
        // SAVE RATIO INFORMATION FOR ENDGAME
        //
        LevelRatios[mapon].kill = kr;
        LevelRatios[mapon].secret = sr;
        LevelRatios[mapon].treasure = tr;
        LevelRatios[mapon].time = min * 60 + sec;
    }
    else
    {
// IOANCH 20130301: unification culling
        if(SPEAR()) 
        {
            switch (mapon)
            {
                case 4:
                    Write (14, 4, " trans\n" " grosse\n" STR_DEFEATED);
                    break;
                case 9:
                    Write (14, 4, "barnacle\n" "wilhelm\n" STR_DEFEATED);
                    break;
                case 15:
                    Write (14, 4, "ubermutant\n" STR_DEFEATED);
                    break;
                case 17:
                    Write (14, 4, " death\n" " knight\n" STR_DEFEATED);
                    break;
                case 18:
                    Write (13, 4, "secret tunnel\n" "    area\n" "  completed!");
                    break;
                case 19:
                    Write (13, 4, "secret castle\n" "    area\n" "  completed!");
                    break;
            }
        }
        else
            Write (14, 4, "secret floor\n completed!");

        Write (10, 16, "15000 bonus!");

        VW_UpdateScreen ();
        VW_FadeIn ();

        GivePoints (15000);
    }


    DrawScore ();
    VW_UpdateScreen ();

    lastBreathTime = GetTimeCount();
    IN_StartAck ();

	 // IOANCH 27.05.2012: let bots automatically hit after 3 seconds
	 int botcount = 0;
    while (!IN_CheckAck ())
	 {
        BJ_Breathe ();
		  if(BotMan::active)
		  {
			  if(++botcount == 600)
				  break;
		  }
	 }

//
// done
//
    // IOANCH 20130301: unification culling



    VW_FadeOut ();
    DrawPlayBorder();

    UnCacheLump (gfxvmap[LEVELEND_LUMP_START][SPEAR()], gfxvmap[LEVELEND_LUMP_END][SPEAR()]);
}



//==========================================================================


/*
=================
=
= PreloadGraphics
=
= Fill the cache up
=
=================
*/

boolean
PreloadUpdate (unsigned current, unsigned total)
{
    unsigned w = WindowW - scaleFactor * 10;

    VWB_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
        w, scaleFactor * 2, BLACK);
    w = ((int32_t) w * current) / total;
    if (w)
    {
        VWB_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
            w, scaleFactor * 2, 0x37);       //SECONDCOLOR);
        VWB_BarScaledCoord (WindowX + scaleFactor * 5, WindowY + WindowH - scaleFactor * 3,
            w - scaleFactor * 1, scaleFactor * 1, 0x32);

    }
    VW_UpdateScreen ();
//      if (LastScan == sc_Escape)
//      {
//              IN_ClearKeysDown();
//              return(true);
//      }
//      else
    return (false);
}

void
PreloadGraphics (void)
{
    DrawLevel ();
    ClearSplitVWB ();           // set up for double buffering in split screen

    VWB_BarScaledCoord (0, 0, screenWidth, screenHeight - scaleFactor * (STATUSLINES - 1), bordercol);
    LatchDrawPicScaledCoord ((screenWidth-scaleFactor*224)/16,
        (screenHeight-scaleFactor*(STATUSLINES+48))/2, gfxvmap[GETPSYCHEDPIC][SPEAR()]);

    WindowX = (screenWidth - scaleFactor*224)/2;
    WindowY = (screenHeight - scaleFactor*(STATUSLINES+48))/2;
    WindowW = scaleFactor * 28 * 8;
    WindowH = scaleFactor * 48;

    VW_UpdateScreen ();
    VW_FadeIn ();

//      PM_Preload (PreloadUpdate);
    PreloadUpdate (10, 10);
    IN_UserInput (70);
    VW_FadeOut ();

    DrawPlayBorder ();
    VW_UpdateScreen ();
}


//==========================================================================

/*
==================
=
= DrawHighScores
=
==================
*/

void
DrawHighScores (void)
{
    char buffer[16];

    char *str;
    // IOANCH 20130301: unification culling

    char buffer1[5];


    word i, w, h;
    HighScore *s;

    if(!SPEAR())
    {
        CA_CacheGrChunk (gfxvmap[HIGHSCORESPIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[STARTFONT][SPEAR()]);
        // IOANCH 20130301: unification culling

        CA_CacheGrChunk (gfxvmap[C_LEVELPIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[C_SCOREPIC][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[C_NAMEPIC][SPEAR()]);




        ClearMScreen ();
        DrawStripes (10);

        VWB_DrawPic (48, 0, gfxvmap[HIGHSCORESPIC][SPEAR()]);
        UNCACHEGRCHUNK (gfxvmap[HIGHSCORESPIC][SPEAR()]);

        // IOANCH 20130301: unification culling

        VWB_DrawPic (4 * 8, 68, gfxvmap[C_NAMEPIC][SPEAR()]);
        VWB_DrawPic (20 * 8, 68, gfxvmap[C_LEVELPIC][SPEAR()]);
        VWB_DrawPic (28 * 8, 68, gfxvmap[C_SCOREPIC][SPEAR()]);



        fontnumber = 0;
    }
    else
    {
        CacheLump (gfxvmap[BACKDROP_LUMP_START][SPEAR()], gfxvmap[BACKDROP_LUMP_END][SPEAR()]);
        ClearMScreen ();
        DrawStripes (10);
        UnCacheLump (gfxvmap[BACKDROP_LUMP_START][SPEAR()], gfxvmap[BACKDROP_LUMP_END][SPEAR()]);

        CacheLump (gfxvmap[HIGHSCORES_LUMP_START][SPEAR()], gfxvmap[HIGHSCORES_LUMP_END][SPEAR()]);
        CA_CacheGrChunk (gfxvmap[STARTFONT][SPEAR()] + 1);
        VWB_DrawPic (0, 0, gfxvmap[HIGHSCORESPIC][SPEAR()]);

        fontnumber = 1;
    }

    if(!SPEAR())
    {
        SETFONTCOLOR (15, 0x29);
    }
    else
    {
        SETFONTCOLOR (HIGHLIGHT, 0x29);
    }

    for (i = 0, s = Scores; i < MaxScores; i++, s++)
    {
        PrintY = 76 + (16 * i);

        //
        // name
        //
        if(!SPEAR())
            PrintX = 4 * 8;
        else
            PrintX = 16;

        US_Print (s->name);

        //
        // level
        //
        itoanoreturn (s->completed, buffer, 10);
        // IOANCH 20130303
        if(!SPEAR())
        {
            for (str = buffer; *str; str++)
                *str = *str + (129 - '0');  // Used fixed-width numbers (129...)
            USL_MeasureString (buffer, &w, &h);
            PrintX = (22 * 8) - w;
        }
        else
        {
            USL_MeasureString (buffer, &w, &h);
            PrintX = 194 - w;
        }
// IOANCH 20130301: unification culling

        if(!SPEAR())
        {
            PrintX -= 6;
            itoanoreturn (s->episode + 1, buffer1, 10);
            US_Print ("E");
            US_Print (buffer1);
            US_Print ("/L");
        }

        if(SPEAR())
        {
            if (s->completed == 21)
                VWB_DrawPic (PrintX + 8, PrintY - 1, gfxvmap[C_WONSPEARPIC][SPEAR()]);
            else
                US_Print (buffer);
        }
        else
            US_Print (buffer);

        //
        // score
        //
        itoanoreturn (s->score, buffer, 10);
        if(!SPEAR())
        {
            for (str = buffer; *str; str++)
                *str = *str + (129 - '0');  // Used fixed-width numbers (129...)
            USL_MeasureString (buffer, &w, &h);
            PrintX = (34 * 8) - 8 - w;
        }
        else
        {
            USL_MeasureString (buffer, &w, &h);
            PrintX = 292 - w;
        }
        US_Print (buffer);

        // IOANCH 20130301: unification culling

    }

    VW_UpdateScreen ();

    if(SPEAR())
    {
        UnCacheLump (gfxvmap[HIGHSCORES_LUMP_START][SPEAR()], gfxvmap[HIGHSCORES_LUMP_END][SPEAR()]);
        fontnumber = 0;
    }
}

//===========================================================================


/*
=======================
=
= CheckHighScore
=
=======================
*/

void
CheckHighScore (int32_t score, word other)
{
    word i, j;
    int n;
    HighScore myscore;

    strcpy (myscore.name, "");
    myscore.score = score;
    myscore.episode = gamestate.episode;
    myscore.completed = other;

    for (i = 0, n = -1; i < MaxScores; i++)
    {
        if ((myscore.score > Scores[i].score)
            || ((myscore.score == Scores[i].score) && (myscore.completed > Scores[i].completed)))
        {
            for (j = MaxScores; --j > i;)
                Scores[j] = Scores[j - 1];
            Scores[i] = myscore;
            n = i;
            break;
        }
    }
            // IOANCH 20130301: unification music
    if (SPEAR())
        StartCPMusic (XAWARD_MUS_sod);
    else
        StartCPMusic (ROSTER_MUS_wl6);

    DrawHighScores ();

    VW_FadeIn ();

    if (n != -1)
    {
        //
        // got a high score
        //
        PrintY = 76 + (16 * n);

        if(!SPEAR())
        {
            PrintX = 4 * 8;
            backcolor = BORDCOLOR;
            fontcolor = 15;
		      // IOANCH 27.05.2012: let the bot write his random name
		      if(BotMan::active)
		      {
			      Basic::MarkovWrite(Scores[n].name, 10);	// maximum 10 chars
			      US_Print(Scores[n].name);
			      VW_UpdateScreen();

			      IN_ClearKeysDown ();
			      IN_UserInput (500);
		      }
		      else
			    US_LineInput (PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 100);
        }
        else
        {

            PrintX = 16;
            fontnumber = 1;
            VWB_Bar (PrintX - 2, PrintY - 2, 145, 15, 0x9c);
            VW_UpdateScreen ();
            backcolor = 0x9c;
            fontcolor = 15;
		      // IOANCH 27.05.2012: let the bot write his random name
		      if(BotMan::active)
		      {
			      Basic::MarkovWrite(Scores[n].name, 10);	// maximum 10 chars
			      US_Print(Scores[n].name);
			      VW_UpdateScreen();

			      IN_ClearKeysDown ();
			      IN_UserInput (500);
		      }
		      else
			    US_LineInput (PrintX, PrintY, Scores[n].name, 0, true, MaxHighName, 130);
        }

    }
    else
    {
        IN_ClearKeysDown ();
        IN_UserInput (500);
    }

}

// IOANCH 20130301: unification culling



////////////////////////////////////////////////////////
//
// NON-SHAREWARE NOTICE
//
////////////////////////////////////////////////////////
void
NonShareware (void)
{
    VW_FadeOut ();

    ClearMScreen ();
    DrawStripes (10);

    CA_CacheGrChunk (gfxvmap[STARTFONT][SPEAR()] + 1);
    fontnumber = 1;

    SETFONTCOLOR (READHCOLOR, BKGDCOLOR);
    PrintX = 110;
    PrintY = 15;

#ifdef SPANISH
    US_Print ("Atencion");
#else
    US_Print ("Attention");
#endif

    SETFONTCOLOR (HIGHLIGHT, BKGDCOLOR);
    WindowX = PrintX = 40;
    PrintY = 60;
#ifdef SPANISH
    US_Print ("Este juego NO es gratis y\n");
    US_Print ("NO es Shareware; favor de\n");
    US_Print ("no distribuirlo.\n\n");
#else
    US_Print ("This game is NOT shareware.\n");
    US_Print ("Please do not distribute it.\n");
    US_Print ("Thanks.\n\n");
#endif
    US_Print ("        Id Software\n");

    VW_UpdateScreen ();
    VW_FadeIn ();
    IN_Ack ();
}



// IOANCH 20130301: unification culling
//===========================================================================