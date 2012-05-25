//
// IOAN_BOT.CPP
//
// File by Ioan Chera, started 17.05.2012
//
// Basic bot setup. I'm working with classes here!
//

#include "ioan_bot.h"
#include "ioan_bas.h"

// static class member definition
boolean BotMan::active;
// protected ones
boolean BotMan::pathexists, BotMan::exitfound;
byte BotMan::nothingleft, BotMan::wakeupfire, BotMan::retreat;
int BotMan::exitx, BotMan::exity, BotMan::exfrontx;
BotMan::SData *BotMan::searchset;
int BotMan::searchsize, BotMan::searchlen;
objtype *BotMan::threater;

//
// BotMan::FindExit
//
// Finds the exit on the map
//
boolean BotMan::FindExit()
{
	int i, j;
	objtype *check1, *check2;

	for(i = 0; i < MAPSIZE; ++i)
	{
		for(j = 0; j < MAPSIZE; ++j)
		{
			// Look if one of the ways is free
			check1 = j > 0 ? actorat[j - 1][i] : (objtype *)1;
			check2 = j < MAPSIZE - 1 ? actorat[j + 1][i] : (objtype *)1;
			
			if(tilemap[j][i] == ELEVATORTILE)
			{
				if(check1 && !ISPOINTER(check1) && check2 && !ISPOINTER(check2))
				{
					// blocked
					continue;
				}
				else
				{
					// found
					exitx = j;
					exity = i;
					if(!check1 || ISPOINTER(check1))
						exfrontx = j - 1;
					else if(!check2 || ISPOINTER(check2))
						exfrontx = j + 1;
					return true;
				}
			}
		}
	}
	return false;
}

//
// BotMan::ApproxDist
//
// Approx. distance (too lazy to use sqrt)
//
int BotMan::ApproxDist(int dx, int dy)
{
	dx = abs(dx);
	dy = abs(dy);
	//if(dx < dy)
	//	return dx + dy - (dx >> 1);
	//return dx + dy - (dy >> 1);
	return dx + dy;
}

//
// BotMan::AddToSet
//
// Adds copy of value to set
//
void BotMan::AddToSet(const SData &data)
{
	if(++searchlen > searchsize)
	{
		searchset = (SData*)realloc(searchset, searchsize < 32 ? (searchsize = 32) * sizeof(SData) 
			: (searchsize = 2 * searchsize) * sizeof(SData));
		CHECKMALLOCRESULT(searchset);
	}
	searchset[searchlen - 1] = data;
}

//
// BotMan::FindPathToExit
//
// Finds the path to an exit (A*)
//
boolean BotMan::FindPathToExit()
{
	int i, j;
	// Look for map exit
	if(!exitfound)
		exitfound = FindExit();
	if(!exitfound)
		return false;

	SData data;
	data.g_score = 0;
	data.f_score = data.h_score = ApproxDist(exfrontx - player->tilex, exity - player->tiley);
	data.open = true;
	data.tilex = player->tilex;
	data.tiley = player->tiley;
	data.prev = -1;

	EmptySet();
	AddToSet(data);

	boolean openfound;
	int fmin, imin, ifound;
	int cx, cy;
	objtype *check;
	boolean neighinvalid, neighfound;
	int tentative_g_score;
	while(1)
	{
		// look for open sets
		openfound = false;
		fmin = INT_MAX;
		for(i = 0; i < searchlen; ++i)
		{
			if(searchset[i].open)
			{
				openfound = true;
				if(searchset[i].f_score < fmin)
				{
					fmin = searchset[i].f_score;
					imin = i;
				}
			}
		}
		if(!openfound)
			break;
		searchset[imin].open = false;

		if(searchset[imin].tilex == exfrontx && searchset[imin].tiley == exity)
		{
			// found goal
			searchset[imin].next = -1;
			ifound = imin;
			for(imin = searchset[imin].prev; imin != -1; ifound = imin, imin = searchset[imin].prev)
				searchset[imin].next = ifound;
			return true;
		}
		
		for(j = 0; j < 4; ++j)
		{
			switch(j)
			{
			case 0:
				cx = searchset[imin].tilex + 1;
				cy = searchset[imin].tiley;
				break;
			case 1:
				cx = searchset[imin].tilex;
				cy = searchset[imin].tiley + 1;
				break;
			case 2:
				cx = searchset[imin].tilex - 1;
				cy = searchset[imin].tiley;
				break;
			case 3:
				cx = searchset[imin].tilex;
				cy = searchset[imin].tiley - 1;
				break;
			}
			check = actorat[cx][cy];
			byte door = tilemap[cx][cy];
			if(check && !ISPOINTER(check) && !(door & 0x80))
				continue;	// solid, can't be passed
			else if (door & 0x80)
			{
				door = door & ~0x80;
				byte lock = doorobjlist[door].lock;
				if (lock >= dr_lock1 && lock <= dr_lock4)
					if ( ! (gamestate.keys & (1 << (lock-dr_lock1) ) ) )
						continue;
			}
			neighinvalid = false;
			neighfound = false;
			for(i = searchlen - 1; i >= 0; --i)
			{
				if(searchset[i].tilex == cx && searchset[i].tiley == cy)
				{
					if(!searchset[i].open)
						neighinvalid = true;
					else
					{
						neighfound = true;
						ifound = i;
					}
					break;
				}
			}
			if(neighinvalid)
				continue;
			tentative_g_score = searchset[imin].g_score + 1;
			if(!neighfound)
			{
				data.g_score = tentative_g_score;
				data.h_score = ApproxDist(exfrontx - cx, exity - cy);
				data.f_score = data.g_score + data.h_score;
				data.open = true;
				data.tilex = cx;
				data.tiley = cy;
				data.prev = imin;
				AddToSet(data);
			}
			else if(tentative_g_score < searchset[ifound].g_score)
			{
				searchset[ifound].g_score = tentative_g_score;
				searchset[ifound].f_score = tentative_g_score + searchset[ifound].h_score;
				searchset[ifound].prev = imin;
			}
		}
	}

	return false;
}

//
// BotMan::ObjectOfInterest
//
// Pickable item, secret door, exit switch, exit pad
//
boolean BotMan::ObjectOfInterest(int tx, int ty)
{
	int i;
	exitx = -1;
	exity = -1;

	// retreat mode

	if(retreat == 1)
	{
		static byte retstep = 0;

		static short eangle;
		static int edist;
		if(retstep > 0 && EnemyVisible(&eangle, &edist))
			retstep = 0;
		else if(!EnemyVisible(&eangle, &edist))
		{
			retstep++;
			if(retstep == 2)
				return true;
		}
		return false;
	}

	// items
	for(i = 0; &statobjlist[i] != laststatobj; ++i)
	{
		if(statobjlist[i].tilex == (byte)tx && statobjlist[i].tiley == (byte)ty && statobjlist[i].flags & FL_BONUS
			&& statobjlist[i].shapenum >= 0)
		{
			switch (statobjlist[i].itemnumber)
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
			case    bo_cross:
			case    bo_chalice:
			case    bo_bible:
			case    bo_crown:
			case    bo_machinegun:
			case    bo_chaingun:
			case    bo_fullheal:
				return true;
			case    bo_clip:
				if(gamestate.ammo <= 91)
					return true;
				break;
			case    bo_clip2:
				if (gamestate.ammo < 95)
					return true;
				break;
#ifdef SPEAR
			case    bo_25clip:
				if (gamestate.ammo < 74)
					return true;
				break;
			case    bo_spear:
				return true;
#endif
			case    bo_gibs:
				if (gamestate.health <= 10)
					return true;
				break;
			}
		}
	}

	objtype *check = actorat[tx][ty];
	if(gamestate.health > 75 && gamestate.ammo > 75 &&
		check && ISPOINTER(check) && Basic::IsEnemy(check->obclass) && check->hitpoints > 0)
	{
		return true;
	}

	// secret door
	if(ty + 3 < MAPSIZE && *(mapsegs[1]+((ty + 1)<<mapshift)+tx) == PUSHABLETILE)
	{
		if(!actorat[tx][ty+2] && !actorat[tx][ty+3])
		{
			exity = ty + 1;
			exitx = tx;
			return true;
		}
	}
	if(ty - 3 >= 0 && *(mapsegs[1]+((ty - 1)<<mapshift)+tx) == PUSHABLETILE)
	{
		if(!actorat[tx][ty-2] && !actorat[tx][ty-3])
		{
			exity = ty - 1;
			exitx = tx;
			return true;
		}
	}
	if(tx + 3 < MAPSIZE && *(mapsegs[1]+(ty<<mapshift)+tx + 1) == PUSHABLETILE)
	{
		if(!actorat[tx+2][ty] && !actorat[tx+3][ty])
		{
			exity = ty;
			exitx = tx + 1;
			return true;
		}
	}
	if(tx - 3 >= 0 && *(mapsegs[1]+(ty<<mapshift)+tx - 1) == PUSHABLETILE)
	{
		if(!actorat[tx-2][ty] && !actorat[tx-3][ty])
		{
			exity = ty;
			exitx = tx - 1;
			return true;
		}
	}

	// exit switch
	if(nothingleft >= 1)
	{
		if(tx - 1 >= 0 && tilemap[tx - 1][ty] == ELEVATORTILE) 
		{
			exitx = tx - 1;
			exity = ty;
			if(tilemap[tx][ty] == ALTELEVATORTILE || nothingleft >= 2)
				return true;
		}
		if(tx + 1 < MAPSIZE && tilemap[tx + 1][ty] == ELEVATORTILE)
		{
			exitx = tx + 1;
			exity = ty;
			if(tilemap[tx][ty] == ALTELEVATORTILE || nothingleft >= 2)
				return true;
		}

		// exit pad
		if(nothingleft >= 2 && *(mapsegs[1]+(ty<<mapshift)+tx) == EXITTILE)
			return true;
	}
	return false;
}

//
// BotMan::FindPathToExit
//
// Finds the path to an exit (A*)
//
boolean BotMan::FindRandomPath()
{
	int i, j;

	SData data;
	data.g_score = 0;
	data.f_score = 0;
	data.open = true;
	data.tilex = player->tilex;
	data.tiley = player->tiley;
	data.prev = -1;

	EmptySet();
	AddToSet(data);

	boolean openfound;
	int fmin, imin, ifound;
	int cx, cy;
	objtype *check;
	boolean neighinvalid, neighfound;
	int tentative_g_score;
	while(1)
	{
		// look for open sets
		openfound = false;
		fmin = INT_MAX;
		for(i = 0; i < searchlen; ++i)
		{
			if(searchset[i].open)
			{
				openfound = true;
				if(searchset[i].f_score < fmin)
				{
					fmin = searchset[i].f_score;
					imin = i;
				}
			}
		}
		if(!openfound)
			break;
		searchset[imin].tilex;
		searchset[imin].tiley;
		searchset[imin].open = false;

		if(ObjectOfInterest(searchset[imin].tilex, searchset[imin].tiley))
		{
			// found goal
			searchset[imin].next = -1;
			ifound = imin;
			for(imin = searchset[imin].prev; imin != -1; ifound = imin, imin = searchset[imin].prev)
				searchset[imin].next = ifound;
			pathexists = true;
			return true;
		}
		
		for(j = 0; j < 4; ++j)
		{
			switch(j)
			{
			case 0:
				cx = searchset[imin].tilex + 1;
				cy = searchset[imin].tiley;
				break;
			case 1:
				cx = searchset[imin].tilex;
				cy = searchset[imin].tiley + 1;
				break;
			case 2:
				cx = searchset[imin].tilex - 1;
				cy = searchset[imin].tiley;
				break;
			case 3:
				cx = searchset[imin].tilex;
				cy = searchset[imin].tiley - 1;
				break;
			}
			check = actorat[cx][cy];
			byte door = tilemap[cx][cy];
			if(check && !ISPOINTER(check) && !(door & 0x80))
				continue;	// solid, can't be passed
			else if (door & 0x80)
			{
				door = door & ~0x80;
				byte lock = doorobjlist[door].lock;
				if (lock >= dr_lock1 && lock <= dr_lock4)
					if ( ! (gamestate.keys & (1 << (lock-dr_lock1) ) ) )
						continue;
			}

			neighinvalid = false;
			neighfound = false;
			for(i = searchlen - 1; i >= 0; --i)
			{
				if(searchset[i].tilex == cx && searchset[i].tiley == cy)
				{
					if(!searchset[i].open)
						neighinvalid = true;
					else
					{
						neighfound = true;
						ifound = i;
					}
					break;
				}
			}
			if(neighinvalid)
				continue;
			tentative_g_score = searchset[imin].g_score + 1;
			if(!neighfound)
			{
				data.g_score = tentative_g_score;
				data.h_score = 0;
				data.f_score = data.g_score;
				data.open = true;
				data.tilex = cx;
				data.tiley = cy;
				data.prev = imin;
				AddToSet(data);
			}
			else if(tentative_g_score < searchset[ifound].g_score)
			{
				searchset[ifound].g_score = tentative_g_score;
				searchset[ifound].f_score = tentative_g_score;
				searchset[ifound].prev = imin;
			}
		}
	}

	if(!pwallstate)
		nothingleft++;

	return false;
}

//
// BotMan::EnemyOnTarget
//
// True if can shoot
//
objtype *BotMan::EnemyOnTarget()
{
	objtype *check,*closest,*oldclosest;
	int32_t  viewdist;

	//
	// find potential targets
	//
	viewdist = 0x7fffffffl;
	closest = NULL;

	while (1)
	{
		oldclosest = closest;

		for (check=player->next ; check ; check=check->next)
		{
			if ((check->flags & FL_SHOOTABLE) && (check->flags & FL_VISABLE)
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
			return NULL;                                         // no more targets, all missed

		//
		// trace a line from player to enemey
		//

		if (CheckLine(closest))
			return closest;
	}
	return NULL;
}

//
// BotMan::EnemyVisible
//
// True if an enemy is in sight
//
objtype *BotMan::EnemyVisible(short *angle, int *distance)
{
	int tx = player->tilex, ty = player->tiley;
	int i, j, k;
	objtype *ret;
	static objtype *oldret;

	for(i = 1; i < 15; ++i)	// absolute maximum range: 21
	{
		for(j = -i; j <= i; ++j)
		{
			for(k = -i; k <= i; ++k)
			{
				if(j > -i && j < i && k > -i && k < i)
					continue;
				if(ty + j < 0 || ty + j >= MAPSIZE || tx + k < 0 || tx + k >= MAPSIZE)
					continue;
				ret = actorat[player->tilex + k][player->tiley + j];
				if(ret && ISPOINTER(ret) && Basic::IsEnemy(ret->obclass) && ret->hitpoints > 0 && CheckLine(ret))
				{
					double dby = -((double)(ret->y) - (double)(player->y));
					double dbx = (double)(ret->x) - (double)(player->x);
					if(abs(*distance - i) >= 2 && ret != oldret || ret == oldret || oldret && oldret->hitpoints <= 0)
					{
						*angle = (short)(180.0/PI*atan2(dby, dbx));
						*distance = i;
									
						if(*angle >= 360)
							*angle -= 360;
						if(*angle < 0)
							*angle += 360;

						oldret = ret;
						return ret;
					}
				}
			}
		}
	}
	return NULL;
}

//
// BotMan::EnemyEager
//
// True if 1 non-ambush enemies are afoot 
//
objtype *BotMan::EnemyEager()
{
	int tx = player->tilex, ty = player->tiley;
	int j, k;
	objtype *ret;

	for(j = -6; j <= 6; ++j)
	{
		for(k = -6; k <= 6; ++k)
		{
			if(ty + j < 0 || ty + j >= MAPSIZE || tx + k < 0 || tx + k >= MAPSIZE)
				continue;
			ret = actorat[player->tilex + k][player->tiley + j];
			if(ret && ISPOINTER(ret) && Basic::IsEnemy(ret->obclass) && ret->hitpoints > 0 
				&& areabyplayer[ret->areanumber] && !(ret->flags & (FL_AMBUSH | FL_ATTACKMODE)))
			{
				return ret;
			}
		}
	}
	return NULL;
}

//
// BotMan::EnemiesArmed
//
// true if enemies are armed
//
objtype *BotMan::EnemiesArmed()
{
	int tx = player->tilex, ty = player->tiley;
	int j, k;
	objtype *ret;

	for(j = -10; j <= 10; ++j)
	{
		for(k = -10; k <= 10; ++k)
		{
			if(ty + j < 0 || ty + j >= MAPSIZE || tx + k < 0 || tx + k >= MAPSIZE)
				continue;
			ret = actorat[player->tilex + k][player->tiley + j];
			if(ret && ISPOINTER(ret) && Basic::IsEnemy(ret->obclass) && ret->hitpoints > 0 
				&& Basic::IsArmed(ret) && Basic::IsAutomatic(ret->obclass) && CheckLine(ret))
			{
				return ret;
			}
		}
	}
	return NULL;
}

//
// BotMan::Crossfire
//
// Returns true if there's a crossfire in that spot
//
objtype *BotMan::Crossfire(int tx, int ty)
{
	int j, k;
	objtype *ret;

	for(j = -10; j <= 10; ++j)
	{
		for(k = -10; k <= 10; ++k)
		{
			if(ty + j < 0 || ty + j >= MAPSIZE || tx + k < 0 || tx + k >= MAPSIZE)
				continue;
			ret = actorat[tx + k][ty + j];
			if(ret && ISPOINTER(ret) && Basic::IsEnemy(ret->obclass) && ret->hitpoints > 0 
				&& Basic::IsArmed(ret) && Basic::IsAutomatic(ret->obclass) 
				&& Basic::GenericCheckLine(ret->x, ret->y, 
				(tx << TILESHIFT) + (1 << (TILESHIFT - 1)),
				(ty << TILESHIFT) + (1 << (TILESHIFT - 1))))
			{
				return ret;
			}
		}
	}
	return NULL;
}

//
// BotMan::DoRetreat
//
// Retreats the bot sliding off walls
//
void BotMan::DoRetreat()
{
	controly += RUNMOVE * tics;
	int backx, backy, sidex, sidey, tx = player->tilex, ty = player->tiley, dir;
	if(player->angle > 0 && player->angle <= 45)
	{
		backx = -1;
		backy = 0;
		sidex = 0;
		sidey = 1;
		dir = RUNMOVE;
	}
	else if(player->angle > 45 && player->angle <= 90)
	{
		backx = 0;
		backy = 1;
		sidex = -1;
		sidey = 0;
		dir = -RUNMOVE;
	}
	else if(player->angle > 90 && player->angle <= 135)
	{
		backx = 0;
		backy = 1;
		sidex = 1;
		sidey = 0;
		dir = RUNMOVE;
	}
	else if(player->angle > 135 && player->angle <= 180)
	{
		backx = 1;
		backy = 0;
		sidex = 0;
		sidey = -1;
		dir = -RUNMOVE;
	}
	else if(player->angle > 180 && player->angle <= 225)
	{
		backx = 1;
		backy = 0;
		sidex = 0;
		sidey = 1;
		dir = RUNMOVE;
	}
	else if(player->angle > 225 && player->angle <= 270)
	{
		backx = 0;
		backy = 1;
		sidex = 1;
		sidey = 0;
		dir = -RUNMOVE;
	}
	else if(player->angle > 270 && player->angle <= 315)
	{
		backx = 0;
		backy = 1;
		sidex = -1;
		sidey = 0;
		dir = RUNMOVE;
	}
	else
	{
		backx = -1;
		backy = 0;
		sidex = 0;
		sidey = -1;
		dir = -RUNMOVE;
	}
	if(tx <= 0 || tx >= MAPSIZE - 1 || ty <= 0 || ty >= MAPSIZE - 1)
		return;
	backx += tx;
	backy += ty;
	sidex += tx;
	sidey += ty;
	objtype *check1 = actorat[backx][backy], *check2 = actorat[sidex][sidey];
	if(check1 && !ISPOINTER(check1) || check1 && ISPOINTER(check1) && check1->flags & FL_SHOOTABLE)
	{
		buttonstate[bt_strafe] = true;
		if(check2 && !ISPOINTER(check2) || check2 && ISPOINTER(check2) && check2->flags & FL_SHOOTABLE)
		{
			controlx -= dir*tics;
		}
		else
			controlx += dir*tics;
	}
	else
		buttonstate[bt_strafe] = false;
}

//
// BotMan::MoveStrafe
//
// During retreating, keep eye on enemy, unless a door has to be opened
//
void BotMan::MoveStrafe(short tangle, short dangle, boolean tryuse, byte pressuse, int nx, int ny)
{
	if(tryuse)
	{
		if(dangle > -45 && dangle < 45)
		{
			// So move
			controly -= RUNMOVE * tics;
			// Press if there's an obstacle ahead
			if((actorat[nx][ny] && !ISPOINTER(actorat[nx][ny])) && pressuse % 4 == 0)
				buttonstate[bt_use] = true;
			else
				buttonstate[bt_use] = false;
		}

		// normally just turn (it's non-combat, no problem)
		buttonstate[bt_strafe] = false;

		if(dangle > 15)
			controlx = -RUNMOVE * tics;
		else if(dangle > 0)
			controlx = -BASEMOVE * tics;
		else if(dangle < -15)
			controlx = RUNMOVE * tics;
		else if(dangle < 0)
			controlx = BASEMOVE * tics;
		else
		{
			// straight line: can strafe now
			buttonstate[bt_strafe] = true;
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
			if(plro - cento > 4096)
				controlx += BASEMOVE * tics;
			else if(plro - cento < -4096)
				controlx -= BASEMOVE * tics;
		}
		return;
	}
	if(dangle >= -22 && dangle < 23)	// forward
	{
		controly -= RUNMOVE*tics;
		controlx = 0;
	}
	else if(dangle >= 23 && dangle < 68)	// forward-left
	{
		controly -= RUNMOVE*tics;
		controlx -= RUNMOVE*tics;
	}
	else if(dangle >= 68 && dangle < 113)	// left
	{
		controly = 0;
		controlx -= RUNMOVE*tics;
	}
	else if(dangle >= 113 && dangle < 158)	// back-left
	{
		controly += RUNMOVE*tics;
		controlx -= RUNMOVE*tics;
	}
	else if(dangle >= 158 || dangle < -157)	// back
	{
		controly += RUNMOVE*tics;
		controlx = 0;
	}
	else if(dangle >= -157 && dangle < -112)	// back-right
	{
		controly += RUNMOVE*tics;
		controlx += RUNMOVE*tics;
	}
	else if(dangle >= -112 && dangle < -67)	// right
	{
		controly = 0;
		controlx += RUNMOVE*tics;
	}
	else	// forward-right
	{
		controly -= RUNMOVE*tics;
		controlx += RUNMOVE*tics;
	}
}

//
// BotMan::FindPath
//
// Finds the path to walk through
//
void BotMan::DoCommand()
{
	// no path got defined
	if(!pathexists)
	{
//		if(!FindPathToExit())
//			return;
		if(!FindRandomPath())
		{
			return;
		}
	}

	static boolean waitpwall;
	if(pwallstate)
		waitpwall = true;

	// found path to exit
	int searchpos = 0;
	int nowon = -1;

	// look if it's there
	for(; searchpos != -1; searchpos = searchset[searchpos].next)
	{
		if(player->tilex == searchset[searchpos].tilex && player->tiley == searchset[searchpos].tiley)
		{
			nowon = searchpos;
			break;
		}
	}

	if(nowon < 0 || !pwallstate && waitpwall)
	{
		// Reset if out of the path, or if a pushwall stopped moving
		waitpwall = false;
		pathexists = false;
		return;
	}

	static short tangle, dangle;
	int nx, ny, mx, my;
	boolean tryuse = false;	// whether to try using

	mx = player->tilex;
	my = player->tiley;
	
	// elevator
	byte tile;
	if(searchset[nowon].next == -1)
	{
		// end of path; start a new search
		retreat = 0;	// stop retreating
		pathexists = false;
		if(exitx >= 0 && exity >= 0)
		{
			// elevator switch: press it now
			nx = exitx;
			ny = exity;
			tryuse = true;
		}
		else if(searchset[nowon].prev >= 0)
		{
			// just go forward
			nx = 2*mx - searchset[searchset[nowon].prev].tilex;
			ny = 2*my - searchset[searchset[nowon].prev].tiley;
		}
		else
		{
			// if undefined, just go east
			nx = mx + 1;
			ny = my;
		}
	}
	else
	{
		// in the path
		nx = searchset[searchset[nowon].next].tilex;
		ny = searchset[searchset[nowon].next].tiley;
		tile = tilemap[nx][ny];
		// whether to press to open a door
		tryuse = (tile & 0x80) == 0x80 
			&& (doorobjlist[tile & ~0x80].action == dr_closed || doorobjlist[tile & ~0x80].action == dr_closing);
	}

	// set up the target angle
	if(nx > mx)
		tangle = 0;
	else if(ny > my)
		tangle = 270;
	else if(nx < mx)
		tangle = 180;
	else if(ny < my)
		tangle = 90;

	static byte pressuse, retreatactive, retreat2;

	if(!retreat)
		retreatactive = 0;
	if(retreat2)
		retreat2--;

	dangle = tangle - player->angle;
	if(dangle > 180)	// centred angle
		dangle -= 360;
	else if(dangle <= -180)
		dangle += 360;

	++pressuse;	// key press timer
	static short eangle = -1;
	static int edist = -1;
	objtype *check;

	if(check = EnemyVisible(&eangle, &edist))
	{
		// Enemy visible mode
		// centred angle here
		dangle = eangle - player->angle;
		if(dangle > 180)
			dangle -= 360;
		else if(dangle <= -180)
			dangle += 360;

		// turn towards nearest target
		buttonstate[bt_strafe] = false;
		if(dangle > 15)
			controlx -= RUNMOVE * tics;
		else if(dangle > 0)
			controlx -= BASEMOVE * tics;
		else if(dangle < -15)
			controlx += RUNMOVE * tics;
		else if(dangle < 0)
			controlx += BASEMOVE * tics;

		// FIXME: work around the numbness bug
		if(EnemyOnTarget())
		{
			// shoot according to how the weapon works
			if(gamestate.weapon <= wp_pistol && pressuse % 4 == 0 || gamestate.weapon > wp_pistol)
				buttonstate[bt_attack] = true;
			else
				buttonstate[bt_attack] = false;

			// TODO: don't always charge when using the knife!
			if(dangle > -45 && dangle < 45 && (gamestate.weapon == wp_knife || edist > 6) && !retreat)
				controly -= RUNMOVE * tics;	// charge
			else if(!retreat /*&& Threatened()*/ &&
				(edist < 3 || Basic::IsArmed(check))/* && !retreatactive*/ && gamestate.weaponframe >= 3
				|| EnemiesArmed())
			{
				// TODO: replace all this with a path searching mode.
				retreat = 1;
				threater = check;
				if(!FindRandomPath())	// unable to find retreat point
				{
					pathexists = false;
					retreat = 2;	// secondary retreat
				}
			}
			else if(retreat == 1)	// standard retreat (go by path, by strafing)
			{
				dangle = tangle - player->angle;
				if(dangle > 180)	// centred angle
					dangle -= 360;
				else if(dangle <= -180)
					dangle += 360;

				// do the possible combinations (eight):
				MoveStrafe(tangle, dangle, tryuse, pressuse, nx, ny);
				
			}
			else if(retreat == 2)	// no hideout found, so just back off
				DoRetreat();
		}
	}
	else if(retreat == 1)	// standard retreat, still moving
	{
		// retreat mode (to be removed?)
		MoveStrafe(tangle, dangle, tryuse, pressuse, nx, ny);
		edist = -1;
	}
	else
	{
		edist = -1;
		retreat = 0;	// stop back-off retreating (if it even happens)
		// Non-combat mode
		if(EnemyEager() && gamestate.weapon >= wp_pistol && gamestate.ammo >= 20)
		{
			// shoot something to alert nazis
			// TODO: more stealth gameplay considerations?
			buttonstate[bt_attack] = true;
		}
		else
			buttonstate[bt_attack] = false;

		wakeupfire = 0;	// near dead
		threater = NULL;	// also near dead?

		// Move forward only if it's safe (replace this bloatness with a function)

		if(dangle > -45 && dangle < 45 && !(searchset[nowon].next >= 0 
			&& Crossfire(searchset[searchset[nowon].next].tilex, searchset[searchset[nowon].next].tiley))
			&& !(searchset[nowon].next >= 0 && searchset[searchset[nowon].next].next >= 0
			&& Crossfire(searchset[searchset[searchset[nowon].next].next].tilex, 
			searchset[searchset[searchset[nowon].next].next].tiley)))
		{
			// So move
			controly -= RUNMOVE * tics;
			// Press if there's an obstacle ahead
			if(tryuse && (actorat[nx][ny] && !ISPOINTER(actorat[nx][ny])) && pressuse % 4 == 0)
				buttonstate[bt_use] = true;
			else
				buttonstate[bt_use] = false;
		}

		// normally just turn (it's non-combat, no problem)
		buttonstate[bt_strafe] = false;

		if(dangle > 15)
			controlx -= RUNMOVE * tics;
		else if(dangle > 0)
			controlx -= BASEMOVE * tics;
		else if(dangle < -15)
			controlx += RUNMOVE * tics;
		else if(dangle < 0)
			controlx += BASEMOVE * tics;
		else
		{
			// straight line: can strafe now
			buttonstate[bt_strafe] = true;
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
			if(plro - cento > 4096)
				controlx += BASEMOVE * tics;
			else if(plro - cento < -4096)
				controlx -= BASEMOVE * tics;
		}
	}
}