#include <opt_selecter.h>
#include <stdlib.h>
#include <stdio.h>
#include <util.h>

static const char* _selectText[] = {
    "[ ]",
    "[*]",
};

static void _optSelecterDrawConsole(struct OptSelecterCtx* pCtx) {
    /* Get current render target and set render target to our console. */
    PrintConsole* pCur = consoleSelect(pCtx->pConsole);

    /* Clear the console. */
    consoleClear();

    /* Draw each option. */
    const int start = pCtx->visibleStartIndex;
    const int end = MIN(start + pCtx->pConsole->consoleHeight - 1, pCtx->optionCount);
    const int selected = pCtx->selectedOptIndex;
    for(int i = start; i < end; i++) {
        /* Print current option. */
        printf("%s %s\n", _selectText[i == selected], pCtx->pOptions[i].name);
    }

    consoleUpdate(NULL);

    /* Reset render target. */
    consoleSelect(pCur);
}

static void _optSelecterHandleDownInput(struct OptSelecterCtx* pCtx) {
    /* Check if we can go down. */
    if(pCtx->selectedOptIndex + 1 >= pCtx->optionCount) {
        return; // Do nothing.
    }

    /* Increment selection index. */
    pCtx->selectedOptIndex++;

    /* Check if we're at the bottom row. */
    if(pCtx->pConsole->consoleHeight - 1 == pCtx->selectedOptIndex - pCtx->visibleStartIndex) {
        /* Shift visible view down one option. */
        pCtx->visibleStartIndex++;
    }
}

static void _optSelecterHandleUpInput(struct OptSelecterCtx* pCtx) {
    /* Check if we can go up. */
    if(pCtx->selectedOptIndex - 1 < 0) {
        return; // Do nothing.
    }

    /* Decrement selection index. */
    pCtx->selectedOptIndex--;

    /* Check if we're at the top row. */
    if(pCtx->selectedOptIndex - pCtx->visibleStartIndex == -1) {
        /* Shift visible view down one option. */
        pCtx->visibleStartIndex--;
    }
}

static void _optSelecterHandleSelectInput(struct OptSelecterCtx* pCtx) {
    /* Invoke callback for selected option. */
    (*pCtx->pCallback)(pCtx->pCallbackData, pCtx->selectedOptIndex);
}

struct OptSelecterCtx* optSelecterSetup(PrintConsole* pConsole, struct OptSelecterOption* pOptions, size_t optionCount, OptSelecterCallback pCallback, void* pCallbackData) {
    /* Allocate new context. */
    struct OptSelecterCtx* pCtx = malloc(sizeof(*pCtx));
    assert(pCtx != NULL);

    /* Setup context variables. */
    pCtx->pConsole          = pConsole; // Use provided console.
    pCtx->pOptions          = pOptions; // Use provided options.
    pCtx->optionCount       = optionCount;
    pCtx->pCallback         = pCallback;
    pCtx->pCallbackData     = pCallbackData;
    pCtx->selectedOptIndex  = 0;        // Start at index 0.
    pCtx->visibleStartIndex = 0;        // Start at row 0.

    /* Return self. */
    return pCtx;
}

void optSelecterDestroy(struct OptSelecterCtx* pCtx) {
    /* Free ourself. */
    free(pCtx);
}

void optSelecterUpdate(struct OptSelecterCtx* pCtx, const PadState* pPadState) {
    /* Get held buttons. */
    u64 down = padGetButtonsDown(pPadState);

    /* Check if we're going up. */
    if(down & HidNpadButton_Up) {
        _optSelecterHandleUpInput(pCtx);
    }

    /* Check if we're going down. */
    if(down & HidNpadButton_Down) {
        _optSelecterHandleDownInput(pCtx);
    }

    /* Check if we're selecting an option. */
    if(down & HidNpadButton_A) {
        _optSelecterHandleSelectInput(pCtx);
    }

    /* Draw console. */
    _optSelecterDrawConsole(pCtx);
}

struct OptSelecterOption* optSelecterGetSelected(struct OptSelecterCtx* pCtx) {
    assert(pCtx != NULL);
    assert(pCtx->selectedOptIndex < pCtx->optionCount);
    return pCtx->pOptions + pCtx->selectedOptIndex;
}
