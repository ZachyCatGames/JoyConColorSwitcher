// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include <opt_selecter.h>

enum State {
    State_SelectColor,
    State_SetupSelectController,
    State_SelectController,
    State_WaitConfirm,
    State_WriteColor,
    State_WaitExit,
};

static enum State state = State_SelectColor;
static int testNum = 0;

enum JcColorType {
    JcColorType_Black,
    JcColorType_Gray,
    JcColorType_NeonRed,
    JcColorType_NeonBlue,
    JcColorType_NeonYellow,
    JcColorType_NeonGreen,
    JcColorType_NeonPink,
    JcColorType_Red,
    JcColorType_Blue,
    JcColorType_NeonPurple,
    JcColorType_NeonOrange,
    JcColorType_White,
    JcColorType_PastelPink,
    JcColorType_PastelYellow,
    JcColorType_PastelPurple,
    JcColorType_PastelGreen,

    // Special Editions
    JcColorType_Splat2Green,
    JcColorType_Splat2Pink,
    JcColorType_SmoRed,
    JcColorType_SmashGray,
    JcColorType_LgpeBrown,
    JcColorType_LgpeYellow,
    JcColorType_Labo,
    JcColorType_DqXi,
    JcColorType_DisneyPurple,
    JcColorType_DisneyPink,
    JcColorType_AcNhGreen,
    JcColorType_AcNhBlue,
    JcColorType_FortniteYellow,
    JcColorType_FortniteBlue,
    JcColorType_MarioRed,
    JcColorType_MhRiseGray,
    JcColorType_Fortnite2Yellow,
    JcColorType_Fortnite2Blue,
    JcColorType_LozSsBlue,
    JcColorType_LozSsPurple,
    JcColorType_Splat3Blue,
    JcColorType_Splat3Yellow,
    JcColorType_PokeSvLeft,
    JcColorType_PokeSvRight,
    JcColorType_LozTotkGold,
};

static enum JcColorType _curJcColorType = JcColorType_Black;
static int _curPadIndex = 0;

static const struct OptSelecterOption _colorOptions[] = {
    { "Black" },
    { "Gray" },
    { "Neon Red" },
    { "Neon Blue" },
    { "Neon Yellow" },
    { "Neon Green" },
    { "Neon Pink" },
    { "Red" },
    { "Blue" },
    { "Neon Purple" },
    { "Neon Orange" },
    { "White" },
    { "Pastel Pink" },
    { "Pastel Yellow" },
    { "Pastel Purple" },
    { "Pastel Green" },

    { "Splatoon 2 Edition Neon-Green Left Joy-Con" },
    { "Splatoon 2 Edition Neon-Pink Right Joy-Con" },
    { "Super Mario Odyssey Edition Red Joy-Con" },
    { "Super Smash Bros. Ultimate Edition Gray Joy-Con" },
    { "Pokémon: Let's Go! Edition Eevee-Brown Left Joy-Con" },
    { "Pokémon: Let's Go! Edition Pikachu-Yellow Right Joy-Con" },
    { "Nintendo Labo Creators Contest Edition \"Cardboard\"-Colored Joy-Con" },
    { "Dragon Quest XI S Lotto Edition Royal-Blue Joy-Con" },
    { "Disney Tsum Tsum Festival Edition Neon-Purple Left Joy-Con" },
    { "Disney Tsum Tsum Festival Edition Neon-Pink Right Joy-Con" },
    { "Animal Crossing: New Horizons Edition Pastel-Green Left Joy-Con" },
    { "Animal Crossing: New Horizons Edition Pastel-Blue Right Joy-Con" },
    { "Fortnite Wildcat Edition Yellow Left Joy-Con" },
    { "Fortnite Wildcat Edition Dark-Blue Right Joy-Con" },
    { "Mario Red × Blue Edition Red Joy-Con" },
    { "Monster Hunter Rise Edition Gray Joy-Con" },
    { "Fortnite Fleet Force Edition Dark-Blue Left Joy-Con" },
    { "Fortnite Fleet Force Edition Yellow Right Joy-Con" },
    { "Legend of Zelda: Skyward Sword Edition Dark-Blue Left Joy-Con" },
    { "Legend of Zelda: Skyward Sword Edition Dark-Purple Right Joy-Con" },
    { "Splatoon 3 OLED Edition Blue-Gradient Left Joy-Con" },
    { "Splatoon 3 OLED Edition Neon-Yellow-Gradient Right Joy-Con" },
    { "Pokémon: Scarlet × Violet OLED Edition Scarlet Left Joy-Con" },
    { "Pokémon: Scarlet × Violet OLED Edition Violet Right Joy-Con" },
    { "Legend of Zelda: Tears of the Kingdom Edition Gold Joy-Con" },
};

static const HidNpadControllerColor _colorCodes[] = {
    { 0x313131, 0x0F0F0F }, // Black
    { 0x828282, 0x0F0F0F }, // Gray
    { 0xFF3C28, 0x1E0A0A }, // NeonRed
    { 0x0AB9E6, 0x001E1E }, // NeonBlue
    { 0xE6FF00, 0x142800 }, // NeonYellow
    { 0x1EDC00, 0x002800 }, // NeonGreen
    { 0xFF3278, 0x28001E }, // NeonPink
    { 0xE10F00, 0x280A0A }, // Red
    { 0x4655F5, 0x00000A }, // Blue
    { 0xB400E6, 0x140014 }, // NeonPurple
    { 0xFAA005, 0x0F0A00 }, // NeonOrange
    { 0xE6E6E6, 0x323232 }, // White
    { 0xFFAFAF, 0x372D2D }, // PastelPink
    { 0xF5FF82, 0x32332D }, // PastelYellow
    { 0xF0CBEB, 0x373037 }, // PastelPurple
    { 0xBCFFC8, 0x2D322D }, // PastelGreen

    { 0x1EDC00, 0x002800 }, // Splat2Green
    { 0xFF3278, 0x28001E }, // Splat2Pink
    { 0xE10F00, 0x280A0A }, // SmoRed
    { 0x828282, 0x0F0F0F }, // SmashGray
    { 0xC88C32, 0x281900 }, // LgpeBrown
    { 0xFFDC00, 0x322800 }, // LgpeYellow
    { 0xD7AA73, 0x1E1914 }, // Labo
    { 0x1473FA, 0x00000F }, // DqXi
    { 0xB400E6, 0x140014 }, // DisneyPurple
    { 0xFF3278, 0x28001E }, // DisneyPink
    { 0x82FF96, 0x0A1E0A }, // AcNhGreen
    { 0x96F5F5, 0x0A1E28 }, // AcNhBlue
    { 0xFFCC00, 0x1A1100 }, // FortniteYellow
    { 0x0084FF, 0x000F1E }, // FortniteBlue
    { 0xE10F00, 0x280A0A }, // MarioRed
    { 0x828282, 0x0F0F0F }, // MhRiseGray
    { 0xFFCC00, 0x1A1100 }, // Fortnite2Yellow
    { 0x0084FF, 0x000F1E }, // Fortnite2Blue
    { 0x2D50F0, 0x1E0F46 }, // LozSsBlue
    { 0x500FC8, 0x00051E }, // LozSsPurple
    { 0x6455F5, 0x28282D }, // Splat3Blue
    { 0xC3FA05, 0x1E1E28 }, // Splat3Yellow
    { 0xF07341, 0x322D1E }, // PokeSvLeft
    { 0x9650AA, 0x32322D }, // PokeSvRight
    { 0xD2BE69, 0x32322D }, // LozTotkGold
};

static const char* const _hidsysUniquePadTypeStrs[] = {
    "Embedded",
    "FullKeyController",
    "RightController",
    "LeftController",
    "DebugPad"
};

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

static void _setColorTypeCallback(void*, int idx) {
    /* Set color type to index. */
    _curJcColorType = idx;
}

static void _setControllerCallback(void*, int idx) {
    /* Set current pad index. */
    _curPadIndex = idx;
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

    /* Init hid:sys and hid:dbg. */
    hidsysInitialize();
    hiddbgInitialize();

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    /* Init color and controller selection menus. */
    struct OptSelecterCtx* pColorSelect,* pControllerSelect;
    pColorSelect = optSelecterSetup(pConsole, _colorOptions, sizeof(_colorOptions) / sizeof(_colorOptions[0]), _setColorTypeCallback, NULL);

    /* Allocate space for controller select options. */
    u32 padIdCount;
    HidsysUniquePadId padIds[16];
    struct OptSelecterOption controllerSelectOptions[16];

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
                /* Set state to select controller. */
                state = State_SetupSelectController;
            }
            break;
        }
        case State_SetupSelectController: {
            /* Get pad IDs. */
            hidsysGetUniquePadIds(padIds, sizeof(padIds) / sizeof(padIds[0]), &padIdCount);

            /* Setup controller selecter. */
            HidsysUniquePadType type;
            HidsysUniquePadSerialNumber serial;
            for(u32 i = 0; i < padIdCount; i++) {
                /* Get pad type. */
                hidsysGetUniquePadType(padIds[i], &type);

                /* Get serial number. */
                serial.serial_number[0] = 0;
                hidsysGetUniquePadSerialNumber(padIds[i], &serial);

                /* Check if serial is valid set to "unknown" if not. */
                if(serial.serial_number[0] == 0) {
                    strcpy(serial.serial_number, "unknown");
                }

                /* Setup option. */
                snprintf(controllerSelectOptions[i].name, sizeof(controllerSelectOptions[i].name),
                    "%s (%s; %lld)", serial.serial_number, _hidsysUniquePadTypeStrs[type], padIds[i]);
            }

            /* Init controller selecter. */
            pControllerSelect = optSelecterSetup(pConsole, controllerSelectOptions, padIdCount, _setControllerCallback, NULL);
            
            /* Set state to select controller. */
            state = State_SelectController;
            break;
        }
        case State_SelectController: {
            /* Update selecter state. */
            optSelecterUpdate(pControllerSelect, &pad);

            /* Continue to next state if an option was selected. */
            if(kDown & HidNpadButton_A) {
                optSelecterDestroy(pControllerSelect);
                state = State_WriteColor; // TODO: confirmation?
            }
            /* Go back if B was pressed. */
            else if(kDown & HidNpadButton_B) {
                optSelecterDestroy(pControllerSelect);
                state = State_SelectColor;
            }
            break;
        }
        case State_WriteColor:
        case State_WaitExit: {
            consoleClear();
            printf("Hello :) %d %d %d\n", state, _curJcColorType, padIdCount);
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
