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
// MODULE FOR THE GAME BOT
//
////////////////////////////////////////////////////////////////////////////////

#include "wl_def.h"

#include "ioan_bot.h"
#include "ioan_bas.h"
#include "ioan_secret.h"
#include "obattrib.h"
#include "HistoryRatio.h"
#include "PathArray.h"
#include "CheckSum.h"
#include "MasterDirectoryFile.h"
#include "wl_act1.h"
#include "wl_draw.h"
#include "wl_game.h"
#include "wl_main.h"
#include "wl_play.h"
#include "wl_state.h"

BotMan bot;

// static class member definition
// protected ones

//
// BotMan::MapInit
//
// Initialize for each map load.
//
void BotMan::MapInit()
{
	shootRatio.initLength(10);
	retreat = 0;
	threater = NULL;
	retreatwaitdelay = 0;
	retreatwaitcount = 0;
	searchstage = SSGeneral;
	path.makeEmpty();
	panic = false;
	int i, j;
	
	memset(explored, 0, sizeof(explored));

    // IOANCH 20121213
	// calculate checksum of it
    CheckSum::CalculateMapsegsChecksum();
	// get explored data
	GetExploredData(CheckSum::digeststring);
	
	// FIXME: this should be taken from loaded game data. I might put a
    // LoadGameInit just for that.
	for(i = 0; i < MAPSIZE; ++i)
    {
		for(j = 0; j < MAPSIZE; ++j)
			enemyrecord[i][j].removeAll();
    }
}

//
// BotMan::SaveData
//
// Saves explored to data file (at death/victory, or quit -- must be ingame)
//
void BotMan::SaveData() const
{
	StoreAcquiredData(CheckSum::digeststring);
	MasterDirectoryFile::MainDir().saveToFile();
}

//
// BotMan::StoreAcquiredData
//
void BotMan::StoreAcquiredData(const uint8_t *digeststring) const
{
    // see if folder exists in AutoWolf/Maps
	
	// now to write the file
	DirectoryFile *dir = MasterDirectoryFile::MainDir()
    .makeDirectory(MASTERDIR_MAPSDIRECTORY)
    ->makeDirectory(PString((const char *)digeststring, 16));
	
    // the hash-named directory
    
    // Looking for files...
    // Get property file from digest folder.
    // If it exists, then access its hash table.
    // If it contains property with name "Explored", change it.
    // If it doesn't, create it with the proper name.
    // If file doesn't exist, create it and give it the explored property.
	
    MAKE_FILE(PropertyFile, propertyFile, dir, PROPERTY_FILE_NAME)
    
    propertyFile->putExplored(explored);
    propertyFile->setIntValue(PROPERTY_KEY_EXITPOS,
                              knownExitPoint.PropertySerializeTile());
}

//
// BotMan::GetExploredData
//
void BotMan::GetExploredData(const uint8_t *digeststring)
{
    // see if folder exists in AutoWolf/Maps
	memset(explored, 0, sizeof(maparea * sizeof(Boolean)));
	
	// now to write the file
	MasterDirectoryFile &mainDir = MasterDirectoryFile::MainDir();
	DirectoryFile *dir;
	
	dir = mainDir.makeDirectory(MASTERDIR_MAPSDIRECTORY);	// the Maps directory
	dir = dir->makeDirectory(PString((const char *)digeststring, 16));
    // the hash-named directory
    
    // Looking for files...
    // Get property file from digest folder.
    // If it exists, then access its hash table.
    // If it contains property with name "Explored", use it.
    // If it doesn't, initialize it empty.
    // If file doesn't exist, do likewise.
    
    PropertyFile *propertyFile =
    (PropertyFile *)dir->getFileWithName(PROPERTY_FILE_NAME);
    
    if(propertyFile)
    {
        propertyFile->getExplored(explored);
        if (propertyFile->hasProperty(PROPERTY_KEY_EXITPOS))
        {
            unsigned prop = (unsigned)propertyFile->getIntValue(PROPERTY_KEY_EXITPOS);
            knownExitPoint.PropertyDeserializeTile((int)prop);
        }
        else
        {
            knownExitPoint.SetValue(-1, -1);
        }
    }
}

//
// BotMan::SetMood
//
// Should always set the same value to mood, throughout the day
//
void BotMan::SetMood()
{
    time_t t = time(NULL);  // seconds
    time_t day = t / 60 / 60 / 24;
    
    // Day will be used as random seed
    srand((unsigned)day);
    mood = 0;   // default it to 0
    if(rand() % 4 == 0)
    {
        mood = (unsigned)rand();    // Just scramble it
    }
    
    // scramble it now
    srand((unsigned)time(NULL));
}

// Bot moods: using today value as seed, use a random strategy, one or more:
// - As soon as elevator is found, take it
// - Avoid taking the secret elevator (and if "as soon", don't take it)
// - Don't look for treasure
// - Don't actively hunt for Nazis (except boss)

//
// BotMan::ObjectOfInterest
//
// Pickable item, secret door, exit switch, exit pad
//
Boolean BotMan::ObjectOfInterest(const Point2D<int> &tPoint,
                                 Boolean knifeinsight)
{
    if(!tPoint.InRange(0, MAPSIZE - 1))
        return true;
	
	objtype *check = actorat[tPoint.x][tPoint.y];
	
	// unexplored tile that's unoccupied by a solid block
	if(!explored[tPoint.x][tPoint.y] && (!check || ISPOINTER(check)))
	{
		return true;
	}
	
	//int i;
    exitPoint = -1;

	// items
	
	byte itemnum;
//	assert((tx!=29 && tx!=38) || ty!=25 );
	for(itemnum = Basic::FirstObjectAt(tPoint.x, tPoint.y);
        itemnum;
        itemnum = Basic::NextObjectAt(tPoint.x, tPoint.y))
	{
		switch(itemnum)
		{
		case    bo_firstaid:
			if (gamestate.health <= 75)
				return true;
			break;
		case    bo_food:
			if (gamestate.health <= 90)
				return true;
			break;
		case    bo_alpo:
			if (gamestate.health <= 96)
				return true;
			break;
		case    bo_key1:
		case    bo_key2:
		case    bo_key3:
		case    bo_key4:
			if(!knifeinsight && !(gamestate.keys & (1<<(itemnum - bo_key1))))
				return true;
			break;
		case    bo_cross:
		case    bo_chalice:
		case    bo_bible:
		case    bo_crown:
			if(!knifeinsight && !(mood & MOOD_DONTHUNTTREASURE))
				return true;
			break;
		case    bo_machinegun:
			if(gamestate.bestweapon < wp_machinegun || gamestate.ammo <= 80)
				return true;
			break;
		case    bo_chaingun:
			if(gamestate.bestweapon < wp_chaingun || gamestate.ammo <= 80)
				return true;
			break;
		case    bo_fullheal:
			return true;
		case    bo_clip:
			if(gamestate.ammo < 80)
				return true;
			break;
		case    bo_clip2:
			if (gamestate.ammo < 80)
				return true;
			break;
// IOANCH 20130202: unification process
		case    bo_25clip:
			if (gamestate.ammo <= 74)
				return true;
			break;
		case    bo_spear:
			if(searchstage >= SSSecretLift || mood & MOOD_TAKEFIRSTEXIT)
				return true;
			break;
                
		case    bo_gibs:
			if (gamestate.health <= 10)
				return true;
			break;
		}
	}


	// look for enemy in remembered record
	check = enemyrecord[tPoint.x][tPoint.y].firstObject();
	
	// only look for check stuff
	if(gamestate.health > 75 && gamestate.ammo > 50 &&
       explored[tPoint.x][tPoint.y] && check)
	{
		while(check && (!Basic::IsEnemy(check->obclass) ||
                        !(check->flags & FL_SHOOTABLE)))
		{
			enemyrecord[tPoint.x][tPoint.y].remove(check);
            // flush dead/invalid records
			check = enemyrecord[tPoint.x][tPoint.y].firstObject();
		}
		if(!(mood & MOOD_DONTHUNTNAZIS) && check)
		{
			searchstage = SSGeneral;	// reset counter if enemies here
			return true;
		}
	}

    // {ty + 1, ty - 1, ty, ty}
    // {tx, tx, tx + 1, tx - 1}
    // {ty + 2, ty - 2, tx + 2, tx - 2}
    // {tx, tx, tx + 2, tx - 2}
    // {ty + 2, ty - 2, ty, ty}
    // {ty + 3, ty - 3, tx + 3, tx - 3}
    // {< MAPSIZE, >= 0, < MAPSIZE, >= 0}
    // {tx, tx, tx + 3, tx - 3}
    // {ty + 3, ty - 3, ty, ty}
    // {ty + 1, ty - 1, ty, ty}
    // {tx, tx, tx + 1, tx - 1}
    
    // Nope. Let's make a lambda function here.
    auto secretVerify = [&](const Point2D<int> &tPointOfs) -> Boolean
    {
        if(*(mapsegs[1] + (tPoint + tPointOfs).MapUnfold(mapshift))
           == PUSHABLETILE)
        {
            Point2D<int> tPoint2Ofs = tPoint.Added(2 * tPointOfs.x,
                                                   2 * tPointOfs.y);
            Point2D<int> tPoint3Ofs = tPoint.Added(3 * tPointOfs.x,
                                                   3 * tPointOfs.y);
			if(!actorat[tPoint2Ofs.x][tPoint2Ofs.y]
               && tPoint2Ofs.InRange(0, MAPSIZE - 1))
			{
				if(searchstage >= SSOnePushWalls ||
                   (!actorat[tPoint3Ofs.x][tPoint3Ofs.y] &&
                    tPoint3Ofs.InRange(0, MAPSIZE - 1)))
				{
                    exitPoint.SetValue(tPoint + tPointOfs);
					return true;
				}
			}
		}
        return false;
    };
    
	// secret door
	if(!knifeinsight && (!(mood & MOOD_DONTHUNTSECRETS) || searchstage >= SSMax))
        // don't look for secret doors if compromised
	{
		// PUSH SOUTH
        if (secretVerify(Point2D<int>::Make(0, 1)))
            return true;
        if (secretVerify(Point2D<int>::Make(0, -1)))
            return true;
        if (secretVerify(Point2D<int>::Make(1, 0)))
            return true;
        if (secretVerify(Point2D<int>::Make(-1, 0)))
            return true;
	}
    
    auto exitVerify = [&](int ofs) -> Boolean
    {
        int ofsadd = ofs + tPoint.x, ofsadd2 = ofsadd + ofs;
        if(ofsadd >= 0 && tilemap[ofsadd][tPoint.y] == ELEVATORTILE)
		{
			if (*(mapsegs[1]+ tPoint.MapUnfold(mapshift)+ofs) != PUSHABLETILE
                || ofsadd2 < 0 || !actorat[ofsadd2][tPoint.y])
			{
                exitPoint.SetValue(ofsadd, tPoint.y);
                knownExitPoint.SetValue(exitPoint);
				if(*(mapsegs[0] + tPoint.MapUnfold(mapshift)) == ALTELEVATORTILE
                   || searchstage >= SSNormalLift || mood & MOOD_TAKEFIRSTEXIT)
					return true;
			}
		}
        return false;
    };

	// exit switch
	if(searchstage >= SSSecretLift || mood & MOOD_TAKEFIRSTEXIT)
	{
		
		// THROW WEST
        if(exitVerify(-1))
            return true;
        if(exitVerify(1))
            return true;

		// exit pad
		if(searchstage >= SSNormalLift || mood & MOOD_TAKEFIRSTEXIT)
           if(*(mapsegs[1]+tPoint.MapUnfold(mapshift)) == EXITTILE)
			return true;
	}
	return false;
}

//
// BotMan::ExploreFill
//
// Recursively explores from the given origin
//
void BotMan::ExploreFill(const Point2D<int> &tPoint, const Point2D<int> &oPoint,
                         Boolean firstcall)
{
    if(!tPoint.InRange(0, MAPSIZE - 1))
        return;
	
	static Boolean explore_visited[MAPSIZE][MAPSIZE];
	if(firstcall)	// origin
	{
		memset(explore_visited, 0, maparea*sizeof(Boolean));
	}
	
	if(explore_visited[tPoint.x][tPoint.y])
		return;
	explore_visited[tPoint.x][tPoint.y] = true;
	
	objtype *check = actorat[tPoint.x][tPoint.y];
	if(check && !ISPOINTER(check))
	{
		if(tilemap[tPoint.x][tPoint.y] < AREATILE)	// is a wall
			return;
	}
	
	if(Basic::GenericCheckLine(oPoint, tPoint))
	{
		explored[tPoint.x][tPoint.y] = true;
		
        ExploreFill(tPoint.Added(-1, 0), oPoint);
        ExploreFill(tPoint.Added( 1, 0), oPoint);
        ExploreFill(tPoint.Added( 0,-1), oPoint);
        ExploreFill(tPoint.Added( 0, 1), oPoint);
	}
}

//
// BotMan::FindShortestPath
//
// Finds the path to the nearest destination
//
Boolean BotMan::FindShortestPath(Boolean ignoreproj, Boolean mindnazis,
                                 byte retreating, Boolean knifeinsight)
{
	int j;
	
	// if stepped and reached on a former enemy position, remove it
	enemyrecord[player->tilex][player->tiley].removeAll();
	
	// if stepped on unexplored terrain, do a scan
	if(!explored[player->tilex][player->tiley])
		ExploreFill(Point2D<int>::Make(player->tilex, player->tiley),
                    Point2D<int>::Make(player->tilex, player->tiley), true);

	path.makeEmpty();
	if(retreating && threater != NULL)
		path.addStartNode(player->tilex, player->tiley, threater->tilex, threater->tiley, true);
	else if(mood & MOOD_JUSTGOTOEXIT && knownExitPoint > -1)
        path.addStartNode(player->tilex, player->tiley,
                          knownExitPoint.x, knownExitPoint.y);
    else
		path.addStartNode(player->tilex, player->tiley);

	int imin, ifound;
	int tx, ty, cx, cy;
	objtype *check, *check2;
	int tentative_g_add;
	
	while(1)
	{
		// This finds the index of the best score node
		imin = path.bestScoreIndex();
		if(imin < 0)
			break;
		
		path.closeNode(imin);
		
		path.getCoords(imin, &tx, &ty);
		
		// This checks if a destination has been found on the spot
		if(!retreating)
		{
			
			if(ObjectOfInterest(Point2D<int>::Make(tx, ty), knifeinsight))
			{
				// found goal
				path.finish(imin);
				return true;
			}
		}
		else
		{
			int iminprev = path.getPrevIndex(imin);
			if(iminprev >= 0)
			{
				int ptx, pty;
				path.getCoords(iminprev, &ptx, &pty);
				if(!Crossfire(Point2D<int>::Make(Basic::Major(tx), Basic::Major(ty))) &&
				   !Crossfire(Point2D<int>::Make(Basic::Major(ptx), Basic::Major(pty))))
				{
					path.finish(imin);
					return true;
				}
			}
		}

		// This looks in all eight directions.
		for(j = 0; j < 8; ++j)
		{
			switch(j)
			{
			case 0:
				cx = tx + 1;
				cy = ty;
				break;
			case 1:
				cx = tx;
				cy = ty + 1;
				break;
			case 2:
				cx = tx - 1;
				cy = ty;
				break;
			case 3:
				cx = tx;
				cy = ty - 1;
				break;
			case 4:	// up-right
				cx = tx + 1;
				cy = ty - 1;
				break;
			case 5:	// up-left
				cx = tx - 1;
				cy = ty - 1;
				break;
			case 6:	// down-left
				cx = tx - 1;
				cy = ty + 1;
				break;
			case 7:	// down-right
				cx = tx + 1;
				cy = ty + 1;
				break;
			}
			
			if(j >= 4 && j < 8)
			{
				if(retreating)
					continue;	// Don't go diagonally if looking for retreat
				check = actorat[cx][ty];
				check2 = actorat[tx][cy];
				if((check && !ISPOINTER(check)) || (check2 && !ISPOINTER(check2)))
				{
					continue;
				}
			}
			
			check = actorat[cx][cy];
			byte door = tilemap[cx][cy];
			
			// This checks for any blocking device: refactor it
			if((check && !ISPOINTER(check) && !(door & 0x80)) ||
               ((abs(cx - player->tilex) > 1 || abs(cy - player->tiley) > 1) && 
                !ignoreproj && IsProjectile(Point2D<int>::Make(cx, cy), 1)) ||
               (mindnazis && (player->tilex != tx || player->tiley != ty || 
                              (check && ISPOINTER(check) && 
                               check->flags & FL_SHOOTABLE)) && 
                IsEnemyBlocking(Point2D<int>::Make(cx, cy))))
			{
				continue;	// solid, can't be passed
			}
			else if (door & 0x80)
			{
				// This checks if it's a door and can be passed
				objtype *checkindoor = actorat[cx][cy];
				
				// Don't attempt to retreat (backpedal) through closed or blocked doors
				// Only if retreating == 1
				if(retreating == 1 && checkindoor && (!ISPOINTER(checkindoor) || (ISPOINTER(checkindoor) && checkindoor != player && checkindoor->hitpoints > 0)))
				{
					continue;
				}
				
				// Don't attempt to pass through locked doors without having the key
				// FIXME: take care with open locked doors, still without the key, not to get stuck
				door = door & ~0x80;
				byte lock = doorobjlist[door].lock;
				if (lock >= dr_lock1 && lock <= dr_lock4)
					if (doorobjlist[door].action != dr_open && doorobjlist[door].action != dr_opening && !(gamestate.keys & (1 << (lock-dr_lock1) ) ) )
					{
						continue;
					}
			}

			// this looks if the entry exists in the list
			ifound = path.openCoordsIndex(cx, cy);
			
			if(ifound == -2)	// found it as closed. Don't try to visit it again, so pass
				continue;
			
			// This sets the score, depending on situation
			if(j < 4)
				tentative_g_add = 0x100;
			else {
				tentative_g_add = Basic::ApproxDist(0x100, 0x100);
			}

			// Increase the effective (time) distance if a closed door is in the way
			if((door & 0x80 && doorobjlist[door].action != dr_open && doorobjlist[door].action != dr_opening) ||
               Crossfire(Point2D<int>::Make(Basic::Major(cx), Basic::Major(cy)), NULL, true))
				tentative_g_add <<= 2;
			
			if(retreating && threater != NULL)
				path.updateNode(ifound, imin, cx, cy, tentative_g_add, threater->tilex, threater->tiley, true);
			else if(mood & MOOD_JUSTGOTOEXIT && knownExitPoint > -1)
                path.updateNode(ifound, imin, cx, cy, tentative_g_add,
                                knownExitPoint.x, knownExitPoint.y);
            else
				path.updateNode(ifound, imin, cx, cy, tentative_g_add);
			
		}
	}

	if(!pwallstate)
		searchstage = (SearchStage)((int)searchstage + 1);

	return false;
}

//
// BotMan::EnemyOnTarget
//
// True if can shoot
//
objtype *BotMan::EnemyOnTarget(Boolean solidActors) const
{
	objtype *closest,*oldclosest;
	int32_t  viewdist;

	//
	// find potential targets
	//
	viewdist = 0x7fffffffl;
	closest = NULL;

	while (1)
	{
		oldclosest = closest;

		for(objtype *
            check = (objtype *)Basic::livingNazis.firstObject(); check;
            check = (objtype *)Basic::livingNazis.nextObject())
		{
			if ((check->flags & FL_VISABLE)
                && abs(check->viewx-centerx) < shootdelta)
			{
				if (check->transx < viewdist)
				{
					viewdist = check->transx;
					closest = check;
				}
			}
		}

		if (closest == oldclosest)
			return NULL;
            // no more targets, all missed
        
		//
		// trace a line from player to enemey
		//

		if (CheckLine(closest, solidActors))
			return closest;
	}
	return NULL;
}

//
// BotMan::RecordEnemyPosition
//
// Update the enemy's known position record
//

void BotMan::RecordEnemyPosition(objtype *enemy)
{
	// hack: recordx and recordy if 0 are handled as if not set. Bite me.
	
	if(enemy->recordx == enemy->x && enemy->recordy == enemy->y)
		return;	// change nothing

	if(enemy->recordx > 0 && enemy->recordy > 0)
	{
		enemyrecord[enemy->recordx >> TILESHIFT][enemy->recordy >> TILESHIFT].remove(enemy);
	}
	enemy->recordx = enemy->x;
	enemy->recordy = enemy->y;
	enemyrecord[enemy->tilex][enemy->tiley].add(enemy);
}
//
// BotMan::EnemyVisible
//
// True if an enemy is in sight
//
objtype *BotMan::EnemyVisible(short *angle, int *distance, Boolean solidActors)
{
	int tx = player->tilex, ty = player->tiley;
	int i, j, k, distmin;
	objtype *ret, *retmin = NULL;
	short angmin;
	static objtype *oldret;
	double dby, dbx;

	distmin = INT_MAX;

	for(ret = (objtype *)Basic::livingNazis.firstObject(); ret; ret = (objtype *)Basic::livingNazis.nextObject())
	{
		k = abs(ret->tilex - tx);
		j = abs(ret->tiley - ty);
		i = k > j ? k : j;

		if(i > 15)
			continue;
		if(!CheckLine(ret, solidActors))
			continue;
		
		// ret visible here: handle their updated location
		RecordEnemyPosition(ret);

		// don't change target if distance difference is too little (don't lose focus on the current threat)
		if((abs(*distance - i) >= 2 && ret != oldret) || ret == oldret || (oldret && !(oldret->flags & FL_SHOOTABLE)))
		{
			// choose closest target
			if(i <= distmin)
			{
				// set angle towards ret
				dby = -((double)(ret->y) - (double)(player->y));
				dbx = (double)(ret->x) - (double)(player->x);
				distmin = i;
				angmin = (short)(180.0/PI*atan2(dby, dbx));

				if(angmin >= 360)
					angmin -= 360;
				if(angmin < 0)
					angmin += 360;

				retmin = ret;
			}
		}
	}

	// found a target
	if(retmin)
	{
		oldret = retmin;
		*angle = angmin;
		*distance = distmin;
	}

	return retmin;
}

//
// BotMan::EnemyEager
//
// True if 1 non-ambush enemies are afoot 
//
objtype *BotMan::EnemyEager() const
{
	for(objtype *
        ret = (objtype *)Basic::livingNazis.firstObject(); ret;
        ret = (objtype *)Basic::livingNazis.nextObject())
	{
		// TODO: don't know about unexplored enemies (consider that it will know if map gets revisited)
		if(areabyplayer[ret->areanumber] && explored[ret->recordx >> TILESHIFT][ret->recordy >> TILESHIFT] && !(ret->flags & (FL_AMBUSH | FL_ATTACKMODE)))
			return ret;
	}
	return NULL;
}

//
// BotMan::DamageThreat
//
// true if damage is imminent and retreat should be done
//
objtype *BotMan::DamageThreat(const objtype *targ) const
{
	const objtype *objignore = targ;
	if(targ && (Basic::IsBoss(targ->obclass) || (targ->obclass == mutantobj && 
        gamestate.weapon < wp_machinegun && gamestate.weaponframe != 1) 
        /*|| (gamestate.weapon < wp_pistol && Basic::CheckKnifeEnemy() != targ)*/))
	{
		objignore = NULL;
	}
	return Crossfire(Point2D<int>::Make(player->x, player->y), objignore);
}

//
// BotMan::Crossfire
//
// Returns true if there's a crossfire in that spot
//
objtype *BotMan::Crossfire(const Point2D<int> &point, const objtype *objignore,
                           Boolean justexists) const
{
	int j, k, dist;
	objtype *ret;

	for(ret = (objtype *)Basic::livingNazis.firstObject(); ret; 
        ret = (objtype *)Basic::livingNazis.nextObject())
	{
		if(!areabyplayer[ret->areanumber] || ret == objignore)
			continue;
		k = (ret->x - point.x) >> TILESHIFT;
		j = (ret->y - point.y) >> TILESHIFT;
		dist = abs(j) > abs(k) ? abs(j) : abs(k);
		if(dist > atr::threatrange[ret->obclass]
           || !Basic::GenericCheckLine(Point2D<int>::Make(ret->recordx,
                                                       ret->recordy), point))
			continue;

		if(Basic::IsDamaging(ret, dist) || justexists)
			return ret;
		
	}
	return NULL;
}

//
// BotMan::DoRetreat
//
// Retreats the bot sliding off walls
//
void BotMan::DoRetreat(Boolean forth, objtype *cause) const
{
	int neg = forth? -1 : 1;
	controly = neg * RUNMOVE * tics;
	int j, backx, backy, sidex, sidey,
        tx = player->tilex, ty = player->tiley, dir;
    
    const int backx_[] = {-neg, 0,    0,   neg, neg,  0,    0,    -neg};
    const int backy_[] = {0,    neg,  neg, 0,   0,    -neg, -neg, 0};
    const int sidex_[] = {0,    -neg, neg, 0,   0,    neg,  -neg, 0};
    const int sidey_[] = {neg,  0,    0,   neg, -neg, 0,    0,    -neg};
    const int dir_[]   = {RUNMOVE * neg, -RUNMOVE * neg};
    
    for(int factor = 0; factor < 8; ++factor)
    {
        if(player->angle >= 45 * factor && player->angle <= 45 * factor + 45)
        {
            backx = backx_[factor];
            backy = backy_[factor];
            sidex = sidex_[factor];
            sidey = sidey_[factor];
            dir = dir_[factor % 2];
            break;
        }
    }
    
	if(tx <= 0 || tx >= MAPSIZE - 1 || ty <= 0 || ty >= MAPSIZE - 1)
		return;

	objtype *check1 = actorat[tx + backx][ty + backy], *check2;
	if((check1 && !ISPOINTER(check1)) || (check1 && ISPOINTER(check1) && check1->flags & FL_SHOOTABLE))
	{
		// Look for way backwards
		if(sidex)	// strafing east or west
		{
			for(j = tx + sidex; ; j += sidex)
			{
				if(j >= MAPSIZE)
					break;
				check2 = actorat[j][ty];
				if((check2 && !ISPOINTER(check2)) || (check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE))
					break;	// stopped

				check2 = actorat[j][ty + backy];
				if(!check2 || (check2 && ISPOINTER(check2) && (check2->obclass == inertobj || !(check2->flags & FL_SHOOTABLE))))
					goto solved;// found here
			}
			for(j = tx - sidex; ; j -= sidex)
			{
				if(j < 0)
					break;
				check2 = actorat[j][ty];
				if((check2 && !ISPOINTER(check2)) || (check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE))
					break;

				check2 = actorat[j][ty + backy];
				if(!check2 || (check2 && ISPOINTER(check2) && (check2->obclass == inertobj || !(check2->flags & FL_SHOOTABLE))))
				{
					dir = -dir;// found here
					sidex = -sidex;
					sidey = -sidey;
					controly = 0;
					break;
				}
			}
		}
		else if(sidey)
		{
			for(j = ty + sidey; ; j += sidey)
			{
				if(j >= MAPSIZE)
					break;
				check2 = actorat[tx][j];
				if((check2 && !ISPOINTER(check2)) || (check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE))
					break;	// stopped

				check2 = actorat[tx + backx][j];
				if(!check2 || (check2 && ISPOINTER(check2) && (check2->obclass == inertobj || !(check2->flags & FL_SHOOTABLE))))
					goto solved;// found here
			}
			for(j = ty - sidey; ; j -= sidey)
			{
				if(j < 0)
					break;
				check2 = actorat[tx][j];
				if((check2 && !ISPOINTER(check2)) || (check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE))
					break;

				check2 = actorat[tx + backx][j];
				if(!check2 || (check2 && ISPOINTER(check2) && (check2->obclass == inertobj || !(check2->flags & FL_SHOOTABLE))))
				{
					dir = -dir;// found here
					sidex = -sidex;
					sidey = -sidey;
					controly = 0;
					break;
				}
			}
		}
solved:
		check2 = actorat[tx + sidex][ty + sidey];
		if((check2 && !ISPOINTER(check2)) || (check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE))
			return;
		buttonstate[bt_strafe] = true;
		controlx = dir*tics;
	}
	else
		buttonstate[bt_strafe] = false;
}

//
// BotMan::IsProjectile
//
// True if a flying projectile exists in this place
//
objtype *BotMan::IsProjectile(const Point2D<int> &tPoint, int dist, short *angle,
                              int *distance) const
{
	objtype *ret;

	for(ret = (objtype *)Basic::thrownProjectiles.firstObject(); ret;
		ret = (objtype *)Basic::thrownProjectiles.nextObject())
	{
		if(abs(ret->tilex - tPoint.x) <= dist && abs(ret->tiley - tPoint.y) <= dist)
		{
			switch(ret->obclass)
			{
                    // IOANCH 20130202: unification process
			case needleobj:
			case fireobj:
				goto retok;
			case rocketobj:
				if(ret->state != &s_boom1 && ret->state != &s_boom2 && ret->state != &s_boom3)
					goto retok;
				break;

			case hrocketobj:
				if(ret->state != &s_hboom1 && ret->state != &s_hboom2 && ret->state != &s_hboom3)
					goto retok;
				break;
			case sparkobj:
				goto retok;

				default:
					;
			}
		}
	}
	return NULL;
retok:


	if(angle && distance)
	{
		
		double dby = -((double)(ret->y) - (double)(player->y));
		double dbx = (double)(ret->x) - (double)(player->x);

		*angle = (short)(180.0/PI*atan2(dby, dbx));
		*distance = dist;
					
		if(*angle >= 360)
			*angle -= 360;
		if(*angle < 0)
			*angle += 360;
	}

	return ret;
}

//
// BotMan::IsEnemyBlocking
//
// True if a living enemy exists around
//
objtype *BotMan::IsEnemyBlocking(const Point2D<int> &tPoint) const
{
	objtype *ret;
	
    Point2D<int> point = Basic::Major(tPoint);
	
	for(ret = (objtype *)Basic::livingNazis.firstObject(); ret; ret = (objtype *)Basic::livingNazis.nextObject())
	{
		if(abs(ret->x - point.x) < 1<<TILESHIFT &&
           abs(ret->y - point.y) < 1<<TILESHIFT)
//			if(ret->flags & FL_ATTACKMODE)
				return ret;
	}
	return NULL;
}

//
// BotMan::IsEnemyNearby
//
// True if a living enemy exists around
//
objtype *BotMan::IsEnemyNearby(const Point2D<int> &tPoint) const
{
	objtype *ret;
	
	for(ret = (objtype *)Basic::livingNazis.firstObject(); ret; ret = (objtype *)Basic::livingNazis.nextObject())
	{
		if(abs(ret->tilex - tPoint.x) <= 1 && abs(ret->tiley - tPoint.y) <= 1)
			return ret;
	}
	return NULL;
}

//
// BotMan::MoveByStrafe
//
// Move by strafing. Only searchset[1] is relevant.
//
void BotMan::MoveByStrafe()
{
	int movedir;
	
	if(!path.length())
	{
		DoRetreat();
		return;
	}
	
	int nx, ny, mx, my;
	Boolean tryuse = false;	// whether to try using
	
	mx = player->tilex;
	my = player->tiley;
	
	// elevator
	byte tile;
	int nexton = path.getNextIndex(0);
	if(nexton == -1)
	{
		// end of path; start a new search
		path.reset();
		if(exitPoint >= 0)
		{
			// elevator switch: press it now
			nx = exitPoint.x;
			ny = exitPoint.y;
			tryuse = true;
		}
		else
		{
			// if undefined, just go east
			return;	// just quit
			// nx = mx + 1;
			// ny = my;
		}
	}
	else
	{
		// in the path
		path.getCoords(nexton, &nx, &ny);
		tile = tilemap[nx][ny];
		// whether to press to open a door
		tryuse = (tile & 0x80) == 0x80 
		&& (doorobjlist[tile & ~0x80].action == dr_closed || doorobjlist[tile & ~0x80].action == dr_closing);
	}
	
	// set up the target angle
	// int tangle = Basic::DirAngle(mx,my,nx,ny);
	int tangle = 0;
	if(nx > mx)
		tangle = 0;
	else if(ny > my)
		tangle = 270;
	else if(nx < mx)
		tangle = 180;
	else if(ny < my)
		tangle = 90;
	int dangle = Basic::CentreAngle(tangle, player->angle);
	
	// STRAFE LOGIC
	
	// if it's door
	if(actorat[nx][ny] && !ISPOINTER(actorat[nx][ny]))
		movedir = 0;
	else if(dangle >= -45 && dangle < 45)
	{
//		dangle -= 45;
		movedir = 1;
	}
	else if(dangle >= 45 && dangle < 135)
	{
		dangle -= 90;
		movedir = 2;	// left
	}
	else if(dangle >= 135 || dangle < -135)
	{
		dangle -= 180;
		if(dangle <= -180)
			dangle += 360;
		movedir = 3;	// back
	}
	else if(dangle >= -135 && dangle < -45)
	{
		dangle += 90;
		movedir = 4;	// right
	}
	
	// Move forward only if it's safe (replace this bloatness with a function)
	
	if(dangle > -45 && dangle < 45)
	{
		// So move
		buttonstate[bt_strafe] = true;
		switch(movedir)
		{
			case 0:
			case 1:
				controly = -RUNMOVE * tics;
				//controly = 0;
				buttonstate[bt_strafe] = false;
				break;
			case 2:	// left
				controly = 0;
				controlx = -RUNMOVE * tics;
				break;
			case 3:	// back
				controly =  RUNMOVE * tics;
				buttonstate[bt_strafe] = false;
				break;
			case 4:	// right
				controly = 0;
				controlx =  RUNMOVE * tics;
				break;
		}
		// Press if there's an obstacle ahead
		if(tryuse && (actorat[nx][ny] && !ISPOINTER(actorat[nx][ny]))
           && pressuse % 4 == 0)
			buttonstate[bt_use] = true;
		else
			buttonstate[bt_use] = false;
	}
	
	if(dangle > 15)
	{
		controlx = -RUNMOVE * tics;
		buttonstate[bt_strafe] = false;
	}
	else if(dangle < -15)
	{
		controlx = +RUNMOVE * tics;
		buttonstate[bt_strafe] = false;
	}
	else
	{
		buttonstate[bt_strafe] = true;
		// straight line: can strafe now
		fixed centx, centy, cento, plro;
		centx = (player->tilex << TILESHIFT);
		centy = (player->tiley << TILESHIFT);
		
		switch(tangle)
		{
			case 0:
				cento = -centy;
				plro = -player->y + (1<<(TILESHIFT - 1));
				break;
			case 90:
				cento = -centx;
				plro = -player->x + (1<<(TILESHIFT - 1));
				break;
			case 180:
				cento = centy;
				plro = player->y - (1<<(TILESHIFT - 1));
				break;
			case 270:
				cento = centx;
				plro = player->x - (1<<(TILESHIFT - 1));
				break;
		}
		if(plro - cento > 4096)	// too left, move right
		{
			switch(movedir)
			{
				case 0:
				case 1:
					controlx = BASEMOVE * tics;
					break;
				case 2:	// left, move forward
					controly = -BASEMOVE * tics;
					break;
				case 3:	// back, move left
					controlx = -BASEMOVE * tics;
					break;
				case 4:	// right, move back
					controly =  BASEMOVE * tics;
			}
		}
		else if(plro - cento < -4096)	// too right, move left
		{
			switch(movedir)
			{
				case 0:
				case 1:
					controlx = -BASEMOVE * tics;
					break;
				case 2:	// left, move back
					controly =  BASEMOVE * tics;
					break;
				case 3:	// back-right
					controlx =  BASEMOVE * tics;
					break;
				case 4:	// right, move forth
					controly = -BASEMOVE * tics;
			}
		}
	}
}

//
// BotMan::ChooseWeapon
//
void BotMan::ChooseWeapon() const
{
	if(gamestate.ammo > 0)
	{
		if(gamestate.weapon == wp_knife)
			buttonstate[bt_readyknife + gamestate.bestweapon - wp_knife] = true;
		if(gamestate.bestweapon == wp_chaingun)
		{
			if(!panic && gamestate.weapon != wp_machinegun
               && (gamestate.ammo < 30 || shootRatio.getValue() <= 2))
				buttonstate[bt_readymachinegun] = true;
			else if(gamestate.weapon != wp_chaingun
                    && (panic || (gamestate.ammo >= 50
                                  && shootRatio.getValue() > 7)))
				buttonstate[bt_readychaingun] = true;
		}
	}
}

//
// BotMan::TurnToAngle
//
void BotMan::TurnToAngle(int dangle) const
{
	buttonstate[bt_strafe] = false;

	if(dangle > 15)
		controlx = -RUNMOVE * tics;
	else if(dangle > 0)
		controlx = -BASEMOVE * tics;
	else if(dangle < -15)
		controlx = +RUNMOVE * tics;
	else if(dangle < 0)
		controlx = +BASEMOVE * tics;
}

//
// BotMan::DoMeleeAI
//
// Use the knife if the gun won't work
//
void BotMan::DoMeleeAI(short eangle, int edist)
{
    // Check to see if there's a visible enemy
//    objtype *check = EnemyVisible(&eangle, &edist, true);
    
    int dangle = Basic::CentreAngle(eangle, player->angle);
    TurnToAngle(dangle);
    objtype *check = EnemyOnTarget();
    objtype *check2 = DamageThreat(check);
//    if(!check)
//    {
//        if(FindShortestPath(false, true, 0, true))
//		{
//			MoveByStrafe();
//			return ;
//		}    
//    }
    
    if(check2 && (check2 != check || (Basic::IsBoss(check->obclass) || (check->obclass == mutantobj && gamestate.weapon < wp_machinegun && gamestate.weaponframe != 1))))
	{
        if(FindShortestPath(false, true, 0, true))
        {
            MoveByStrafe();
            return ;
        }
        else
        {
            threater = check2;
            if(FindShortestPath(false, true, 1))
            {
                panic = false;
                MoveByStrafe();
            }
            else
            {
                panic = true;
                if(FindShortestPath(false, true, 2))	// try to pass by opening doors now
                    MoveByStrafe();
                else
                    controly = RUNMOVE*tics;
                //DoRetreat(false, check2);
            }

            retreat = 10;
            if(check2->obclass == mutantobj)
                retreat = 5;
            return;
        }
	}
	else
		retreat = 0;
    
    buttonstate[bt_strafe] = false;
    if(check)
    {
        if(pressuse % 4 == 0 && edist <= 3)
			buttonstate[bt_attack] = true;
        if (dangle > -45 && dangle < 45)
        {
            controly = -RUNMOVE * tics;
//            buttonstate[bt_strafe] = true;
            controlx += (US_RndTBot() % 30 - 15) * tics;
            
        }
    }


	return ;
}

//
// BotMan::DoCombatAI
//
void BotMan::DoCombatAI(int eangle, int edist)
{
	// Set correct weapon
	ChooseWeapon();
	
	searchstage = SSGeneral;	// reset elevator counter if distracted
	
	
	path.reset();
	// Enemy visible mode
    // centred angle here
    if(gamestate.weapon == wp_knife)
    {
		DoMeleeAI((short)eangle, edist);
        return;
    }
    
	int dangle = Basic::CentreAngle(eangle, player->angle);
	
	TurnToAngle(dangle);
	
	objtype *check = EnemyOnTarget();
	objtype *check2 = DamageThreat(check);
	short int pangle, epangle; 
	int proj = 0;
	
	if(!check2)
	{
		check2 = IsProjectile(Point2D<int>::Make(player->tilex, player->tiley),
                              2, &pangle, &proj);
		proj = 1;
	}
	
	if(check2 && (check2 != check || (Basic::IsBoss(check->obclass) || (check->obclass == mutantobj && gamestate.weapon < wp_machinegun && gamestate.weaponframe != 1))) && gamestate.weapon != wp_knife)
	{
		threater = check2;
		if(FindShortestPath(false, true, 1))
		{
			panic = false;
			MoveByStrafe();
		}
		else
		{
			panic = true;
			if(FindShortestPath(false, true, 2))	// try to pass by opening doors now
				MoveByStrafe();
			else
				controly = RUNMOVE*tics;
			//DoRetreat(false, check2);
		}
		
		// FIXME: make complete projectile avoider
		if(proj)
		{
			epangle = pangle - player->angle;
			if(epangle > 180)
				epangle -= 360;
			else if(epangle <= -180)
				epangle += 360;
			buttonstate[bt_strafe] = true;
			if(epangle >= 0)	// strafe right
				controlx = RUNMOVE * tics;
			else
				controlx = -RUNMOVE * tics;
		}
		retreat = 10;
		if(check2->obclass == mutantobj)
			retreat = 5;
	}
	else
		retreat = 0;
	
	if(!check && dangle >= -5 && dangle <= 5 && retreat <= 0 &&
       !IsEnemyNearby(Point2D<int>::Make(player->tilex, player->tiley)))
	{
		// Do NOT charge if there's a risk ahead!
		fixed plx = player->x, ply = player->y;
		plx += 3*costable[player->angle]/2;
		ply -= 3*sintable[player->angle]/2;
		
		if(!Crossfire(Point2D<int>::Make(plx, ply)))
			controly = -BASEMOVE * tics;	// something's wrong, so move a bit
	}
	
	if(check)
	{
		// shoot according to how the weapon works
		if(((gamestate.weapon <= wp_pistol && pressuse % 4 == 0) || gamestate.weapon > wp_pistol) && edist <= 10)
			buttonstate[bt_attack] = true;
		
		if((retreat <= 0 && (!check2 || check2 == check) && edist > 6) || 
		   (dangle > -45 && dangle < 45 && gamestate.weapon == wp_knife))
		{
			
			// otherwise
			if(gamestate.weapon == wp_knife)
				controly = -RUNMOVE * tics;
			else
			{
				// Do NOT charge if there's a risk ahead!
				fixed plx = player->x, ply = player->y;
				plx += 3*costable[player->angle]/2;
				ply -= 3*sintable[player->angle]/2;
				
				if(!Crossfire(Point2D<int>::Make(plx, ply), check))
					DoRetreat(true);
			}
		}
	}
}

//
// BotMan::DoNonCombatAI
//
void BotMan::DoNonCombatAI()
{
	
	// FIXME: don't go through open locked doors if there's no escape the other side!
	panic = false;
	// no path got defined
	threater = NULL;
	if(!path.exists())
	{
		if(!FindShortestPath(false, gamestate.weapon == wp_knife))
		{
			if(!FindShortestPath(true, gamestate.weapon == wp_knife))
				return;
		}
	}
	
	static Boolean waitpwall;
	if(pwallstate)
		waitpwall = true;
	
	// found path to exit
	int nowon = path.pathCoordsIndex(player->tilex, player->tiley);
	
	if(nowon < 0 || (!pwallstate && waitpwall && !(mood & MOOD_DONTBACKFORSECRETS)))
	{
		// Reset if out of the path, or if a pushwall stopped moving
		searchstage = SSGeneral;	// new areas revealed, so look
		waitpwall = false;
		path.reset();
		return;
	}
	
	int nx, ny, mx, my;
	Boolean tryuse = false;	// whether to try using
	
	
	mx = player->x;
	my = player->y;
	 
	
	// elevator
	byte tile;
	int nexton = path.getNextIndex(nowon);
	int nexton2 = path.getNextIndex(nexton);
	int nx2, ny2;
	if(nexton2 >= 0)
		path.getCoords(nexton2, &nx2, &ny2);
	
	if(nexton == -1)
	{
		int prevon = path.getPrevIndex(nowon);
		// end of path; start a new search
		path.reset();
		if(exitPoint >= 0)
		{
			// elevator switch: press it now
			nx = exitPoint.x;
			ny = exitPoint.y;
			tryuse = true;
		}
		else if(prevon >= 0)
		{
			int ptx, pty;
			path.getCoords(prevon, &ptx, &pty);
			// just go forward
			nx = 2*(mx>>TILESHIFT) - ptx;
			ny = 2*(my>>TILESHIFT) - pty;
		}
		else
		{
			// if undefined, just go east
			nx = (mx>>TILESHIFT) + 1;
			ny = my>>TILESHIFT;
		}
	}
	else
	{
		// in the path
		path.getCoords(nexton, &nx, &ny);
		tile = tilemap[nx][ny];
		// whether to press to open a door
		tryuse = (tile & 0x80) == 0x80 && (doorobjlist[tile & ~0x80].action == dr_closed || doorobjlist[tile & ~0x80].action == dr_closing);
	}
	
	// set up the target angle
	int tangle = Basic::DirAngle(Point2D<int>::Make(mx,my),
                                 Point2D<int>::Make(nx,ny));
	int dangle = Basic::CentreAngle(tangle, player->angle);
	
	// Non-combat mode
	if(gamestate.weapon >= wp_pistol && gamestate.ammo >= 20 && pressuse % 64 == 0 && EnemyEager())
	{
		// shoot something to alert nazis
		// FIXME: more stealth gameplay considerations?
		// Example: if destination won't wake up any enemies, don't wake them up
		// Still, often it's better to shoot early, than be ambushed later
		buttonstate[bt_attack] = true;
	}
	else
		buttonstate[bt_attack] = false;
	
	// Move forward only if it's safe (replace this bloatness with a function)
	//
	// Logic: if there is danger ahead, set up timer and backpedal.
	// If there is not, decrease timer, then move ahead.
	// What to do: do not set timer for the 10th consecutive time, 
	// and just pretend to go forward (default first case)
	//
	// Detect going back right after being free.
	// How? retreatwaitdelay just below 0
	//
	// Solution? retreatwaitcount variable
	//
	if(retreatwaitcount >= 10 || Crossfire(Point2D<int>::Make(player->x, player->y)) || (!(nexton >= 0 && Crossfire(Point2D<int>::Make(Basic::Major(nx), Basic::Major(ny)))) && !(nexton2 >= 0 && Crossfire(Point2D<int>::Make(Basic::Major(nx2), Basic::Major(ny2))))))
	{
		if(retreatwaitcount < 10 && retreatwaitdelay > 0)
			retreatwaitdelay -= tics;
		else if(dangle > -45 && dangle < 45)
		{
			if(retreatwaitdelay > -100)	// set to an arbitrary negative, as a threshold
				retreatwaitdelay -= tics;
			else
				retreatwaitcount = 0;	// reset retreatwaitcount
			// So move
			controly = -RUNMOVE * tics;
			// Press if there's an obstacle ahead
			if(tryuse && (actorat[nx][ny] && !ISPOINTER(actorat[nx][ny])) && pressuse % 4 == 0)
				buttonstate[bt_use] = true;
			else	
				buttonstate[bt_use] = false;
		}
	}
	else if (dangle > -45 && dangle < 45)
	{
		if(retreatwaitdelay > -20 && retreatwaitdelay <= 0)	// re-retreating too soon.
		{
			retreatwaitcount++;	// so increase counter
		}
		controly += BASEMOVE * tics;	// back off a little after retreating
		retreatwaitdelay = 35;
	}
	// normally just turn (it's non-combat, no problem)
	TurnToAngle(dangle);
}


//
// BotMan::DoCommand
//
// Bot execution entry
//
void BotMan::DoCommand()
{
	++pressuse;	// key press timer
	static short eangle = -1;
	static int edist = -2;
	
	objtype *check0;
	
	// A dead threater is no threat
	if(threater && threater->hitpoints <= 0)
		threater = NULL;

	check0 = EnemyVisible(&eangle, &edist);
	if(check0)
	{
			damagetaken = NULL;
			DoCombatAI(eangle, edist);
	}
	else if(retreat > 0)	// standard retreat, still moving
	{
		damagetaken = NULL;
		edist = -1;
		retreat -= tics;
		MoveByStrafe();
	}
	else
	{
		if(damagetaken)	// someone hurt me but I couldn't see him
		{
			threater = damagetaken;
			if(FindShortestPath(false, true, 1))
			{
				MoveByStrafe();
			}
			else
			{
				controly = RUNMOVE*tics;
			}
			retreat = 5;
			damagetaken = NULL;
		}
		else
		{
			DoNonCombatAI();
		}
	}
}