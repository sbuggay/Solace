/*
 * Nothing here right now, but will have stuff
 * for dealing with maps I guess.
 */

 // Returns a boolean grid indicating whether each square is in the field of view of (xLoc, yLoc).
// forbiddenTerrain is the set of terrain flags that will block vision (but the blocking cell itself is
// illuminated); forbiddenFlags is the set of map flags that will block vision.
// If cautiousOnWalls is set, we will not illuminate blocking tiles unless the tile one space closer to the origin
// is visible to the player; this is to prevent lights from illuminating a wall when the player is on the other
// side of the wall.
void getFOVMask(char grid[VIEWWIDTH][VIEWHEIGHT], short xLoc, short yLoc, float maxRadius,
				unsigned long forbiddenTerrain,	unsigned long forbiddenFlags, boolean cautiousOnWalls) {
	short i;
	
	for (i=1; i<=8; i++) {
		scanOctantFOV(grid, xLoc, yLoc, i, maxRadius, 1, LOS_SLOPE_GRANULARITY * -1, 0,
					  forbiddenTerrain, forbiddenFlags, cautiousOnWalls);
	}
}

// This is a custom implementation of recursive shadowcasting.
void scanOctantFOV(char grid[VIEWWIDTH][VIEWHEIGHT], short xLoc, short yLoc, short octant, float maxRadius,
				   short columnsRightFromOrigin, long startSlope, long endSlope, unsigned long forbiddenTerrain,
				   unsigned long forbiddenFlags, boolean cautiousOnWalls) {
	
	if (columnsRightFromOrigin >= maxRadius) return;
	
	short i, a, b, iStart, iEnd, x, y, x2, y2; // x and y are temporary variables on which we do the octant transform
	long newStartSlope, newEndSlope;
	boolean cellObstructed;
	
	newStartSlope = startSlope;
	
	a = ((LOS_SLOPE_GRANULARITY / -2 + 1) + startSlope * columnsRightFromOrigin) / LOS_SLOPE_GRANULARITY;
	b = ((LOS_SLOPE_GRANULARITY / -2 + 1) + endSlope * columnsRightFromOrigin) / LOS_SLOPE_GRANULARITY;
	
	iStart = min(a, b);
	iEnd = max(a, b);
	
	// restrict vision to a circle of radius maxRadius
	if ((columnsRightFromOrigin*columnsRightFromOrigin + iEnd*iEnd) >= maxRadius*maxRadius) {
		return;
	}
	if ((columnsRightFromOrigin*columnsRightFromOrigin + iStart*iStart) >= maxRadius*maxRadius) {
		iStart = (int) (-1 * sqrt(maxRadius*maxRadius - columnsRightFromOrigin*columnsRightFromOrigin) + FLOAT_FUDGE);
	}
	
	x = xLoc + columnsRightFromOrigin;
	y = yLoc + iStart;
	betweenOctant1andN(&x, &y, xLoc, yLoc, octant);
	boolean currentlyLit = coordinatesAreInMap(x, y) && !(cellHasTerrainFlag(x, y, forbiddenTerrain) ||
														  (pmap[x][y].flags & forbiddenFlags));
	for (i = iStart; i <= iEnd; i++) {
		x = xLoc + columnsRightFromOrigin;
		y = yLoc + i;
		betweenOctant1andN(&x, &y, xLoc, yLoc, octant);
		if (!coordinatesAreInMap(x, y)) {
			// We're off the map -- here there be memory corruption.
			continue;
		}
		cellObstructed = (cellHasTerrainFlag(x, y, forbiddenTerrain) || (pmap[x][y].flags & forbiddenFlags));
		// if we're cautious on walls and this is a wall:
		if (cautiousOnWalls && cellObstructed) {
			// (x2, y2) is the tile one space closer to the origin from the tile we're on:
			x2 = xLoc + columnsRightFromOrigin - 1;
			y2 = yLoc + i;
			if (i < 0) {
				y2++;
			} else if (i > 0) {
				y2--;
			}
			betweenOctant1andN(&x2, &y2, xLoc, yLoc, octant);
			
			if (pmap[x2][y2].flags & IN_FIELD_OF_VIEW) {
				// previous tile is visible, so illuminate
				grid[x][y] = 1;
			}
		} else {
			// illuminate
			grid[x][y] = 1;
		}
		if (!cellObstructed && !currentlyLit) { // next column slope starts here
			newStartSlope = (long int) ((LOS_SLOPE_GRANULARITY * (i) - LOS_SLOPE_GRANULARITY / 2) / (columnsRightFromOrigin + 0.5));
			currentlyLit = true;
		} else if (cellObstructed && currentlyLit) { // next column slope ends here
			newEndSlope = (long int) ((LOS_SLOPE_GRANULARITY * (i) - LOS_SLOPE_GRANULARITY / 2)
							/ (columnsRightFromOrigin - 0.5));
			if (newStartSlope <= newEndSlope) {
				// run next column
				scanOctantFOV(grid, xLoc, yLoc, octant, maxRadius, columnsRightFromOrigin + 1, newStartSlope, newEndSlope,
							  forbiddenTerrain, forbiddenFlags, cautiousOnWalls);
			}
			currentlyLit = false;
		}
	}
	if (currentlyLit) { // got to the bottom of the scan while lit
		newEndSlope = endSlope;
		if (newStartSlope <= newEndSlope) {
			// run next column
			scanOctantFOV(grid, xLoc, yLoc, octant, maxRadius, columnsRightFromOrigin + 1, newStartSlope, newEndSlope,
						  forbiddenTerrain, forbiddenFlags, cautiousOnWalls);
		}
	}
}