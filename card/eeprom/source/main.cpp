/*---------------------------------------------------------------------------------

	Simple eeprom example
	-- davr

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

//---------------------------------------------------------------------------------
void ButtonWait() {
//---------------------------------------------------------------------------------
	iprintf("Press a key, start to exit...\n");
	while(1) {
		scanKeys();
		int buttons = keysDown();
		if (!pmMainLoop() || (buttons & KEY_START)) {
			ntrcardClose();
			exit(0);
		}
		if(buttons) break;
		swiWaitForVBlank();
	}

	scanKeys();
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	consoleDemoInit();

	static u8 header1[512];
	static u8 header2[512];

	// Try to obtain ARM9 access to the cart
	if (!ntrcardOpen()) {
		iprintf("Cannot open Slot-1\n");
		ButtonWait();
		return 0;
	}

	iprintf("Reading cart info...\n");

	while(1) {
		// Read the header twice to verify.
		// If the card is being encrypted, we will get random junk
		cardReadHeader(header1);
		cardReadHeader(header2);

		// Make sure we got the same data twice
		while(memcmp(header1, header2, 32) != 0) {
			// If not, the card needs ejected and reinserted into the DS
			iprintf("Please eject & reinsert DS card.\n");
			ButtonWait();
			cardReadHeader(header1);
			cardReadHeader(header2);
		}

		// Add a null char right after the game title, so we can print it
		header1[32] = '\0';

		// Read some various info about the EEPROM
		int type = cardEepromGetType();
		int size = cardEepromGetSize();

		iprintf("Game ID: %s\n", header1);
		iprintf("EEPROM:\n");
		iprintf(" Type: %d\n", type);
		iprintf(" Size: %d\n", size);
		ButtonWait();

		// Read the first 512 bytes of EEPROM
		static u8 data[512];
		cardReadEeprom(0, data, 512, type);

		iprintf("First 160 bytes of EEPROM: \n");

		// Print 20 rows of 8 bytes each
		for(int y=0; y<20; y++) {

			// print 8 bytes as hex
			for(int x=0; x<8; x++) {
				iprintf("%02x ", data[y*8 + x]);
			}

			// print 8 bytes as characters
			for(int x=0; x<8; x++) {
				u8 c = data[y*8 + x];
				if(isprint(c)) // only display if it's a printable character
					iprintf("%c", c);
				else
					iprintf(".");
			}
		}

		iprintf("Insert a new card to read again\n");
		ButtonWait();
	}

	return 0;
}
