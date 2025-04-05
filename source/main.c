// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include <opt_selecter.h>

enum State {
    State_SelectColor,
    State_SelectController,
    State_WaitConfirm,
    State_WaitExit,
};

static enum State state = State_SelectColor;
static int testNum = 0;

struct OptSelecterOption options[] = {
    { "TEST00" },
    { "TEST01" },
    { "TEST02" },
    { "TEST03" },
    { "TEST04" },
    { "TEST05" },
    { "TEST06" },
    { "TEST07" },
    { "TEST08" },
    { "TEST09" },
    { "TEST10" },
    { "TEST11" },
    { "TEST12" },
    { "TEST13" },
    { "TEST14" },
    { "TEST15" },
    { "TEST16" },
    { "TEST17" },
    { "TEST18" },
    { "TEST19" },
    { "TEST20" },
    { "TEST21" },
    { "TEST22" },
    { "TEST23" },
    { "TEST24" },
    { "TEST25" },
    { "TEST26" },
    { "TEST27" },
    { "TEST28" },
    { "TEST29" },
    { "TEST30" },
    { "TEST31" },
    { "TEST32" },
    { "TEST33" },
    { "TEST34" },
    { "TEST35" },
    { "TEST36" },
    { "TEST37" },
    { "TEST38" },
    { "TEST39" },
    { "TEST40" },
    { "TEST41" },
    { "TEST42" },
    { "TEST43" },
    { "TEST44" },
    { "TEST45" },
    { "TEST46" },
    { "TEST47" },
    { "TEST48" },
    { "TEST49" },
};

static void _testCallback(void*, int idx) {
    /* Set state to WaitExit. */
    state = State_WaitExit;

    testNum = idx;
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    PrintConsole* pConsole = consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    /* Init color and controller selection menus. */
    struct OptSelecterCtx* pColorSelect,* pControllerSelect;
    pColorSelect = optSelecterSetup(pConsole, options, sizeof(options) / sizeof(options[0]), _testCallback, NULL);

    // Other initialization goes here. As a demonstration, we print hello world.
    //printf("Hello World!\n");

    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        switch(state) {
        case State_SelectColor: {
            optSelecterUpdate(pColorSelect, &pad);
            if(kDown & HidNpadButton_A) {
                state = State_WaitExit;
            }
            break;
        }
        case State_WaitExit: {
            consoleClear();
            printf("Hello :) %d\n", testNum);
            consoleUpdate(pConsole);
            break;
        }
        }

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

        // Your code goes here

        // Update the console, sending a new frame to the display
        //consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
