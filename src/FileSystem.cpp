// Copyright (C) 2014  Ioan Chera
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
// File system routines
//
////////////////////////////////////////////////////////////////////////////////

#include "FileSystem.h"
#include "StdStringExtensions.h"

std::string FileSystem::FindCaseInsensitive(const std::string& dirname, const std::string& basename)
{
#ifndef _WIN32
	// POSIX
	DIR *dir = opendir(dirname);
	if (dir)
	{
		PString ret(dirname);
		bool found = false;
		dirent *ent;
		while ((ent = readdir(dir)))
		{
			if (!strcasecmp(ent->d_name, basename))
			{
				ret.concatSubpath(ent->d_name);
				found = true;
				break;
			}
		}
		closedir(dir);
		if (found)
			return ret;
	}
#endif
	// not found or indifferent
	return ConcatSubpath(std::string(dirname), basename);
}