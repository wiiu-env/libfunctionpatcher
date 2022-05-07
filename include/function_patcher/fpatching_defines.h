#pragma once

#include <stdint.h>

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
    FUNCTION_PATCHER_STATIC_FUNCTION  = 0,
    FUNCTION_PATCHER_DYNAMIC_FUNCTION = 1
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
#define FUNCTION_REPLACEMENT_DATA_STRUCT_VERSION 0x00000002

typedef struct function_replacement_data_t {
    uint32_t VERSION;
    uint32_t physicalAddr;                       /* [needs to be filled]  */
    uint32_t virtualAddr;                        /* [needs to be filled]  */
    uint32_t replaceAddr;                        /* [needs to be filled] Address of our replacement function */
    uint32_t *replaceCall;                       /* [needs to be filled] Address to access the real_function */
    function_replacement_library_type_t library; /* [needs to be filled] rpl where the function we want to replace is. */
    const char *function_name;                   /* [needs to be filled] name of the function we want to replace */
    FunctionPatcherTargetProcess targetProcess;  /* [will be filled] */
} function_replacement_data_t;


#define REPLACE_FUNCTION(x, lib, function_name) \
    REPLACE_FUNCTION_FOR_PROCESS(x, lib, function_name, FP_TARGET_PROCESS_GAME_AND_MENU)

#define REPLACE_FUNCTION_FOR_PROCESS(x, lib, function_name, process) \
    REPLACE_FUNCTION_EX(x, lib, #function_name, 0, 0, process)

#define REPLACE_FUNCTION_VIA_ADDRESS(x, physicalAddress, effectiveAddress) \
    REPLACE_FUNCTION_VIA_ADDRESS_FOR_PROCESS(x, physicalAddress, effectiveAddress, FP_TARGET_PROCESS_ALL)

#define REPLACE_FUNCTION_VIA_ADDRESS_FOR_PROCESS(x, physicalAddress, effectiveAddress, process) \
    REPLACE_FUNCTION_EX(x, LIBRARY_OTHER, #x, physicalAddress, effectiveAddress, process)

#define REPLACE_FUNCTION_EX(x, lib, function_name, physicalAddress, effectiveAddress, process) \
    {                                                                                          \
        FUNCTION_REPLACEMENT_DATA_STRUCT_VERSION,                                              \
                physicalAddress,                                                               \
                effectiveAddress,                                                              \
                (uint32_t) my_##x,                                                             \
                (uint32_t *) &real_##x,                                                        \
                lib,                                                                           \
                function_name,                                                                 \
                process                                                                        \
    }

#define DECL_FUNCTION(res, name, ...)                                  \
    res (*real_##name)(__VA_ARGS__) __attribute__((section(".data"))); \
    res my_##name(__VA_ARGS__)
