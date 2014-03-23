//
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
////////////////////////////////////////////////////////////////////////////////
//
// Master flags for Spear of Destiny and associated similars
//
////////////////////////////////////////////////////////////////////////////////

#include "wl_def.h"
#include "FileSystem.h"
#include "i_system.h"
#include "SODFlag.h"
#include "wl_menu.h"

const char SPEAR::wolftitle[] = "Automatic Wolfenstein 3D";
const char SPEAR::speartitle[] = "Automatic Spear of Destiny";
Boolean8 SPEAR::flag;

//
// SODFlag::gfxvmap
//
// Map for interface graphics
//

const unsigned int SPEAR::gfxvmap[][2] =
{
    {3,                     BJCOLLAPSE1PIC_sod},
    {3,                     BJCOLLAPSE2PIC_sod},
    {3,                     BJCOLLAPSE3PIC_sod},
    {3,                     BJCOLLAPSE4PIC_sod},
    {3,                     BJOUCHPIC_sod},
    {3,                     BJWAITING1PIC_sod},
    {3,                     BJWAITING2PIC_sod},
    {3,                     BOSSPIC1PIC_sod},
    {3,                     BOSSPIC2PIC_sod},
    {3,                     BOSSPIC3PIC_sod},
    {3,                     BOSSPIC4PIC_sod},
    {C_BABYMODEPIC_wl6,     C_BABYMODEPIC_sod},
    {3,                     C_BACKDROPPIC_sod},
    {C_CODEPIC_wl6,         3},
    {C_CONTROLPIC_wl6,      C_CONTROLPIC_sod},
    {C_CURSOR1PIC_wl6,      C_CURSOR1PIC_sod},
    {C_CURSOR2PIC_wl6,      C_CURSOR2PIC_sod},
    {C_CUSTOMIZEPIC_wl6,    C_CUSTOMIZEPIC_sod},
    {C_DIGITITLEPIC_wl6,    C_DIGITITLEPIC_sod},
    {C_DISKLOADING1PIC_wl6, C_DISKLOADING1PIC_sod},
    {C_DISKLOADING2PIC_wl6, C_DISKLOADING2PIC_sod},
    {C_EASYPIC_wl6,         C_EASYPIC_sod},
    {C_EPISODE1PIC_wl6,     3},
    {C_EPISODE2PIC_wl6,     3},
    {C_EPISODE3PIC_wl6,     3},
    {C_EPISODE4PIC_wl6,     3},
    {C_EPISODE5PIC_wl6,     3},
    {C_EPISODE6PIC_wl6,     3},
    {C_FXTITLEPIC_wl6,      C_FXTITLEPIC_sod},
    {C_HARDPIC_wl6,         C_HARDPIC_sod},
    {3,                     C_HOWTOUGHPIC_sod},
    {C_JOY1PIC_wl6,         C_JOY1PIC_sod},
    {C_JOY2PIC_wl6,         C_JOY2PIC_sod},
    {3,                     C_JOYSTICKPIC_sod},
    {3,                     C_KEYBOARDPIC_sod},
    {C_LEVELPIC_wl6,        3},
    {C_LOADGAMEPIC_wl6,     C_LOADGAMEPIC_sod},
    {C_LOADSAVEDISKPIC_wl6, 3},
    {C_MOUSELBACKPIC_wl6,   C_MOUSELBACKPIC_sod},
    {3,                     C_MOUSEPIC_sod},
    {C_MUSICTITLEPIC_wl6,   C_MUSICTITLEPIC_sod},
    {C_NAMEPIC_wl6,         3},
    {C_NORMALPIC_wl6,       C_NORMALPIC_sod},
    {C_NOTSELECTEDPIC_wl6,  C_NOTSELECTEDPIC_sod},
    {C_OPTIONSPIC_wl6,      C_OPTIONSPIC_sod},
    {C_SAVEGAMEPIC_wl6,     C_SAVEGAMEPIC_sod},
    {C_SCOREPIC_wl6,        3},
    {C_SELECTEDPIC_wl6,     C_SELECTEDPIC_sod},
    {C_TIMECODEPIC_wl6,     3},
    {3,                     C_WONSPEARPIC_sod},
    {3,                     COPYPROTBOXPIC_sod},
    {3,                     COPYPROTTOPPIC_sod},
    {CREDITSPIC_wl6,        CREDITSPIC_sod},
    {3,                     END1PALETTE_sod},
    {3,                     END2PALETTE_sod},
    {3,                     END3PALETTE_sod},
    {3,                     END4PALETTE_sod},
    {3,                     END5PALETTE_sod},
    {3,                     END6PALETTE_sod},
    {3,                     END7PALETTE_sod},
    {3,                     END8PALETTE_sod},
    {3,                     END9PALETTE_sod},
    {3,                     ENDPICPIC_sod},
    {3,                     ENDSCREEN11PIC_sod},
    {3,                     ENDSCREEN12PIC_sod},
    {3,                     ENDSCREEN3PIC_sod},
    {3,                     ENDSCREEN4PIC_sod},
    {3,                     ENDSCREEN5PIC_sod},
    {3,                     ENDSCREEN6PIC_sod},
    {3,                     ENDSCREEN7PIC_sod},
    {3,                     ENDSCREEN8PIC_sod},
    {3,                     ENDSCREEN9PIC_sod},
    {ERRORSCREEN_wl6,       ERRORSCREEN_sod},
    {FACE1APIC_wl6,         FACE1APIC_sod},
    {FACE1BPIC_wl6,         FACE1BPIC_sod},
    {FACE1CPIC_wl6,         FACE1CPIC_sod},
    {FACE2APIC_wl6,         FACE2APIC_sod},
    {FACE2BPIC_wl6,         FACE2BPIC_sod},
    {FACE2CPIC_wl6,         FACE2CPIC_sod},
    {FACE3APIC_wl6,         FACE3APIC_sod},
    {FACE3BPIC_wl6,         FACE3BPIC_sod},
    {FACE3CPIC_wl6,         FACE3CPIC_sod},
    {FACE4APIC_wl6,         FACE4APIC_sod},
    {FACE4BPIC_wl6,         FACE4BPIC_sod},
    {FACE4CPIC_wl6,         FACE4CPIC_sod},
    {FACE5APIC_wl6,         FACE5APIC_sod},
    {FACE5BPIC_wl6,         FACE5BPIC_sod},
    {FACE5CPIC_wl6,         FACE5CPIC_sod},
    {FACE6APIC_wl6,         FACE6APIC_sod},
    {FACE6BPIC_wl6,         FACE6BPIC_sod},
    {FACE6CPIC_wl6,         FACE6CPIC_sod},
    {FACE7APIC_wl6,         FACE7APIC_sod},
    {FACE7BPIC_wl6,         FACE7BPIC_sod},
    {FACE7CPIC_wl6,         FACE7CPIC_sod},
    {FACE8APIC_wl6,         FACE8APIC_sod},
    {GATLINGGUNPIC_wl6,     GATLINGGUNPIC_sod},
    {GETPSYCHEDPIC_wl6,     GETPSYCHEDPIC_sod},
    {3,                     GODMODEFACE1PIC_sod},
    {3,                     GODMODEFACE2PIC_sod},
    {3,                     GODMODEFACE3PIC_sod},
    {GOLDKEYPIC_wl6,        GOLDKEYPIC_sod},
    {GOTGATLINGPIC_wl6,     GOTGATLINGPIC_sod},
    {GUNPIC_wl6,            GUNPIC_sod},
    {H_BJPIC_wl6,           3},
    {H_BLAZEPIC_wl6,        3},
    {H_BOTTOMINFOPIC_wl6,   3},
    {H_CASTLEPIC_wl6,       3},
    {H_LEFTWINDOWPIC_wl6,   3},
    {H_RIGHTWINDOWPIC_wl6,  3},
    {H_TOPWINDOWPIC_wl6,    3},
    {HIGHSCORESPIC_wl6,     HIGHSCORESPIC_sod},
    {3,                     IDGUYS1PIC_sod},
    {3,                     IDGUYS2PIC_sod},
    {3,                     IDGUYSPALETTE_sod},
    {KNIFEPIC_wl6,          KNIFEPIC_sod},
    {L_APIC_wl6,            L_APIC_sod},
    {L_APOSTROPHEPIC_wl6,   L_APOSTROPHEPIC_sod},
    {L_BJWINSPIC_wl6,       L_BJWINSPIC_sod},
    {L_BPIC_wl6,            L_BPIC_sod},
    {L_COLONPIC_wl6,        L_COLONPIC_sod},
    {L_CPIC_wl6,            L_CPIC_sod},
    {L_DPIC_wl6,            L_DPIC_sod},
    {L_EPIC_wl6,            L_EPIC_sod},
    {L_EXPOINTPIC_wl6,      L_EXPOINTPIC_sod},
    {L_FPIC_wl6,            L_FPIC_sod},
    {L_GPIC_wl6,            L_GPIC_sod},
    {L_GUY2PIC_wl6,         L_GUY2PIC_sod},
    {L_GUYPIC_wl6,          L_GUYPIC_sod},
    {L_HPIC_wl6,            L_HPIC_sod},
    {L_IPIC_wl6,            L_IPIC_sod},
    {L_JPIC_wl6,            L_JPIC_sod},
    {L_KPIC_wl6,            L_KPIC_sod},
    {L_LPIC_wl6,            L_LPIC_sod},
    {L_MPIC_wl6,            L_MPIC_sod},
    {L_NPIC_wl6,            L_NPIC_sod},
    {L_NUM0PIC_wl6,         L_NUM0PIC_sod},
    {L_NUM1PIC_wl6,         L_NUM1PIC_sod},
    {L_NUM2PIC_wl6,         L_NUM2PIC_sod},
    {L_NUM3PIC_wl6,         L_NUM3PIC_sod},
    {L_NUM4PIC_wl6,         L_NUM4PIC_sod},
    {L_NUM5PIC_wl6,         L_NUM5PIC_sod},
    {L_NUM6PIC_wl6,         L_NUM6PIC_sod},
    {L_NUM7PIC_wl6,         L_NUM7PIC_sod},
    {L_NUM8PIC_wl6,         L_NUM8PIC_sod},
    {L_NUM9PIC_wl6,         L_NUM9PIC_sod},
    {L_OPIC_wl6,            L_OPIC_sod},
    {L_PERCENTPIC_wl6,      L_PERCENTPIC_sod},
    {L_PPIC_wl6,            L_PPIC_sod},
    {L_QPIC_wl6,            L_QPIC_sod},
    {L_RPIC_wl6,            L_RPIC_sod},
    {L_SPIC_wl6,            L_SPIC_sod},
    {L_TPIC_wl6,            L_TPIC_sod},
    {L_UPIC_wl6,            L_UPIC_sod},
    {L_VPIC_wl6,            L_VPIC_sod},
    {L_WPIC_wl6,            L_WPIC_sod},
    {L_XPIC_wl6,            L_XPIC_sod},
    {L_YPIC_wl6,            L_YPIC_sod},
    {L_ZPIC_wl6,            L_ZPIC_sod},
    {MACHINEGUNPIC_wl6,     MACHINEGUNPIC_sod},
    {MUTANTBJPIC_wl6,       3},
    {N_0PIC_wl6,            N_0PIC_sod},
    {N_1PIC_wl6,            N_1PIC_sod},
    {N_2PIC_wl6,            N_2PIC_sod},
    {N_3PIC_wl6,            N_3PIC_sod},
    {N_4PIC_wl6,            N_4PIC_sod},
    {N_5PIC_wl6,            N_5PIC_sod},
    {N_6PIC_wl6,            N_6PIC_sod},
    {N_7PIC_wl6,            N_7PIC_sod},
    {N_8PIC_wl6,            N_8PIC_sod},
    {N_9PIC_wl6,            N_9PIC_sod},
    {N_BLANKPIC_wl6,        N_BLANKPIC_sod},
    {NOKEYPIC_wl6,          NOKEYPIC_sod},
    {ORDERSCREEN_wl6,       ORDERSCREEN_sod},
    {PAUSEDPIC_wl6,         PAUSEDPIC_sod},
    {PG13PIC_wl6,           PG13PIC_sod},
    {SILVERKEYPIC_wl6,      SILVERKEYPIC_sod},
    {STATUSBARPIC_wl6,      STATUSBARPIC_sod},
    {T_DEMO0_wl6,           T_DEMO0_sod},
    {T_DEMO1_wl6,           T_DEMO1_sod},
    {T_DEMO2_wl6,           T_DEMO2_sod},
    {T_DEMO3_wl6,           T_DEMO3_sod},
    {T_ENDART1_wl6,         T_ENDART1_sod},
    {T_ENDART2_wl6,         3},
    {T_ENDART3_wl6,         3},
    {T_ENDART4_wl6,         3},
    {T_ENDART5_wl6,         3},
    {T_ENDART6_wl6,         3},
    {T_HELPART_wl6,         3},
    {3,                     TILE8_sod},
    {3,                     TITLE1PIC_sod},
    {3,                     TITLE2PIC_sod},
    {3,                     TITLEPALETTE_sod},
    {TITLEPIC_wl6,          3},
    
    {3,BACKDROP_LUMP_END_sod},
    {3,BACKDROP_LUMP_START_sod},
    {3,CONTROL_LUMP_END_sod},
    {3,CONTROL_LUMP_START_sod},
    {CONTROLS_LUMP_END_wl6,3},
    {CONTROLS_LUMP_START_wl6,3},
    {3,COPYPROT_LUMP_END_sod},
    {3,COPYPROT_LUMP_START_sod},
    {3,EASTEREGG_LUMP_END_sod},
    {3,EASTEREGG_LUMP_START_sod},
    {3,ENDGAME1_LUMP_END_sod},
    {3,ENDGAME1_LUMP_START_sod},
    {3,ENDGAME2_LUMP_END_sod},
    {3,ENDGAME2_LUMP_START_sod},
    {3,ENDGAME_LUMP_END_sod},
    {3,ENDGAME_LUMP_START_sod},
    {3,HIGHSCORES_LUMP_END_sod},
    {3,HIGHSCORES_LUMP_START_sod},
    {LATCHPICS_LUMP_END_wl6,LATCHPICS_LUMP_END_sod},
    {LATCHPICS_LUMP_START_wl6,LATCHPICS_LUMP_START_sod},
    {LEVELEND_LUMP_END_wl6,LEVELEND_LUMP_END_sod},
    {LEVELEND_LUMP_START_wl6,LEVELEND_LUMP_START_sod},
    {3,LOADSAVE_LUMP_END_sod},
    {3,LOADSAVE_LUMP_START_sod},
    {3,NEWGAME_LUMP_END_sod},
    {3,NEWGAME_LUMP_START_sod},
    {NUMCHUNKS_wl6,NUMCHUNKS_sod},
    {NUMEXTERNS_wl6,NUMEXTERNS_sod},
    {NUMFONT_wl6,NUMFONT_sod},
    {NUMFONTM_wl6,NUMFONTM_sod},
    {NUMPICM_wl6,NUMPICM_sod},
    {NUMPICS_wl6,NUMPICS_sod},
    {NUMSPRITES_wl6,NUMSPRITES_sod},
    {NUMTILE16_wl6,NUMTILE16_sod},
    {NUMTILE16M_wl6,NUMTILE16M_sod},
    {NUMTILE32_wl6,NUMTILE32_sod},
    {NUMTILE32M_wl6,NUMTILE32M_sod},
    {NUMTILE8_wl6,NUMTILE8_sod},
    {NUMTILE8M_wl6,NUMTILE8M_sod},
    {3,OPTIONS_LUMP_END_sod},
    {3,OPTIONS_LUMP_START_sod},
    {README_LUMP_END_wl6,3},
    {README_LUMP_START_wl6,3},
    {3,SOUND_LUMP_END_sod},
    {3,SOUND_LUMP_START_sod},
    {STARTEXTERNS_wl6,STARTEXTERNS_sod},
    {STARTFONT_wl6,STARTFONT_sod},
    {STARTFONTM_wl6,STARTFONTM_sod},
    {STARTPICM_wl6,STARTPICM_sod},
    {STARTPICS_wl6,STARTPICS_sod},
    {STARTSPRITES_wl6,STARTSPRITES_sod},
    {STARTTILE16_wl6,STARTTILE16_sod},
    {STARTTILE16M_wl6,STARTTILE16M_sod},
    {STARTTILE32_wl6,STARTTILE32_sod},
    {STARTTILE32M_wl6,STARTTILE32M_sod},
    {STARTTILE8_wl6,STARTTILE8_sod},
    {STARTTILE8M_wl6,STARTTILE8M_sod},
    {STRUCTPIC_wl6,STRUCTPIC_sod},
    {3,TITLESCREEN_LUMP_END_sod},
    {3,TITLESCREEN_LUMP_START_sod},
};

//
// SODFlag::sprmap
//
// Map for sprites
//

const unsigned int SPEAR::sprmap[][2] =
{
    {MACHINEGUNATK3_wl6, MACHINEGUNATK3_sod},
    {0, SPR_ANGEL_DEAD_sod},
    {0, SPR_ANGEL_DIE1_sod},
    {0, SPR_ANGEL_DIE2_sod},
    {0, SPR_ANGEL_DIE3_sod},
    {0, SPR_ANGEL_DIE4_sod},
    {0, SPR_ANGEL_DIE5_sod},
    {0, SPR_ANGEL_DIE6_sod},
    {0, SPR_ANGEL_DIE7_sod},
    {0, SPR_ANGEL_SHOOT1_sod},
    {0, SPR_ANGEL_SHOOT2_sod},
    {0, SPR_ANGEL_TIRED1_sod},
    {0, SPR_ANGEL_TIRED2_sod},
    {0, SPR_ANGEL_W1_sod},
    {0, SPR_ANGEL_W2_sod},
    {0, SPR_ANGEL_W3_sod},
    {0, SPR_ANGEL_W4_sod},
    {SPR_BJ_JUMP1_wl6, 0},
    {SPR_BJ_JUMP2_wl6, 0},
    {SPR_BJ_JUMP3_wl6, 0},
    {SPR_BJ_JUMP4_wl6, 0},
    {SPR_BJ_W1_wl6, 0},
    {SPR_BJ_W2_wl6, 0},
    {SPR_BJ_W3_wl6, 0},
    {SPR_BJ_W4_wl6, 0},
    {SPR_BLINKY_W1_wl6, 0},
    {SPR_BLINKY_W2_wl6, 0},
    {SPR_BOOM_1_wl6, SPR_BOOM_1_sod},
    {SPR_BOOM_2_wl6, SPR_BOOM_2_sod},
    {SPR_BOOM_3_wl6, SPR_BOOM_3_sod},
    {SPR_BOSS_DEAD_wl6, 0},
    {SPR_BOSS_DIE1_wl6, 0},
    {SPR_BOSS_DIE2_wl6, 0},
    {SPR_BOSS_DIE3_wl6, 0},
    {SPR_BOSS_SHOOT1_wl6, 0},
    {SPR_BOSS_SHOOT2_wl6, 0},
    {SPR_BOSS_SHOOT3_wl6, 0},
    {SPR_BOSS_W1_wl6, 0},
    {SPR_BOSS_W2_wl6, 0},
    {SPR_BOSS_W3_wl6, 0},
    {SPR_BOSS_W4_wl6, 0},
    {SPR_CHAINATK1_wl6, SPR_CHAINATK1_sod},
    {SPR_CHAINATK2_wl6, SPR_CHAINATK2_sod},
    {SPR_CHAINATK3_wl6, SPR_CHAINATK3_sod},
    {SPR_CHAINATK4_wl6, SPR_CHAINATK4_sod},
    {SPR_CHAINREADY_wl6, SPR_CHAINREADY_sod},
    {SPR_CLYDE_W1_wl6, 0},
    {SPR_CLYDE_W2_wl6, 0},
    {0, SPR_DEATH_DEAD_sod},
    {0, SPR_DEATH_DIE1_sod},
    {0, SPR_DEATH_DIE2_sod},
    {0, SPR_DEATH_DIE3_sod},
    {0, SPR_DEATH_DIE4_sod},
    {0, SPR_DEATH_DIE5_sod},
    {0, SPR_DEATH_DIE6_sod},
    {0, SPR_DEATH_SHOOT1_sod},
    {0, SPR_DEATH_SHOOT2_sod},
    {0, SPR_DEATH_SHOOT3_sod},
    {0, SPR_DEATH_SHOOT4_sod},
    {0, SPR_DEATH_W1_sod},
    {0, SPR_DEATH_W2_sod},
    {0, SPR_DEATH_W3_sod},
    {0, SPR_DEATH_W4_sod},
    {SPR_DEATHCAM_wl6, SPR_DEATHCAM_sod},
    {SPR_DEMO_wl6, SPR_DEMO_sod},
    {SPR_DOG_DEAD_wl6, SPR_DOG_DEAD_sod},
    {SPR_DOG_DIE_1_wl6, SPR_DOG_DIE_1_sod},
    {SPR_DOG_DIE_2_wl6, SPR_DOG_DIE_2_sod},
    {SPR_DOG_DIE_3_wl6, SPR_DOG_DIE_3_sod},
    {SPR_DOG_JUMP1_wl6, SPR_DOG_JUMP1_sod},
    {SPR_DOG_JUMP2_wl6, SPR_DOG_JUMP2_sod},
    {SPR_DOG_JUMP3_wl6, SPR_DOG_JUMP3_sod},
    {SPR_DOG_W1_1_wl6, SPR_DOG_W1_1_sod},
    {SPR_DOG_W1_2_wl6, SPR_DOG_W1_2_sod},
    {SPR_DOG_W1_3_wl6, SPR_DOG_W1_3_sod},
    {SPR_DOG_W1_4_wl6, SPR_DOG_W1_4_sod},
    {SPR_DOG_W1_5_wl6, SPR_DOG_W1_5_sod},
    {SPR_DOG_W1_6_wl6, SPR_DOG_W1_6_sod},
    {SPR_DOG_W1_7_wl6, SPR_DOG_W1_7_sod},
    {SPR_DOG_W1_8_wl6, SPR_DOG_W1_8_sod},
    {SPR_DOG_W2_1_wl6, SPR_DOG_W2_1_sod},
    {SPR_DOG_W2_2_wl6, SPR_DOG_W2_2_sod},
    {SPR_DOG_W2_3_wl6, SPR_DOG_W2_3_sod},
    {SPR_DOG_W2_4_wl6, SPR_DOG_W2_4_sod},
    {SPR_DOG_W2_5_wl6, SPR_DOG_W2_5_sod},
    {SPR_DOG_W2_6_wl6, SPR_DOG_W2_6_sod},
    {SPR_DOG_W2_7_wl6, SPR_DOG_W2_7_sod},
    {SPR_DOG_W2_8_wl6, SPR_DOG_W2_8_sod},
    {SPR_DOG_W3_1_wl6, SPR_DOG_W3_1_sod},
    {SPR_DOG_W3_2_wl6, SPR_DOG_W3_2_sod},
    {SPR_DOG_W3_3_wl6, SPR_DOG_W3_3_sod},
    {SPR_DOG_W3_4_wl6, SPR_DOG_W3_4_sod},
    {SPR_DOG_W3_5_wl6, SPR_DOG_W3_5_sod},
    {SPR_DOG_W3_6_wl6, SPR_DOG_W3_6_sod},
    {SPR_DOG_W3_7_wl6, SPR_DOG_W3_7_sod},
    {SPR_DOG_W3_8_wl6, SPR_DOG_W3_8_sod},
    {SPR_DOG_W4_1_wl6, SPR_DOG_W4_1_sod},
    {SPR_DOG_W4_2_wl6, SPR_DOG_W4_2_sod},
    {SPR_DOG_W4_3_wl6, SPR_DOG_W4_3_sod},
    {SPR_DOG_W4_4_wl6, SPR_DOG_W4_4_sod},
    {SPR_DOG_W4_5_wl6, SPR_DOG_W4_5_sod},
    {SPR_DOG_W4_6_wl6, SPR_DOG_W4_6_sod},
    {SPR_DOG_W4_7_wl6, SPR_DOG_W4_7_sod},
    {SPR_DOG_W4_8_wl6, SPR_DOG_W4_8_sod},
    {SPR_FAKE_DEAD_wl6, 0},
    {SPR_FAKE_DIE1_wl6, 0},
    {SPR_FAKE_DIE2_wl6, 0},
    {SPR_FAKE_DIE3_wl6, 0},
    {SPR_FAKE_DIE4_wl6, 0},
    {SPR_FAKE_DIE5_wl6, 0},
    {SPR_FAKE_SHOOT_wl6, 0},
    {SPR_FAKE_W1_wl6, 0},
    {SPR_FAKE_W2_wl6, 0},
    {SPR_FAKE_W3_wl6, 0},
    {SPR_FAKE_W4_wl6, 0},
    {SPR_FAT_DEAD_wl6, 0},
    {SPR_FAT_DIE1_wl6, 0},
    {SPR_FAT_DIE2_wl6, 0},
    {SPR_FAT_DIE3_wl6, 0},
    {SPR_FAT_SHOOT1_wl6, 0},
    {SPR_FAT_SHOOT2_wl6, 0},
    {SPR_FAT_SHOOT3_wl6, 0},
    {SPR_FAT_SHOOT4_wl6, 0},
    {SPR_FAT_W1_wl6, 0},
    {SPR_FAT_W2_wl6, 0},
    {SPR_FAT_W3_wl6, 0},
    {SPR_FAT_W4_wl6, 0},
    {SPR_FIRE1_wl6, 0},
    {SPR_FIRE2_wl6, 0},
    {SPR_GIFT_DEAD_wl6, 0},
    {SPR_GIFT_DIE1_wl6, 0},
    {SPR_GIFT_DIE2_wl6, 0},
    {SPR_GIFT_DIE3_wl6, 0},
    {SPR_GIFT_SHOOT1_wl6, 0},
    {SPR_GIFT_SHOOT2_wl6, 0},
    {SPR_GIFT_W1_wl6, 0},
    {SPR_GIFT_W2_wl6, 0},
    {SPR_GIFT_W3_wl6, 0},
    {SPR_GIFT_W4_wl6, 0},
    {SPR_GRD_DEAD_wl6, SPR_GRD_DEAD_sod},
    {SPR_GRD_DIE_1_wl6, SPR_GRD_DIE_1_sod},
    {SPR_GRD_DIE_2_wl6, SPR_GRD_DIE_2_sod},
    {SPR_GRD_DIE_3_wl6, SPR_GRD_DIE_3_sod},
    {SPR_GRD_PAIN_1_wl6, SPR_GRD_PAIN_1_sod},
    {SPR_GRD_PAIN_2_wl6, SPR_GRD_PAIN_2_sod},
    {SPR_GRD_S_1_wl6, SPR_GRD_S_1_sod},
    {SPR_GRD_S_2_wl6, SPR_GRD_S_2_sod},
    {SPR_GRD_S_3_wl6, SPR_GRD_S_3_sod},
    {SPR_GRD_S_4_wl6, SPR_GRD_S_4_sod},
    {SPR_GRD_S_5_wl6, SPR_GRD_S_5_sod},
    {SPR_GRD_S_6_wl6, SPR_GRD_S_6_sod},
    {SPR_GRD_S_7_wl6, SPR_GRD_S_7_sod},
    {SPR_GRD_S_8_wl6, SPR_GRD_S_8_sod},
    {SPR_GRD_SHOOT1_wl6, SPR_GRD_SHOOT1_sod},
    {SPR_GRD_SHOOT2_wl6, SPR_GRD_SHOOT2_sod},
    {SPR_GRD_SHOOT3_wl6, SPR_GRD_SHOOT3_sod},
    {SPR_GRD_W1_1_wl6, SPR_GRD_W1_1_sod},
    {SPR_GRD_W1_2_wl6, SPR_GRD_W1_2_sod},
    {SPR_GRD_W1_3_wl6, SPR_GRD_W1_3_sod},
    {SPR_GRD_W1_4_wl6, SPR_GRD_W1_4_sod},
    {SPR_GRD_W1_5_wl6, SPR_GRD_W1_5_sod},
    {SPR_GRD_W1_6_wl6, SPR_GRD_W1_6_sod},
    {SPR_GRD_W1_7_wl6, SPR_GRD_W1_7_sod},
    {SPR_GRD_W1_8_wl6, SPR_GRD_W1_8_sod},
    {SPR_GRD_W2_1_wl6, SPR_GRD_W2_1_sod},
    {SPR_GRD_W2_2_wl6, SPR_GRD_W2_2_sod},
    {SPR_GRD_W2_3_wl6, SPR_GRD_W2_3_sod},
    {SPR_GRD_W2_4_wl6, SPR_GRD_W2_4_sod},
    {SPR_GRD_W2_5_wl6, SPR_GRD_W2_5_sod},
    {SPR_GRD_W2_6_wl6, SPR_GRD_W2_6_sod},
    {SPR_GRD_W2_7_wl6, SPR_GRD_W2_7_sod},
    {SPR_GRD_W2_8_wl6, SPR_GRD_W2_8_sod},
    {SPR_GRD_W3_1_wl6, SPR_GRD_W3_1_sod},
    {SPR_GRD_W3_2_wl6, SPR_GRD_W3_2_sod},
    {SPR_GRD_W3_3_wl6, SPR_GRD_W3_3_sod},
    {SPR_GRD_W3_4_wl6, SPR_GRD_W3_4_sod},
    {SPR_GRD_W3_5_wl6, SPR_GRD_W3_5_sod},
    {SPR_GRD_W3_6_wl6, SPR_GRD_W3_6_sod},
    {SPR_GRD_W3_7_wl6, SPR_GRD_W3_7_sod},
    {SPR_GRD_W3_8_wl6, SPR_GRD_W3_8_sod},
    {SPR_GRD_W4_1_wl6, SPR_GRD_W4_1_sod},
    {SPR_GRD_W4_2_wl6, SPR_GRD_W4_2_sod},
    {SPR_GRD_W4_3_wl6, SPR_GRD_W4_3_sod},
    {SPR_GRD_W4_4_wl6, SPR_GRD_W4_4_sod},
    {SPR_GRD_W4_5_wl6, SPR_GRD_W4_5_sod},
    {SPR_GRD_W4_6_wl6, SPR_GRD_W4_6_sod},
    {SPR_GRD_W4_7_wl6, SPR_GRD_W4_7_sod},
    {SPR_GRD_W4_8_wl6, SPR_GRD_W4_8_sod},
    {SPR_GRETEL_DEAD_wl6, 0},
    {SPR_GRETEL_DIE1_wl6, 0},
    {SPR_GRETEL_DIE2_wl6, 0},
    {SPR_GRETEL_DIE3_wl6, 0},
    {SPR_GRETEL_SHOOT1_wl6, 0},
    {SPR_GRETEL_SHOOT2_wl6, 0},
    {SPR_GRETEL_SHOOT3_wl6, 0},
    {SPR_GRETEL_W1_wl6, 0},
    {SPR_GRETEL_W2_wl6, 0},
    {SPR_GRETEL_W3_wl6, 0},
    {SPR_GRETEL_W4_wl6, 0},
    {0, SPR_HBOOM_1_sod},
    {0, SPR_HBOOM_2_sod},
    {0, SPR_HBOOM_3_sod},
    {SPR_HITLER_DEAD_wl6, 0},
    {SPR_HITLER_DIE1_wl6, 0},
    {SPR_HITLER_DIE2_wl6, 0},
    {SPR_HITLER_DIE3_wl6, 0},
    {SPR_HITLER_DIE4_wl6, 0},
    {SPR_HITLER_DIE5_wl6, 0},
    {SPR_HITLER_DIE6_wl6, 0},
    {SPR_HITLER_DIE7_wl6, 0},
    {SPR_HITLER_SHOOT1_wl6, 0},
    {SPR_HITLER_SHOOT2_wl6, 0},
    {SPR_HITLER_SHOOT3_wl6, 0},
    {SPR_HITLER_W1_wl6, 0},
    {SPR_HITLER_W2_wl6, 0},
    {SPR_HITLER_W3_wl6, 0},
    {SPR_HITLER_W4_wl6, 0},
    {0, SPR_HROCKET_1_sod},
    {0, SPR_HROCKET_2_sod},
    {0, SPR_HROCKET_3_sod},
    {0, SPR_HROCKET_4_sod},
    {0, SPR_HROCKET_5_sod},
    {0, SPR_HROCKET_6_sod},
    {0, SPR_HROCKET_7_sod},
    {0, SPR_HROCKET_8_sod},
    {0, SPR_HSMOKE_1_sod},
    {0, SPR_HSMOKE_2_sod},
    {0, SPR_HSMOKE_3_sod},
    {0, SPR_HSMOKE_4_sod},
    {SPR_HYPO1_wl6, 0},
    {SPR_HYPO2_wl6, 0},
    {SPR_HYPO3_wl6, 0},
    {SPR_HYPO4_wl6, 0},
    {SPR_INKY_W1_wl6, 0},
    {SPR_INKY_W2_wl6, 0},
    {SPR_KNIFEATK1_wl6, SPR_KNIFEATK1_sod},
    {SPR_KNIFEATK2_wl6, SPR_KNIFEATK2_sod},
    {SPR_KNIFEATK3_wl6, SPR_KNIFEATK3_sod},
    {SPR_KNIFEATK4_wl6, SPR_KNIFEATK4_sod},
    {SPR_KNIFEREADY_wl6, SPR_KNIFEREADY_sod},
    {SPR_MACHINEGUNATK1_wl6, SPR_MACHINEGUNATK1_sod},
    {SPR_MACHINEGUNATK2_wl6, SPR_MACHINEGUNATK2_sod},
    {SPR_MACHINEGUNATK4_wl6, SPR_MACHINEGUNATK4_sod},
    {SPR_MACHINEGUNREADY_wl6, SPR_MACHINEGUNREADY_sod},
    {SPR_MECHA_DEAD_wl6, 0},
    {SPR_MECHA_DIE1_wl6, 0},
    {SPR_MECHA_DIE2_wl6, 0},
    {SPR_MECHA_DIE3_wl6, 0},
    {SPR_MECHA_SHOOT1_wl6, 0},
    {SPR_MECHA_SHOOT2_wl6, 0},
    {SPR_MECHA_SHOOT3_wl6, 0},
    {SPR_MECHA_W1_wl6, 0},
    {SPR_MECHA_W2_wl6, 0},
    {SPR_MECHA_W3_wl6, 0},
    {SPR_MECHA_W4_wl6, 0},
    {SPR_MUT_DEAD_wl6, SPR_MUT_DEAD_sod},
    {SPR_MUT_DIE_1_wl6, SPR_MUT_DIE_1_sod},
    {SPR_MUT_DIE_2_wl6, SPR_MUT_DIE_2_sod},
    {SPR_MUT_DIE_3_wl6, SPR_MUT_DIE_3_sod},
    {SPR_MUT_DIE_4_wl6, SPR_MUT_DIE_4_sod},
    {SPR_MUT_PAIN_1_wl6, SPR_MUT_PAIN_1_sod},
    {SPR_MUT_PAIN_2_wl6, SPR_MUT_PAIN_2_sod},
    {SPR_MUT_S_1_wl6, SPR_MUT_S_1_sod},
    {SPR_MUT_S_2_wl6, SPR_MUT_S_2_sod},
    {SPR_MUT_S_3_wl6, SPR_MUT_S_3_sod},
    {SPR_MUT_S_4_wl6, SPR_MUT_S_4_sod},
    {SPR_MUT_S_5_wl6, SPR_MUT_S_5_sod},
    {SPR_MUT_S_6_wl6, SPR_MUT_S_6_sod},
    {SPR_MUT_S_7_wl6, SPR_MUT_S_7_sod},
    {SPR_MUT_S_8_wl6, SPR_MUT_S_8_sod},
    {SPR_MUT_SHOOT1_wl6, SPR_MUT_SHOOT1_sod},
    {SPR_MUT_SHOOT2_wl6, SPR_MUT_SHOOT2_sod},
    {SPR_MUT_SHOOT3_wl6, SPR_MUT_SHOOT3_sod},
    {SPR_MUT_SHOOT4_wl6, SPR_MUT_SHOOT4_sod},
    {SPR_MUT_W1_1_wl6, SPR_MUT_W1_1_sod},
    {SPR_MUT_W1_2_wl6, SPR_MUT_W1_2_sod},
    {SPR_MUT_W1_3_wl6, SPR_MUT_W1_3_sod},
    {SPR_MUT_W1_4_wl6, SPR_MUT_W1_4_sod},
    {SPR_MUT_W1_5_wl6, SPR_MUT_W1_5_sod},
    {SPR_MUT_W1_6_wl6, SPR_MUT_W1_6_sod},
    {SPR_MUT_W1_7_wl6, SPR_MUT_W1_7_sod},
    {SPR_MUT_W1_8_wl6, SPR_MUT_W1_8_sod},
    {SPR_MUT_W2_1_wl6, SPR_MUT_W2_1_sod},
    {SPR_MUT_W2_2_wl6, SPR_MUT_W2_2_sod},
    {SPR_MUT_W2_3_wl6, SPR_MUT_W2_3_sod},
    {SPR_MUT_W2_4_wl6, SPR_MUT_W2_4_sod},
    {SPR_MUT_W2_5_wl6, SPR_MUT_W2_5_sod},
    {SPR_MUT_W2_6_wl6, SPR_MUT_W2_6_sod},
    {SPR_MUT_W2_7_wl6, SPR_MUT_W2_7_sod},
    {SPR_MUT_W2_8_wl6, SPR_MUT_W2_8_sod},
    {SPR_MUT_W3_1_wl6, SPR_MUT_W3_1_sod},
    {SPR_MUT_W3_2_wl6, SPR_MUT_W3_2_sod},
    {SPR_MUT_W3_3_wl6, SPR_MUT_W3_3_sod},
    {SPR_MUT_W3_4_wl6, SPR_MUT_W3_4_sod},
    {SPR_MUT_W3_5_wl6, SPR_MUT_W3_5_sod},
    {SPR_MUT_W3_6_wl6, SPR_MUT_W3_6_sod},
    {SPR_MUT_W3_7_wl6, SPR_MUT_W3_7_sod},
    {SPR_MUT_W3_8_wl6, SPR_MUT_W3_8_sod},
    {SPR_MUT_W4_1_wl6, SPR_MUT_W4_1_sod},
    {SPR_MUT_W4_2_wl6, SPR_MUT_W4_2_sod},
    {SPR_MUT_W4_3_wl6, SPR_MUT_W4_3_sod},
    {SPR_MUT_W4_4_wl6, SPR_MUT_W4_4_sod},
    {SPR_MUT_W4_5_wl6, SPR_MUT_W4_5_sod},
    {SPR_MUT_W4_6_wl6, SPR_MUT_W4_6_sod},
    {SPR_MUT_W4_7_wl6, SPR_MUT_W4_7_sod},
    {SPR_MUT_W4_8_wl6, SPR_MUT_W4_8_sod},
    {SPR_OFC_DEAD_wl6, SPR_OFC_DEAD_sod},
    {SPR_OFC_DIE_1_wl6, SPR_OFC_DIE_1_sod},
    {SPR_OFC_DIE_2_wl6, SPR_OFC_DIE_2_sod},
    {SPR_OFC_DIE_3_wl6, SPR_OFC_DIE_3_sod},
    {SPR_OFC_DIE_4_wl6, SPR_OFC_DIE_4_sod},
    {SPR_OFC_PAIN_1_wl6, SPR_OFC_PAIN_1_sod},
    {SPR_OFC_PAIN_2_wl6, SPR_OFC_PAIN_2_sod},
    {SPR_OFC_S_1_wl6, SPR_OFC_S_1_sod},
    {SPR_OFC_S_2_wl6, SPR_OFC_S_2_sod},
    {SPR_OFC_S_3_wl6, SPR_OFC_S_3_sod},
    {SPR_OFC_S_4_wl6, SPR_OFC_S_4_sod},
    {SPR_OFC_S_5_wl6, SPR_OFC_S_5_sod},
    {SPR_OFC_S_6_wl6, SPR_OFC_S_6_sod},
    {SPR_OFC_S_7_wl6, SPR_OFC_S_7_sod},
    {SPR_OFC_S_8_wl6, SPR_OFC_S_8_sod},
    {SPR_OFC_SHOOT1_wl6, SPR_OFC_SHOOT1_sod},
    {SPR_OFC_SHOOT2_wl6, SPR_OFC_SHOOT2_sod},
    {SPR_OFC_SHOOT3_wl6, SPR_OFC_SHOOT3_sod},
    {SPR_OFC_W1_1_wl6, SPR_OFC_W1_1_sod},
    {SPR_OFC_W1_2_wl6, SPR_OFC_W1_2_sod},
    {SPR_OFC_W1_3_wl6, SPR_OFC_W1_3_sod},
    {SPR_OFC_W1_4_wl6, SPR_OFC_W1_4_sod},
    {SPR_OFC_W1_5_wl6, SPR_OFC_W1_5_sod},
    {SPR_OFC_W1_6_wl6, SPR_OFC_W1_6_sod},
    {SPR_OFC_W1_7_wl6, SPR_OFC_W1_7_sod},
    {SPR_OFC_W1_8_wl6, SPR_OFC_W1_8_sod},
    {SPR_OFC_W2_1_wl6, SPR_OFC_W2_1_sod},
    {SPR_OFC_W2_2_wl6, SPR_OFC_W2_2_sod},
    {SPR_OFC_W2_3_wl6, SPR_OFC_W2_3_sod},
    {SPR_OFC_W2_4_wl6, SPR_OFC_W2_4_sod},
    {SPR_OFC_W2_5_wl6, SPR_OFC_W2_5_sod},
    {SPR_OFC_W2_6_wl6, SPR_OFC_W2_6_sod},
    {SPR_OFC_W2_7_wl6, SPR_OFC_W2_7_sod},
    {SPR_OFC_W2_8_wl6, SPR_OFC_W2_8_sod},
    {SPR_OFC_W3_1_wl6, SPR_OFC_W3_1_sod},
    {SPR_OFC_W3_2_wl6, SPR_OFC_W3_2_sod},
    {SPR_OFC_W3_3_wl6, SPR_OFC_W3_3_sod},
    {SPR_OFC_W3_4_wl6, SPR_OFC_W3_4_sod},
    {SPR_OFC_W3_5_wl6, SPR_OFC_W3_5_sod},
    {SPR_OFC_W3_6_wl6, SPR_OFC_W3_6_sod},
    {SPR_OFC_W3_7_wl6, SPR_OFC_W3_7_sod},
    {SPR_OFC_W3_8_wl6, SPR_OFC_W3_8_sod},
    {SPR_OFC_W4_1_wl6, SPR_OFC_W4_1_sod},
    {SPR_OFC_W4_2_wl6, SPR_OFC_W4_2_sod},
    {SPR_OFC_W4_3_wl6, SPR_OFC_W4_3_sod},
    {SPR_OFC_W4_4_wl6, SPR_OFC_W4_4_sod},
    {SPR_OFC_W4_5_wl6, SPR_OFC_W4_5_sod},
    {SPR_OFC_W4_6_wl6, SPR_OFC_W4_6_sod},
    {SPR_OFC_W4_7_wl6, SPR_OFC_W4_7_sod},
    {SPR_OFC_W4_8_wl6, SPR_OFC_W4_8_sod},
    {SPR_PINKY_W1_wl6, 0},
    {SPR_PINKY_W2_wl6, 0},
    {SPR_PISTOLATK1_wl6, SPR_PISTOLATK1_sod},
    {SPR_PISTOLATK2_wl6, SPR_PISTOLATK2_sod},
    {SPR_PISTOLATK3_wl6, SPR_PISTOLATK3_sod},
    {SPR_PISTOLATK4_wl6, SPR_PISTOLATK4_sod},
    {SPR_PISTOLREADY_wl6, SPR_PISTOLREADY_sod},
    {SPR_ROCKET_1_wl6, SPR_ROCKET_1_sod},
    {SPR_ROCKET_2_wl6, SPR_ROCKET_2_sod},
    {SPR_ROCKET_3_wl6, SPR_ROCKET_3_sod},
    {SPR_ROCKET_4_wl6, SPR_ROCKET_4_sod},
    {SPR_ROCKET_5_wl6, SPR_ROCKET_5_sod},
    {SPR_ROCKET_6_wl6, SPR_ROCKET_6_sod},
    {SPR_ROCKET_7_wl6, SPR_ROCKET_7_sod},
    {SPR_ROCKET_8_wl6, SPR_ROCKET_8_sod},
    {SPR_SCHABB_DEAD_wl6, 0},
    {SPR_SCHABB_DIE1_wl6, 0},
    {SPR_SCHABB_DIE2_wl6, 0},
    {SPR_SCHABB_DIE3_wl6, 0},
    {SPR_SCHABB_SHOOT1_wl6, 0},
    {SPR_SCHABB_SHOOT2_wl6, 0},
    {SPR_SCHABB_W1_wl6, 0},
    {SPR_SCHABB_W2_wl6, 0},
    {SPR_SCHABB_W3_wl6, 0},
    {SPR_SCHABB_W4_wl6, 0},
    {SPR_SMOKE_1_wl6, SPR_SMOKE_1_sod},
    {SPR_SMOKE_2_wl6, SPR_SMOKE_2_sod},
    {SPR_SMOKE_3_wl6, SPR_SMOKE_3_sod},
    {SPR_SMOKE_4_wl6, SPR_SMOKE_4_sod},
    {0, SPR_SPARK1_sod},
    {0, SPR_SPARK2_sod},
    {0, SPR_SPARK3_sod},
    {0, SPR_SPARK4_sod},
    {0, SPR_SPECTRE_F1_sod},
    {0, SPR_SPECTRE_F2_sod},
    {0, SPR_SPECTRE_F3_sod},
    {0, SPR_SPECTRE_F4_sod},
    {0, SPR_SPECTRE_W1_sod},
    {0, SPR_SPECTRE_W2_sod},
    {0, SPR_SPECTRE_W3_sod},
    {0, SPR_SPECTRE_W4_sod},
    {SPR_SS_DEAD_wl6, SPR_SS_DEAD_sod},
    {SPR_SS_DIE_1_wl6, SPR_SS_DIE_1_sod},
    {SPR_SS_DIE_2_wl6, SPR_SS_DIE_2_sod},
    {SPR_SS_DIE_3_wl6, SPR_SS_DIE_3_sod},
    {SPR_SS_PAIN_1_wl6, SPR_SS_PAIN_1_sod},
    {SPR_SS_PAIN_2_wl6, SPR_SS_PAIN_2_sod},
    {SPR_SS_S_1_wl6, SPR_SS_S_1_sod},
    {SPR_SS_S_2_wl6, SPR_SS_S_2_sod},
    {SPR_SS_S_3_wl6, SPR_SS_S_3_sod},
    {SPR_SS_S_4_wl6, SPR_SS_S_4_sod},
    {SPR_SS_S_5_wl6, SPR_SS_S_5_sod},
    {SPR_SS_S_6_wl6, SPR_SS_S_6_sod},
    {SPR_SS_S_7_wl6, SPR_SS_S_7_sod},
    {SPR_SS_S_8_wl6, SPR_SS_S_8_sod},
    {SPR_SS_SHOOT1_wl6, SPR_SS_SHOOT1_sod},
    {SPR_SS_SHOOT2_wl6, SPR_SS_SHOOT2_sod},
    {SPR_SS_SHOOT3_wl6, SPR_SS_SHOOT3_sod},
    {SPR_SS_W1_1_wl6, SPR_SS_W1_1_sod},
    {SPR_SS_W1_2_wl6, SPR_SS_W1_2_sod},
    {SPR_SS_W1_3_wl6, SPR_SS_W1_3_sod},
    {SPR_SS_W1_4_wl6, SPR_SS_W1_4_sod},
    {SPR_SS_W1_5_wl6, SPR_SS_W1_5_sod},
    {SPR_SS_W1_6_wl6, SPR_SS_W1_6_sod},
    {SPR_SS_W1_7_wl6, SPR_SS_W1_7_sod},
    {SPR_SS_W1_8_wl6, SPR_SS_W1_8_sod},
    {SPR_SS_W2_1_wl6, SPR_SS_W2_1_sod},
    {SPR_SS_W2_2_wl6, SPR_SS_W2_2_sod},
    {SPR_SS_W2_3_wl6, SPR_SS_W2_3_sod},
    {SPR_SS_W2_4_wl6, SPR_SS_W2_4_sod},
    {SPR_SS_W2_5_wl6, SPR_SS_W2_5_sod},
    {SPR_SS_W2_6_wl6, SPR_SS_W2_6_sod},
    {SPR_SS_W2_7_wl6, SPR_SS_W2_7_sod},
    {SPR_SS_W2_8_wl6, SPR_SS_W2_8_sod},
    {SPR_SS_W3_1_wl6, SPR_SS_W3_1_sod},
    {SPR_SS_W3_2_wl6, SPR_SS_W3_2_sod},
    {SPR_SS_W3_3_wl6, SPR_SS_W3_3_sod},
    {SPR_SS_W3_4_wl6, SPR_SS_W3_4_sod},
    {SPR_SS_W3_5_wl6, SPR_SS_W3_5_sod},
    {SPR_SS_W3_6_wl6, SPR_SS_W3_6_sod},
    {SPR_SS_W3_7_wl6, SPR_SS_W3_7_sod},
    {SPR_SS_W3_8_wl6, SPR_SS_W3_8_sod},
    {SPR_SS_W4_1_wl6, SPR_SS_W4_1_sod},
    {SPR_SS_W4_2_wl6, SPR_SS_W4_2_sod},
    {SPR_SS_W4_3_wl6, SPR_SS_W4_3_sod},
    {SPR_SS_W4_4_wl6, SPR_SS_W4_4_sod},
    {SPR_SS_W4_5_wl6, SPR_SS_W4_5_sod},
    {SPR_SS_W4_6_wl6, SPR_SS_W4_6_sod},
    {SPR_SS_W4_7_wl6, SPR_SS_W4_7_sod},
    {SPR_SS_W4_8_wl6, SPR_SS_W4_8_sod},
    {SPR_STAT_0_wl6, SPR_STAT_0_sod},
    {SPR_STAT_10_wl6, SPR_STAT_10_sod},
    {SPR_STAT_11_wl6, SPR_STAT_11_sod},
    {SPR_STAT_12_wl6, SPR_STAT_12_sod},
    {SPR_STAT_13_wl6, SPR_STAT_13_sod},
    {SPR_STAT_14_wl6, SPR_STAT_14_sod},
    {SPR_STAT_15_wl6, SPR_STAT_15_sod},
    {SPR_STAT_16_wl6, SPR_STAT_16_sod},
    {SPR_STAT_17_wl6, SPR_STAT_17_sod},
    {SPR_STAT_18_wl6, SPR_STAT_18_sod},
    {SPR_STAT_19_wl6, SPR_STAT_19_sod},
    {SPR_STAT_1_wl6, SPR_STAT_1_sod},
    {SPR_STAT_20_wl6, SPR_STAT_20_sod},
    {SPR_STAT_21_wl6, SPR_STAT_21_sod},
    {SPR_STAT_22_wl6, SPR_STAT_22_sod},
    {SPR_STAT_23_wl6, SPR_STAT_23_sod},
    {SPR_STAT_24_wl6, SPR_STAT_24_sod},
    {SPR_STAT_25_wl6, SPR_STAT_25_sod},
    {SPR_STAT_26_wl6, SPR_STAT_26_sod},
    {SPR_STAT_27_wl6, SPR_STAT_27_sod},
    {SPR_STAT_28_wl6, SPR_STAT_28_sod},
    {SPR_STAT_29_wl6, SPR_STAT_29_sod},
    {SPR_STAT_2_wl6, SPR_STAT_2_sod},
    {SPR_STAT_30_wl6, SPR_STAT_30_sod},
    {SPR_STAT_31_wl6, SPR_STAT_31_sod},
    {SPR_STAT_32_wl6, SPR_STAT_32_sod},
    {SPR_STAT_33_wl6, SPR_STAT_33_sod},
    {SPR_STAT_34_wl6, SPR_STAT_34_sod},
    {SPR_STAT_35_wl6, SPR_STAT_35_sod},
    {SPR_STAT_36_wl6, SPR_STAT_36_sod},
    {SPR_STAT_37_wl6, SPR_STAT_37_sod},
    {SPR_STAT_38_wl6, SPR_STAT_38_sod},
    {SPR_STAT_39_wl6, SPR_STAT_39_sod},
    {SPR_STAT_3_wl6, SPR_STAT_3_sod},
    {SPR_STAT_40_wl6, SPR_STAT_40_sod},
    {SPR_STAT_41_wl6, SPR_STAT_41_sod},
    {SPR_STAT_42_wl6, SPR_STAT_42_sod},
    {SPR_STAT_43_wl6, SPR_STAT_43_sod},
    {SPR_STAT_44_wl6, SPR_STAT_44_sod},
    {SPR_STAT_45_wl6, SPR_STAT_45_sod},
    {SPR_STAT_46_wl6, SPR_STAT_46_sod},
    {SPR_STAT_47_wl6, SPR_STAT_47_sod},
    {0, SPR_STAT_48_sod},
    {0, SPR_STAT_49_sod},
    {SPR_STAT_4_wl6, SPR_STAT_4_sod},
    {0, SPR_STAT_50_sod},
    {0, SPR_STAT_51_sod},
    {SPR_STAT_5_wl6, SPR_STAT_5_sod},
    {SPR_STAT_6_wl6, SPR_STAT_6_sod},
    {SPR_STAT_7_wl6, SPR_STAT_7_sod},
    {SPR_STAT_8_wl6, SPR_STAT_8_sod},
    {SPR_STAT_9_wl6, SPR_STAT_9_sod},
    {0, SPR_TRANS_DEAD_sod},
    {0, SPR_TRANS_DIE1_sod},
    {0, SPR_TRANS_DIE2_sod},
    {0, SPR_TRANS_DIE3_sod},
    {0, SPR_TRANS_SHOOT1_sod},
    {0, SPR_TRANS_SHOOT2_sod},
    {0, SPR_TRANS_SHOOT3_sod},
    {0, SPR_TRANS_W1_sod},
    {0, SPR_TRANS_W2_sod},
    {0, SPR_TRANS_W3_sod},
    {0, SPR_TRANS_W4_sod},
    {0, SPR_UBER_DEAD_sod},
    {0, SPR_UBER_DIE1_sod},
    {0, SPR_UBER_DIE2_sod},
    {0, SPR_UBER_DIE3_sod},
    {0, SPR_UBER_DIE4_sod},
    {0, SPR_UBER_SHOOT1_sod},
    {0, SPR_UBER_SHOOT2_sod},
    {0, SPR_UBER_SHOOT3_sod},
    {0, SPR_UBER_SHOOT4_sod},
    {0, SPR_UBER_W1_sod},
    {0, SPR_UBER_W2_sod},
    {0, SPR_UBER_W3_sod},
    {0, SPR_UBER_W4_sod},
    {0, SPR_WILL_DEAD_sod},
    {0, SPR_WILL_DIE1_sod},
    {0, SPR_WILL_DIE2_sod},
    {0, SPR_WILL_DIE3_sod},
    {0, SPR_WILL_SHOOT1_sod},
    {0, SPR_WILL_SHOOT2_sod},
    {0, SPR_WILL_SHOOT3_sod},
    {0, SPR_WILL_SHOOT4_sod},
    {0, SPR_WILL_W1_sod},
    {0, SPR_WILL_W2_sod},
    {0, SPR_WILL_W3_sod},
    {0, SPR_WILL_W4_sod},
};

//
// SODFlag::soundmap
//
// Map for sounds
//

const unsigned int SPEAR::soundmap[][2] =
{
    {AHHHGSND_wl6,          AHHHGSND_sod},
	{0,                     ANGELDEATHSND_sod},
	{0,                     ANGELFIRESND_sod},
	{0,                     ANGELSIGHTSND_sod},
	{0,                     ANGELTIREDSND_sod},
	{ATKGATLINGSND_wl6,     ATKGATLINGSND_sod},
	{ATKKNIFESND_wl6,       ATKKNIFESND_sod},
	{ATKMACHINEGUNSND_wl6,  ATKMACHINEGUNSND_sod},
	{ATKPISTOLSND_wl6,      ATKPISTOLSND_sod},
	{BONUS1SND_wl6,         BONUS1SND_sod},
	{BONUS1UPSND_wl6,       BONUS1UPSND_sod},
	{BONUS2SND_wl6,         BONUS2SND_sod},
	{BONUS3SND_wl6,         BONUS3SND_sod},
	{BONUS4SND_wl6,         BONUS4SND_sod},
	{BOSSACTIVESND_wl6,     BOSSACTIVESND_sod},
	{BOSSFIRESND_wl6,       BOSSFIRESND_sod},
	{CLOSEDOORSND_wl6,      CLOSEDOORSND_sod},
	{DEATHSCREAM1SND_wl6,   DEATHSCREAM1SND_sod},
	{DEATHSCREAM2SND_wl6,   DEATHSCREAM2SND_sod},
	{DEATHSCREAM3SND_wl6,   DEATHSCREAM3SND_sod},
	{DEATHSCREAM4SND_wl6,   DEATHSCREAM4SND_sod},
	{DEATHSCREAM5SND_wl6,   DEATHSCREAM5SND_sod},
	{DEATHSCREAM6SND_wl6,   DEATHSCREAM6SND_sod},
	{DEATHSCREAM7SND_wl6,   DEATHSCREAM7SND_sod},
	{DEATHSCREAM8SND_wl6,   DEATHSCREAM8SND_sod},
	{DEATHSCREAM9SND_wl6,   DEATHSCREAM9SND_sod},
	{DIESND_wl6,            0},
	{DOGATTACKSND_wl6,      DOGATTACKSND_sod},
	{DOGBARKSND_wl6,        DOGBARKSND_sod},
	{DOGDEATHSND_wl6,       DOGDEATHSND_sod},
	{DONNERSND_wl6,         0},
	{DONOTHINGSND_wl6,      DONOTHINGSND_sod},
	{EINESND_wl6,           0},
	{ENDBONUS1SND_wl6,      ENDBONUS1SND_sod},
	{ENDBONUS2SND_wl6,      ENDBONUS2SND_sod},
	{ERLAUBENSND_wl6,       0},
	{ESCPRESSEDSND_wl6,     ESCPRESSEDSND_sod},
	{EVASND_wl6,            0},
	{FLAMETHROWERSND_wl6,   0},
	{GAMEOVERSND_wl6,       GAMEOVERSND_sod},
	{0,                     GETAMMOBOXSND_sod},
	{GETAMMOSND_wl6,        GETAMMOSND_sod},
	{GETGATLINGSND_wl6,     GETGATLINGSND_sod},
	{GETKEYSND_wl6,         GETKEYSND_sod},
	{GETMACHINESND_wl6,     GETMACHINESND_sod},
	{0,                     GETSPEARSND_sod},
	{0,                     GHOSTFADESND_sod},
    {0,                     GHOSTSIGHTSND_sod},
	{GOOBSSND_wl6,          0},
	{GUTENTAGSND_wl6,       0}, 
	{HALTSND_wl6,           HALTSND_sod},
	{HEALTH1SND_wl6,        HEALTH1SND_sod},
	{HEALTH2SND_wl6,        HEALTH2SND_sod},
	{HEARTBEATSND_wl6,      0},
	{HITENEMYSND_wl6,       HITENEMYSND_sod},
	{HITLERHASND_wl6,       0},
	{HITWALLSND_wl6,        HITWALLSND_sod},
	{KEINSND_wl6,           0},
	{0,                     KNIGHTDEATHSND_sod},
	{0,                     KNIGHTMISSILESND_sod},
	{0,                     KNIGHTSIGHTSND_sod},
	{LEBENSND_wl6,          LEBENSND_sod},
	{LEVELDONESND_wl6,      LEVELDONESND_sod},
	{MECHSTEPSND_wl6,       0},
	{MEINGOTTSND_wl6,       0},
	{MEINSND_wl6,           0},
	{MISSILEFIRESND_wl6,    MISSILEFIRESND_sod},
	{MISSILEHITSND_wl6,     MISSILEHITSND_sod},
	{MOVEGUN1SND_wl6,       MOVEGUN1SND_sod},
	{MOVEGUN2SND_wl6,       MOVEGUN2SND_sod},
	{MUTTISND_wl6,          0},
	{NAZIFIRESND_wl6,       NAZIFIRESND_sod},
	{NAZIHITPLAYERSND_wl6,  NAZIHITPLAYERSND_sod},
	{NEINSOVASSND_wl6,      NEINSOVASSND_sod},
	{NOBONUSSND_wl6,        NOBONUSSND_sod},
	{NOITEMSND_wl6,         NOITEMSND_sod},
	{NOWAYSND_wl6,          NOWAYSND_sod},
	{OPENDOORSND_wl6,       OPENDOORSND_sod},
	{PERCENT100SND_wl6,     PERCENT100SND_sod},
	{PLAYERDEATHSND_wl6,    PLAYERDEATHSND_sod},
	{PUSHWALLSND_wl6,       PUSHWALLSND_sod},
	{ROSESND_wl6,           0},
	{SCHABBSHASND_wl6,      0},
	{SCHABBSTHROWSND_wl6,   0},
	{SCHEISTSND_wl6,        0},
	{SCHUTZADSND_wl6,       SCHUTZADSND_sod},
	{SELECTITEMSND_wl6,     SELECTITEMSND_sod},
	{SELECTWPNSND_wl6,      0},
	{SHOOTDOORSND_wl6,      SHOOTDOORSND_sod},
	{SHOOTSND_wl6,          SHOOTSND_sod},
	{SLURPIESND_wl6,        SLURPIESND_sod},
	{SPIONSND_wl6,          SPIONSND_sod},
	{SSFIRESND_wl6,         SSFIRESND_sod},
	{TAKEDAMAGESND_wl6,     TAKEDAMAGESND_sod},
	{TOT_HUNDSND_wl6,       0},
	{0,                     TRANSDEATHSND_sod},
	{0,                     TRANSSIGHTSND_sod},
	{0,                     UBERDEATHSND_sod},
	{WALK1SND_wl6,          WALK1SND_sod},
	{WALK2SND_wl6,          WALK2SND_sod},
	{0,                     WILHELMDEATHSND_sod},
	{0,                     WILHELMSIGHTSND_sod},
	{YEAHSND_wl6,           0},
};

//
// SODFlag::Initialize
//
// Initializes the SOD flag depending on some key file names.
//

void SPEAR::Initialize(const std::string &basePath)
{
    FILE *f;
   
	f = fopen(FileSystem::FindCaseInsensitive(basePath, "VSWAP.SD3").c_str(), "rb");
    if(!f)
    {
		f = fopen(FileSystem::FindCaseInsensitive(basePath, "VSWAP.SD2").c_str(), "rb");
        if(!f)
        {
			f = fopen(FileSystem::FindCaseInsensitive(basePath, "VSWAP.SD1").c_str(), "rb");
            if(!f)
            {
				f = fopen(FileSystem::FindCaseInsensitive(basePath, "VSWAP.SOD").c_str(), "rb");
                if(!f)
                {
                    flag = false;
                   return;	// none found: assume Wolf3D
                }
            }
        }
    }
    fclose(f);
	// One of the ifs failed - fall here and return SPEAR::flag 1
    flag = true;
}


void SPEAR::SetGlobalValues()
{
	Menu::SetSpearModuleValues();
}