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

// ID_VL.C

#include "wl_def.h"
#include "wl_draw.h"
#include "Config.h"
#include "i_system.h"
#include "i_video.h"
// Uncomment the following line, if you get destination out of bounds
// assertion errors and want to ignore them during debugging
//#define IGNORE_BAD_DEST

#ifdef IGNORE_BAD_DEST
#undef assert
#define assert(x) if(!(x)) return
#define assert_ret(x) if(!(x)) return 0
#else
#define assert_ret(x) assert(x)
#endif

unsigned vid_scaleFactor;
Boolean8	 vid_screenfaded;

SDL_Color palette1[256], palette2[256];
SDL_Color vid_curpal[256];


#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
#define PAL_RGB(r, g, b) {(r)*255/63, (g)*255/63, (b)*255/63, 255}

// IOANCH 20130202: unification process
// SDL_Color gamepal[]={
SDL_Colour vid_palette_sod[]={
// #ifdef SPEAR::flag
    #include "sodpal.inc"
};
// #else
SDL_Colour vid_palette_wl6[]={
    #include "wolfpal.inc"
// #endif
};

// IOANCH 20130202: unification process
CASSERT(lengthof(vid_palette_sod) == 256 && lengthof(vid_palette_wl6) == 256)

//===========================================================================


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_Shutdown
// =
//
////////////////////////////////////////////////////////////////////////////////


void	VL_Shutdown ()
{
	//VL_SetTextMode ();
}


// IOANCH 20130726: Moved SetVGAPlaneMode to i_system

////////////////////////////////////////////////////////////////////////////////
//
//                        PALETTE OPS
//
//        To avoid snow, do a WaitVBL BEFORE calling these
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_ConvertPalette
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_ConvertPalette(const byte *srcpal, SDL_Color *destpal, int numColors)
{
    for(int i=0; i<numColors; i++)
    {
        destpal[i].r = *srcpal++ * 255 / 63;
        destpal[i].g = *srcpal++ * 255 / 63;
        destpal[i].b = *srcpal++ * 255 / 63;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_FillPalette
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_FillPalette (int red, int green, int blue)
{
    int i;
    SDL_Color pal[256];

    for(i=0; i<256; i++)
    {
        pal[i].r = red;
        pal[i].g = green;
        pal[i].b = blue;
    }

    I_SetPalette(pal, true);
}
// IOANCH: abstracted away

//===========================================================================

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_GetPalette
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_GetPalette (SDL_Color *palette)
{
    memcpy(palette, vid_curpal, sizeof(SDL_Color) * 256);
}


//===========================================================================

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_FadeOut
// =
// = Fades the current palette to the given color in the given number of steps
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_FadeOut (int start, int end, int red, int green, int blue, int steps)
{
	int		    i,j,orig,delta;
	SDL_Color   *origptr, *newptr;

    red = red * 255 / 63;
    green = green * 255 / 63;
    blue = blue * 255 / 63;

	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		origptr = &palette1[start];
		newptr = &palette2[start];
		for (j=start;j<=end;j++)
		{
			orig = origptr->r;
			delta = red-orig;
			newptr->r = orig + delta * i / steps;
			orig = origptr->g;
			delta = green-orig;
			newptr->g = orig + delta * i / steps;
			orig = origptr->b;
			delta = blue-orig;
			newptr->b = orig + delta * i / steps;
			origptr++;
			newptr++;
		}

		if(!cfg_usedoublebuffering || cfg_screenBits == 8) VL_WaitVBL(1);
		I_SetPalette (palette2, true);
	}

//
// final color
//
	VL_FillPalette (red,green,blue);

	vid_screenfaded = true;
}


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_FadeIn
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_FadeIn (int start, int end, SDL_Color *palette, int steps)
{
	int i,j,delta;

	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

//
// fade through intermediate frames
//
	for (i=0;i<steps;i++)
	{
		for (j=start;j<=end;j++)
		{
			delta = palette[j].r-palette1[j].r;
			palette2[j].r = palette1[j].r + delta * i / steps;
			delta = palette[j].g-palette1[j].g;
			palette2[j].g = palette1[j].g + delta * i / steps;
			delta = palette[j].b-palette1[j].b;
			palette2[j].b = palette1[j].b + delta * i / steps;
		}

		if(!cfg_usedoublebuffering || cfg_screenBits == 8) VL_WaitVBL(1);
		I_SetPalette(palette2, true);
	}

//
// final color
//
	I_SetPalette (palette, true);
	vid_screenfaded = false;
}
// IOANCH 20130726: moved to i_system

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_Plot
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_Plot (int x, int y, int color)
{
    byte *ptr;

    assert(x >= 0 && (unsigned) x < cfg_screenWidth
            && y >= 0 && (unsigned) y < cfg_screenHeight
            && "VL_Plot: Pixel out of bounds!");

    ptr = I_LockBuffer();
    if(ptr == NULL) return;

    ptr[y * vid_bufferPitch + x] = color;

    I_UnlockBuffer();
}

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_GetPixel
// =
//
////////////////////////////////////////////////////////////////////////////////


byte VL_GetPixel (int x, int y)
{
    byte *ptr;
    byte col;

    assert_ret(x >= 0 && (unsigned) x < cfg_screenWidth
            && y >= 0 && (unsigned) y < cfg_screenHeight
            && "VL_GetPixel: Pixel out of bounds!");

    ptr = I_LockBuffer();
    if(ptr == NULL) return 0;

    col = ptr[y * vid_bufferPitch + x];   // IOANCH: just use ptr

    I_UnlockBuffer();

    return col;
}


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_Hlin
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_Hlin (unsigned x, unsigned y, unsigned width, int color)
{
    byte *ptr;

    assert(x >= 0 && x + width <= cfg_screenWidth
            && y >= 0 && y < cfg_screenHeight
            && "VL_Hlin: Destination rectangle out of bounds!");

    ptr = I_LockBuffer();
    if(ptr == NULL) return;

    memset(ptr + y * vid_bufferPitch + x, color, width);

    I_UnlockBuffer();
}


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_Vlin
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_Vlin (int x, int y, int height, int color)
{
	byte *ptr;

	assert(x >= 0 && (unsigned) x < cfg_screenWidth
			&& y >= 0 && (unsigned) y + height <= cfg_screenHeight
			&& "VL_Vlin: Destination rectangle out of bounds!");

	ptr = I_LockBuffer();
	if(ptr == NULL) return;

	ptr += y * vid_bufferPitch + x;

	while (height--)
	{
		*ptr = color;
		ptr += vid_bufferPitch;
	}

	I_UnlockBuffer();
}


////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_Bar
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_BarScaledCoord (int scx, int scy, int scwidth, int scheight, int color)
{
	byte *ptr;

	assert(scx >= 0 && (unsigned) scx + scwidth <= cfg_screenWidth
			&& scy >= 0 && (unsigned) scy + scheight <= cfg_screenHeight
			&& "VL_BarScaledCoord: Destination rectangle out of bounds!");

	ptr = I_LockBuffer();
	if(ptr == NULL) return;

	ptr += scy * vid_bufferPitch + scx;

	while (scheight--)
	{
		memset(ptr, color, scwidth);
		ptr += vid_bufferPitch;
	}
	I_UnlockBuffer();
}

////////////////////////////////////////////////////////////////////////////////
//
//                            MEMORY OPS
//
////////////////////////////////////////////////////////////////////////////////



//===========================================================================


////////////////////////////////////////////////////////////////////////////////
//
// VL_MemToScreenScaledCoord
//
// Draws a block of data to the screen with scaling according to vid_scaleFactor.
//
////////////////////////////////////////////////////////////////////////////////


void VL_MemToScreenScaledCoord (const byte *source, int width, int height,
                                int destx, int desty)
{
    byte *ptr;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * vid_scaleFactor <= cfg_screenWidth
            && desty >= 0 && desty + height * vid_scaleFactor <= cfg_screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    ptr = I_LockBuffer();
    if(ptr == NULL)
       return;

    for(j = 0, scj = 0; j < height; j++, scj += vid_scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += vid_scaleFactor)
        {
            byte col = source[(j * (width >> 2) + (i >> 2)) + (i & 3) *
                              (width >> 2) * height];
            for(m = 0; m < vid_scaleFactor; m++)
            {
                for(n = 0; n < vid_scaleFactor; n++)
                {
                    ptr[(scj + m + desty) * vid_bufferPitch + sci + n + destx] = col;
                }
            }
        }
    }
    I_UnlockBuffer();
}

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VL_MemToScreenScaledCoord
// =
// = Draws a part of a block of data to the screen.
// = The block has the size origwidth*origheight.
// = The part at (srcx, srcy) has the size width*height
// = and will be painted to (destx, desty) with scaling according to 
// vid_scaleFactor.
// =
//
////////////////////////////////////////////////////////////////////////////////


void VL_MemToScreenScaledCoord (const byte *source, int origwidth, int origheight, int srcx, int srcy,
                                int destx, int desty, int width, int height)
{
    byte *ptr;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * vid_scaleFactor <= cfg_screenWidth
            && desty >= 0 && desty + height * vid_scaleFactor <= cfg_screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    ptr = I_LockBuffer();
    if(ptr == NULL) return;

    for(j = 0, scj = 0; j < height; j++, scj += vid_scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += vid_scaleFactor)
        {
            byte col = source[((j + srcy) * (origwidth >> 2) + ((i + srcx) >>2 ))
                + ((i + srcx) & 3) * (origwidth >> 2) * origheight];

            for(m = 0; m < vid_scaleFactor; m++)
            {
                for(n = 0; n < vid_scaleFactor; n++)
                {
                    ptr[(scj + m + desty) * vid_bufferPitch + sci + n + destx] = col;
                }
            }
        }
    }
    I_UnlockBuffer();
}

//
// VL_SetViewportValues
//
// IOANCH: moved from I_ here, because not low-level enough
//
void VL_SetViewportValues()
{
   vid_scaleFactor = cfg_screenWidth/320;
   if(cfg_screenHeight/200 < vid_scaleFactor)
      vid_scaleFactor = cfg_screenHeight/200;
   
   vid_pixelangle = (short *) I_CheckedMalloc(cfg_screenWidth * sizeof(short));

   vid_wallheight = (int *) I_CheckedMalloc(cfg_screenWidth * sizeof(int));

}