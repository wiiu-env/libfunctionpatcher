#include "logger.h"
#include <coreinit/debug.h>
#include <coreinit/dynload.h>
#include <function_patcher/function_patching.h>

static OSDynLoad_Module sModuleHandle = nullptr;

// Introduced in API version 2.
static FunctionPatcherStatus (*sFPGetVersion)(FunctionPatcherAPIVersion *)                                                                                 = nullptr;
static FunctionPatcherStatus (*sFPAddFunctionPatch)(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle, bool *outHasBeenPatched) = nullptr;
static FunctionPatcherStatus (*sFPRemoveFunctionPatch)(PatchedFunctionHandle handle)                                                                       = nullptr;
static FunctionPatcherStatus (*sFPIsFunctionPatched)(PatchedFunctionHandle handle, bool *outIsPatched)                                                     = nullptr;

// Deprecated functions
static bool (*sFunctionPatcherPatchFunction)(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle) = nullptr;
static bool (*sFunctionPatcherRestoreFunction)(PatchedFunctionHandle handle)                                               = nullptr;

static FunctionPatcherAPIVersion sFunctionPatcherVersion = FUNCTION_PATCHER_MODULE_API_VERSION_ERROR;

const char *FunctionPatcher_GetStatusStr(FunctionPatcherStatus status) {
    switch (status) {
        case FUNCTION_PATCHER_RESULT_SUCCESS:
            return "FUNCTION_PATCHER_RESULT_SUCCESS";
        case FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND:
            return "FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND";
        case FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT:
            return "FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT";
        case FUNCTION_PATCHER_RESULT_UNSUPPORTED_VERSION:
            return "FUNCTION_PATCHER_RESULT_UNSUPPORTED_VERSION";
        case FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT:
            return "FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT";
        case FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED:
            return "FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED";
        case FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND:
            return "FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND";
        case FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR:
            return "FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR";
        case FUNCTION_PATCHER_RESULT_PATCH_NOT_FOUND:
            return "FUNCTION_PATCHER_RESULT_PATCH_NOT_FOUND";
        case FUNCTION_PATCHER_RESULT_UNSUPPORTED_STRUCT_VERSION:
            return "FUNCTION_PATCHER_RESULT_UNSUPPORTED_STRUCT_VERSION";
    }
    return "FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR";
}

FunctionPatcherStatus FunctionPatcher_InitLibrary() {
    if (OSDynLoad_Acquire("homebrew_functionpatcher", &sModuleHandle) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("OSDynLoad_Acquire failed.");
        return FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FPGetVersion", (void **) &sFPGetVersion) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("FindExport FPGetVersion failed.");
        return FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT;
    }

    auto res = FunctionPatcher_GetVersion(&sFunctionPatcherVersion);
    if (res != FUNCTION_PATCHER_RESULT_SUCCESS) {
        sFunctionPatcherVersion = FUNCTION_PATCHER_MODULE_API_VERSION_ERROR;
        return FUNCTION_PATCHER_RESULT_UNSUPPORTED_VERSION;
    }

    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FPAddFunctionPatch", (void **) &sFPAddFunctionPatch) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("FindExport FPAddFunctionPatch failed.");
        sFPAddFunctionPatch = nullptr;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FPRemoveFunctionPatch", (void **) &sFPRemoveFunctionPatch) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("FindExport FPRemoveFunctionPatch failed.");
        sFPRemoveFunctionPatch = nullptr;
    }
    if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FPIsFunctionPatched", (void **) &sFPIsFunctionPatched) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("FindExport FPIsFunctionPatched failed.");
        sFPIsFunctionPatched = nullptr;
    }

    return FUNCTION_PATCHER_RESULT_SUCCESS;
}

FunctionPatcherStatus FunctionPatcher_DeInitLibrary() {
    sFPAddFunctionPatch     = nullptr;
    sFPRemoveFunctionPatch  = nullptr;
    sFPIsFunctionPatched    = nullptr;
    sFunctionPatcherVersion = FUNCTION_PATCHER_MODULE_API_VERSION_ERROR;
    OSDynLoad_Release(sModuleHandle);
    sModuleHandle = nullptr;
    return FUNCTION_PATCHER_RESULT_SUCCESS;
}

bool FunctionPatcherPatchFunctionDecl(function_replacement_data_t *, PatchedFunctionHandle *);
bool FunctionPatcherPatchFunction(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle) {
    if (sFunctionPatcherPatchFunction == nullptr) {
        if (OSDynLoad_Acquire("homebrew_functionpatcher", &sModuleHandle) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("OSDynLoad_Acquire failed.");
            return false;
        }

        if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FunctionPatcherPatchFunction", (void **) &sFunctionPatcherPatchFunction) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("FindExport FunctionPatcherPatchFunction failed.");
            return false;
        }
    }
    return reinterpret_cast<decltype(&FunctionPatcherPatchFunctionDecl)>(sFunctionPatcherPatchFunction)(function_data, outHandle);
}

bool FunctionPatcherRestoreFunctionDecl(PatchedFunctionHandle);
bool FunctionPatcherRestoreFunction(PatchedFunctionHandle handle) {
    if (sFunctionPatcherRestoreFunction == nullptr) {
        if (OSDynLoad_Acquire("homebrew_functionpatcher", &sModuleHandle) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("OSDynLoad_Acquire failed.");
            return false;
        }

        if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FunctionPatcherRestoreFunction", (void **) &sFunctionPatcherRestoreFunction) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("FindExport FunctionPatcherRestoreFunction failed.");
            return false;
        }
    }
    return reinterpret_cast<decltype(&FunctionPatcherRestoreFunctionDecl)>(sFunctionPatcherRestoreFunction)(handle);
}

FunctionPatcherStatus FunctionPatcher_GetVersion(FunctionPatcherAPIVersion *outVersion) {
    if (sFPGetVersion == nullptr) {
        if (OSDynLoad_Acquire("homebrew_functionpatcher", &sModuleHandle) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("OSDynLoad_Acquire failed.");
            return FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND;
        }

        if (OSDynLoad_FindExport(sModuleHandle, OS_DYNLOAD_EXPORT_FUNC, "FPGetVersion", (void **) &sFPGetVersion) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("FindExport FPGetVersion failed.");
            return FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT;
        }
    }

    return reinterpret_cast<decltype(&FunctionPatcher_GetVersion)>(sFPGetVersion)(outVersion);
}

FunctionPatcherStatus FunctionPatcher_AddFunctionPatch(function_replacement_data_t *function_data, PatchedFunctionHandle *outHandle, bool *outHasBeenPatched) {
    if (sFunctionPatcherVersion == FUNCTION_PATCHER_MODULE_API_VERSION_ERROR) {
        return FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED;
    }
    if (sFPAddFunctionPatch == nullptr || sFunctionPatcherVersion < 2) {
        return FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND;
    }

    if (function_data == nullptr) {
        return FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT;
    }

    return reinterpret_cast<decltype(&FunctionPatcher_AddFunctionPatch)>(sFPAddFunctionPatch)(function_data, outHandle, outHasBeenPatched);
}

FunctionPatcherStatus FunctionPatcher_RemoveFunctionPatch(PatchedFunctionHandle handle) {
    if (sFunctionPatcherVersion == FUNCTION_PATCHER_MODULE_API_VERSION_ERROR) {
        return FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED;
    }
    if (sFPRemoveFunctionPatch == nullptr || sFunctionPatcherVersion < 2) {
        return FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND;
    }

    if (handle == 0) {
        return FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT;
    }

    return reinterpret_cast<decltype(&FunctionPatcher_RemoveFunctionPatch)>(sFPRemoveFunctionPatch)(handle);
}

FunctionPatcherStatus FunctionPatcher_IsFunctionPatched(PatchedFunctionHandle handle, bool *outIsFunctionPatched) {
    if (sFunctionPatcherVersion == FUNCTION_PATCHER_MODULE_API_VERSION_ERROR) {
        return FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED;
    }
    if (sFPIsFunctionPatched == nullptr || sFunctionPatcherVersion < 2) {
        return FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND;
    }

    if (handle == 0 || outIsFunctionPatched == nullptr) {
        return FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT;
    }

    return reinterpret_cast<decltype(&FunctionPatcher_IsFunctionPatched)>(sFPIsFunctionPatched)(handle, outIsFunctionPatched);
}
