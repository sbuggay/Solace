/*
 *  RogueMain.c
 *  Brogue
 *
 *  Created by Brian Walker on 12/26/08.
 *  Copyright 2012. All rights reserved.
 *  
 *  This file is part of Brogue.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rogue.h"
#include "globals.h"
#include <math.h>
#include <time.h>

void rogueMain() {

}

void executeEvent(rogueEvent *theEvent) {
	rogue.playbackBetweenTurns = false;
	if (theEvent->eventType == KEYSTROKE) {
		executeKeystroke(theEvent->param1, theEvent->controlKey, theEvent->shiftKey);
	} else if (theEvent->eventType == MOUSE_UP
			   || theEvent->eventType == RIGHT_MOUSE_UP) {
		executeMouseClick(theEvent);
	}
}

void initializeRogue(unsigned long seed) {

}

void startLevel(short oldLevelNumber, short stairDirection) {
	unsigned long oldSeed;
	item *theItem;
	short loc[2], i, j, x, y, px, py, flying, dir;
	boolean isAlreadyAmulet = false, placedPlayer;
	creature *monst;
	enum dungeonLayers layer;
	unsigned long timeAway;
	short **mapToStairs;
	short **mapToPit;
	boolean connectingStairsDiscovered;
    
    if (oldLevelNumber == DEEPEST_LEVEL && stairDirection != -1) {
        return;
    }
    
    synchronizePlayerTimeState();
    
    rogue.updatedSafetyMapThisTurn			= false;
    rogue.updatedAllySafetyMapThisTurn		= false;
    rogue.updatedMapToSafeTerrainThisTurn	= false;
	
	rogue.cursorLoc[0] = -1;
	rogue.cursorLoc[1] = -1;
	rogue.lastTarget = NULL;
	
	if (stairDirection == 0) { // fallen
		connectingStairsDiscovered = (pmap[rogue.downLoc[0]][rogue.downLoc[1]].flags & (DISCOVERED | MAGIC_MAPPED) ? true : false);
		levels[oldLevelNumber-1].playerExitedVia[0] = player.xLoc;
		levels[oldLevelNumber-1].playerExitedVia[1] = player.yLoc;
	} else {
        connectingStairsDiscovered = false;
    }
	
	if (oldLevelNumber != rogue.depthLevel) {
		px = player.xLoc;
		py = player.yLoc;
		if (cellHasTerrainFlag(player.xLoc, player.yLoc, T_AUTO_DESCENT)) {
			for (i=0; i<8; i++) {
				if (!cellHasTerrainFlag(player.xLoc+nbDirs[i][0], player.yLoc+nbDirs[i][1], (T_PATHING_BLOCKER))) {
					px = player.xLoc+nbDirs[i][0];
					py = player.yLoc+nbDirs[i][1];
					break;
				}
			}
		}
		mapToStairs = allocGrid();
		fillGrid(mapToStairs, 0);
		for (flying = 0; flying <= 1; flying++) {
			fillGrid(mapToStairs, 0);
			calculateDistances(mapToStairs, px, py, (flying ? T_OBSTRUCTS_PASSABILITY : T_PATHING_BLOCKER), NULL, true, true);
			for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
				x = monst->xLoc;
				y = monst->yLoc;
				if (((monst->creatureState == MONSTER_TRACKING_SCENT && (stairDirection != 0 || monst->status[STATUS_LEVITATING]))
					 || monst->creatureState == MONSTER_ALLY)
					&& (stairDirection != 0 || monst->currentHP > 10 || monst->status[STATUS_LEVITATING])
					&& ((flying != 0) == ((monst->status[STATUS_LEVITATING] != 0)
										  || cellHasTerrainFlag(x, y, T_PATHING_BLOCKER)
										  || cellHasTerrainFlag(px, py, T_AUTO_DESCENT)))
					&& !(monst->bookkeepingFlags & MONST_CAPTIVE)
					&& !(monst->info.flags & (MONST_WILL_NOT_USE_STAIRS | MONST_RESTRICTED_TO_LIQUID))
					&& !(cellHasTerrainFlag(x, y, T_OBSTRUCTS_PASSABILITY))
					&& !monst->status[STATUS_ENTRANCED]
					&& !monst->status[STATUS_PARALYZED]
					&& mapToStairs[monst->xLoc][monst->yLoc] < 30000) {
					
					monst->status[STATUS_ENTERS_LEVEL_IN] = max(1, mapToStairs[monst->xLoc][monst->yLoc] * monst->movementSpeed / 100 + 1);
					switch (stairDirection) {
						case 1:
							monst->bookkeepingFlags |= MONST_APPROACHING_DOWNSTAIRS;
							break;
						case -1:
							monst->bookkeepingFlags |= MONST_APPROACHING_UPSTAIRS;
							break;
						case 0:
							monst->bookkeepingFlags |= MONST_APPROACHING_PIT;
							break;
						default:
							break;
					}
				}
			}
		}
		freeGrid(mapToStairs);
	}
	
	for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
		if (monst->mapToMe) {
			freeGrid(monst->mapToMe);
			monst->mapToMe = NULL;
		}
		if (monst->safetyMap) {
			freeGrid(monst->safetyMap);
			monst->safetyMap = NULL;
		}
	}
	levels[oldLevelNumber-1].monsters = monsters->nextCreature;
	levels[oldLevelNumber-1].dormantMonsters = dormantMonsters->nextCreature;
	levels[oldLevelNumber-1].items = floorItems->nextItem;
	
	for (i=0; i<DCOLS; i++) {
		for (j=0; j<DROWS; j++) {
			for (layer = 0; layer < NUMBER_TERRAIN_LAYERS; layer++) {
				levels[oldLevelNumber - 1].mapStorage[i][j].layers[layer] = pmap[i][j].layers[layer];
			}
			levels[oldLevelNumber - 1].mapStorage[i][j].volume = pmap[i][j].volume;
			levels[oldLevelNumber - 1].mapStorage[i][j].flags = (pmap[i][j].flags & PERMANENT_TILE_FLAGS);
			levels[oldLevelNumber - 1].mapStorage[i][j].rememberedAppearance = pmap[i][j].rememberedAppearance;
			levels[oldLevelNumber - 1].mapStorage[i][j].rememberedTerrain = pmap[i][j].rememberedTerrain;
			levels[oldLevelNumber - 1].mapStorage[i][j].rememberedItemCategory = pmap[i][j].rememberedItemCategory;
			levels[oldLevelNumber - 1].mapStorage[i][j].machineNumber = pmap[i][j].machineNumber;
		}
	}
	
	levels[oldLevelNumber - 1].awaySince = rogue.absoluteTurnNumber;
	
	//	Prepare the new level
	
	rogue.minersLightRadius = 2.25 + (DCOLS - 1) * (float) pow(0.85, rogue.depthLevel);
	updateColors();
	updateRingBonuses(); // also updates miner's light
	
	if (!levels[rogue.depthLevel - 1].visited) { // level has not already been visited
        levels[rogue.depthLevel - 1].scentMap = allocGrid();
        scentMap = levels[rogue.depthLevel - 1].scentMap;
        fillGrid(levels[rogue.depthLevel - 1].scentMap, 0);
		// generate new level
		oldSeed = (unsigned long) rand_range(0, 9999);
        oldSeed += (unsigned long) 10000 * rand_range(0, 9999);
		seedRandomGenerator(levels[rogue.depthLevel - 1].levelSeed);
		
		// Load up next level's monsters and items, since one might have fallen from above.
		monsters->nextCreature			= levels[rogue.depthLevel-1].monsters;
		dormantMonsters->nextCreature	= levels[rogue.depthLevel-1].dormantMonsters;
		floorItems->nextItem			= levels[rogue.depthLevel-1].items;
		
		levels[rogue.depthLevel-1].monsters = NULL;
		levels[rogue.depthLevel-1].dormantMonsters = NULL;
		levels[rogue.depthLevel-1].items = NULL;
		
		digDungeon();
		initializeLevel();
		setUpWaypoints();
		
		shuffleTerrainColors(100, false);
		
		if (rogue.depthLevel >= AMULET_LEVEL && !numberOfMatchingPackItems(AMULET, 0, 0, false)
			&& levels[rogue.depthLevel-1].visited == false) {
			for (theItem = floorItems->nextItem; theItem != NULL; theItem = theItem->nextItem) {
				if (theItem->category & AMULET) {
					isAlreadyAmulet = true;
					break;
				}
			}
			if (!isAlreadyAmulet) {
				placeItem(generateItem(AMULET, 0), 0, 0);
			}
		}
		seedRandomGenerator(oldSeed);
		
		//logLevel();
		
		// Simulate 50 turns so the level is broken in (swamp gas accumulating, brimstone percolating, etc.).
		timeAway = 50;
		
	} else { // level has already been visited
		
		// restore level
        scentMap = levels[rogue.depthLevel - 1].scentMap;
		timeAway = clamp(0, rogue.absoluteTurnNumber - levels[rogue.depthLevel - 1].awaySince, 30000);
		
		for (i=0; i<DCOLS; i++) {
			for (j=0; j<DROWS; j++) {
				for (layer = 0; layer < NUMBER_TERRAIN_LAYERS; layer++) {
					pmap[i][j].layers[layer] = levels[rogue.depthLevel - 1].mapStorage[i][j].layers[layer];
				}
				pmap[i][j].volume = levels[rogue.depthLevel - 1].mapStorage[i][j].volume;
				pmap[i][j].flags = (levels[rogue.depthLevel - 1].mapStorage[i][j].flags & PERMANENT_TILE_FLAGS);
				pmap[i][j].rememberedAppearance = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedAppearance;
				pmap[i][j].rememberedTerrain = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedTerrain;
				pmap[i][j].rememberedItemCategory = levels[rogue.depthLevel - 1].mapStorage[i][j].rememberedItemCategory;
				pmap[i][j].machineNumber = levels[rogue.depthLevel - 1].mapStorage[i][j].machineNumber;
			}
		}
		
		setUpWaypoints();
		
		rogue.downLoc[0]	= levels[rogue.depthLevel - 1].downStairsLoc[0];
		rogue.downLoc[1]	= levels[rogue.depthLevel - 1].downStairsLoc[1];
		rogue.upLoc[0]		= levels[rogue.depthLevel - 1].upStairsLoc[0];
		rogue.upLoc[1]		= levels[rogue.depthLevel - 1].upStairsLoc[1];
		
		monsters->nextCreature = levels[rogue.depthLevel - 1].monsters;
		dormantMonsters->nextCreature = levels[rogue.depthLevel - 1].dormantMonsters;
		floorItems->nextItem = levels[rogue.depthLevel - 1].items;
		
		levels[rogue.depthLevel-1].monsters = NULL;
		levels[rogue.depthLevel-1].dormantMonsters = NULL;
		levels[rogue.depthLevel-1].items = NULL;
		
		if (numberOfMatchingPackItems(AMULET, 0, 0, false)) {
			isAlreadyAmulet = true;
		}
		
		for (theItem = floorItems->nextItem; theItem != NULL; theItem = theItem->nextItem) {
			if (theItem->category & AMULET) {
				if (isAlreadyAmulet) {
					
					pmap[theItem->xLoc][theItem->yLoc].flags &= ~(HAS_ITEM | ITEM_DETECTED);
					
					removeItemFromChain(theItem, floorItems);
					deleteItem(theItem);
					
					continue;
				}
				isAlreadyAmulet = true;
			}
			restoreItem(theItem);
		}
		
		mapToStairs = allocGrid();
		mapToPit = allocGrid();
		fillGrid(mapToStairs, 0);
		fillGrid(mapToPit, 0);
		calculateDistances(mapToStairs, player.xLoc, player.yLoc, T_PATHING_BLOCKER, NULL, true, true);
		calculateDistances(mapToPit, levels[rogue.depthLevel-1].playerExitedVia[0],
						   levels[rogue.depthLevel-1].playerExitedVia[0], T_PATHING_BLOCKER, NULL, true, true);
		for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
			restoreMonster(monst, mapToStairs, mapToPit);
		}
		freeGrid(mapToStairs);
		freeGrid(mapToPit);
	}
	
	// Simulate the environment!
	// First bury the player in limbo while we run the simulation,
	// so that any harmful terrain doesn't affect her during the process.
	px = player.xLoc;
	py = player.yLoc;
	player.xLoc = player.yLoc = 0;
	for (i = 0; i < 100 && i < (short) timeAway; i++) {
		updateEnvironment();
	}
	player.xLoc = px;
	player.yLoc = py;
	
    if (!levels[rogue.depthLevel-1].visited) {
        levels[rogue.depthLevel-1].visited = true;
        if (rogue.depthLevel == AMULET_LEVEL) {
            messageWithColor("An alien energy permeates the area. The Amulet of Yendor must be nearby!", &itemMessageColor, false);
        } else if (rogue.depthLevel == DEEPEST_LEVEL) {
            messageWithColor("An overwhelming sense of peace and tranquility settles upon you.", &lightBlue, false);
        }
    }
	
	// Position the player.
	if (stairDirection == 0) { // fell into the level
		
		getQualifyingLocNear(loc, player.xLoc, player.yLoc, true, 0,
							 (T_PATHING_BLOCKER),
							 (HAS_MONSTER | HAS_ITEM | HAS_UP_STAIRS | HAS_DOWN_STAIRS | IS_IN_MACHINE), false, false);
	} else {
		if (stairDirection == 1) { // heading downward
			player.xLoc = rogue.upLoc[0];
			player.yLoc = rogue.upLoc[1];
		} else if (stairDirection == -1) { // heading upward
			player.xLoc = rogue.downLoc[0];
			player.yLoc = rogue.downLoc[1];
		}
        
        placedPlayer = false;
        for (dir=0; dir<4 && !placedPlayer; dir++) {
            loc[0] = player.xLoc + nbDirs[dir][0];
            loc[1] = player.yLoc + nbDirs[dir][1];
            if (!cellHasTerrainFlag(loc[0], loc[1], T_PATHING_BLOCKER)
                && !(pmap[loc[0]][loc[1]].flags & (HAS_MONSTER | HAS_ITEM | HAS_UP_STAIRS | HAS_DOWN_STAIRS | IS_IN_MACHINE))) {
                placedPlayer = true;
            }
        }
		if (!placedPlayer) {
            getQualifyingPathLocNear(&loc[0], &loc[1],
                                     player.xLoc, player.yLoc,
                                     true,
                                     T_DIVIDES_LEVEL, NULL,
                                     T_PATHING_BLOCKER, (HAS_MONSTER | HAS_ITEM | HAS_UP_STAIRS | HAS_DOWN_STAIRS | IS_IN_MACHINE),
                                     false);
        }
	}
	player.xLoc = loc[0];
	player.yLoc = loc[1];
		
	pmap[player.xLoc][player.yLoc].flags |= HAS_PLAYER;
	
	if (connectingStairsDiscovered) {
        for (i = rogue.upLoc[0]-1; i <= rogue.upLoc[0] + 1; i++) {
            for (j = rogue.upLoc[1]-1; j <= rogue.upLoc[1] + 1; j++) {
                if (coordinatesAreInMap(i, j) && !(pmap[i][j].flags & DISCOVERED)) {
                    pmap[i][j].flags |= DISCOVERED;
                    if (!cellHasTerrainFlag(i, j, T_PATHING_BLOCKER)) {
                        rogue.xpxpThisTurn++;
                    }
                }
            }
        }
	}
	if (cellHasTerrainFlag(player.xLoc, player.yLoc, T_IS_DEEP_WATER) && !player.status[STATUS_LEVITATING]
		&& !cellHasTerrainFlag(player.xLoc, player.yLoc, (T_ENTANGLES | T_OBSTRUCTS_PASSABILITY))) {
		rogue.inWater = true;
	}
	
	updateMapToShore();
	updateVision(true);
    rogue.aggroRange = currentAggroValue();
	
	// update monster states so none are hunting if there is no scent and they can't see the player
	for (monst = monsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
		updateMonsterState(monst);
	}
	
	rogue.playbackBetweenTurns = true;
	displayLevel();
	refreshSideBar(-1, -1, false);
	
	if (rogue.playerTurnNumber) {
		rogue.playerTurnNumber++; // Increment even though no time has passed.
	}
	RNGCheck();
	flushBufferToFile();
    deleteAllFlares(); // So discovering something on the same turn that you fall down a level doesn't flash stuff on the previous level.
}



// takes a flag of the form Fl(n) and returns n
short unflag(unsigned long flag) {
	short i;
	for (i=0; i<32; i++) {
		if (flag >> i == 1) {
			return i;
		}
	}
	return -1;
}
