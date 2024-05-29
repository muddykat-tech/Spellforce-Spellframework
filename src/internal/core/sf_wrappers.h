#ifndef UTILITY_H
#define UTILITY_H

#include "../../api/sf_general_structures.h"
typedef SF_String * (__thiscall *SF_String_ctor_ptr)(SF_String *, const char *);
typedef void (__thiscall *SF_String_dtor_ptr)(SF_String *);

extern "C" SF_String_ctor_ptr SF_String_ctor;
extern "C" SF_String_dtor_ptr SF_String_dtor;

extern "C" void logWarning(const char *message);
extern "C" void logInfo(const char *message);
extern "C" void logError(const char *message);
#endif