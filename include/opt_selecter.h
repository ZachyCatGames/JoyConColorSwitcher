#pragma once
#include <switch.h>

typedef void (*OptSelecterCallback)(void*, int);

struct OptSelecterOption {
    char name[96];
};

struct OptSelecterCtx {
    PrintConsole* pConsole;

    struct OptSelecterOption* pOptions;
    size_t optionCount;
    OptSelecterCallback pCallback;
    void* pCallbackData;

    int selectedOptIndex;
    int visibleStartIndex;
};

struct OptSelecterCtx* optSelecterSetup(PrintConsole* pConsole, const struct OptSelecterOption* pOptions, size_t optionCount, OptSelecterCallback pCallback, void* pCallbackData);

void optSelecterDestroy(struct OptSelecterCtx* pCtx);

void optSelecterUpdate(struct OptSelecterCtx* pCtx, const PadState* pPadState);

struct OptSelecterOption* optSelecterGetSelected(struct OptSelecterCtx* pCtx);
