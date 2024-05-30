// SpellforceSpellFramework.h
// DEPRECATED
#pragma once

#include "sf_general_structures.h"
#include "sf_figure_functions.h"
#include "sf_iterator_functions.h"
#include "sf_spell_functions.h"
#include "sf_toolbox_functions.h"

#include <stdint.h>

typedef void (*initializeModule_ptr)(void*);
typedef void (*log_warning_ptr)(const char*);
typedef void (*logInfoFunc)(const char*);
typedef void (__thiscall *handler_ptr) (SF_CGdSpell *, uint16_t);
typedef void (*register_spelltype_handler_ptr)(uint16_t spell_index, handler_ptr handler);
typedef void (*register_effect_handler_ptr)(uint16_t spell_job, handler_ptr handler);

// Declare the structure
struct SpellforceSpellFramework {
    initializeModule_ptr initializeModule;
    register_spelltype_handler_ptr registerSpellTypeHandler;
    register_effect_handler_ptr registerEffectHandler;

    SpellFunctions *spellAPI;
    ToolboxFunctions *toolboxAPI;
    FigureFunctions *figureAPI; 
    IteratorFunctions *iteratorAPI;

    // Perhaps create a struct for a proper logger? 
    log_warning_ptr logWarning;
    logInfoFunc logInfo;
};

typedef struct SpellforceSpellFramework FrameworkAPI;
typedef struct SpellforceSpellFramework *pFrameworkAPI;