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

#include <string.h>
#include "wl_def.h"
#include "wl_draw.h"
// #pragma hdrstop

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


boolean fullscreen = true;



#if defined(_arch_dreamcast)
boolean usedoublebuffering = false;
unsigned screenWidth = 320;
unsigned screenHeight = 200;
unsigned screenBits = 8;
#elif defined(GP2X)
boolean usedoublebuffering = true;
unsigned screenWidth = 320;
unsigned screenHeight = 240;
#if defined(GP2X_940)
unsigned screenBits = 8;
#else
unsigned screenBits = 16;
#endif
#else
boolean usedoublebuffering = true;
unsigned screenWidth = 640;
unsigned screenHeight = 400;
unsigned screenBits = -1;      // use "best" color depth according to libSDL
#endif

SDL_Surface *screen = NULL;
unsigned screenPitch;

SDL_Surface *screenBuffer = NULL;
unsigned bufferPitch;

SDL_Surface *curSurface = NULL;
unsigned curPitch;

unsigned scaleFactor;

boolean	 screenfaded;
unsigned bordercolor;

SDL_Color palette1[256], palette2[256];
SDL_Color curpal[256];


#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
#define RGB(r, g, b) {(r)*255/63, (g)*255/63, (b)*255/63, 0}

// IOANCH 20130202: unification process
// SDL_Color gamepal[]={
SDL_Colour palette_sod[]={
// #ifdef SPEAR
    #include "sodpal.inc"
};
// #else
SDL_Colour palette_wl6[]={
    #include "wolfpal.inc"
// #endif
};

// IOANCH 20130202: unification process
CASSERT(lengthof(palette_sod) == 256 && lengthof(palette_wl6) == 256)

//===========================================================================


/*
=======================
=
= VL_Shutdown
=
=======================
*/

void	VL_Shutdown (void)
{
	//VL_SetTextMode ();
}


/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void	VL_SetVGAPlaneMode (void)
{
	// IOANCH 12.06.2012: bumped "Automatic" on the title bars
    // IOANCH 20130202: unification process
    if( SPEAR)
        SDL_WM_SetCaption("Automatic Spear of Destiny", NULL);
    else
        SDL_WM_SetCaption("Automatic Wolfenstein 3D", NULL);

    if(screenBits == -1)
    {
        const SDL_VideoInfo *vidInfo = SDL_GetVideoInfo();
        screenBits = vidInfo->vfmt->BitsPerPixel;
    }

    screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBits,
          (usedoublebuffering ? SDL_HWSURFACE | SDL_DOUBLEBUF : 0)
        | (screenBits == 8 ? SDL_HWPALETTE : 0)
        | (fullscreen ? SDL_FULLSCREEN : 0));
    if(!screen)
    {
        printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth,
            screenHeight, screenBits, SDL_GetError());
        exit(1);
    }
    if((screen->flags & SDL_DOUBLEBUF) != SDL_DOUBLEBUF)
        usedoublebuffering = false;
    SDL_ShowCursor(SDL_DISABLE);

    // IOANCH 20130202: unification process
    SDL_SetColors(screen, IMPALE(palette), 0, 256);
    memcpy(curpal, IMPALE(palette), sizeof(SDL_Color) * 256);

    screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth,
        screenHeight, 8, 0, 0, 0, 0);
    if(!screenBuffer)
    {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }
    
    // IOANCH 20130202: unification process
    SDL_SetColors(screenBuffer, IMPALE(palette), 0, 256);

    screenPitch = screen->pitch;
    bufferPitch = screenBuffer->pitch;

    curSurface = screenBuffer;
    curPitch = bufferPitch;

    scaleFactor = screenWidth/320;
    if(screenHeight/200 < scaleFactor) scaleFactor = screenHeight/200;

    pixelangle = (short *) malloc(screenWidth * sizeof(short));
    CHECKMALLOCRESULT(pixelangle);
    wallheight = (int *) malloc(screenWidth * sizeof(int));
    CHECKMALLOCRESULT(wallheight);
}

/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

/*
=================
=
= VL_ConvertPalette
=
=================
*/

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors)
{
    for(int i=0; i<numColors; i++)
    {
        destpal[i].r = *srcpal++ * 255 / 63;
        destpal[i].g = *srcpal++ * 255 / 63;
        destpal[i].b = *srcpal++ * 255 / 63;
    }
}

/*
=================
=
= VL_FillPalette
=
=================
*/

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

    VL_SetPalette(pal, true);
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/

void VL_SetColor	(int color, int red, int green, int blue)
{
    SDL_Color col = { static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue) };
    curpal[color] = col;

    if(screenBits == 8)
        SDL_SetPalette(screen, SDL_PHYSPAL, &col, color, 1);
    else
    {
        SDL_SetPalette(curSurface, SDL_LOGPAL, &col, color, 1);
        SDL_BlitSurface(curSurface, NULL, screen, NULL);
        SDL_Flip(screen);
    }
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor	(int color, int *red, int *green, int *blue)
{
    SDL_Color *col = &curpal[color];
    *red = col->r;
    *green = col->g;
    *blue = col->b;
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/

void VL_SetPalette (SDL_Color *palette, bool forceupdate)
{
    memcpy(curpal, palette, sizeof(SDL_Color) * 256);

    if(screenBits == 8)
        SDL_SetPalette(screen, SDL_PHYSPAL, palette, 0, 256);
    else
    {
        SDL_SetPalette(curSurface, SDL_LOGPAL, palette, 0, 256);
        if(forceupdate)
        {
            SDL_BlitSurface(curSurface, NULL, screen, NULL);
            SDL_Flip(screen);
        }
    }
}


//===========================================================================

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette (SDL_Color *palette)
{
    memcpy(palette, curpal, sizeof(SDL_Color) * 256);
}


//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

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

		if(!usedoublebuffering || screenBits == 8) VL_WaitVBL(1);
		VL_SetPalette (palette2, true);
	}

//
// final color
//
	VL_FillPalette (red,green,blue);

	screenfaded = true;
}


/*
=================
=
= VL_FadeIn
=
=================
*/

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

		if(!usedoublebuffering || screenBits == 8) VL_WaitVBL(1);
		VL_SetPalette(palette2, true);
	}

//
// final color
//
	VL_SetPalette (palette, true);
	screenfaded = false;
}

/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

byte *VL_LockSurface(SDL_Surface *surface)
{
    if(SDL_MUSTLOCK(surface))
    {
        if(SDL_LockSurface(surface) < 0)
            return NULL;
    }
    return (byte *) surface->pixels;
}

void VL_UnlockSurface(SDL_Surface *surface)
{
    if(SDL_MUSTLOCK(surface))
    {
        SDL_UnlockSurface(surface);
    }
}

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot (int x, int y, int color)
{
    byte *ptr;

    assert(x >= 0 && (unsigned) x < screenWidth
            && y >= 0 && (unsigned) y < screenHeight
            && "VL_Plot: Pixel out of bounds!");

    ptr = VL_LockSurface(curSurface);
    if(ptr == NULL) return;

    ptr[y * curPitch + x] = color;

    VL_UnlockSurface(curSurface);
}

/*
=================
=
= VL_GetPixel
=
=================
*/

byte VL_GetPixel (int x, int y)
{
    byte *ptr;
    byte col;

    assert_ret(x >= 0 && (unsigned) x < screenWidth
            && y >= 0 && (unsigned) y < screenHeight
            && "VL_GetPixel: Pixel out of bounds!");

    ptr = VL_LockSurface(curSurface);
    if(ptr == NULL) return 0;

    col = ((byte *) curSurface->pixels)[y * curPitch + x];

    VL_UnlockSurface(curSurface);

    return col;
}


/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin (unsigned x, unsigned y, unsigned width, int color)
{
    byte *ptr;

    assert(x >= 0 && x + width <= screenWidth
            && y >= 0 && y < screenHeight
            && "VL_Hlin: Destination rectangle out of bounds!");

    ptr = VL_LockSurface(curSurface);
    if(ptr == NULL) return;

    memset(ptr + y * curPitch + x, color, width);

    VL_UnlockSurface(curSurface);
}


/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin (int x, int y, int height, int color)
{
	byte *ptr;

	assert(x >= 0 && (unsigned) x < screenWidth
			&& y >= 0 && (unsigned) y + height <= screenHeight
			&& "VL_Vlin: Destination rectangle out of bounds!");

	ptr = VL_LockSurface(curSurface);
	if(ptr == NULL) return;

	ptr += y * curPitch + x;

	while (height--)
	{
		*ptr = color;
		ptr += curPitch;
	}

	VL_UnlockSurface(curSurface);
}


/*
=================
=
= VL_Bar
=
=================
*/

void VL_BarScaledCoord (int scx, int scy, int scwidth, int scheight, int color)
{
	byte *ptr;

	assert(scx >= 0 && (unsigned) scx + scwidth <= screenWidth
			&& scy >= 0 && (unsigned) scy + scheight <= screenHeight
			&& "VL_BarScaledCoord: Destination rectangle out of bounds!");

	ptr = VL_LockSurface(curSurface);
	if(ptr == NULL) return;

	ptr += scy * curPitch + scx;

	while (scheight--)
	{
		memset(ptr, color, scwidth);
		ptr += curPitch;
	}
	VL_UnlockSurface(curSurface);
}

/*
============================================================================

							MEMORY OPS

============================================================================
*/

/*
=================
=
= VL_MemToLatch
=
=================
*/

void VL_MemToLatch(byte *source, int width, int height, SDL_Surface *destSurface, int x, int y)
{
    byte *ptr;
    int xsrc, ysrc, pitch;

    assert(x >= 0 && (unsigned) x + width <= screenWidth
            && y >= 0 && (unsigned) y + height <= screenHeight
            && "VL_MemToLatch: Destination rectangle out of bounds!");

    ptr = VL_LockSurface(destSurface);
    if(ptr == NULL) return;

    pitch = destSurface->pitch;
    ptr += y * pitch + x;
    for(ysrc = 0; ysrc < height; ysrc++)
    {
        for(xsrc = 0; xsrc < width; xsrc++)
        {
            ptr[ysrc * pitch + xsrc] = source[(ysrc * (width >> 2) + (xsrc >> 2))
                + (xsrc & 3) * (width >> 2) * height];
        }
    }
    VL_UnlockSurface(destSurface);
}

//===========================================================================


/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a block of data to the screen with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreenScaledCoord (byte *source, int width, int height, int destx, int desty)
{
    byte *ptr;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * scaleFactor <= screenWidth
            && desty >= 0 && desty + height * scaleFactor <= screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    ptr = VL_LockSurface(curSurface);
    if(ptr == NULL) return;

    for(j = 0, scj = 0; j < height; j++, scj += scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += scaleFactor)
        {
            byte col = source[(j * (width >> 2) + (i >> 2)) + (i & 3) * (width >> 2) * height];
            for(m = 0; m < scaleFactor; m++)
            {
                for(n = 0; n < scaleFactor; n++)
                {
                    ptr[(scj + m + desty) * curPitch + sci + n + destx] = col;
                }
            }
        }
    }
    VL_UnlockSurface(curSurface);
}

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a part of a block of data to the screen.
= The block has the size origwidth*origheight.
= The part at (srcx, srcy) has the size width*height
= and will be painted to (destx, desty) with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreenScaledCoord (byte *source, int origwidth, int origheight, int srcx, int srcy,
                                int destx, int desty, int width, int height)
{
    byte *ptr;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * scaleFactor <= screenWidth
            && desty >= 0 && desty + height * scaleFactor <= screenHeight
            && "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    ptr = VL_LockSurface(curSurface);
    if(ptr == NULL) return;

    for(j = 0, scj = 0; j < height; j++, scj += scaleFactor)
    {
        for(i = 0, sci = 0; i < width; i++, sci += scaleFactor)
        {
            byte col = source[((j + srcy) * (origwidth >> 2) + ((i + srcx) >>2 ))
                + ((i + srcx) & 3) * (origwidth >> 2) * origheight];

            for(m = 0; m < scaleFactor; m++)
            {
                for(n = 0; n < scaleFactor; n++)
                {
                    ptr[(scj + m + desty) * curPitch + sci + n + destx] = col;
                }
            }
        }
    }
    VL_UnlockSurface(curSurface);
}

//==========================================================================

/*
=================
=
= VL_LatchToScreen
=
=================
*/

void VL_LatchToScreenScaledCoord(SDL_Surface *source, int xsrc, int ysrc,
    int width, int height, int scxdest, int scydest)
{
	assert(scxdest >= 0 && scxdest + width * scaleFactor <= screenWidth
			&& scydest >= 0 && scydest + height * scaleFactor <= screenHeight
			&& "VL_LatchToScreenScaledCoord: Destination rectangle out of bounds!");

	if(scaleFactor == 1)
    {
        // HACK: If screenBits is not 8 and the screen is faded out, the
        //       result will be black when using SDL_BlitSurface. The reason
        //       is that the logical palette needed for the transformation
        //       to the screen color depth is not equal to the logical
        //       palette of the latch (the latch is not faded). Therefore,
        //       SDL tries to map the colors...
        //       The result: All colors are mapped to black.
        //       So, we do the blit on our own...
        if(screenBits != 8)
        {
            byte *src, *dest;
            unsigned srcPitch;
            int i, j;

            src = VL_LockSurface(source);
            if(src == NULL) return;

            srcPitch = source->pitch;

            dest = VL_LockSurface(curSurface);
            if(dest == NULL) return;

            for(j = 0; j < height; j++)
            {
                for(i = 0; i < width; i++)
                {
                    byte col = src[(ysrc + j)*srcPitch + xsrc + i];
                    dest[(scydest + j) * curPitch + scxdest + i] = col;
                }
            }
            VL_UnlockSurface(curSurface);
            VL_UnlockSurface(source);
        }
        else
        {
            SDL_Rect srcrect = { static_cast<Sint16>(xsrc), static_cast<Sint16>(ysrc), static_cast<Uint16>(width), static_cast<Uint16>(height) };
            SDL_Rect destrect = { static_cast<Sint16>(scxdest), static_cast<Sint16>(scydest), 0, 0 }; // width and height are ignored
            SDL_BlitSurface(source, &srcrect, curSurface, &destrect);
        }
    }
    else
    {
        byte *src, *dest;
        unsigned srcPitch;
        int i, j, sci, scj;
        unsigned m, n;

        src = VL_LockSurface(source);
        if(src == NULL) return;

        srcPitch = source->pitch;

        dest = VL_LockSurface(curSurface);
        if(dest == NULL) return;

        for(j = 0, scj = 0; j < height; j++, scj += scaleFactor)
        {
            for(i = 0, sci = 0; i < width; i++, sci += scaleFactor)
            {
                byte col = src[(ysrc + j)*srcPitch + xsrc + i];
                for(m = 0; m < scaleFactor; m++)
                {
                    for(n = 0; n < scaleFactor; n++)
                    {
                        dest[(scydest + scj + m) * curPitch + scxdest + sci + n] = col;
                    }
                }
            }
        }
        VL_UnlockSurface(curSurface);
        VL_UnlockSurface(source);
    }
}

//===========================================================================

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen (SDL_Surface *source, SDL_Surface *dest)
{
    SDL_BlitSurface(source, NULL, dest, NULL);
}
