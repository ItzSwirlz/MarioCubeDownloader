#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include <wiisocket.h>
#include <wiiuse/wpad.h>

#include <file_download.h>
#include <initialization.h>

int main(int argc, char **argv) {
	initialize();

    char *url = "https://repo.mariocube.com/WADs/_WiiWare,%20VC,%20DLC,%20Channels%20&%20IOS/W/Wii%20&%20the%20Internet%20(United%20Kingdom)%20(v1)%20(Channel).wad";
  
    char *outfile = "sd:/SMBNESUSA.wad";
    download(url, outfile);

	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & WPAD_BUTTON_HOME ) exit(0);

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}
