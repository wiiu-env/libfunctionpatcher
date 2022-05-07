#pragma once

#include "fpatching_defines.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern bool FunctionPatcherPatchFunction(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle);

extern bool FunctionPatcherRestoreFunction(PatchedFunctionHandle outHandle);

#ifdef __cplusplus
}
#endif
