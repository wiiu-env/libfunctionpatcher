#pragma once

#include "fpatching_defines.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void FunctionPatcherRestoreDynamicFunctions(function_replacement_data_t *replacements, uint32_t size);

extern void FunctionPatcherPatchFunction(function_replacement_data_t *replacements, uint32_t size);

extern void FunctionPatcherRestoreFunctions(function_replacement_data_t *replacements, uint32_t size);

#ifdef __cplusplus
}
#endif
