// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// See also libnx pad.h / hid.h.


#define ARRAY_SIZE(array) \
	((size_t) ((sizeof(array) / sizeof((array)[0]))))

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(8, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pads[8];

    for(size_t i=0; i<ARRAY_SIZE(pads); i++) {
        padInitializeWithMask(&pads[i], 1 << i);
    }

    //Matrix containing the name of each key. Useful for printing when a key is pressed
    char keysNames[28][32] = {
        "A", "B", "X", "Y",
        "StickL", "StickR", "L", "R",
        "ZL", "ZR", "Plus", "Minus",
        "Left", "Up", "Right", "Down",
        "StickLLeft", "StickLUp", "StickLRight", "StickLDown",
        "StickRLeft", "StickRUp", "StickRRight", "StickRDown",
        "LeftSL", "LeftSR", "RightSL", "RightSR",
    };

    printf("\x1b[1;1HPress PLUS to exit.");
    printf("\x1b[2;1HLeft joystick position:");
    printf("\x1b[4;1HRight joystick position:");

    // Main loop
    while(appletMainLoop())
    {
        // Clear console
        consoleClear();

        for(size_t i=0; i<ARRAY_SIZE(pads); i++) {
            printf("==== %zu ====\n", i);

            PadState *pad = &pads[i];
            // Scan the gamepad. This should be done once for each frame
            padUpdate(pad);

            // padGetButtonsDown returns the set of buttons that have been
            // newly pressed in this frame compared to the previous one
            u64 kDown = padGetButtonsDown(pad);

            // padGetButtons returns the set of buttons that are currently pressed
            u64 kHeld = padGetButtons(pad);

            // padGetButtonsUp returns the set of buttons that have been
            // newly released in this frame compared to the previous one
            u64 kUp = padGetButtonsUp(pad);

            if (kDown & HidNpadButton_Plus)
                goto app_exit; // break in order to return to hbmenu

            // Read the sticks' position
            HidAnalogStickState analog_stick_l = padGetStickPos(pad, 0);
            HidAnalogStickState analog_stick_r = padGetStickPos(pad, 1);

            // Print the sticks' position
            printf("LEFT: %04d; %04d\n", analog_stick_l.x, analog_stick_l.y);
            printf("RIGHT: %04d; %04d\n", analog_stick_r.x, analog_stick_r.y);
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

app_exit:
    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
