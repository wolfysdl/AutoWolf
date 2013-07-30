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
// Miscellaneous system operations. Inspired from Doom/Eternity's
//
////////////////////////////////////////////////////////////////////////////////

#include <sys/stat.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <dirent.h>
#include <unistd.h>
#endif
#include "wl_def.h"
#include "Config.h"
#include "i_system.h"
#include "macosx/CocoaFun.h"
#include "wl_draw.h"
#include "wl_main.h"

//
// I_Delay
//
// Delays the system for the given milliseconds
//
void I_Delay(unsigned ms)
{
   SDL_Delay((Uint32)ms);
}

//
// I_GetTicks
//
// Gets the number of ticks since start
//
uint32_t I_GetTicks(void)
{
   return (uint32_t)SDL_GetTicks();
}

//
// I_Notify
//
// Posts a notification, depending on platform
//
void I_Notify(const char *msg)
{
#ifdef __APPLE__
   Cocoa_Notify(SPEAR.FullTitle(), msg);
#endif
}

//
// I_MakeDir
//
// Makes a directory
//
Boolean I_MakeDir(const char *dirname)
{
#ifdef _WIN32
   return CreateDirectory(dirname, NULL);
#else
   return !mkdir(dirname, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
#endif
}

//
// I_ChangeDir
//
// Sets current dir
//
Boolean I_ChangeDir(const char *dirname)
{
#ifdef _WIN32
   return SetCurrentDirectory(dirname);
#else
   return !chdir(dirname);
#endif
}

//
// I_GetSettingsDir
//
// Gets the settings directory
//
PString I_GetSettingsDir()
{
#ifdef __APPLE__
   const char *appsupdir = Cocoa_ApplicationSupportDirectory();
   if(appsupdir == NULL)
      Quit("Your Application Support directory is not defined. You must "
           "set this before playing.");
   return appsupdir;
#elif defined(_WIN32)
   TCHAR appdatdir[MAX_PATH];
   if(!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdatdir)))
      Quit("Your Application Data directory is not defined. You must "
           "set this before playing.");
   return PString(appdatdir).concatSubpath("AutoWolf");
#else
   const char *homeenv = getenv("HOME");
   if(homeenv == NULL)
      Quit("Your $HOME directory is not defined. You must set this before "
           "playing.");
   return PString(homeenv).concatSubpath(".autowolf");
#endif

}

//
// I_ResolveCaseInsensitivePath
//
// Returns the first file in a case insensitive file system at path
// Designed to be used for finding files which weren't named with case sensiti-
// vity in mind.
//
// If there are several files with the same letters (different cases), the
// return value depends on readdir
//
// If directory can't be accessed or anything like that, just return what's
// given - this function doesn't do more than that.
//
PString I_ResolveCaseInsensitivePath(const char *dirname, const char *basename)
{
#ifndef _WIN32
   // POSIX
   DIR *dir = opendir(dirname);
   if(dir)
   {
      dirent *ent;
      while ((ent = readdir(dir)))
      {
         if(!strcasecmp(ent->d_name, basename))
            return PString(dirname).concatSubpath(ent->d_name);
      }
      closedir(dir);
   }
#endif
   // not found or indifferent
   return PString(dirname).concatSubpath(basename);

}

//
// I_CheckedMalloc
//
// Does malloc and quits if null. Replaced all CHECKMALLOCRESULT occurences with
// this. I lost the file and line information with this, though.
//
void *I_CheckedMalloc(size_t sz)
{
   void *ret = malloc(sz);
   if(!ret)
      Quit("Out of memory!");
   return ret;
}

//
// I_CheckedRealloc
//
// Same as above, but with realloc
//
void *I_CheckedRealloc(void *ptr, size_t sz)
{
   void *ret = realloc(ptr, sz);
   if(!ret)
      Quit("Out of memory!");
   return ret;
}

//
// I_DisplayAlertOnError
//
// 
//
void I_DisplayAlertOnError()
{
   if(global_error.length() <= 0)
      return;
#ifdef __APPLE__
   Cocoa_DisplayErrorAlert(global_error());
#endif
}