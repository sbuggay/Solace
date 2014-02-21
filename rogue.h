#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define boolean					char

#define false					0
#define true					1

#define Fl(N)					(1 << (N))

// Size of the portion of the terminal window devoted to displaying the dungeon:
#define DCOLS					(COLS - STAT_BAR_WIDTH - 1)
#define DROWS					(ROWS - MESSAGE_LINES - 2)	// n lines at the top for messages;
															// one line at the bottom for flavor text;
															// another line at the bottom for the menu bar.



#define WEAPON_KILLS_TO_AUTO_ID	20
#define ARMOR_DELAY_TO_AUTO_ID	1000
#define RING_DELAY_TO_AUTO_ID	1500

#define FALL_DAMAGE_MIN         8
#define FALL_DAMAGE_MAX         10

#define INPUT_RECORD_BUFFER		1000		// how many bytes of input data to keep in memory before saving it to disk
#define DEFAULT_PLAYBACK_DELAY	50

#define HIGH_SCORES_COUNT		30

// color escapes
#define COLOR_ESCAPE			25
#define COLOR_VALUE_INTERCEPT	25

#define FLOOR_CHAR		'.'
#define LIQUID_CHAR		'~'
#define CHASM_CHAR		':'
#define TRAP_CHAR		'%'
#define FIRE_CHAR		'^'
#define GRASS_CHAR		'"'
#define BRIDGE_CHAR		'='
#define DESCEND_CHAR	'>'
#define ASCEND_CHAR		'<'
#define WALL_CHAR		'#'
#define DOOR_CHAR		'+'
#define OPEN_DOOR_CHAR	'\''
#define ASH_CHAR		'\''
#define BONES_CHAR		','
#define MUD_CHAR		','
#define WEB_CHAR		':'
#define FOLIAGE_CHAR	'&'
#define ALTAR_CHAR		'|'
#define LEVER_CHAR      '/'
#define LEVER_PULLED_CHAR '\\'
#define STATUE_CHAR		'&'
#define VENT_CHAR		'='

#define TRAMPLED_FOLIAGE_CHAR	'"'

#define PLAYER_CHAR		'@'

#define AMULET_CHAR		','
#define FOOD_CHAR		':'
#define SCROLL_CHAR		'?'
#define RING_CHAR		'='
#define CHARM_CHAR      '+'
#define POTION_CHAR		'!'
#define ARMOR_CHAR		'['
#define WEAPON_CHAR		'('
#define STAFF_CHAR		'\\'
#define WAND_CHAR		'~'
#define GOLD_CHAR		'*'
#define GEM_CHAR		'+'
#define TOTEM_CHAR		'0'
#define TURRET_CHAR		'*'
#define UNICORN_CHAR    'U'
#define KEY_CHAR		'-'

#define UP_ARROW_CHAR		'^'
#define DOWN_ARROW_CHAR		'v'
#define LEFT_ARROW_CHAR		'<'
#define RIGHT_ARROW_CHAR	'>'
#define UP_TRIANGLE_CHAR	'^'
#define DOWN_TRIANGLE_CHAR	'v'
#define OMEGA_CHAR			'^'
#define THETA_CHAR			'0'
#define LAMDA_CHAR			'\\'
#define KOPPA_CHAR			'k'
#define LOZENGE_CHAR		'+'
#define CROSS_PRODUCT_CHAR	'x'

#define CHAIN_TOP_LEFT		'\\'
#define CHAIN_BOTTOM_RIGHT	'\\'
#define CHAIN_TOP_RIGHT		'/'
#define CHAIN_BOTTOM_LEFT	'/'
#define CHAIN_TOP			'|'
#define CHAIN_BOTTOM		'|'
#define CHAIN_LEFT			'-'
#define CHAIN_RIGHT			'-'

#define BAD_MAGIC_CHAR		'+'
#define GOOD_MAGIC_CHAR		'$'

enum eventTypes {
	KEYSTROKE,
	MOUSE_UP,
	MOUSE_DOWN,
	RIGHT_MOUSE_DOWN,
	RIGHT_MOUSE_UP,
	MOUSE_ENTERED_CELL,
	RNG_CHECK,
	SAVED_GAME_LOADED,
	END_OF_RECORDING,
	EVENT_ERROR,
	NUMBER_OF_EVENT_TYPES, // unused
};

typedef struct rogueEvent {
	enum eventTypes eventType;
	signed long param1;
	signed long param2;
	boolean controlKey;
	boolean shiftKey;
} rogueEvent;


enum directions {
    NO_DIRECTION    = -1,
	// Cardinal directions; must be 0-3:
	UP				= 0,
	DOWN			= 1,
	LEFT			= 2,
	RIGHT			= 3,
	// Secondary directions; must be 4-7:
	UPLEFT			= 4,
	DOWNLEFT		= 5,
	UPRIGHT			= 6,
	DOWNRIGHT		= 7
};

enum textEntryTypes {
	TEXT_INPUT_NORMAL = 0,
	TEXT_INPUT_FILENAME,
	TEXT_INPUT_NUMBERS,
	TEXT_INPUT_TYPES,
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

enum lightType {
	NO_LIGHT,
	MINERS_LIGHT,
	BURNING_CREATURE_LIGHT,
};

// Item categories
enum itemCategory {
	FOOD				= Fl(0),
	WEAPON				= Fl(1),
	ARMOR				= Fl(2),
	POTION				= Fl(3),
	SCROLL				= Fl(4),
	STAFF				= Fl(5),
	WAND				= Fl(6),
	RING				= Fl(7),
    CHARM               = Fl(8),
	GOLD				= Fl(9),
	AMULET				= Fl(10),
	GEM					= Fl(11),
	KEY					= Fl(12),
	
	CAN_BE_DETECTED		= (WEAPON | ARMOR | POTION | SCROLL | RING | CHARM | WAND | STAFF | AMULET),
	PRENAMED_CATEGORY	= (FOOD | GOLD | AMULET | GEM | KEY),
    NEVER_IDENTIFIABLE  = (FOOD | CHARM | GOLD | AMULET | GEM | KEY),
    COUNTS_TOWARD_SCORE = (GOLD | AMULET | GEM),
	ALL_ITEMS			= (FOOD|POTION|WEAPON|ARMOR|STAFF|WAND|SCROLL|RING|CHARM|GOLD|AMULET|GEM|KEY),
};

enum potionKind {
	POTION_LIFE,
	POTION_STRENGTH,
	POTION_TELEPATHY,
	POTION_LEVITATION,
	POTION_DETECT_MAGIC,
	POTION_HASTE_SELF,
	POTION_FIRE_IMMUNITY,
	POTION_INVISIBILITY,
	POTION_POISON,
	POTION_PARALYSIS,
	POTION_HALLUCINATION,
	POTION_CONFUSION,
	POTION_INCINERATION,
	POTION_DARKNESS,
	POTION_DESCENT,
	POTION_LICHEN,
	NUMBER_POTION_KINDS
};

enum weaponKind {
	DAGGER,

	SWORD,
	RAPIER,
	
	MACE,
	
	SPEAR,
	
	AXE,
	
	NUMBER_WEAPON_KINDS
};

enum weaponEnchants {
	W_SPEED,
	W_FIRE,
	W_ICE,
	W_LIGHTNING,
};

enum armorKind {
	LEATHER_ARMOR,
	SCALE_MAIL,
	CHAIN_MAIL,
	BANDED_MAIL,
	SPLINT_MAIL,
	PLATE_MAIL,
	NUMBER_ARMOR_KINDS
};

enum armorEnchants {
	A_MULTIPLICITY,
	A_MUTUALITY,
	A_ABSORPTION,
	A_REPRISAL,
	A_IMMUNITY,
	A_REFLECTION,
    A_RESPIRATION,
    A_DAMPENING,
	A_BURDEN,
	NUMBER_GOOD_ARMOR_ENCHANT_KINDS = A_BURDEN,
	A_VULNERABILITY,
    A_IMMOLATION,
	NUMBER_ARMOR_ENCHANT_KINDS,
};


enum ringKind {
	RING_CLAIRVOYANCE,
	RING_STEALTH,
	RING_REGENERATION,
	RING_TRANSFERENCE,
	RING_LIGHT,
	RING_AWARENESS,
	RING_WISDOM,
	NUMBER_RING_KINDS
};

enum charmKind {
	CHARM_HEALTH,
    CHARM_PROTECTION,
    CHARM_HASTE,
    CHARM_FIRE_IMMUNITY,
    CHARM_INVISIBILITY,
    CHARM_TELEPATHY,
    CHARM_LEVITATION,
    CHARM_SHATTERING,
    CHARM_GUARDIAN,
    CHARM_TELEPORTATION,
    CHARM_RECHARGING,
    CHARM_NEGATION,
    NUMBER_CHARM_KINDS
};

enum scrollKind {
	SCROLL_ENCHANTING,
	SCROLL_IDENTIFY,
	SCROLL_TELEPORT,
	SCROLL_REMOVE_CURSE,
	SCROLL_RECHARGING,
	SCROLL_PROTECT_ARMOR,
	SCROLL_PROTECT_WEAPON,
	SCROLL_MAGIC_MAPPING,
	SCROLL_NEGATION,
	SCROLL_SHATTERING,
	SCROLL_AGGRAVATE_MONSTER,
	SCROLL_SUMMON_MONSTER,
	NUMBER_SCROLL_KINDS
};

#define MAX_PACK_ITEMS				26

enum monsterTypes {
	MK_YOU,
	MK_RAT,
	MK_KOBOLD,
	MK_JACKAL,
	MK_EEL,
	MK_MONKEY,
	MK_BLOAT,
	MK_PIT_BLOAT,
	MK_GOBLIN,
	MK_GOBLIN_CONJURER,
	MK_GOBLIN_MYSTIC,
	MK_GOBLIN_TOTEM,
	MK_PINK_JELLY,
	MK_TOAD,
	MK_VAMPIRE_BAT,
	MK_ARROW_TURRET,
	MK_ACID_MOUND,
	MK_CENTIPEDE,
	MK_OGRE,
	MK_BOG_MONSTER,
	MK_OGRE_TOTEM,
	MK_SPIDER,
	MK_SPARK_TURRET,
	MK_WILL_O_THE_WISP,
	MK_WRAITH,
	MK_ZOMBIE,
	MK_TROLL,
	MK_OGRE_SHAMAN,
	MK_NAGA,
	MK_SALAMANDER,
	MK_EXPLOSIVE_BLOAT,
	MK_DAR_BLADEMASTER,
	MK_DAR_PRIESTESS,
	MK_DAR_BATTLEMAGE,
	MK_ACID_JELLY,
	MK_CENTAUR,
	MK_UNDERWORM,
	MK_SENTINEL,
	MK_ACID_TURRET,
	MK_DART_TURRET,
	MK_KRAKEN,
	MK_LICH,
	MK_PHYLACTERY,
	MK_PIXIE,
	MK_PHANTOM,
	MK_FLAME_TURRET,
	MK_IMP,
	MK_FURY,
	MK_REVENANT,
	MK_TENTACLE_HORROR,
	MK_GOLEM,
	MK_DRAGON,
	
	MK_GOBLIN_CHIEFTAN,
	MK_BLACK_JELLY,
	MK_VAMPIRE,
	MK_FLAMESPIRIT,
	
	MK_SPECTRAL_BLADE,
	MK_SPECTRAL_IMAGE,
    MK_GUARDIAN,
    MK_WINGED_GUARDIAN,
    MK_CHARM_GUARDIAN,
    MK_ELDRITCH_TOTEM,
    MK_MIRRORED_TOTEM,
	
	MK_UNICORN,
	MK_IFRIT,
	MK_PHOENIX,
	MK_PHOENIX_EGG,
	
	NUMBER_MONSTER_KINDS
};


// Dungeon flags
enum tileFlags {
	DISCOVERED					= Fl(0),
	VISIBLE						= Fl(1),	// cell has sufficient light and is in field of view, ready to draw.
	HAS_PLAYER					= Fl(2),
	HAS_MONSTER					= Fl(3),
	HAS_DORMANT_MONSTER			= Fl(4),	// hidden monster on the square
	HAS_ITEM					= Fl(5),
	IN_FIELD_OF_VIEW			= Fl(6),	// player has unobstructed line of sight whether or not there is enough light
	WAS_VISIBLE					= Fl(7),
	HAS_DOWN_STAIRS				= Fl(8),
	HAS_UP_STAIRS				= Fl(9),
	IS_IN_SHADOW				= Fl(10),	// so that a player gains an automatic stealth bonus
	MAGIC_MAPPED				= Fl(11),
	ITEM_DETECTED				= Fl(12),
	CLAIRVOYANT_VISIBLE			= Fl(13),
	WAS_CLAIRVOYANT_VISIBLE		= Fl(14),
	CLAIRVOYANT_DARKENED		= Fl(15),	// magical blindness from a cursed ring of clairvoyance
	CAUGHT_FIRE_THIS_TURN		= Fl(16),	// so that fire does not spread asymmetrically
	PRESSURE_PLATE_DEPRESSED	= Fl(17),	// so that traps do not trigger repeatedly while you stand on them
	STABLE_MEMORY				= Fl(18),	// redraws will simply be pulled from the memory array, not recalculated
	KNOWN_TO_BE_TRAP_FREE		= Fl(19),	// keep track of where the player has stepped as he knows no traps are there
	IS_IN_PATH					= Fl(20),	// the yellow trail leading to the cursor
	IN_LOOP						= Fl(21),	// this cell is part of a terrain loop
	IS_CHOKEPOINT				= Fl(22),	// if this cell is blocked, part of the map will be rendered inaccessible
	IS_GATE_SITE				= Fl(23),	// consider placing a locked door here
	IS_IN_ROOM_MACHINE			= Fl(24),
	IS_IN_AREA_MACHINE			= Fl(25),
	IS_POWERED					= Fl(26),	// has been activated by machine power this turn (can probably be eliminate if needed)
	IMPREGNABLE					= Fl(27),	// no tunneling allowed!
	TERRAIN_COLORS_DANCING		= Fl(28),	// colors here will sparkle when the game is idle
	TELEPATHIC_VISIBLE			= Fl(29),	// potions of telepathy let you see through other creatures' eyes
	WAS_TELEPATHIC_VISIBLE		= Fl(30),	// potions of telepathy let you see through other creatures' eyes
	
    HAS_STAIRS                  = (HAS_UP_STAIRS | HAS_DOWN_STAIRS),
	IS_IN_MACHINE				= (IS_IN_ROOM_MACHINE | IS_IN_AREA_MACHINE), 	// sacred ground; don't generate items here, or teleport randomly to it
	
	PERMANENT_TILE_FLAGS = (DISCOVERED | MAGIC_MAPPED | ITEM_DETECTED | HAS_ITEM | HAS_DORMANT_MONSTER
							| HAS_UP_STAIRS | HAS_DOWN_STAIRS | PRESSURE_PLATE_DEPRESSED
							| STABLE_MEMORY | KNOWN_TO_BE_TRAP_FREE | IN_LOOP
							| IS_CHOKEPOINT | IS_GATE_SITE | IS_IN_MACHINE | IMPREGNABLE),
	
	ANY_KIND_OF_VISIBLE			= (VISIBLE | CLAIRVOYANT_VISIBLE | TELEPATHIC_VISIBLE),
};

#define TURNS_FOR_FULL_REGEN				300
#define STOMACH_SIZE						2150
#define HUNGER_THRESHOLD					(STOMACH_SIZE - 1800)
#define WEAK_THRESHOLD						150
#define FAINT_THRESHOLD						50
#define MAX_EXP_LEVEL						20
#define MAX_EXP								100000000L



// Keyboard commands:
#define UP_KEY				'k'
#define DOWN_KEY			'j'
#define LEFT_KEY			'h'
#define RIGHT_KEY			'l'
#define UP_ARROW			63232
#define LEFT_ARROW			63234
#define DOWN_ARROW			63233
#define RIGHT_ARROW			63235
#define UPLEFT_KEY			'y'
#define UPRIGHT_KEY			'u'
#define DOWNLEFT_KEY		'b'
#define DOWNRIGHT_KEY		'n'
#define DESCEND_KEY			'>'
#define ASCEND_KEY			'<'
#define REST_KEY			'z'
#define AUTO_REST_KEY		'Z'
#define SEARCH_KEY			's'
#define INVENTORY_KEY		'i'
#define ACKNOWLEDGE_KEY		' '
#define EQUIP_KEY			'e'
#define UNEQUIP_KEY			'r'
#define APPLY_KEY			'a'
#define THROW_KEY			't'
#define TRUE_COLORS_KEY		'\\'
#define AGGRO_DISPLAY_KEY   ']'
#define DROP_KEY			'd'
#define CALL_KEY			'c'
#define QUIT_KEY			'Q'
#define MESSAGE_ARCHIVE_KEY	'M'
#define HELP_KEY			'?'
#define DISCOVERIES_KEY		'D'
#define EXPLORE_KEY			'x'
#define AUTOPLAY_KEY		'A'
#define SEED_KEY			'~'
#define EASY_MODE_KEY		'&'
#define ESCAPE_KEY			'\033'
#define RETURN_KEY			'\015'
#define ENTER_KEY			'\012'
#define DELETE_KEY			'\177'
#define TAB_KEY				'\t'
#define PERIOD_KEY			'.'
#define VIEW_RECORDING_KEY	'V'
#define LOAD_SAVED_GAME_KEY	'O'
#define SAVE_GAME_KEY		'S'
#define NEW_GAME_KEY		'N'
#define NUMPAD_0			48
#define NUMPAD_1			49
#define NUMPAD_2			50
#define NUMPAD_3			51
#define NUMPAD_4			52
#define NUMPAD_5			53
#define NUMPAD_6			54
#define NUMPAD_7			55
#define NUMPAD_8			56
#define NUMPAD_9			57
#define PAGE_UP_KEY			63276
#define PAGE_DOWN_KEY		63277

#define UNKNOWN_KEY			(128+19)

#define min(x, y)		(((x) < (y)) ? (x) : (y))
#define max(x, y)		(((x) > (y)) ? (x) : (y))
#define clamp(x, low, hi)	(min(hi, max(x, low))) // pins x to the [y, z] interval

#define terrainFlags(x, y)					(tileCatalog[pmap[x][y].layers[DUNGEON]].flags \
											| tileCatalog[pmap[x][y].layers[LIQUID]].flags \
											| tileCatalog[pmap[x][y].layers[SURFACE]].flags \
											| tileCatalog[pmap[x][y].layers[GAS]].flags)

#define terrainMechFlags(x, y)				(tileCatalog[pmap[x][y].layers[DUNGEON]].mechFlags \
                                            | tileCatalog[pmap[x][y].layers[LIQUID]].mechFlags \
                                            | tileCatalog[pmap[x][y].layers[SURFACE]].mechFlags \
                                            | tileCatalog[pmap[x][y].layers[GAS]].mechFlags)



#define playerCanDirectlySee(x, y)			(pmap[x][y].flags & VISIBLE)
#define playerCanSee(x, y)					(pmap[x][y].flags & ANY_KIND_OF_VISIBLE)
#define playerCanSeeOrSense(x, y)			((pmap[x][y].flags & ANY_KIND_OF_VISIBLE) \
											|| (rogue.playbackOmniscience && (pmap[x][y].layers[DUNGEON] != GRANITE)))



// structs

enum dungeonLayers {
    NO_LAYER = -1,
	DUNGEON = 0,		// dungeon-level tile	(e.g. walls)
	LIQUID,				// liquid-level tile	(e.g. lava)
	GAS,				// gas-level tile		(e.g. fire, smoke, swamp gas)
	SURFACE,			// surface-level tile	(e.g. grass)
	NUMBER_TERRAIN_LAYERS
};


typedef struct pcell {								// permanent cell; have to remember this stuff to save levels
	enum tileType layers[NUMBER_TERRAIN_LAYERS];	// terrain
	unsigned long flags;							// non-terrain cell flags

} pcell;




typedef struct item {
	unsigned short category;
	short kind;
	unsigned long flags;
	randomRange damage;
	short armor;
	short charges;
	short enchant1;
	short enchant2;
	enum monsterTypes vorpalEnemy;
	short strengthRequired;
	unsigned short quiverNumber;
	char dipslayChar;
	short quantity;
	char inventoryLetter;
	char inscription[DCOLS];
	short xLoc;
	short yLoc;
	keyLocationProfile keyLoc[KEY_ID_MAXIMUM];
	short keyZ;
	struct item *nextItem;
} item;

typedef struct itemTable {
	char *name;
	char *flavor;
	char callTitle[30];
	short frequency;
	short marketValue;
	short strengthRequired;
	randomRange range;
	boolean identified;
	boolean called;
	char description[1500];
} itemTable;

enum dungeonFeatureTypes {
	DF_GRANITE_COLUMN = 1,
	DF_CRYSTAL_WALL,
	DF_LUMINESCENT_FUNGUS,
	DF_GRASS,
	DF_DEAD_GRASS,
	DF_BONES,
	DF_RUBBLE,
	DF_FOLIAGE,
	DF_FUNGUS_FOREST,
	DF_DEAD_FOLIAGE,
	
	DF_SUNLIGHT,
	DF_DARKNESS,
	
	DF_SHOW_DOOR,
	DF_SHOW_POISON_GAS_TRAP,
	DF_SHOW_PARALYSIS_GAS_TRAP,
	DF_SHOW_TRAPDOOR_HALO,
	DF_SHOW_TRAPDOOR,
	DF_SHOW_CONFUSION_GAS_TRAP,
	DF_SHOW_FLAMETHROWER_TRAP,
	DF_SHOW_FLOOD_TRAP,
    DF_SHOW_NET_TRAP,
	
	DF_RED_BLOOD,
	DF_GREEN_BLOOD,
	DF_PURPLE_BLOOD,
	DF_WORM_BLOOD,
	DF_ACID_BLOOD,
	DF_ASH_BLOOD,
	DF_EMBER_BLOOD,
	DF_ECTOPLASM_BLOOD,
	DF_RUBBLE_BLOOD,
	DF_ROT_GAS_BLOOD,
	
	DF_VOMIT,
	DF_BLOAT_DEATH,
	DF_BLOAT_EXPLOSION,
	DF_BLOOD_EXPLOSION,
	DF_FLAMEDANCER_CORONA,
    
	DF_MUTATION_EXPLOSION,
    DF_MUTATION_LICHEN,
	
	DF_REPEL_CREATURES,
	DF_ROT_GAS_PUFF,
	DF_STEAM_PUFF,
	DF_STEAM_ACCUMULATION,
	DF_METHANE_GAS_PUFF,
	DF_SALAMANDER_FLAME,
	DF_URINE,
	DF_UNICORN_POOP,
	DF_PUDDLE,
	DF_ASH,
	DF_ECTOPLASM_DROPLET,
	DF_FORCEFIELD,
	DF_LICHEN_GROW,
	DF_TUNNELIZE,
    DF_SHATTERING_SPELL,
	
	DF_TRAMPLED_FOLIAGE,
	DF_SMALL_DEAD_GRASS,
	DF_FOLIAGE_REGROW,
	DF_TRAMPLED_FUNGUS_FOREST,
	DF_FUNGUS_FOREST_REGROW,
	
    // brimstone
	DF_ACTIVE_BRIMSTONE,
	DF_INERT_BRIMSTONE,
    
    // bloodwort
    DF_BLOODFLOWER_PODS_GROW_INITIAL,
    DF_BLOODFLOWER_PODS_GROW,
    DF_BLOODFLOWER_POD_BURST,
    
    // algae
    DF_BUILD_ALGAE_WELL,
    DF_ALGAE_1,
    DF_ALGAE_2,
    DF_ALGAE_REVERT,
	
	DF_OPEN_DOOR,
	DF_CLOSED_DOOR,
	DF_OPEN_IRON_DOOR_INERT,
	DF_ITEM_CAGE_OPEN,
	DF_ITEM_CAGE_CLOSE,
	DF_ALTAR_INERT,
	DF_ALTAR_RETRACT,
	DF_PORTAL_ACTIVATE,
    DF_INACTIVE_GLYPH,
    DF_ACTIVE_GLYPH,
    DF_SILENT_GLYPH_GLOW,
    DF_GUARDIAN_STEP,
    DF_MIRROR_TOTEM_STEP,
    DF_GLYPH_CIRCLE,
    DF_REVEAL_LEVER,
    DF_PULL_LEVER,
    DF_CREATE_LEVER,
	
	DF_PLAIN_FIRE,
	DF_GAS_FIRE,
	DF_EXPLOSION_FIRE,
	DF_DART_EXPLOSION,
	DF_BRIMSTONE_FIRE,
	DF_BRIDGE_FIRE,
	DF_FLAMETHROWER,
	DF_EMBERS,
	DF_EMBERS_PATCH,
	DF_OBSIDIAN,
    DF_ITEM_FIRE,
    DF_CREATURE_FIRE,
    
	DF_FLOOD,
	DF_FLOOD_2,
	DF_FLOOD_DRAIN,
	DF_HOLE_2,
	DF_HOLE_DRAIN,
	
	DF_POISON_GAS_CLOUD,
	DF_CONFUSION_GAS_TRAP_CLOUD,
    DF_NET,
	DF_METHANE_GAS_ARMAGEDDON,
	
	// potions
	DF_POISON_GAS_CLOUD_POTION,
	DF_PARALYSIS_GAS_CLOUD_POTION,
	DF_CONFUSION_GAS_CLOUD_POTION,
	DF_INCINERATION_POTION,
	DF_DARKNESS_POTION,
	DF_HOLE_POTION,
	DF_LICHEN_PLANTED,
    
    // other items
    DF_ARMOR_IMMOLATION,
    DF_STAFF_HOLE,
    DF_STAFF_HOLE_EDGE,
	
	// vampire in coffin
	DF_COFFIN_BURSTS,
	DF_COFFIN_BURNS,
	DF_TRIGGER_AREA,
	
	// throwing tutorial -- button in chasm
	DF_CAGE_DISAPPEARS,
	DF_MEDIUM_HOLE,
	DF_MEDIUM_LAVA_POND,
    DF_MACHINE_PRESSURE_PLATE_USED,
    
    // rat trap
    DF_WALL_CRACK,
	
	// wooden barricade at entrance
	DF_WOODEN_BARRICADE_BURN,
	
	// wooden barricade around altar, dead grass all around
	DF_SURROUND_WOODEN_BARRICADE,
	
	// pools of water that, when triggered, slowly expand to fill the room
	DF_SPREADABLE_WATER,
	DF_SHALLOW_WATER,
	DF_WATER_SPREADS,
	DF_SPREADABLE_WATER_POOL,
	DF_SPREADABLE_DEEP_WATER_POOL,
	
	// when triggered, the ground gradually turns into chasm:
	DF_SPREADABLE_COLLAPSE,
	DF_COLLAPSE,
	DF_COLLAPSE_SPREADS,
	DF_ADD_MACHINE_COLLAPSE_EDGE_DORMANT,
	
	// when triggered, a bridge appears:
    DF_BRIDGE_ACTIVATE,
    DF_BRIDGE_ACTIVATE_ANNOUNCE,
	DF_BRIDGE_APPEARS,
    DF_ADD_DORMANT_CHASM_HALO,
	
	// when triggered, the lava retracts:
    DF_LAVA_RETRACTABLE,
	DF_RETRACTING_LAVA,
	DF_OBSIDIAN_WITH_STEAM,
	
	// when triggered, the door seals and poison gas fills the room
	DF_SHOW_POISON_GAS_VENT,
	DF_POISON_GAS_VENT_OPEN,
	DF_ACTIVATE_PORTCULLIS,
	DF_OPEN_PORTCULLIS,
	DF_VENT_SPEW_POISON_GAS,
	
	// when triggered, pilot light ignites and explosive gas fills the room
	DF_SHOW_METHANE_VENT,
	DF_METHANE_VENT_OPEN,
	DF_VENT_SPEW_METHANE,
	DF_PILOT_LIGHT,
    
    // paralysis trap: trigger plate with gas vents nearby
	DF_DISCOVER_PARALYSIS_VENT,
	DF_PARALYSIS_VENT_SPEW,
	DF_REVEAL_PARALYSIS_VENT_SILENTLY,
	
	// thematic dungeon
	DF_AMBIENT_BLOOD,
	
	// statues crack for a few turns and then shatter, revealing the monster inside
	DF_CRACKING_STATUE,
	DF_STATUE_SHATTER,
	
	// a turret appears:
	DF_TURRET_EMERGE,
    
    // an elaborate worm catacomb opens up
    DF_WORM_TUNNEL_MARKER_DORMANT,
    DF_WORM_TUNNEL_MARKER_ACTIVE,
    DF_GRANITE_CRUMBLES,
    DF_WALL_OPEN,
	
	// the room gradually darkens
	DF_DARKENING_FLOOR,
	DF_DARK_FLOOR,
    DF_HAUNTED_TORCH_TRANSITION,
    DF_HAUNTED_TORCH,
	
	// bubbles rise from the mud and bog monsters spawn
	DF_MUD_DORMANT,
	DF_MUD_ACTIVATE,
	
	// idyll:
	DF_WATER_POOL,
    DF_DEEP_WATER_POOL,
	
	// swamp:
	DF_SWAMP_WATER,
	DF_SWAMP,
	DF_SWAMP_MUD,
	
	// camp:
	DF_HAY,
	DF_JUNK,
	
	// remnants:
	DF_REMNANT,
	DF_REMNANT_ASH,
	
	// chasm catwalk:
	DF_CHASM_HOLE,
	DF_CATWALK_BRIDGE,
	
	// lake catwalk:
	DF_LAKE_CELL,
	DF_LAKE_HALO,
	
	// worm den:
	DF_WALL_SHATTER,
	
	// monster cages open:
	DF_MONSTER_CAGE_OPENS,
    
    // goblin warren:
    DF_STENCH_BURN,
    DF_STENCH_SMOLDER,
	
	NUMBER_DUNGEON_FEATURES,
};


// Dungeon features, spawned from Architect.c:
typedef struct dungeonFeature {
	// tile info:
	enum tileType tile;
	enum dungeonLayers layer;
	
	// spawning pattern:
	short startProbability;
	short probabilityDecrement;
	unsigned long flags;
	char description[DCOLS];
    enum lightType lightFlare;
	const color *flashColor;
	short flashRadius;
	enum tileType propagationTerrain;
	enum dungeonFeatureTypes subsequentDF;
	boolean messageDisplayed;
} dungeonFeature;

typedef struct floorTileType {
	// appearance:
	uchar displayChar;
	const color *foreColor;
	const color *backColor;
	// draw priority (lower number means higher priority):
	short drawPriority;
	char chanceToIgnite;					// chance to burn if a flame terrain is on one of the four cardinal neighbors
	enum dungeonFeatureTypes fireType;		// spawn this DF when the terrain ignites (or, if it's T_IS_DF_TRAP, when the pressure plate clicks)
	enum dungeonFeatureTypes discoverType;	// spawn this DF when successfully searched if T_IS_SECRET is set
	enum dungeonFeatureTypes promoteType;	// creates this dungeon spawn type when it promotes for some other reason (random promotion or promotion through machine activation)
	short promoteChance;					// percent chance per turn to spawn the promotion type; will also vanish upon doing so if T_VANISHES_UPON_PROMOTION is set
	short glowLight;						// if it glows, this is the ID of the light type
	unsigned long flags;
    unsigned long mechFlags;
	char description[COLS];
	char flavorText[COLS];
} floorTileType;



enum statusEffects {
    STATUS_DONNING = 0,
	STATUS_WEAKENED,
	STATUS_TELEPATHIC,
	STATUS_HALLUCINATING,
	STATUS_LEVITATING,
	STATUS_SLOWED,
	STATUS_HASTED,
	STATUS_CONFUSED,
	STATUS_BURNING,
	STATUS_PARALYZED,
	STATUS_POISONED,
	STATUS_STUCK,
	STATUS_NAUSEOUS,
	STATUS_DISCORDANT,
	STATUS_IMMUNE_TO_FIRE,
	STATUS_EXPLOSION_IMMUNITY,
	STATUS_NUTRITION,
	STATUS_ENTERS_LEVEL_IN,
	STATUS_MAGICAL_FEAR,
	STATUS_ENTRANCED,
	STATUS_DARKNESS,
	STATUS_LIFESPAN_REMAINING,
	STATUS_SHIELDED,
    STATUS_INVISIBLE,
	NUMBER_OF_STATUS_EFFECTS,
};


enum monsterBehaviorFlags {
	MONST_INVISIBLE					= Fl(0),	// monster is invisible
	MONST_INANIMATE					= Fl(1),	// monster has abbreviated stat bar display and is immune to many things
	MONST_IMMOBILE					= Fl(2),	// monster won't move or perform melee attacks
	MONST_CARRY_ITEM_100			= Fl(3),	// monster carries an item 100% of the time
	MONST_CARRY_ITEM_25				= Fl(4),	// monster carries an item 25% of the time
	MONST_ALWAYS_HUNTING			= Fl(5),	// monster is never asleep or in wandering mode
	MONST_FLEES_NEAR_DEATH			= Fl(6),	// monster flees when under 25% health and re-engages when over 75%
	MONST_ATTACKABLE_THRU_WALLS		= Fl(7),	// can be attacked when embedded in a wall
	MONST_DEFEND_DEGRADE_WEAPON		= Fl(8),	// hitting the monster damages the weapon
	MONST_IMMUNE_TO_WEAPONS			= Fl(9),	// weapons ineffective
	MONST_FLIES						= Fl(10),	// permanent levitation
	MONST_FLITS						= Fl(11),	// moves randomly a third of the time
	MONST_IMMUNE_TO_FIRE			= Fl(12),	// won't burn, won't die in lava
	MONST_CAST_SPELLS_SLOWLY		= Fl(13),	// takes twice the attack duration to cast a spell
	MONST_IMMUNE_TO_WEBS			= Fl(14),	// monster passes freely through webs
	MONST_REFLECT_4					= Fl(15),	// monster reflects projectiles as though wearing +4 armor of reflection
	MONST_NEVER_SLEEPS				= Fl(16),	// monster is always awake
	MONST_FIERY						= Fl(17),	// monster carries an aura of flame (but no automatic fire light)
	MONST_INTRINSIC_LIGHT			= Fl(18),	// monster carries an automatic light of the specified kind
	MONST_IMMUNE_TO_WATER			= Fl(19),	// monster moves at full speed in deep water and (if player) doesn't drop items
	MONST_RESTRICTED_TO_LIQUID		= Fl(20),	// monster can move only on tiles that allow submersion
	MONST_SUBMERGES					= Fl(21),	// monster can submerge in appropriate terrain
	MONST_MAINTAINS_DISTANCE		= Fl(22),	// monster tries to keep a distance of 3 tiles between it and player
	MONST_WILL_NOT_USE_STAIRS		= Fl(23),	// monster won't chase the player between levels
	MONST_DIES_IF_NEGATED			= Fl(24),	// monster will die if exposed to negation magic
	MONST_MALE						= Fl(25),	// monster is male (or 50% likely to be male if also has MONST_FEMALE)
	MONST_FEMALE					= Fl(26),	// monster is female (or 50% likely to be female if also has MONST_MALE)
    MONST_NOT_LISTED_IN_SIDEBAR     = Fl(27),   // monster doesn't show up in the sidebar
    MONST_GETS_TURN_ON_ACTIVATION   = Fl(28),   // monster never gets a turn, except when its machine is activated
    MONST_ALWAYS_USE_ABILITY        = Fl(29),   // monster will never fail to use special ability if eligible (no random factor)
    MONST_NO_POLYMORPH              = Fl(30),   // monster cannot result from a polymorph spell (liches and phoenixes)
	
	NEGATABLE_TRAITS				= (MONST_INVISIBLE | MONST_DEFEND_DEGRADE_WEAPON | MONST_IMMUNE_TO_WEAPONS | MONST_FLIES
									   | MONST_FLITS | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4 | MONST_FIERY | MONST_MAINTAINS_DISTANCE),
	MONST_TURRET					= (MONST_IMMUNE_TO_WEBS | MONST_NEVER_SLEEPS | MONST_IMMOBILE | MONST_INANIMATE |
									   MONST_ALWAYS_HUNTING | MONST_ATTACKABLE_THRU_WALLS | MONST_WILL_NOT_USE_STAIRS),
	LEARNABLE_BEHAVIORS				= (MONST_INVISIBLE | MONST_FLIES | MONST_IMMUNE_TO_FIRE | MONST_REFLECT_4),
	MONST_NEVER_VORPAL_ENEMY		= (MONST_INANIMATE | MONST_IMMOBILE | MONST_RESTRICTED_TO_LIQUID | MONST_GETS_TURN_ON_ACTIVATION | MONST_MAINTAINS_DISTANCE),
    MONST_NEVER_MUTATED             = (MONST_INVISIBLE | MONST_INANIMATE | MONST_IMMOBILE),
};

enum monsterAbilityFlags {
	MA_HIT_HALLUCINATE				= Fl(0),	// monster can hit to cause hallucinations
	MA_HIT_STEAL_FLEE				= Fl(1),	// monster can steal an item and then run away
	MA_ENTER_SUMMONS				= Fl(2),	// monster will "become" its summoned leader, reappearing when that leader is defeated
	MA_HIT_DEGRADE_ARMOR			= Fl(3),	// monster damages armor
	MA_CAST_HEAL					= Fl(4),
	MA_CAST_HASTE					= Fl(5),
	MA_CAST_PROTECTION				= Fl(6),
	MA_CAST_SUMMON					= Fl(7),	// requires that there be one or more summon hordes with this monster type as the leader
	MA_CAST_BLINK					= Fl(8),
	MA_CAST_NEGATION				= Fl(9),
	MA_CAST_SPARK					= Fl(10),
	MA_CAST_FIRE					= Fl(11),
	MA_CAST_SLOW					= Fl(12),
	MA_CAST_DISCORD					= Fl(13),
    MA_CAST_BECKONING               = Fl(14),
	MA_BREATHES_FIRE				= Fl(15),	// shoots dragonfire at player from a distance
	MA_SHOOTS_WEBS					= Fl(16),	// monster shoots webs at the player
	MA_ATTACKS_FROM_DISTANCE		= Fl(17),	// monster shoots from a distance for its attack
	MA_SEIZES						= Fl(18),	// monster seizes enemies before attacking and cannot attack flying enemies
	MA_POISONS						= Fl(19),	// monster's damage is dealt in the form of poison
	MA_DF_ON_DEATH					= Fl(20),	// monster spawns its DF when it dies
	MA_CLONE_SELF_ON_DEFEND			= Fl(21),	// monster splits in two when struck
	MA_KAMIKAZE						= Fl(22),	// monster dies instead of attacking
	MA_TRANSFERENCE					= Fl(23),	// monster recovers 40 or 90% of the damage that it inflicts as health
	MA_CAUSES_WEAKNESS				= Fl(24),	// monster attacks cause weakness status in target
    
    MA_ATTACKS_PENETRATE            = Fl(25),   // monster attacks all adjacent enemies, like an axe
    MA_ATTACKS_ALL_ADJACENT         = Fl(26),   // monster attacks penetrate one layer of enemies, like a spear
	
	MAGIC_ATTACK					= (MA_CAST_HEAL | MA_CAST_HASTE | MA_CAST_PROTECTION | MA_CAST_NEGATION | MA_CAST_SPARK | MA_CAST_FIRE | MA_CAST_SUMMON
									   | MA_CAST_SLOW | MA_CAST_DISCORD | MA_BREATHES_FIRE | MA_SHOOTS_WEBS | MA_ATTACKS_FROM_DISTANCE | MA_CAST_BECKONING),
	SPECIAL_HIT						= (MA_HIT_HALLUCINATE | MA_HIT_STEAL_FLEE | MA_HIT_DEGRADE_ARMOR | MA_POISONS | MA_TRANSFERENCE | MA_CAUSES_WEAKNESS),
	LEARNABLE_ABILITIES				= (MA_CAST_HEAL | MA_CAST_HASTE | MA_CAST_PROTECTION | MA_CAST_BLINK | MA_CAST_NEGATION | MA_CAST_SPARK | MA_CAST_FIRE
									   | MA_CAST_SLOW | MA_CAST_DISCORD | MA_TRANSFERENCE | MA_CAUSES_WEAKNESS),
    
    MA_NON_NEGATABLE_ABILITIES      = (MA_ATTACKS_FROM_DISTANCE | MA_ATTACKS_PENETRATE | MA_ATTACKS_ALL_ADJACENT),
    MA_NEVER_VORPAL_ENEMY           = (MA_KAMIKAZE),
    MA_NEVER_MUTATED                = (MA_KAMIKAZE),
};

enum monsterBookkeepingFlags {
	MONST_WAS_VISIBLE				= Fl(0),	// monster was visible to player last turn
	MONST_TELEPATHICALLY_REVEALED   = Fl(1),    // player can magically see monster and adjacent cells
	MONST_PREPLACED					= Fl(2),	// monster dropped onto the level and requires post-processing
	MONST_APPROACHING_UPSTAIRS		= Fl(3),	// following the player up the stairs
	MONST_APPROACHING_DOWNSTAIRS	= Fl(4),	// following the player down the stairs
	MONST_APPROACHING_PIT			= Fl(5),	// following the player down a pit
	MONST_LEADER					= Fl(6),	// monster is the leader of a horde
	MONST_FOLLOWER					= Fl(7),	// monster is a member of a horde
	MONST_CAPTIVE					= Fl(8),	// monster is all tied up
	MONST_SEIZED					= Fl(9),	// monster is being held
	MONST_SEIZING					= Fl(10),	// monster is holding another creature immobile
	MONST_SUBMERGED					= Fl(11),	// monster is currently submerged and hence invisible until it attacks
	MONST_JUST_SUMMONED				= Fl(12),	// used to mark summons so they can be post-processed
	MONST_WILL_FLASH				= Fl(13),	// this monster will flash as soon as control is returned to the player
	MONST_BOUND_TO_LEADER			= Fl(14),	// monster will die if the leader dies or becomes separated from the leader
	MONST_ABSORBING					= Fl(15),	// currently learning a skill by absorbing an enemy corpse
	MONST_DOES_NOT_TRACK_LEADER		= Fl(16),	// monster will not follow its leader around
	MONST_IS_FALLING				= Fl(17),	// monster is plunging downward at the end of the turn
	MONST_IS_DYING					= Fl(18),	// monster has already been killed and is awaiting the end-of-turn graveyard sweep.
	MONST_GIVEN_UP_ON_SCENT			= Fl(19),	// to help the monster remember that the scent map is a dead end
	MONST_IS_DORMANT				= Fl(20),	// lurking, waiting to burst out
    MONST_HAS_SOUL                  = Fl(21),   // slaying the monster will count toward weapon auto-ID
};

// Defines all creatures, which include monsters and the player:
typedef struct creatureType {
	enum monsterTypes monsterID; // index number for the monsterCatalog
	char monsterName[COLS];
	uchar displayChar;
	const color *foreColor;
	short maxHP;
	short defense;
	short accuracy;
	randomRange damage;
	long turnsBetweenRegen;		// turns to wait before regaining 1 HP
	short movementSpeed;
	short attackSpeed;
	enum dungeonFeatureTypes bloodType;
	enum lightType intrinsicLightType;
	short DFChance;						// percent chance to spawn the dungeon feature per awake turn
	enum dungeonFeatureTypes DFType;	// kind of dungeon feature
	unsigned long flags;
	unsigned long abilityFlags;
} creatureType;

typedef struct monsterWords {
	char flavorText[COLS*5];
	char absorbing[40];
	char absorbStatus[40];
	char attack[5][30];
	char DFMessage[DCOLS * 2];
	char summonMessage[DCOLS * 2];
} monsterWords;

enum creatureStates {
	MONSTER_SLEEPING,
	MONSTER_TRACKING_SCENT,
	MONSTER_WANDERING,
	MONSTER_FLEEING,
	MONSTER_ALLY,
};

enum creatureModes {
	MODE_NORMAL,
	MODE_PERM_FLEEING
};

typedef struct creature {
	creatureType info;
	short xLoc;
	short yLoc;
	short depth;
	short currentHP;
	long turnsUntilRegen;
	short regenPerTurn;					// number of HP to regenerate every single turn
	enum creatureStates creatureState;	// current behavioral state
	enum creatureModes creatureMode;	// current behavioral mode (higher-level than state)
    
    short mutationIndex;                // what mutation the monster has (or -1 for none)
    
    // Waypoints:
    short targetWaypointIndex;          // the index number of the waypoint we're pathing toward
    boolean waypointAlreadyVisited[MAX_WAYPOINT_COUNT]; // checklist of waypoints
	
	unsigned long absorptionFlags;		// ability/behavior flags that the monster will gain when absorption is complete
	boolean absorbBehavior;				// above flag is behavior instead of ability
	short corpseAbsorptionCounter;		// used to measure both the time until the monster stops being interested in the corpse,
										// and, later, the time until the monster finishes absorbing the corpse.
	short ticksUntilTurn;				// how long before the creature gets its next move
	
	// Locally cached statistics that may be temporarily modified:
	short movementSpeed;
	short attackSpeed;
	
	short status[NUMBER_OF_STATUS_EFFECTS];
	unsigned long bookkeepingFlags;
	struct creature *nextCreature;
	struct item *carriedItem;			// only used for monsters
} creature;

enum NGCommands {
	NG_NOTHING = 0,
	NG_NEW_GAME,
	NG_NEW_GAME_WITH_SEED,
	NG_OPEN_GAME,
	NG_VIEW_RECORDING,
	NG_HIGH_SCORES,
    NG_SCUM,
	NG_QUIT,
};

enum featTypes {
    FEAT_PURE_MAGE = 0,
    FEAT_PURE_WARRIOR,
    FEAT_PACIFIST,
    FEAT_ARCHIVIST,
    FEAT_COMPANION,
    FEAT_SPECIALIST,
    FEAT_JELLYMANCER,
    FEAT_INDOMITABLE,
    FEAT_MYSTIC,
    FEAT_DRAGONSLAYER,
    FEAT_PALADIN,
    
    FEAT_COUNT,
};

// these are basically global variables pertaining to the game state and player's unique variables:
typedef struct playerCharacter {
	short depthLevel;					// which dungeon level are we on
    short deepestLevel;
	boolean disturbed;					// player should stop auto-acting
	boolean gameHasEnded;				// stop everything and go to death screen
	boolean highScoreSaved;				// so that it saves the high score only once
	boolean blockCombatText;			// busy auto-fighting
	boolean autoPlayingLevel;			// seriously, don't interrupt
	boolean automationActive;			// cut some corners during redraws to speed things up
	boolean justRested;					// previous turn was a rest -- used in stealth
	boolean cautiousMode;				// used to prevent careless deaths caused by holding down a key
	boolean receivedLevitationWarning;	// only warn you once when you're hovering dangerously over liquid
	boolean updatedSafetyMapThisTurn;	// so it's updated no more than once per turn
	boolean updatedAllySafetyMapThisTurn;	// so it's updated no more than once per turn
	boolean updatedMapToSafeTerrainThisTurn;// so it's updated no more than once per turn
	boolean updatedMapToShoreThisTurn;		// so it's updated no more than once per turn
	boolean easyMode;					// enables easy mode
	boolean inWater;					// helps with the blue water filter effect
	boolean heardCombatThisTurn;		// so you get only one "you hear combat in the distance" per turn
	boolean creaturesWillFlashThisTurn;	// there are creatures out there that need to flash before the turn ends
	boolean staleLoopMap;				// recalculate the loop map at the end of the turn
	boolean alreadyFell;				// so the player can fall only one depth per turn
	boolean eligibleToUseStairs;		// so the player uses stairs only when he steps onto them
	boolean trueColorMode;				// whether lighting effects are disabled
    boolean displayAggroRangeMode;      // whether your stealth range is displayed
	boolean quit;						// to skip the typical end-game theatrics when the player quits
	unsigned long seed;					// the master seed for generating the entire dungeon
	short RNG;							// which RNG are we currently using?
	unsigned long gold;					// how much gold we have
	unsigned long goldGenerated;		// how much gold has been generated on the levels, not counting gold held by monsters
	short strength;
	unsigned short monsterSpawnFuse;	// how much longer till a random monster spawns
	
	item *weapon;
	item *armor;
	item *ringLeft;
	item *ringRight;
    
    flare **flares;
    short flareCount;
    short flareCapacity;
	
	lightSource minersLight;
	float minersLightRadius;
	short ticksTillUpdateEnvironment;	// so that some periodic things happen in objective time
	unsigned short scentTurnNumber;		// helps make scent-casting work
	unsigned long playerTurnNumber;     // number of input turns in recording. Does not increment during paralysis.
    unsigned long absoluteTurnNumber;   // number of turns since the beginning of time. Always increments.
	signed long milliseconds;			// milliseconds since launch, to decide whether to engage cautious mode
	short xpxpThisTurn;					// how many squares the player explored this turn
    short aggroRange;                   // distance from which monsters will notice you
    
	short previousHealthPercent;        // remembers what your health proportion was at the start of the turn,
    short previousPoisonPercent;        // and your poison proportion, to display percentage alerts for each
	
	short upLoc[2];						// upstairs location this level
	short downLoc[2];					// downstairs location this level
	
	short cursorLoc[2];					// used for the return key functionality
	creature *lastTarget;				// to keep track of the last monster the player has thrown at or zapped
	short rewardRoomsGenerated;			// to meter the number of reward machines
	short machineNumber;				// so each machine on a level gets a unique number
	short sidebarLocationList[ROWS*2][2];	// to keep track of which location each line of the sidebar references
	
	// maps
	short **mapToShore;					// how many steps to get back to shore
	short **mapToSafeTerrain;			// so monsters can get to safety
	
	// recording info
	boolean playbackMode;				// whether we're viewing a recording instead of playing
	unsigned long currentTurnNumber;	// how many turns have elapsed
	unsigned long howManyTurns;			// how many turns are in this recording
	short howManyDepthChanges;			// how many times the player changes depths
	short playbackDelayPerTurn;			// base playback speed; modified per turn by events
	short playbackDelayThisTurn;		// playback speed as modified
	boolean playbackPaused;
	boolean playbackFastForward;		// for loading saved games and such -- disables drawing and prevents pauses
	boolean playbackOOS;				// playback out of sync -- no unpausing allowed
	boolean playbackOmniscience;		// whether to reveal all the map during playback
	boolean playbackBetweenTurns;		// i.e. waiting for a top-level input -- iff, permit playback commands
	unsigned long nextAnnotationTurn;	// the turn number during which to display the next annotation
	char nextAnnotation[5000];			// the next annotation
	unsigned long locationInAnnotationFile; // how far we've read in the annotations file
	
	// metered items
	long foodSpawned;					// amount of nutrition units spawned so far this game
	short lifePotionFrequency;
    short lifePotionsSpawned;
	short strengthPotionFrequency;
	short enchantScrollFrequency;
	
	// ring bonuses:
	short clairvoyance;
	short stealthBonus;
	short regenerationBonus;
	short lightMultiplier;
	short aggravating;
	short awarenessBonus;
	short transference;
	short wisdomBonus;
    
    // feats:
    boolean featRecord[FEAT_COUNT];
    
    // waypoints:
    short **wpDistance[MAX_WAYPOINT_COUNT];
    short wpCount;
    short wpCoordinates[MAX_WAYPOINT_COUNT][2];
    short wpRefreshTicker;
	
	// cursor trail:
	short cursorPathIntensity;
	
	// What do you want to do, player -- play, play with seed, resume, recording, high scores or quit?
	enum NGCommands nextGame;
	char nextGamePath[BROGUE_FILENAME_MAX];
	unsigned long nextGameSeed;
} playerCharacter;

// Stores the necessary info about a level so it can be regenerated:
typedef struct levelData {
	boolean visited;
	pcell mapStorage[DCOLS][DROWS];
	struct item *items;
	struct creature *monsters;
	unsigned long levelSeed;
	short upStairsLoc[2];
	short downStairsLoc[2];
	short playerExitedVia[2];
	unsigned long awaySince;
} levelData;





#define PDS_FORBIDDEN   -1
#define PDS_OBSTRUCTION -2
#define PDS_CELL(map, x, y) ((map)->links + ((x) + DCOLS * (y)))

typedef struct pdsLink pdsLink;
typedef struct pdsMap pdsMap;


enum buttonDrawStates {
	BUTTON_NORMAL = 0,
	BUTTON_HOVER,
	BUTTON_PRESSED,
};



#if defined __cplusplus
extern "C" {
#endif
	
	void rogueMain();
	void executeEvent(rogueEvent *theEvent);
	boolean fileExists(const char *pathname);
	boolean chooseFile(char *path, char *prompt, char *defaultName, char *suffix);
	boolean openFile(const char *path);
	void initializeRogue(unsigned long seed);
	void gameOver(char *killedBy, boolean useCustomPhrasing);
    void victory(boolean superVictory);
	void enableEasyMode();
	int rand_range(int lowerBound, int upperBound);
	unsigned long seedRandomGenerator(unsigned long seed);
	short randClumpedRange(short lowerBound, short upperBound, short clumpFactor);
	short randClump(randomRange theRange);
	boolean rand_percent(short percent);
	void shuffleList(short *list, short listLength);
    void fillSequentialList(short *list, short listLength);
	short unflag(unsigned long flag);
	void considerCautiousMode();
	void refreshScreen();
	void displayLevel();
	void storeColorComponents(char components[3], const color *theColor);
	boolean separateColors(color *fore, color *back);
	void bakeColor(color *theColor);
	void shuffleTerrainColors(short percentOfCells, boolean refreshCells);
	void getCellAppearance(short x, short y, uchar *returnChar, color *returnForeColor, color *returnBackColor);
	void logBuffer(char array[DCOLS][DROWS]);
	//void logBuffer(short **array);
	boolean search(short searchStrength);
	void routeTo(short x, short y, char *failureMessage);
	boolean useStairs(short stairDirection);
	short passableArcCount(short x, short y);
	void analyzeMap(boolean calculateChokeMap);
	boolean buildAMachine(enum machineTypes bp,
						  short originX, short originY,
						  unsigned long requiredMachineFlags,
						  item *adoptiveItem,
						  item *parentSpawnedItems[50],
						  creature *parentSpawnedMonsters[50]);
    void attachRooms(short **grid, const short roomFrequencies[ROOM_TYPE_COUNT], short corridorPercent, short attempts, short maxRoomCount);
	void digDungeon();
	void updateMapToShore();
	short levelIsDisconnectedWithBlockingMap(char blockingMap[DCOLS][DROWS], boolean countRegionSize);
	void resetDFMessageEligibility();
	boolean fillSpawnMap(enum dungeonLayers layer,
						 enum tileType surfaceTileType,
						 char spawnMap[DCOLS][DROWS],
						 boolean blockedByOtherLayers,
						 boolean refresh,
						 boolean superpriority);
	boolean spawnDungeonFeature(short x, short y, dungeonFeature *feat, boolean refreshCell, boolean abortIfBlocking);
	void restoreMonster(creature *monst, short **mapToStairs, short **mapToPit);
	void restoreItem(item *theItem);
    void refreshWaypoint(short wpIndex);
	void setUpWaypoints();
	void zeroOutGrid(char grid[DCOLS][DROWS]);
	short oppositeDirection(short theDir);

	void plotChar(uchar inputChar,
				  short xLoc, short yLoc,
				  short backRed, short backGreen, short backBlue,
				  short foreRed, short foreGreen, short foreBlue);
	void pausingTimerStartsNow();
	boolean pauseForMilliseconds(short milliseconds);
	void nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance);
	boolean controlKeyIsDown();
	boolean shiftKeyIsDown();
	short getHighScoresList(rogueHighScoresEntry returnList[HIGH_SCORES_COUNT]);
	boolean saveHighScore(rogueHighScoresEntry theEntry);
	void initializeBrogueSaveLocation();
	fileEntry *listFiles(short *fileCount, char **dynamicMemoryBuffer);
	void initializeLaunchArguments(enum NGCommands *command, char *path, unsigned long *seed);
	
	char nextKeyPress(boolean textInput);
	void refreshSideBar(short focusX, short focusY, boolean focusedEntityMustGoFirst);
	void printHelpScreen();
	void printDiscoveriesScreen();
	void printHighScores(boolean hiliteMostRecent);
	void displayGrid(short **map);
	void printSeed();
	void printProgressBar(short x, short y, const char barLabel[COLS], long amtFilled, long amtMax, color *fillColor, boolean dim);
	short printMonsterInfo(creature *monst, short y, boolean dim, boolean highlight);
	short printItemInfo(item *theItem, short y, boolean dim, boolean highlight);
    short printTerrainInfo(short x, short y, short py, const char *description, boolean dim, boolean highlight);
	void rectangularShading(short x, short y, short width, short height,
							const color *backColor, short opacity, cellDisplayBuffer dbuf[COLS][ROWS]);
	short printTextBox(char *textBuf, short x, short y, short width,
					   color *foreColor, color *backColor,
					   cellDisplayBuffer rbuf[COLS][ROWS],
					   brogueButton *buttons, short buttonCount);
	void printMonsterDetails(creature *monst, cellDisplayBuffer rbuf[COLS][ROWS]);
	void printFloorItemDetails(item *theItem, cellDisplayBuffer rbuf[COLS][ROWS]);
	unsigned long printCarriedItemDetails(item *theItem,
										  short x, short y, short width,
										  boolean includeButtons,
										  cellDisplayBuffer rbuf[COLS][ROWS]);
	void funkyFade(cellDisplayBuffer displayBuf[COLS][ROWS], const color *colorStart, const color *colorEnd, short stepCount, short x, short y, boolean invert);
	void displayCenteredAlert(char *message);
	void flashMessage(char *message, short x, short y, int time, color *fColor, color *bColor);
	void flashTemporaryAlert(char *message, int time);
	void waitForAcknowledgment();
	void waitForKeystrokeOrMouseClick();
	boolean confirm(char *prompt, boolean alsoDuringPlayback);
	void refreshDungeonCell(short x, short y);
	void applyColorMultiplier(color *baseColor, const color *multiplierColor);
	void applyColorAverage(color *baseColor, const color *newColor, short averageWeight);
	void applyColorAugment(color *baseColor, const color *augmentingColor, short augmentWeight);
    void applyColorScalar(color *baseColor, short scalar);
    void applyColorBounds(color *baseColor, short lowerBound, short upperBound);
	void desaturate(color *baseColor, short weight);
	void randomizeColor(color *baseColor, short randomizePercent);
    void irisFadeBetweenBuffers(cellDisplayBuffer fromBuf[COLS][ROWS],
                                cellDisplayBuffer toBuf[COLS][ROWS],
                                short x, short y,
                                short frameCount,
                                boolean outsideIn);
	void colorBlendCell(short x, short y, color *hiliteColor, short hiliteStrength);
	void hiliteCell(short x, short y, const color *hiliteColor, short hiliteStrength, boolean distinctColors);
	void colorMultiplierFromDungeonLight(short x, short y, color *editColor);
	void plotCharWithColor(uchar inputChar, short xLoc, short yLoc, const color *cellForeColor, const color *cellBackColor);
	void plotCharToBuffer(uchar inputChar, short x, short y, color *foreColor, color *backColor, cellDisplayBuffer dbuf[COLS][ROWS]);
	void commitDraws();
	void dumpLevelToScreen();
	void hiliteCharGrid(char hiliteCharGrid[DCOLS][DROWS], color *hiliteColor, short hiliteStrength);
	void blackOutScreen();
    void colorOverDungeon(const color *color);
	void copyDisplayBuffer(cellDisplayBuffer toBuf[COLS][ROWS], cellDisplayBuffer fromBuf[COLS][ROWS]);
	void clearDisplayBuffer(cellDisplayBuffer dbuf[COLS][ROWS]);
	color colorFromComponents(char rgb[3]);
	void overlayDisplayBuffer(cellDisplayBuffer overBuf[COLS][ROWS], cellDisplayBuffer previousBuf[COLS][ROWS]);
	void flashForeground(short *x, short *y, color **flashColor, short *flashStrength, short count, short frames);
	void flash(color *theColor, short frames, short x, short y);
	void colorFlash(const color *theColor, unsigned long reqTerrainFlags, unsigned long reqTileFlags, short frames, short maxRadius, short x, short y);
	void printString(const char *theString, short x, short y, color *foreColor, color*backColor, cellDisplayBuffer dbuf[COLS][ROWS]);
	short wrapText(char *to, const char *sourceText, short width);
	short printStringWithWrapping(char *theString, short x, short y, short width, color *foreColor,
								  color*backColor, cellDisplayBuffer dbuf[COLS][ROWS]);
	boolean getInputTextString(char *inputText,
							   const char *prompt,
							   short maxLength,
							   const char *defaultEntry,
							   const char *promptSuffix,
							   short textEntryType,
							   boolean useDialogBox);
	void displayChokeMap();
	void displayLoops();
	boolean pauseBrogue(short milliseconds);
	void nextBrogueEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance, boolean realInputEvenInPlayback);
	void executeMouseClick(rogueEvent *theEvent);
	void executeKeystroke(signed long keystroke, boolean controlKey, boolean shiftKey);
	void initializeLevel();
	void startLevel (short oldLevelNumber, short stairDirection);
	void updateMinersLightRadius();
	void freeCreature(creature *monst);
	void emptyGraveyard();
	void freeEverything();
	boolean randomMatchingLocation(short *x, short *y, short dungeonType, short liquidType, short terrainType);
	enum dungeonLayers highestPriorityLayer(short x, short y, boolean skipGas);
    enum dungeonLayers layerWithTMFlag(short x, short y, unsigned long flag);
	enum dungeonLayers layerWithFlag(short x, short y, unsigned long flag);
	char *tileFlavor(short x, short y);
	char *tileText(short x, short y);
	void describedItemCategory(short theCategory, char *buf);
	void describeLocation(char buf[DCOLS], short x, short y);
	void printLocationDescription(short x, short y);
    void useKeyAt(item *theItem, short x, short y);
	void playerRuns(short direction);
	void exposeCreatureToFire(creature *monst);
	void updateFlavorText();
    void updatePlayerUnderwaterness();
    boolean monsterShouldFall(creature *monst);
	void applyInstantTileEffectsToCreature(creature *monst);
	void vomit(creature *monst);
	void becomeAllyWith(creature *monst);
	void freeCaptive(creature *monst);
	boolean freeCaptivesEmbeddedAt(short x, short y);
    boolean diagonalBlocked(short x1, short y1, short x2, short y2);
	boolean playerMoves(short direction);
	void calculateDistances(short **distanceMap,
							short destinationX, short destinationY,
							unsigned long blockingTerrainFlags,
							creature *traveler,
							boolean canUseSecretDoors,
							boolean eightWays);
	short pathingDistance(short x1, short y1, short x2, short y2, unsigned long blockingTerrainFlags);
    short nextStep(short **distanceMap, short x, short y, creature *monst, boolean reverseDirections);
	void travelRoute(short path[1000][2], short steps);
	void travel(short x, short y, boolean autoConfirm);
    void populateGenericCostMap(short **costMap);
	void populateCreatureCostMap(short **costMap, creature *monst);
	void getExploreMap(short **map, boolean headingToStairs);
	boolean explore(short frameDelay);
	void clearCursorPath();
	void mainInputLoop();
	boolean isDisturbed(short x, short y);
	void discover(short x, short y);
	short randValidDirectionFrom(creature *monst, short x, short y, boolean respectAvoidancePreferences);
	boolean exposeTileToFire(short x, short y, boolean alwaysIgnite);
	boolean cellCanHoldGas(short x, short y);
	void monstersFall();
	void updateEnvironment();
	void updateAllySafetyMap();
	void updateSafetyMap();
	void updateSafeTerrainMap();
	void extinguishFireOnCreature(creature *monst);
	void autoRest();
	void startFighting(enum directions dir, boolean tillDeath);
	void autoFight(boolean tillDeath);
    void synchronizePlayerTimeState();
	void playerTurnEnded();
	void resetScentTurnNumber();
	void displayMonsterFlashes(boolean flashingEnabled);
	void displayMessageArchive();
	void temporaryMessage(char *msg1, boolean requireAcknowledgment);
	void messageWithColor(char *msg, color *theColor, boolean requireAcknowledgment);
	void flavorMessage(char *msg);
	void message(const char *msg, boolean requireAcknowledgment);
    void displayMoreSignWithoutWaitingForAcknowledgment();
	void displayMoreSign();
	short encodeMessageColor(char *msg, short i, const color *theColor);
	short decodeMessageColor(const char *msg, short i, color *returnColor);
	color *messageColorFromVictim(creature *monst);
	void upperCase(char *theChar);
	void updateMessageDisplay();
	void deleteMessages();
	void confirmMessages();
	void stripShiftFromMovementKeystroke(signed long *keystroke);
	
	void updateFieldOfViewDisplay(boolean updateDancingTerrain, boolean refreshDisplay);
	void updateFieldOfView(short xLoc, short yLoc, short radius, boolean paintScent,
						   boolean passThroughCreatures, boolean setFieldOfView, short theColor[3], short fadeToPercent);
	void betweenOctant1andN(short *x, short *y, short x0, short y0, short n);
	
	void getFOVMask(char grid[DCOLS][DROWS], short xLoc, short yLoc, float maxRadius,
					unsigned long forbiddenTerrain,	unsigned long forbiddenFlags, boolean cautiousOnWalls);
	void scanOctantFOV(char grid[DCOLS][DROWS], short xLoc, short yLoc, short octant, float maxRadius,
					   short columnsRightFromOrigin, long startSlope, long endSlope, unsigned long forbiddenTerrain,
					   unsigned long forbiddenFlags, boolean cautiousOnWalls);
	
    creature *generateMonster(short monsterID, boolean itemPossible, boolean mutationPossible);
	short chooseMonster(short forLevel);
	creature *spawnHorde(short hordeID, short x, short y, unsigned long forbiddenFlags, unsigned long requiredFlags);
	void fadeInMonster(creature *monst);
	boolean removeMonsterFromChain(creature *monst, creature *theChain);
    boolean monsterWillAttackTarget(const creature *attacker, const creature *defender);
	boolean monstersAreTeammates(const creature *monst1, const creature *monst2);
	boolean monstersAreEnemies(const creature *monst1, const creature *monst2);
	void initializeGender(creature *monst);
    boolean stringsMatch(const char *str1, const char *str2);
	void resolvePronounEscapes(char *text, creature *monst);
	short pickHordeType(short depth, enum monsterTypes summonerType, unsigned long forbiddenFlags, unsigned long requiredFlags);
	creature *cloneMonster(creature *monst, boolean announce, boolean placeClone);
    void empowerMonster(creature *monst);
	unsigned long forbiddenFlagsForMonster(creatureType *monsterType);
    unsigned long avoidedFlagsForMonster(creatureType *monsterType);
	boolean monsterCanSubmergeNow(creature *monst);
	void populateMonsters();
	void updateMonsterState(creature *monst);
	void decrementMonsterStatus(creature *monst);
	boolean specifiedPathBetween(short x1, short y1, short x2, short y2,
								 unsigned long blockingTerrain, unsigned long blockingFlags);
    boolean traversiblePathBetween(creature *monst, short x2, short y2);
	boolean openPathBetween(short x1, short y1, short x2, short y2);
	creature *monsterAtLoc(short x, short y);
	creature *dormantMonsterAtLoc(short x, short y);
	void perimeterCoords(short returnCoords[2], short n);
	boolean monsterBlinkToPreferenceMap(creature *monst, short **preferenceMap, boolean blinkUphill);
    boolean monsterSummons(creature *monst, boolean alwaysUse);
	void unAlly(creature *monst);
    boolean monsterFleesFrom(creature *monst, creature *defender);
	void monstersTurn(creature *monst);
	void spawnPeriodicHorde();
	void clearStatus(creature *monst);
	void monsterShoots(creature *attacker, short targetLoc[2], uchar projChar, color *projColor);
	void shootWeb(creature *breather, short targetLoc[2], short kindOfWeb);
	void moralAttack(creature *attacker, creature *defender);
	short runicWeaponChance(item *theItem, boolean customEnchantLevel, float enchantLevel);
	void magicWeaponHit(creature *defender, item *theItem, boolean backstabbed);
    void teleport(creature *monst, short x, short y, boolean respectTerrainAvoidancePreferences);
	void chooseNewWanderDestination(creature *monst);
	boolean canPass(creature *mover, creature *blocker);
	boolean isPassableOrSecretDoor(short x, short y);
	boolean moveMonster(creature *monst, short dx, short dy);
    unsigned long burnedTerrainFlagsAtLoc(short x, short y);
    unsigned long discoveredTerrainFlagsAtLoc(short x, short y);
	boolean monsterAvoids(creature *monst, short x, short y);
	short distanceBetween(short x1, short y1, short x2, short y2);
	void wakeUp(creature *monst);
    boolean monsterRevealed(creature *monst);
	boolean canSeeMonster(creature *monst);
	boolean canDirectlySeeMonster(creature *monst);
	void monsterName(char *buf, creature *monst, boolean includeArticle);
	float strengthModifier(item *theItem);
	float netEnchant(item *theItem);
	short hitProbability(creature *attacker, creature *defender);
	boolean attackHit(creature *attacker, creature *defender);
	void applyArmorRunicEffect(char returnString[DCOLS], creature *attacker, short *damage, boolean melee);
    boolean attack(creature *attacker, creature *defender, boolean lungeAttack);
	void inflictLethalDamage(creature *defender);
	boolean inflictDamage(creature *defender, short damage, const color *flashColor, boolean ignoresProtectionShield);
    void addPoison(creature *monst, short damage);
	void killCreature(creature *decedent, boolean administrativeDeath);
    void buildHitList(creature **hitList,
                      const creature *attacker, creature *defender,
                      const boolean penetrate, const boolean sweep);
	void addScentToCell(short x, short y, short distance);
	void populateItems(short upstairsX, short upstairsY);
	item *placeItem(item *theItem, short x, short y);
	void removeItemFrom(short x, short y);
	void pickUpItemAt(short x, short y);
	item *addItemToPack(item *theItem);
	short getLineCoordinates(short listOfCoordinates[][2], short originLoc[2], short targetLoc[2]);
	void getImpactLoc(short returnLoc[2], short originLoc[2], short targetLoc[2],
					  short maxDistance, boolean returnLastEmptySpace);
	void negate(creature *monst);
    short monsterAccuracyAdjusted(const creature *monst);
    float monsterDamageAdjustmentAmount(const creature *monst);
    short monsterDefenseAdjusted(const creature *monst);
	void weaken(creature *monst, short maxDuration);
	void slow(creature *monst, short turns);
	void haste(creature *monst, short turns);
	void heal(creature *monst, short percent);
	boolean projectileReflects(creature *attacker, creature *defender);
	short reflectBolt(short targetX, short targetY, short listOfCoordinates[][2], short kinkCell, boolean retracePath);
	void checkForMissingKeys(short x, short y);
	boolean zap(short originLoc[2], short targetLoc[2], enum boltType bolt, short boltLevel, boolean hideDetails);
    boolean nextTargetAfter(short *returnX,
                            short *returnY,
                            short targetX,
                            short targetY,
                            boolean targetEnemies,
                            boolean targetAllies,
                            boolean targetItems,
                            boolean targetTerrain,
                            boolean requireOpenPath);
	boolean moveCursor(boolean *targetConfirmed,
					   boolean *canceled,
					   boolean *tabKey,
					   short targetLoc[2],
					   rogueEvent *event,
					   buttonState *state,
					   boolean colorsDance,
					   boolean keysMoveCursor,
					   boolean targetCanLeaveMap);
    void identifyItemKind(item *theItem);
    void autoIdentify(item *theItem);
	short numberOfItemsInPack();
	char nextAvailableInventoryCharacter();
    void updateFloorItems();
	void itemName(item *theItem, char *root, boolean includeDetails, boolean includeArticle, color *baseColor);
	char displayInventory(unsigned short categoryMask,
						  unsigned long requiredFlags,
						  unsigned long forbiddenFlags,
						  boolean waitForAcknowledge,
						  boolean includeButtons);
	short numberOfMatchingPackItems(unsigned short categoryMask,
									unsigned long requiredFlags, unsigned long forbiddenFlags,
									boolean displayErrors);
	void clearInventory(char keystroke);
	item *generateItem(unsigned short theCategory, short theKind);
	short chooseKind(itemTable *theTable, short numKinds);
	item *makeItemInto(item *theItem, unsigned long itemCategory, short itemKind);
	void updateEncumbrance();
    short displayedArmorValue();
	void strengthCheck(item *theItem);
	void recalculateEquipmentBonuses();
	void equipItem(item *theItem, boolean force);
	void equip(item *theItem);
	item *keyInPackFor(short x, short y);
	item *keyOnTileAt(short x, short y);
	void unequip(item *theItem);
	void drop(item *theItem);
	void findAlternativeHomeFor(creature *monst, short *x, short *y, boolean chooseRandomly);
	boolean getQualifyingLocNear(short loc[2],
								 short x, short y,
								 boolean hallwaysAllowed,
								 char blockingMap[DCOLS][DROWS],
								 unsigned long forbiddenTerrainFlags,
								 unsigned long forbiddenMapFlags,
								 boolean forbidLiquid,
								 boolean deterministic);
	boolean getQualifyingGridLocNear(short loc[2],
									 short x, short y,
									 boolean grid[DCOLS][DROWS],
									 boolean deterministic);
    
    // Grid operations
	short **allocGrid();
	void freeGrid(short **array);
	void copyGrid(short **to, short **from);
	void fillGrid(short **grid, short fillValue);
    void hiliteGrid(short **grid, color *hiliteColor, short hiliteStrength);
    void findReplaceGrid(short **grid, short findValueMin, short findValueMax, short fillValue);
    short floodFillGrid(short **grid, short x, short y, short eligibleValueMin, short eligibleValueMax, short fillValue);
    void drawRectangleOnGrid(short **grid, short x, short y, short width, short height, short value);
    void drawCircleOnGrid(short **grid, short x, short y, short radius, short value);
    void getTerrainGrid(short **grid, short value, unsigned long terrainFlags, unsigned long mapFlags);
    void getTMGrid(short **grid, short value, unsigned long TMflags);
    short validLocationCount(short **grid, short validValue);
    void randomLocationInGrid(short **grid, short *x, short *y, short validValue);
    boolean getQualifyingPathLocNear(short *retValX, short *retValY,
                                     short x, short y,
                                     boolean hallwaysAllowed,
                                     unsigned long blockingTerrainFlags,
                                     unsigned long blockingMapFlags,
                                     unsigned long forbiddenTerrainFlags,
                                     unsigned long forbiddenMapFlags,
                                     boolean deterministic);
    void createBlobOnGrid(short **grid,
                          short *retMinX, short *retMinY, short *retWidth, short *retHeight,
                          short roundCount,
                          short minBlobWidth, short minBlobHeight,
                          short maxBlobWidth, short maxBlobHeight, short percentSeeded,
                          char birthParameters[9], char survivalParameters[9]);
    
	void demoteMonsterFromLeadership(creature *monst);
	void toggleMonsterDormancy(creature *monst);
	void monsterDetails(char buf[], creature *monst);
	void makeMonsterDropItem(creature *monst);
	void throwCommand(item *theItem);
	void apply(item *theItem, boolean recordCommands);
	boolean itemCanBeCalled(item *theItem);
	void call(item *theItem);
	enum monsterTypes chooseVorpalEnemy();
	void identify(item *theItem);
	void updateIdentifiableItem(item *theItem);
	void updateIdentifiableItems();
	void readScroll(item *theItem);
	void updateRingBonuses();
	void updatePlayerRegenerationDelay();
	boolean removeItemFromChain(item *theItem, item *theChain);
    void addItemToChain(item *theItem, item *theChain);
	void drinkPotion(item *theItem);
	item *promptForItemOfType(unsigned short category,
							  unsigned long requiredFlags,
							  unsigned long forbiddenFlags,
							  char *prompt,
							  boolean allowInventoryActions);
	item *itemOfPackLetter(char letter);
	void unequipItem(item *theItem, boolean force);
	short magicCharDiscoverySuffix(short category, short kind);
	uchar itemMagicChar(item *theItem);
	item *itemAtLoc(short x, short y);
	item *dropItem(item *theItem);
	itemTable *tableForItemCategory(enum itemCategory theCat);
	boolean isVowelish(char *theChar);
    short charmEffectDuration(short charmKind, short enchant);
    short charmRechargeDelay(short charmKind, short enchant);
	boolean itemIsCarried(item *theItem);
	void itemDetails(char *buf, item *theItem);
	void deleteItem(item *theItem);
	void shuffleFlavors();
	unsigned long itemValue(item *theItem);
	short strLenWithoutEscapes(const char *str);
	void combatMessage(char *theMsg, color *theColor);
	void displayCombatText();
	void flashMonster(creature *monst, const color *theColor, short strength);
	
    boolean paintLight(lightSource *theLight, short x, short y, boolean isMinersLight, boolean maintainShadows);
    void backUpLighting(short lights[DCOLS][DROWS][3]);
    void restoreLighting(short lights[DCOLS][DROWS][3]);
	void updateLighting();
	boolean playerInDarkness();
    flare *newFlare(lightSource *light, short x, short y, short changePerFrame, short limit);
    void createFlare(short x, short y, enum lightType lightIndex);
    void animateFlares(flare **flares, short count);
    void deleteAllFlares();
	void demoteVisibility();
	void updateVision(boolean refreshDisplay);
	void burnItem(item *theItem);
	void promoteTile(short x, short y, enum dungeonLayers layer, boolean useFireDF);
	void autoPlayLevel(boolean fastForward);
	void updateClairvoyance();
    short scentDistance(short x1, short y1, short x2, short y2);
    short armorAggroAdjustment(item *theArmor);
    short currentAggroValue();
	
	void initRecording();
	void flushBufferToFile();
	void fillBufferFromFile();
	void recordEvent(rogueEvent *event);
	void recallEvent(rogueEvent *event);
	void pausePlayback();
	void displayAnnotation();
	void loadSavedGame();
	void recordKeystroke(uchar keystroke, boolean controlKey, boolean shiftKey);
	void recordKeystrokeSequence(unsigned char *commandSequence);
	void recordMouseClick(short x, short y, boolean controlKey, boolean shiftKey);
	void OOSCheck(unsigned long x, short numberOfBytes);
	void RNGCheck();
	void executePlaybackInput(rogueEvent *recordingInput);
	void getAvailableFilePath(char *filePath, const char *defaultPath, const char *suffix);
    boolean characterForbiddenInFilename(const char theChar);
	void saveGame();
	void saveRecording();
	void parseFile();
	void RNGLog(char *message);
	
	void checkForDungeonErrors();
	
	boolean dialogChooseFile(char *path, const char *suffix, const char *prompt);
	void dialogAlert(char *message);
	void mainBrogueJunction();
	
	void initializeButton(brogueButton *button);
	void drawButtonsInState(buttonState *state);
	void initializeButtonState(buttonState *state,
							   brogueButton *buttons,
							   short buttonCount,
							   short winX,
							   short winY,
							   short winWidth,
							   short winHeight);
	short processButtonInput(buttonState *state, boolean *canceled, rogueEvent *event);
	void drawButton(brogueButton *button, enum buttonDrawStates highlight, cellDisplayBuffer dbuf[COLS][ROWS]);
	short buttonInputLoop(brogueButton *buttons,
						  short buttonCount,
						  short winX,
						  short winY,
						  short winWidth,
						  short winHeight,
						  rogueEvent *returnEvent);
	
	void dijkstraScan(short **distanceMap, short **costMap, boolean useDiagonals);
	void pdsClear(pdsMap *map, short maxDistance, boolean eightWays);
	void pdsSetDistance(pdsMap *map, short x, short y, short distance);
	void pdsBatchOutput(pdsMap *map, short **distanceMap);


