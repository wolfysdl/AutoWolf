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

#include "wl_def.h"
#include "Config.h"
#include "i_video.h"   // IOANCH

pictabletype	*pictable;
SDL_Surface     *vid_latchpics[NUMLATCHPICS];

int	    px,py;
byte	fontcolor,backcolor;
int	    fontnumber;

//==========================================================================

//
// VWB_DrawPropString
//
void VWB_DrawPropString(const char* string)
{
	fontstruct  *font;
	int		    width, step, height;
	byte	    *source, *dest;
	byte	    ch;
	int i;
	unsigned sx, sy;

	byte *vbuf = I_LockSurface(vid_curSurface);
	if(vbuf == NULL) return;

	font = (fontstruct *) ca_grsegs[SPEAR.g(STARTFONT)+fontnumber];
	height = font->height;
	dest = vbuf + vid_scaleFactor * (py * vid_curPitch + px);

	while ((ch = (byte)*string++)!=0)
	{
		width = step = font->width[ch];
		source = ((byte *)font)+font->location[ch];
		while (width--)
		{
			for(i=0; i<height; i++)
			{
				if(source[i*step])
				{
					for(sy=0; sy<vid_scaleFactor; sy++)
						for(sx=0; sx<vid_scaleFactor; sx++)
							dest[(vid_scaleFactor*i+sy)*vid_curPitch+sx]=fontcolor;
				}
			}

			source++;
			px++;
			dest+=vid_scaleFactor;
		}
	}

	I_UnlockSurface(vid_curSurface);
}

////////////////////////////////////////////////////////////////////////////////
//
// VL_MungePic
//
////////////////////////////////////////////////////////////////////////////////

void VL_MungePic (byte *source, unsigned width, unsigned height)
{
	unsigned x,y,plane,size,pwidth;
	byte *temp, *dest, *srcline;

	size = width*height;

	if (width&3)
		Quit ("VL_MungePic: Not divisable by 4!");

//
// copy the pic to a temp buffer
//
	temp=(byte *) malloc(size);
    CHECKMALLOCRESULT(temp);
	memcpy (temp,source,size);

//
// munge it back into the original buffer
//
	dest = source;
	pwidth = width/4;

	for (plane=0;plane<4;plane++)
	{
		srcline = temp;
		for (y=0;y<height;y++)
		{
			for (x=0;x<pwidth;x++)
				*dest++ = *(srcline+x*4+plane);
			srcline+=width;
		}
	}

	free(temp);
}

//
// VWL_MeasureString
//
void VWL_MeasureString (const char *string, word *width, word *height, fontstruct *font)
{
	*height = font->height;
	for (*width = 0;*string;string++)
		*width += font->width[*((byte *)string)];	// proportional width
}

//
// VW_MeasurePropString
//
void VW_MeasurePropString (const char *string, word *width, word *height)
{
	VWL_MeasureString(string,width,height,(fontstruct *)ca_grsegs[SPEAR.g(STARTFONT)+fontnumber]);
}

////////////////////////////////////////////////////////////////////////////////
//
//                Double buffer management routines
//
////////////////////////////////////////////////////////////////////////////////


// IOANCH 20130726: moved updatescreen to i_system
//
// VWB_DrawTile8
//
void VWB_DrawTile8 (int x, int y, int tile)
{
	LatchDrawChar(x,y,tile);
}

//
// VWB_DrawTile8M
//
void VWB_DrawTile8M (int x, int y, int tile)
{
	VL_MemToScreen (((byte *)ca_grsegs[SPEAR.g(STARTTILE8M)])+tile*64,8,8,x,y);
}

//
// VWB_DrawPic
//
void VWB_DrawPic (int x, int y, int chunknum)
{
	int	picnum = chunknum - SPEAR.g(STARTPICS);
	unsigned width,height;

	x &= ~7;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

	VL_MemToScreen (ca_grsegs[chunknum],width,height,x,y);
}

//
// VWB_DrawPicScaledCoord
//
void VWB_DrawPicScaledCoord (int scx, int scy, int chunknum)
{
	int	picnum = chunknum - SPEAR.g(STARTPICS);
	unsigned width,height;

	width = pictable[picnum].width;
	height = pictable[picnum].height;

    VL_MemToScreenScaledCoord (ca_grsegs[chunknum],width,height,scx,scy);
}

// IOANCH: removed VWB_Bar, was redundant

//
// VWB_Plot
//
void VWB_Plot (int x, int y, int color)
{
    if(vid_scaleFactor == 1)
        VL_Plot(x,y,color);
    else
        VL_Bar(x, y, 1, 1, color);
}

//
// VWB_Hlin
//
void VWB_Hlin (int x1, int x2, int y, int color)
{
    if(vid_scaleFactor == 1)
    	VW_Hlin(x1,x2,y,color);
    else
        VL_Bar(x1, y, x2-x1+1, 1, color);
}

//
// VWB_Vlin
//
void VWB_Vlin (int y1, int y2, int x, int color)
{
    if(vid_scaleFactor == 1)
		VW_Vlin(y1,y2,x,color);
    else
        VL_Bar(x, y1, 1, y2-y1+1, color);
}


////////////////////////////////////////////////////////////////////////////////
//
//                        WOLFENSTEIN STUFF
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
// =
// = LatchDrawPic
// =
//
////////////////////////////////////////////////////////////////////////////////


void LatchDrawPic (unsigned x, unsigned y, unsigned picnum)
{
	VL_LatchToScreen (vid_latchpics[2+picnum-SPEAR.g(LATCHPICS_LUMP_START)], x*8, y);
}

//
// LatchDrawPicScaledCoord
//
void LatchDrawPicScaledCoord (unsigned scx, unsigned scy, unsigned picnum)
{
	VL_LatchToScreenScaledCoord (vid_latchpics[2+picnum-SPEAR.g(LATCHPICS_LUMP_START)], scx*8, scy);
}


//==========================================================================

//
// FreeLatchMem
// 
void FreeLatchMem()
{
    int i;
    for(i = 0; i < (2 + (signed int)SPEAR.g(LATCHPICS_LUMP_END) - (signed int)SPEAR.g(LATCHPICS_LUMP_START)); i++)
    {
        SDL_FreeSurface(vid_latchpics[i]);
        vid_latchpics[i] = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// =
// = VH_LoadLatchMem
// =
//
////////////////////////////////////////////////////////////////////////////////


void VH_LoadLatchMem ()
{
	int	i,width,height,start,end;
	byte *src;
	SDL_Surface *surf;

//
// tile 8s
//
   // IOANCH: use I_ call
   surf = I_CreateSurface(SDL_HWSURFACE, 8 * 8, (SPEAR.g(NUMTILE8) + 7) / 8 * 8);

	vid_latchpics[0] = surf;
	CA_CacheGrChunk (SPEAR.g(STARTTILE8));
	src = ca_grsegs[SPEAR.g(STARTTILE8)];

	for (i=0;i<(signed int)SPEAR.g(NUMTILE8);i++)
	{
		VL_MemToLatch (src, 8, 8, surf, (i & 7) * 8, (i >> 3) * 8);
		src += 64;
	}
	UNCACHEGRCHUNK (SPEAR.g(STARTTILE8));

	vid_latchpics[1] = NULL;  // not used

//
// pics
//
	start = SPEAR.g(LATCHPICS_LUMP_START);
	end = SPEAR.g(LATCHPICS_LUMP_END);

	for (i=start;i<=end;i++)
	{
		width = pictable[i-SPEAR.g(STARTPICS)].width;
		height = pictable[i-SPEAR.g(STARTPICS)].height;
      surf = I_CreateSurface(SDL_HWSURFACE, width, height);

		vid_latchpics[2+i-start] = surf;
		CA_CacheGrChunk (i);
		VL_MemToLatch (ca_grsegs[i], width, height, surf, 0, 0);
		UNCACHEGRCHUNK(i);
	}
}

//==========================================================================

////////////////////////////////////////////////////////////////////////////////
//
// =
// = FizzleFade
// =
// = returns true if aborted
// =
// = It uses maximum-length Linear Feedback Shift Registers (LFSR) counters.
// = You can find a list of them with lengths from 3 to 168 at:
// = http://www.xilinx.com/support/documentation/application_notes/xapp052.pdf
// = Many thanks to Xilinx for this list!!!
// =
//
////////////////////////////////////////////////////////////////////////////////


// XOR masks for the pseudo-random number sequence starting with n=17 bits
static const uint32_t fiz_rndmasks[] = {
                    // n    XNOR from (starting at 1, not 0 as usual)
    0x00012000,     // 17   17,14
    0x00020400,     // 18   18,11
    0x00040023,     // 19   19,6,2,1
    0x00090000,     // 20   20,17
    0x00140000,     // 21   21,19
    0x00300000,     // 22   22,21
    0x00420000,     // 23   23,18
    0x00e10000,     // 24   24,23,22,17
    0x01200000,     // 25   25,22      (this is enough for 8191x4095)
};

static unsigned int fiz_rndbits_y;
static unsigned int fiz_rndmask;

// IOANCH 20130726: made extern

//
// log2_ceil
//
// Returns the number of bits needed to represent the given value
//
static int log2_ceil(uint32_t x)
{
    int n = 0;
    uint32_t v = 1;
    while(v < x)
    {
        n++;
        v <<= 1;
    }
    return n;
}

//
// VH_Startup
//
void VH_Startup()
{
    int rndbits_x = log2_ceil(cfg_screenWidth);
    fiz_rndbits_y = log2_ceil(cfg_screenHeight);

    int rndbits = rndbits_x + fiz_rndbits_y;
    if(rndbits < 17)
        rndbits = 17;       // no problem, just a bit slower
    else if(rndbits > 25)
        rndbits = 25;       // fizzle fade will not fill whole screen

    fiz_rndmask = fiz_rndmasks[rndbits - 17];
}

//
// FizzleFade
//
Boolean FizzleFade (SDL_Surface *source, int x1, int y1, unsigned width,
                    unsigned height, unsigned frames, Boolean abortable)
{
    unsigned x, y, frame, pixperframe;
    int32_t  rndval, lastrndval;
    int      first = 1;

    lastrndval = 0;
    pixperframe = width * height / frames;

    IN_StartAck ();

    frame = GetTimeCount();
    byte *srcptr = I_LockSurface(source);
    if(srcptr == NULL)
       return false;

    do
    {
        IN_ProcessEvents();

        if(abortable && IN_CheckAck ())
        {
            I_UnlockSurface(source);
            SDL_BlitSurface(source, NULL, vid_screen, NULL);
            SDL_Flip(vid_screen);
            return true;
        }

        byte *destptr = I_LockSurface(vid_screen);

        if(destptr != NULL)
        {
            rndval = lastrndval;

            // When using double buffering, we have to copy the pixels of the last AND the current frame.
            // Only for the first frame, there is no "last frame"
            for(int i = first; i < 2; i++)
            {
                for(unsigned p = 0; p < pixperframe; p++)
                {
                    //
                    // seperate random value into x/y pair
                    //

                    x = rndval >> fiz_rndbits_y;
                    y = rndval & ((1 << fiz_rndbits_y) - 1);

                    //
                    // advance to next random element
                    //

                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : fiz_rndmask);

                    if(x >= width || y >= height)
                    {
                        if(rndval == 0)     // entire sequence has been completed
                            goto finished;
                        p--;
                        continue;
                    }

                    //
                    // copy one pixel
                    //

                    if(cfg_screenBits == 8)
                    {
                        *(destptr + (y1 + y) * vid_screen->pitch + x1 + x)
                            = *(srcptr + (y1 + y) * source->pitch + x1 + x);
                    }
                    else
                    {
                        byte col = *(srcptr + (y1 + y) * source->pitch + x1 + x);
                        uint32_t fullcol = SDL_MapRGB(vid_screen->format,
                                                      vid_curpal[col].r,
                                                      vid_curpal[col].g,
                                                      vid_curpal[col].b);
                        memcpy(destptr + (y1 + y) * vid_screen->pitch +
                               (x1 + x) * vid_screen->format->BytesPerPixel,
                            &fullcol, vid_screen->format->BytesPerPixel);
                    }

                    if(rndval == 0)		// entire sequence has been completed
                        goto finished;
                }

                if(!i || first)
                   lastrndval = rndval;
            }

            // If there is no double buffering, we always use the "first frame"
            // case
            if(cfg_usedoublebuffering)
               first = 0;

            I_UnlockSurface(vid_screen);
            SDL_Flip(vid_screen);
        }
        else
        {
            // No surface, so only enhance rndval
            for(int i = first; i < 2; i++)
            {
                for(unsigned p = 0; p < pixperframe; p++)
                {
                    rndval = (rndval >> 1) ^ (rndval & 1 ? 0 : fiz_rndmask);
                    if(rndval == 0)
                        goto finished;
                }
            }
        }

        frame++;
        Delay(frame - GetTimeCount());        // don't go too fast
    } while (1);

finished:
    I_UnlockSurface(source);
    I_UnlockSurface(vid_screen);
    SDL_BlitSurface(source, NULL, vid_screen, NULL);
    SDL_Flip(vid_screen);
    return false;
}
