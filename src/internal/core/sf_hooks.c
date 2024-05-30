#include "sf_wrappers.h"
#include "sf_modloader.h"
#include "sf_hooks.h"
#include "../handlers/sf_spelltype_handlers.h"
#include "../registry/sf_spelltype_registry.h"
#include "../registry/sf_spelleffect_registry.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

console_print_ptr console_print;
get_spell_spell_line_ptr get_spell_spell_line;
figure_toolbox_get_unkn_ptr figure_toolbox_get_unkn;
figure_toolbox_add_spell_ptr figure_toolbox_add_spell;
figure_toolbox_is_targetable_ptr figure_toolbox_is_targetable;

FUN_0069eaf0_ptr FUN_0069eaf0;
fidfree_ptr fidFree;

uint32_t CMnuScrConsole_ptr = 0;
SF_String_ctor_ptr SF_String_ctor;
SF_String_dtor_ptr SF_String_dtor;

SpellFunctions api_spell_functions;
ToolboxFunctions api_toolbox_functions;
FigureFunctions api_figure_functions;
IteratorFunctions api_iterator_functions;

int __thiscall CheckCanApply_hook_beta(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of refresh handlers?
    // And a default handler that does nothing
    // handler takes (SF_CGdSpell *_this, uint16_t spell_index) as params
    return 0;
}

void __thiscall EndSpell_hook_beta(SF_CGdSpell *_this, uint16_t spell_index)
{
    // We need a map of end spell handlers?
    // And a default handler that does nothing
    // handler takes (SF_CGdSpell *_this, uint16_t spell_index) as params
}

void __thiscall effect_trigger_hook(SF_CGdSpell *_this)
{
    uint16_t spell_index;
    for (spell_index = 1; spell_index <= _this->max_used; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id != 0)
        {
            short to_do_count = _this->active_spell_list[spell_index].to_do_count;
            if (to_do_count == 0)
            {
                to_do_count = 0;
            }
            else
            {
                to_do_count = to_do_count + -1;
            }
            _this->active_spell_list[spell_index].to_do_count = to_do_count;
            if (to_do_count == 0)
            {
                uint16_t spell_job = _this->active_spell_list[spell_index].spell_job;
                handler_ptr func = get_spell_effect(spell_job);
                if (func != NULL)
                {
                    func(_this, spell_index);
                }
            }
        }
    }
}

void console_log(const char *message)
{
    if (!CMnuScrConsole_ptr)
        return;
    SF_String sf_string;
    SF_String_ctor(&sf_string, message);
    console_print(CMnuScrConsole_ptr, &sf_string);
    SF_String_dtor(&sf_string);
}

uint16_t __thiscall add_spell_hook_beta(SF_CGdSpell *_this, uint16_t spell_id, uint16_t param2, SF_CGdTargetData *source, SF_CGdTargetData *target, uint16_t param5)
{
    uint16_t spell_index;
    uint16_t spell_line;
    if (target->entity_type == 1)
    {
        if (!figure_toolbox_get_unkn(_this->SF_CGdFigureToolBox, target->entity_index))
        {
            return 0;
        }
    }

    // find 1st free slot for spell
    for (spell_index = 1; spell_index < 799; ++spell_index)
    {
        if (_this->active_spell_list[spell_index].spell_id == 0)
        {
            break;
        }
    }
    if (spell_index == 799)
    {
        log_warning("Maximum spell limit (800) reached. Additional spells may not be registered.");
        return 0;
    }
    // max_used spells update
    if (_this->max_used < spell_index)
    {
        _this->max_used = spell_index;
    }

    // somewhere here is smth goes south
    _this->active_spell_list[spell_index].spell_id = spell_id;
    spell_line = get_spell_spell_line(_this->SF_CGdResource, spell_id);
    _this->active_spell_list[spell_index].spell_line = spell_line;
    _this->active_spell_list[spell_index].source.entity_type = source->entity_type;
    _this->active_spell_list[spell_index].source.entity_index = source->entity_index;
    _this->active_spell_list[spell_index].source.position.X = (source->position).X;
    _this->active_spell_list[spell_index].source.position.Y = (source->position).Y;
    _this->active_spell_list[spell_index].target.entity_type = target->entity_type;
    _this->active_spell_list[spell_index].target.entity_index = target->entity_index;
    _this->active_spell_list[spell_index].target.position.X = (target->position).X;
    _this->active_spell_list[spell_index].target.position.Y = (target->position).Y;
    _this->active_spell_list[spell_index].to_do_count = param2 - (uint16_t)(_this->OpaqueClass->current_step);

    handler_ptr func = get_spell_handler(spell_line);
    func(_this, spell_index);

    if (target->entity_type == 1)
    {
        if (spell_line != 0x5A)
        {
            figure_toolbox_add_spell(_this->SF_CGdFigureToolBox, target->entity_index, spell_index);
        }
    }
    return spell_index;
}


void __thiscall add_bonus_mult_to_statistic(SF_CGdFigure *figure, StatisticDataKey key, uint16_t target, int8_t value)
{
    bool invalid = FALSE;
    FigureStatistic statistic;
    switch (key)
    {
    case ARMOR:
        statistic = figure->figures[target].armor;
        break;
    case AGILITY:
        statistic = figure->figures[target].agility;
        break;
    case CHARISMA:
        statistic = figure->figures[target].charisma;
        break;
    case DEXTERITY:
        statistic = figure->figures[target].dexterity;
        break;
    case INTELLIGENCE:
        statistic = figure->figures[target].intelligence;
        break;
    case STRENGTH:
        statistic = figure->figures[target].strength;
        break;
    case WISDOM:
        statistic = figure->figures[target].wisdom;
        break;
    case RESISTANCE_FIRE:
        statistic = figure->figures[target].resistance_fire;
        break;
    case RESISTANCE_ICE:
        statistic = figure->figures[target].resistance_ice;
        break;
    case RESISTANCE_MENTAL:
        statistic = figure->figures[target].resistance_mental;
        break;
    case RESISTANCE_BLACK:
        statistic = figure->figures[target].resistance_black;
        break;
    case WALK_SPEED:
        statistic = figure->figures[target].walk_speed;
        break;
    case FLIGHT_SPEED:
        statistic = figure->figures[target].flight_speed;
        break;
    case CAST_SPEED:
        statistic = figure->figures[target].cast_speed;
        break;
    default:
        // Handle default case if needed
        invalid = TRUE;
        break;
    }

    if (invalid)
    {
        log_warning("INVALID STATISTIC KEY");
        return;
    }

    api_figure_functions.add_bonus_mult(statistic, value);
}

/* 
	These functions, and future types, will eventually be moved into "sf_api" or some such
*/ 
void initialize_console_hook()
{
    uint32_t CAppMain_ptr = ASI::AddrOf(0x9229A8);
    uint32_t CAppMenu_ptr = *(uint32_t *)(CAppMain_ptr + 0x4);
    uint32_t CMnuScrConsole_ptr = *(uint32_t *)(CAppMenu_ptr + 0x80);
    console_print = (console_print_ptr)ASI::AddrOf(0x534e70);
    SF_String_ctor = (SF_String_ctor_ptr)ASI::AddrOf(0x3837e0);
    SF_String_dtor = (SF_String_dtor_ptr)ASI::AddrOf(0x3839c0);
}

void __thiscall setup_figure_iterator(CGdFigureIterator *iterator, SF_CGdSpell *spell)
{
    api_iterator_functions.figureIteratorInit(iterator, 0x0, 0x0, 0x3ff, 0x3ff);
    api_iterator_functions.figureIteratorSetPointers(iterator, spell->SF_CGdFigure, spell->unkn3, spell->SF_CGdWorld);
}

// Some funky stuff to clean up Iterator memory, not 100% sure if correct
void __thiscall disposeFigureIterator(CGdFigureIterator iterator)
{
    uint32_t unused;
    FUN_0069eaf0(&iterator.data.offset_0x30, &unused, ((AutoClass69 *)iterator.data.offset_0x30.ac69_ptr1)->ac69_ptr1, iterator.data.offset_0x30.ac69_ptr1);
    fidFree(iterator.data.offset_0x30.ac69_ptr1);
}

void initialize_data_hooks()
{

    // Required for internal use
    get_spell_spell_line = (get_spell_spell_line_ptr)(ASI::AddrOf(0x26E100));
    figure_toolbox_get_unkn = (figure_toolbox_get_unkn_ptr)(ASI::AddrOf(0x2FE704));
    figure_toolbox_add_spell = (figure_toolbox_add_spell_ptr)(ASI::AddrOf(0x2F673A));

    // used in Iterator for AOE Spells Dispose
    FUN_0069eaf0 = (FUN_0069eaf0_ptr)(ASI::AddrOf(0x29EAF0));
    fidFree = (fidfree_ptr)(ASI::AddrOf(0x6B6E25));

    // More defined for external use in api
    DEFINE_FUNCTION(figure, is_alive, 0x1BE4D0);
    DEFINE_FUNCTION(figure, set_walk_speed, 0x2B7190);
    DEFINE_FUNCTION(figure, add_action, 0x2AE0B0);
    DEFINE_FUNCTION(figure, add_bonus_mult, 0x35A3E0);
    DEFINE_FUNCTION(figure, decreaseHealth, 0x2b5b40);
    DEFINE_FUNCTION(figure, getCurrentHealth, 0x279350);
    DEFINE_FUNCTION(figure, getCurrentManaMax, 0x2b2a20);
    DEFINE_FUNCTION(figure, rescaleMana, 0x2b5d50);

    // Define the function pointers for SpellFunctions group
    DEFINE_FUNCTION(spell, setXData, 0x329C40);
    DEFINE_FUNCTION(spell, setEffectDone, 0x32A730);
    DEFINE_FUNCTION(spell, addToXData, 0x32AAC0);
    DEFINE_FUNCTION(spell, getChanceToResistSpell, 0x317BA0);
    DEFINE_FUNCTION(spell, getRandom, 0x2AD200);
    DEFINE_FUNCTION(spell, addVisualEffect, 0x329B30);
    DEFINE_FUNCTION(spell, figureAggro, 0x329c90);
    DEFINE_FUNCTION(spell, getResourceSpellData, 0x26dc40);
    DEFINE_FUNCTION(spell, getXData, 0x32aa60);
    DEFINE_FUNCTION(spell, getTargetsRectangle, 0x329D80);
    DEFINE_FUNCTION(spell, removeDLLNode, 0x34B030);

    DEFINE_FUNCTION(toolbox, dealDamage, 0x2f4a57);
    DEFINE_FUNCTION(toolbox, isTargetable, 0x2fe704);
    DEFINE_FUNCTION(toolbox, figuresCheckHostile, 0x2fe7b9);
    DEFINE_FUNCTION(toolbox, hasSpellOnHit, 0x2fe4ea);
    DEFINE_FUNCTION(toolbox, rescaleLevelStats, 0x2fff48);

    DEFINE_FUNCTION(toolbox, buildingDealDamage, 0x2d6d80);

    DEFINE_FUNCTION(iterator, figureIteratorInit, 0x3183f0);
    DEFINE_FUNCTION(iterator, figureIteratorSetPointers, 0x31a680);
    DEFINE_FUNCTION(iterator, iteratorSetArea, 0x3195d0);
    DEFINE_FUNCTION(iterator, getNextFigure, 0x318f50);

	DEFINE_FUNCTION(iterator, buildingIteratorInit, 0x318290);
	DEFINE_FUNCTION(iterator, buildingIteratorSetPointers,0x31A640);

    // Method to include functions WE define in the Internal code.
    INCLUDE_FUNCTION(spell, initializeSpellData, &initializeSpellData);
    INCLUDE_FUNCTION(figure, add_bonus_mult_to_statistic, &add_bonus_mult_to_statistic);

	INCLUDE_FUNCTION(iterator, setup_figure_iterator, &setup_figure_iterator);
	INCLUDE_FUNCTION(iterator, disposeFigureIterator, &disposeFigureIterator);
}

void initialize_spelltype_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x328d60), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x328d60)) = 0xE9; // jmp instruction
    *(int *)(ASI::AddrOf(0x328d61)) = (int)(&add_spell_hook_beta) - ASI::AddrOf(0x328d65);
    ASI::EndRewrite(add_spell_mreg);
}

void initialize_spell_trigger_hook()
{
    ASI::MemoryRegion add_spell_mreg(ASI::AddrOf(0x278773), 5);
    ASI::BeginRewrite(add_spell_mreg);
    *(unsigned char *)(ASI::AddrOf(0x278773)) = 0xE8; // CALL instruction
    *(int *)(ASI::AddrOf(0x278774)) = (int)(&effect_trigger_hook) - ASI::AddrOf(0x278778);
    ASI::EndRewrite(add_spell_mreg);
}

// Exposed in sf_hooks.h
void initialize_beta_hooks()
{
    logInfo("Hooking Game Console");
    initialize_console_hook();

    logInfo("Hooking Spell Types");
    initialize_spelltype_hook();

    logInfo("Hooking Spell Triggers");
    initialize_spell_trigger_hook();
}