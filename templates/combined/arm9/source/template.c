/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	touchPosition touch;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("\n\n\tHello DS dev'rs\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\twww.devkitpro.org");

	while(pmMainLoop()) {

		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;

		if (keysHeld()&KEY_TOUCH) {
			touchRead(&touch);
			iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);
			iprintf("Touch y = %04i, %04i\n", touch.rawy, touch.py);
		}

	}

	return 0;
}
