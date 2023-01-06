#pragma once

#include "fpatching_defines.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a FunctionPatcherStatus as a string
 * @param status
 * @return String representation of a given status
**/
const char *FunctionPatcher_GetStatusStr(FunctionPatcherStatus status);

/**
 * This function has to be called before any other function of this lib (except FunctionPatcher_GetVersion) can be used.
 *
 * @return  FUNCTION_PATCHER_RESULT_SUCCESS:                 The library has been initialized successfully. Other functions can now be used.<br>
 *          FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND:        The module could not be found. Make sure the module is loaded.<br>
 *          FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT:   The module is missing an expected export.<br>
 *          FUNCTION_PATCHER_RESULT_UNSUPPORTED_API_VERSION: The version of the loaded module is not compatible with this version of the lib.
**/
FunctionPatcherStatus FunctionPatcher_InitLibrary();

/**
 * Deinitializes the RPXLoader lib
 * @return FUNCTION_PATCHER_RESULT_SUCCESS or FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR
 */
FunctionPatcherStatus FunctionPatcher_DeInitLibrary();

/**
 * Retrieves the API Version of the loaded FunctionPatcherModule.<br>
 * <br>
 * Requires FunctionPatcher API version 2 or higher
 * @param outVersion pointer to the variable where the version will be stored.
 *
 * @return FUNCTION_PATCHER_RESULT_SUCCESS:               The API version has been store in the version ptr.<br>
 *         FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND:      The module could not be found. Make sure the module is loaded.<br>
 *         FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT: The module is missing an expected export.<br>
 *         FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT:      Invalid version pointer.<br>
 *         FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR:         Retrieving the module version failed.
**/
FunctionPatcherStatus FunctionPatcher_GetVersion(FunctionPatcherAPIVersion *outVersion);

/**
 * Registers a patch of a function. You can use the `REPLACE_FUNCTION_XXX` macro for filling the function_replacement_data_t struct. <br>
 * When adding this function it'll be attempted to apply this patch, see `outHasBeenPatched` this initial patch was successful. <br>
 * If the target could not be patched when adding it (e.g. because the target library is not loaded), <br>
 * the function will be patched as soon as the RPL of the library is loaded (via OSDynLoad_XXX or due to a application switch) and unloaded <br>
 * if a the RPL unloads <br>
 * The function patches will survive an application switches, the only way to deregister it is to call FunctionPatcher_RemoveFunctionPatch <br>
 * with the PatchedFunctionHandle. <br>
 * <br>
 * It is possible to patch the same function multiple times, the function patches will stack. <br>
 * The most recent patch to a function will be called first. real_xxx inside this patch refers then to the "previous patch"<br>
 * <br>
 * Requires FunctionPatcher API version 2 or higher
 * @param function_data Provides information about which function should be patched in which manner. See `REPLACE_FUNCTION_XXX` macros.
 * @param outHandle (optional) stores a handle of the function patch on success
 * @param outHasBeenPatched (optional) stores is the initial function **patch** was successful
 * @return FUNCTION_PATCHER_RESULT_SUCCESS:                     The function function patch has been **added**. This does **NOT** mean the function <br>
 *                                                              has already been patched. See `outHasBeenPatched` if the function actually is already patched. <br>
 *         FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED:           Library was not initialized. Call FunctionPatcher_InitLibrary() before using this function.<br>
 *         FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND:         Command not supported by the currently loaded FunctionPatcherModule version.<br>
 *         FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT:            The given function_data was NULL.<br>
 *         FUNCTION_PATCHER_RESULT_UNSUPPORTED_STRUCT_VERSION:  Given function_data was in a unsupported version.<br>
**/
FunctionPatcherStatus FunctionPatcher_AddFunctionPatch(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle, bool *outHasBeenPatched);

/**
 * Remove a given function patch by handle. If the function is currently patched, the function will be restored. <br>
 * It is possible to to restore a function that has been patched multiple times. Other function patches of the function will <br>
 * stay intact. To achieve this the target function might the restored/patched multiple times and should not be called <br>
 * until this function returns. <br>
 * <br>
 * Requires FunctionPatcher API version 2 or higher
 * @param handle The function patch that should be removed.
 * @return FUNCTION_PATCHER_RESULT_SUCCESS:                     The function function patch has been **removed**.<br>
 *         FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED:           Library was not initialized. Call FunctionPatcher_InitLibrary() before using this function.<br>
 *         FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND:         Command not supported by the currently loaded FunctionPatcherModule version.<br>
 *         FUNCTION_PATCHER_RESULT_PATCH_NOT_FOUND:             The given handle does not refer to a valid patch.<br>
**/
FunctionPatcherStatus FunctionPatcher_RemoveFunctionPatch(PatchedFunctionHandle handle);

/**
 * Check if a function patch is actually applied. Functions only will be applied if the target <br>
 * library is loaded.
 * <br>
 * Requires FunctionPatcher API version 2 or higher
 * @param handle Handle of the function patch to check.
 * @param outIsFunctionPatched Stores the result on success.
 * @return FUNCTION_PATCHER_RESULT_SUCCESS:                     The result has been stored in `outIsFunctionPatched`.<br>
 *         FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED:           Library was not initialized. Call FunctionPatcher_InitLibrary() before using this function.<br>
 *         FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND:         Command not supported by the currently loaded FunctionPatcherModule version.<br>
 *         FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT:            The given handle was NULL.<br>
 *         FUNCTION_PATCHER_RESULT_PATCH_NOT_FOUND:             The given handle does not refer to a valid patch.<br>
**/
FunctionPatcherStatus FunctionPatcher_IsFunctionPatched(PatchedFunctionHandle handle, bool *outIsFunctionPatched);

WUT_DEPRECATED("Please use FunctionPatcher_AddFunctionPatch instead")
bool FunctionPatcherPatchFunction(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle);

WUT_DEPRECATED("Please use FunctionPatcher_RemoveFunctionPatch instead")
bool FunctionPatcherRestoreFunction(PatchedFunctionHandle handle);

#ifdef __cplusplus
}
#endif
