/******************/
/** Dungeon Data **/
/******************/

#define Fl(N)					(1 << (N))

#define MAX_NUM_COLS  100
#define MAX_NUM_ROWS  100

#define MAX_NUM_UP_STAIRS  2
#define MAX_NUM_DOWN_STAIRS 2

// Dungeon flags
enum tileFlags {
	DISCOVERED					= Fl(0),
	VISIBLE						= Fl(1),	// cell has sufficient light and is in field of view, ready to draw.
	// HAS_PLAYER					= Fl(2),
	// HAS_MONSTER					= Fl(3),
	// HAS_ITEM					= Fl(4),
	// IN_FIELD_OF_VIEW			= Fl(5),	// player has unobstructed line of sight whether or not there is enough light
	// HAS_DOWN_STAIRS				= Fl(6),
	// HAS_UP_STAIRS				= Fl(7),
	// ITEM_DETECTED				= Fl(8),
	// PRESSURE_PLATE_DEPRESSED	= Fl(9),	// so that traps do not trigger repeatedly while you stand on them
	// KNOWN_TO_BE_TRAP_FREE		= Fl(10),	// keep track of where the player has stepped as he knows no traps are there
	
 //    HAS_STAIRS                  = (HAS_UP_STAIRS | HAS_DOWN_STAIRS),
};

enum tileType {
	NOTHING = 0,
	GRANITE,
	FLOOR,
	WALL,
	DOOR,
	OPEN_DOOR,
	NUMBER_TILE_KINDS
};

typedef struct {
	bool discovered;
	enum tileFlags flags;
	enum tileType type;
} TileData;

typedef struct {
	bool visited;
	short stairsUpPos[MAX_NUM_UP_STAIRS];
	short stairsDownPos[MAX_NUM_DOWN_STAIRS];
	unsigned long timeLeft;
	// creature *monsters;
	// item *items;
	TileData tiles[MAX_NUM_COLS][MAX_NUM_ROWS];
} LevelData;