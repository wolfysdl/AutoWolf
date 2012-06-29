//
// IOAN_BOT.H
//
// File by Ioan Chera, started 17.05.2012
//
// Basic bot setup. I'm working with classes here!
//

#ifndef __IOAN_BOT_H__
#define __IOAN_BOT_H__

#include "wl_def.h"
class HistoryRatio;

//
// SearchStage
//
// Separate search objects into stages. Must be in order here
//
enum SearchStage
{
	SSGeneral,
	SSOnePushWalls,
	SSSecretLift,
	SSNormalLift
};

//
// BotMan
//
// Bot manager class. Contains basic settings.
//
class BotMan
{
protected:
	// search data structure
	struct SData
	{
		int tilex, tiley;
		int f_score, g_score, h_score;
		int prev, next;
		boolean open;
	};

	static SData *searchset;
	static int searchsize, searchlen;

	// protected variables
	static boolean pathexists, exitfound, panic;
	static byte retreatwaitdelay, retreat, pressuse;
	static SearchStage nothingleft;
	static int exitx, exity, exfrontx;
	static objtype *threater;

	// protected functions
	// Finds the closest object of interest (object, hidden door, exit)
	static boolean FindRandomPath(boolean ignoreproj = false, boolean mindnazis = false, boolean retreating = false,
								  boolean knifeinsight = false);
	// Empty set
	inline static void EmptySet()
	{
		searchsize = searchlen = 0;
		if(searchset)
		{
			free(searchset);
			searchset = NULL;
		}
	}
	// move by strafing
	static void MoveStrafe(short tangle, short dangle, boolean tryuse, byte pressuse, int nx, int ny);
	// Add set
	static void AddToSet(const SData &data);
	// ApproximateDistance
	static int ApproxDist(int dx, int dy);
	// Object of interest
	static boolean ObjectOfInterest(int dx, int dy, boolean knifeinsight = false);
	// Enemy on spot
	static objtype *EnemyFound(int dx, int dy);
	// Enemy targetted
	static objtype *EnemyOnTarget();
	// Enemy visible
	static objtype *EnemyVisible(short *angle, int *distance);
	// Enemy eager to follow
	static objtype *EnemyEager();
	// Automated and armed
	static objtype *DamageThreat(objtype *targ);
	// do retreat
	static void DoRetreat(boolean forth = false, objtype *cause = NULL);
	// like EnemiesArmed, but not restricted to the player
	static objtype *Crossfire(int x, int y, objtype *objignore = NULL, boolean justexists = false);
	// Like enemyvisible, but for any spot
	static objtype *GenericEnemyVisible(int tx, int ty);
	// Test if there's a projectile there
	static objtype *IsProjectile(int tx, int ty, int dist = 1, short *angle = NULL, int *distance = NULL);
	// Test if there's an enemy
	static objtype *IsEnemyBlocking(int tx, int ty);
	static objtype *IsEnemyNearby(int tx, int ty);
	// Move by path only by strafing
	static void MoveByStrafe();
	// Choose weapon
	static void ChooseWeapon();
	// Do combat AI
	static void DoCombatAI(int eangle, int edist);
	// Do noncombat AI
	static void DoNonCombatAI();
public:
	static boolean active;	// true if bots are activated
	static HistoryRatio shootRatio;

	// Finds the path to walk through
	static void DoCommand();

	// Unfinds the exit (on load and setup)
	static void MapInit();
};

#endif