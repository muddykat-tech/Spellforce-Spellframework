#pragma once
#include <stdint.h>
#include <stdbool.h>

/* |-========== General Structures ==========-| */

typedef struct __attribute__((packed))
{
	uint16_t X;
	uint16_t Y;
} SF_Coord;

typedef struct __attribute__((packed))
{
	uint16_t spell_id;
	uint16_t job_id;
} SF_SpellEffectInfo;

typedef struct __attribute__((packed))
{
	uint8_t entity_type; //1 for figure, 2 for building, 3 for object
	uint16_t entity_index;
	SF_Coord position;
} SF_CGdTargetData;

typedef struct __attribute__((packed))
{
    void *raw_data;
    uint32_t str_length;
    uint32_t unknown_length_var;
    char *data;
} SF_String;

typedef struct __attribute__((packed))
{
    uint32_t partA;
    uint32_t partB;
} SF_Rectangle;

/* |-========== Figure Start ==========-| */

typedef struct __attribute__((packed)){
	uint32_t unknown1;
	uint32_t unknown2;
	uint32_t unknown3;
	uint16_t unknown4;
} CGdFigureWeaponStats;

typedef struct __attribute__((packed)){
	uint16_t figure_index;
	uint16_t agro_value;
	uint16_t hate_value;
} CGdFigureHateEntry;

typedef struct __attribute__((packed))
{
	uint16_t base_val;
	uint16_t bonus_val;
	uint8_t bonus_multiplier;
	uint8_t statistic_type; // NOT CONFIRMED
} FigureStatistic;

typedef struct __attribute__((packed))
{
	uint16_t base_val;
	uint16_t bonus_val;
	uint8_t bonus_multiplier;
	uint8_t statistic_type; // NOT CONFIRMED?
	uint16_t missing_val; // No idea what this is
} FigureStatisticExt;

typedef struct __attribute__((packed))
{
	SF_Coord position;
	SF_Coord destination;
	uint16_t to_do_count;
	uint16_t to_do_count_remainder;
	uint16_t anim_length;
	uint16_t anim_length_remainder;
	uint8_t activity;
	uint8_t unknown1;
	uint16_t building;
	uint32_t flags; //Actual Type CdFigureFlags
	uint8_t race;
	uint8_t level;
	uint16_t owner;
	uint8_t min_attack_range;
	uint8_t max_attack_range;
	uint16_t master_figure;
	uint16_t group_leader;
	uint32_t npc_id;
	uint16_t unit_data_id;
	FigureStatistic armor;
	FigureStatistic agility;
	FigureStatistic charisma;
	FigureStatistic dexterity;
	FigureStatisticExt health;
	FigureStatistic intelligence;
	FigureStatisticExt mana_stuff;
	FigureStatisticExt stamina;
	FigureStatistic strength;
	FigureStatistic wisdom;
	FigureStatistic resistance_fire;
	FigureStatistic resistance_ice;
	FigureStatistic resistance_mental;
	FigureStatistic resistance_black;
	FigureStatistic walk_speed;
	FigureStatistic flight_speed;
	FigureStatistic cast_speed;
	uint8_t equipment[20]; // No idea how this works, may need a class (undefined2[16] in ghidra, but is 20 bytes FigureStatistic is 6 bytes)
	uint16_t head; //Not sure what this does
	uint32_t unknown2[3]; // three 4 byte data points in a row, no name known for these.
	uint8_t unknown3[167]; // Many 1 byte sections in a row 
	uint32_t unknown4[7]; // Many 4 byte sections in a row
	uint16_t unknown5;
	uint8_t	unknown6[2];
	CGdFigureWeaponStats weapon_stats;
	uint8_t unknown7[12];
	uint8_t good; // I assume perhaps alignment?
	uint8_t direction;
	uint8_t unknown8;
	uint8_t path_dir;
	uint8_t path_wish_dir;
	uint32_t path_bits;
	uint8_t unknown9;
	uint32_t unknown10[3];
	uint16_t unknown11;
	CGdFigureHateEntry hate_entry;
	uint8_t debug_flags;
	uint8_t unknown12;
	uint16_t formation;
	uint8_t faction;
	uint8_t unknown13;
	uint32_t clan_relations;
	uint8_t unknown14[170]; // Skipping some variable sections, contains differences Also Check what AutoClass24 is, it's used multiple times in this section
	uint8_t dwarf_rank;
	uint8_t set_type;
} GdFigure;

typedef struct __attribute__((packed))
{
	uint16_t max_used;
	GdFigure figures[2000];
} SF_CGdFigure;

/* |-========== AutoClass Start ==========-| */
// These classes are currently only partially understood, and are not fully annotated

typedef struct __attribute__((packed))
{
	uint32_t *ac69_ptr1;
	uint32_t *ac69_ptr2;
	uint32_t *ac69_ptr3;
	uint8_t unkn1;
	uint8_t unkn2;
	uint16_t figure_index;
} AutoClass69;

// This class is required for the RANDOM function, AutoClass14 also seems to hold some relevance to the game world as well
// This class is initialized in the GameInit function, so it is likely very important.
typedef struct __attribute__((packed))
{
	uint16_t unknown_field_0;
	uint16_t unknown_field_2;
	uint16_t unknown_field_4;
	uint8_t unknown1[6];
	uint32_t current_step;
	uint8_t unknown_field_10;
	uint8_t unknown2[3];
	uint32_t unknown_field_14;
	uint8_t unknown_field_18;
	uint8_t unknown_field_19;
} AutoClass14;

/* |-========== Spell Start ==========-| */
typedef struct __attribute__((packed))
{
	uint16_t to_do_count;
	uint16_t spell_id;
	uint16_t spell_line; //aka spell type in shovel's editor
	uint16_t spell_job; //aka spell line in older code
	SF_CGdTargetData source;
	SF_CGdTargetData target;
	uint8_t xdata_key; // IDK (Seems to be used in ref for things, seen usage in getting target data and others)
	uint8_t undefinded1;
	uint8_t DLLNode;
	uint8_t underfined2; 
	uint8_t flags;
} SF_GdSpell;


typedef struct __attribute__((packed))
{
	void *SF_CGdAiMain;
	AutoClass14 *OpaqueClass; // For Random
	void *SF_CGdBuilding;
	void *SF_CGdBuildingToolbox;
	void *SF_CGdDoubleLinkedList;
	void *SF_CGdEffect;
	void *unkn1;
	SF_CGdFigure *SF_CGdFigure;
	void *SF_CGdFigureJobs;
	void *SF_CGdFigureToolBox;
	void *SF_CGdFormation;
	void *unkn2; //Seems to be used as first param for GetChanceToResistSpell 
	void *SF_CGdObject;
	void *SF_CGdObjectToolBox;
	void *SF_CGdPlayer;
	void *SF_CGdResource;
	void *unkn3;
	void *unkn4;
	void *SF_CGdWorld;
	void *SF_CGdWorldToolBox;
	void *SF_CGdXDataList;
	uint16_t max_used;
	SF_GdSpell active_spell_list[800];
	void *unkn5;
	uint32_t unkn6;
} SF_CGdSpell;


/* |-========== Internal Functions ==========-| */
// These functions are used in SFSF internally, and may be moved.

typedef void (__thiscall *ConsolePrint_ptr)(uint32_t, SF_String*);
typedef uint16_t (__thiscall *get_spell_spell_line_ptr) (void *, uint16_t);
typedef uint32_t (__thiscall *figure_toolbox_get_unkn_ptr)(void *, uint16_t);
typedef void (__thiscall *figure_toolbox_add_spell_ptr)(void *, uint16_t, uint16_t);
typedef bool (__thiscall *figure_toolbox_is_targetable_ptr)(void *CGdFigureToolbox, uint16_t figure_index);
typedef uint32_t (__thiscall *FUN_0069eaf0_ptr)(void* ac69, void* ac69_2, void* ac69_3, void* ac69_4);
typedef void (*fidfree_ptr)(uint32_t* memory_ptr);

/* |-========== Macros ==========-| */
// Here comes a better method for setting up our exposed functions, to define functions also check sf_hooks.h
#define DECLARE_FUNCTION(type, name, ...) \
    typedef type (__thiscall *name##_ptr)(__VA_ARGS__);

#define DECLARE_FUNCTION_GROUP(group, ...) \
    typedef struct { \
        __VA_ARGS__ \
    } group##Functions;