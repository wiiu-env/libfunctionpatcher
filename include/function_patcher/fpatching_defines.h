#pragma once

#include <stdint.h>
#include <wut.h>

typedef enum FunctionPatcherStatus {
    FUNCTION_PATCHER_RESULT_SUCCESS                    = 0,
    FUNCTION_PATCHER_RESULT_MODULE_NOT_FOUND           = -0x1,
    FUNCTION_PATCHER_RESULT_MODULE_MISSING_EXPORT      = -0x2,
    FUNCTION_PATCHER_RESULT_UNSUPPORTED_VERSION        = -0x3,
    FUNCTION_PATCHER_RESULT_INVALID_ARGUMENT           = -0x10,
    FUNCTION_PATCHER_RESULT_PATCH_NOT_FOUND            = -0x11,
    FUNCTION_PATCHER_RESULT_UNSUPPORTED_STRUCT_VERSION = -0x12,
    FUNCTION_PATCHER_RESULT_LIB_UNINITIALIZED          = -0x20,
    FUNCTION_PATCHER_RESULT_UNSUPPORTED_COMMAND        = -0x21,
    FUNCTION_PATCHER_RESULT_UNKNOWN_ERROR              = -0x1000,
} FunctionPatcherStatus;

typedef uint32_t FunctionPatcherAPIVersion;

#define FUNCTION_PATCHER_MODULE_API_VERSION_ERROR 0xFFFFFFFF

typedef enum function_replacement_library_type_t {
    LIBRARY_AVM,
    LIBRARY_CAMERA,
    LIBRARY_COREINIT,
    LIBRARY_DC,
    LIBRARY_DMAE,
    LIBRARY_DRMAPP,
    LIBRARY_ERREULA,
    LIBRARY_GX2,
    LIBRARY_H264,
    LIBRARY_LZMA920,
    LIBRARY_MIC,
    LIBRARY_NFC,
    LIBRARY_NIO_PROF,
    LIBRARY_NLIBCURL,
    LIBRARY_NLIBNSS,
    LIBRARY_NLIBNSS2,
    LIBRARY_NN_AC,
    LIBRARY_NN_ACP,
    LIBRARY_NN_ACT,
    LIBRARY_NN_AOC,
    LIBRARY_NN_BOSS,
    LIBRARY_NN_CCR,
    LIBRARY_NN_CMPT,
    LIBRARY_NN_DLP,
    LIBRARY_NN_EC,
    LIBRARY_NN_FP,
    LIBRARY_NN_HAI,
    LIBRARY_NN_HPAD,
    LIBRARY_NN_IDBE,
    LIBRARY_NN_NDM,
    LIBRARY_NN_NETS2,
    LIBRARY_NN_NFP,
    LIBRARY_NN_NIM,
    LIBRARY_NN_OLV,
    LIBRARY_NN_PDM,
    LIBRARY_NN_SAVE,
    LIBRARY_NN_SL,
    LIBRARY_NN_SPM,
    LIBRARY_NN_TEMP,
    LIBRARY_NN_UDS,
    LIBRARY_NN_VCTL,
    LIBRARY_NSYSCCR,
    LIBRARY_NSYSHID,
    LIBRARY_NSYSKBD,
    LIBRARY_NSYSNET,
    LIBRARY_NSYSUHS,
    LIBRARY_NSYSUVD,
    LIBRARY_NTAG,
    LIBRARY_PADSCORE,
    LIBRARY_PROC_UI,
    LIBRARY_SND_CORE,
    LIBRARY_SND_USER,
    LIBRARY_SNDCORE2,
    LIBRARY_SNDUSER2,
    LIBRARY_SWKBD,
    LIBRARY_SYSAPP,
    LIBRARY_TCL,
    LIBRARY_TVE,
    LIBRARY_UAC,
    LIBRARY_UAC_RPL,
    LIBRARY_USB_MIC,
    LIBRARY_UVC,
    LIBRARY_UVD,
    LIBRARY_VPAD,
    LIBRARY_VPADBASE,
    LIBRARY_ZLIB125,
    LIBRARY_OTHER,
} function_replacement_library_type_t;

#define MAXIMUM_FUNCTION_NAME_LENGTH       100
#define FUNCTION_PATCHER_METHOD_STORE_SIZE 40

typedef enum FunctionPatcherFunctionType {
    FUNCTION_PATCHER_REPLACE_BY_LIB_OR_ADDRESS         = 0,
    FUNCTION_PATCHER_REPLACE_FOR_EXECUTABLE_BY_ADDRESS = 1,
    FUNCTION_PATCHER_REPLACE_FOR_EXECUTABLE_BY_NAME    = 2,
} FunctionPatcherFunctionType;

typedef enum FunctionPatcherTargetProcess {
    FP_TARGET_PROCESS_ALL              = 0xFF,
    FP_TARGET_PROCESS_ROOT_RPX         = 1,
    FP_TARGET_PROCESS_WII_U_MENU       = 2,
    FP_TARGET_PROCESS_TVII             = 3,
    FP_TARGET_PROCESS_E_MANUAL         = 4,
    FP_TARGET_PROCESS_HOME_MENU        = 5,
    FP_TARGET_PROCESS_ERROR_DISPLAY    = 6,
    FP_TARGET_PROCESS_MINI_MIIVERSE    = 7,
    FP_TARGET_PROCESS_BROWSER          = 8,
    FP_TARGET_PROCESS_MIIVERSE         = 9,
    FP_TARGET_PROCESS_ESHOP            = 10,
    FP_TARGET_PROCESS_PFID_11          = 11,
    FP_TARGET_PROCESS_DOWNLOAD_MANAGER = 12,
    FP_TARGET_PROCESS_PFID_13          = 13,
    FP_TARGET_PROCESS_PFID_14          = 14,
    FP_TARGET_PROCESS_GAME             = 15,
    FP_TARGET_PROCESS_GAME_AND_MENU    = 16,
} FunctionPatcherTargetProcess;

typedef uint32_t PatchedFunctionHandle;
#define FUNCTION_REPLACEMENT_DATA_STRUCT_VERSION 0x00000003

typedef struct function_replacement_data_t {
    uint32_t version;
    FunctionPatcherFunctionType type;
    uint32_t physicalAddr; /* [needs to be filled]  */
    uint32_t virtualAddr;  /* [needs to be filled]  */
    uint32_t replaceAddr;  /* [needs to be filled] Address of our replacement function */
    uint32_t *replaceCall; /* [needs to be filled] Address to access the real_function */
    FunctionPatcherTargetProcess targetProcess;
    union {
        struct {
            const char *function_name;                   /* [needs to be filled] name of the function we want to replace */
            function_replacement_library_type_t library; /* [needs to be filled] rpl where the function we want to replace is. */
        } ReplaceInRPL;
        struct {
            const uint64_t *targetTitleIds;
            uint32_t targetTitleIdsCount;
            uint16_t versionMin;
            uint16_t versionMax;
            const char *executableName;
            uint32_t textOffset;
            const char *functionName;
        } ReplaceInRPX;
    };
} function_replacement_data_t;

typedef function_replacement_data_t function_replacement_data_v3_t;

#define REPLACE_FUNCTION_OF_EXECUTABLE_BY_FUNCTION_NAME(__replacementFunctionName,               \
                                                        __targetTitleIds, __targetTitleIdsCount, \
                                                        __executableName,                        \
                                                        __functionName)                          \
    REPLACE_FUNCTION_OF_EXECUTABLE_BY_FUNCTION_NAME_WITH_VERSION(__replacementFunctionName,      \
                                                                 __targetTitleIds,               \
                                                                 __targetTitleIdsCount,          \
                                                                 __executableName,               \
                                                                 __functionName,                 \
                                                                 0, 0xFFFF)

#define REPLACE_FUNCTION_OF_EXECUTABLE_BY_FUNCTION_NAME_WITH_VERSION(__replacementFunctionName,               \
                                                                     __targetTitleIds, __targetTitleIdsCount, \
                                                                     __executableName,                        \
                                                                     __functionName,                          \
                                                                     __versionMin, __versionMax)              \
    REPLACE_FUNCTION_OF_EXECUTABLE_WITH_VERSION_EX(__replacementFunctionName,                                 \
                                                   FUNCTION_PATCHER_REPLACE_FOR_EXECUTABLE_BY_NAME,           \
                                                   __targetTitleIds,                                          \
                                                   __targetTitleIdsCount,                                     \
                                                   __executableName,                                          \
                                                   0,                                                         \
                                                   __functionName,                                            \
                                                   __versionMin, __versionMax)

#define REPLACE_FUNCTION_OF_EXECUTABLE_BY_ADDRESS(__replacementFunctionName,               \
                                                  __targetTitleIds, __targetTitleIdsCount, \
                                                  __executableName,                        \
                                                  __textOffset)                            \
    REPLACE_FUNCTION_OF_EXECUTABLE_BY_ADDRESS_WITH_VERSION(__replacementFunctionName,      \
                                                           __targetTitleIds,               \
                                                           __targetTitleIdsCount,          \
                                                           __executableName,               \
                                                           __textOffset,                   \
                                                           0, 0xFFFF)

#define REPLACE_FUNCTION_OF_EXECUTABLE_BY_ADDRESS_WITH_VERSION(__replacementFunctionName,               \
                                                               __targetTitleIds, __targetTitleIdsCount, \
                                                               __executableName,                        \
                                                               __textOffset,                            \
                                                               __versionMin, __versionMax)              \
    REPLACE_FUNCTION_OF_EXECUTABLE_WITH_VERSION_EX(__replacementFunctionName,                           \
                                                   FUNCTION_PATCHER_REPLACE_FOR_EXECUTABLE_BY_ADDRESS,  \
                                                   __targetTitleIds,                                    \
                                                   __targetTitleIdsCount,                               \
                                                   __executableName,                                    \
                                                   __textOffset,                                        \
                                                   0,                                                   \
                                                   __versionMin, __versionMax)

#define REPLACE_FUNCTION_OF_EXECUTABLE_WITH_VERSION_EX(__replacementFunctionName,               \
                                                       __type,                                  \
                                                       __targetTitleIds, __targetTitleIdsCount, \
                                                       __executableName,                        \
                                                       __textOffset,                            \
                                                       __functionName,                          \
                                                       __versionMin, __versionMax)              \
    {                                                                                           \
        .version       = FUNCTION_REPLACEMENT_DATA_STRUCT_VERSION,                              \
        .type          = __type,                                                                \
        .physicalAddr  = 0,                                                                     \
        .virtualAddr   = 0,                                                                     \
        .replaceAddr   = (uint32_t) my_##__replacementFunctionName,                             \
        .replaceCall   = (uint32_t *) &real_##__replacementFunctionName,                        \
        .targetProcess = FP_TARGET_PROCESS_ALL,                                                 \
        .ReplaceInRPX  = {                                                                      \
             .targetTitleIds      = __targetTitleIds,                                           \
             .targetTitleIdsCount = __targetTitleIdsCount,                                      \
             .versionMin          = __versionMin,                                               \
             .versionMax          = __versionMax,                                               \
             .executableName      = __executableName,                                           \
             .textOffset          = __textOffset,                                               \
             .functionName        = __functionName,                                             \
        }                                                                                       \
    }

#define REPLACE_FUNCTION(x, lib, function_name) \
    REPLACE_FUNCTION_FOR_PROCESS(x, lib, function_name, FP_TARGET_PROCESS_GAME_AND_MENU)

#define REPLACE_FUNCTION_FOR_PROCESS(x, lib, function_name, process) \
    REPLACE_FUNCTION_EX(x, lib, #function_name, 0, 0, process)

#define REPLACE_FUNCTION_VIA_ADDRESS(x, physicalAddress, effectiveAddress) \
    REPLACE_FUNCTION_VIA_ADDRESS_FOR_PROCESS(x, physicalAddress, effectiveAddress, FP_TARGET_PROCESS_ALL)

#define REPLACE_FUNCTION_VIA_ADDRESS_FOR_PROCESS(x, physicalAddress, effectiveAddress, process) \
    REPLACE_FUNCTION_EX(x, LIBRARY_OTHER, #x, physicalAddress, effectiveAddress, process)

#define REPLACE_FUNCTION_EX(__replacementFunctionName, __lib, __function_name, \
                            __physicalAddress, __effectiveAddress, __process)  \
    {                                                                          \
        .version       = FUNCTION_REPLACEMENT_DATA_STRUCT_VERSION,             \
        .type          = FUNCTION_PATCHER_REPLACE_BY_LIB_OR_ADDRESS,           \
        .physicalAddr  = __physicalAddress,                                    \
        .virtualAddr   = __effectiveAddress,                                   \
        .replaceAddr   = (uint32_t) my_##__replacementFunctionName,            \
        .replaceCall   = (uint32_t *) &real_##__replacementFunctionName,       \
        .targetProcess = __process,                                            \
        .ReplaceInRPL  = {                                                     \
             .function_name = __function_name,                                 \
             .library       = __lib,                                           \
        }                                                                      \
    }

#define DECL_FUNCTION(res, name, ...)                                  \
    res (*real_##name)(__VA_ARGS__) __attribute__((section(".data"))); \
    res my_##name(__VA_ARGS__)
