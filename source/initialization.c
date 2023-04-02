// A lot of this code comes from Open Shop Channel's source code.
// Do not attribute the entire file to me.

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include <gccore.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>

#include <wiisocket.h>
#include <wiiuse/wpad.h>
/*
* ----------------------------------------------
* Networking
* ----------------------------------------------
*/
bool ensure_wifi() {
	u32 ip = 0;
	int attempts = 0;

	if (attempts < 3) {
		do {
			ip = gethostid();
			if (!ip) printf("Unable to obtain IP, retrying...\n");
			attempts++;
		} while (!ip && attempts < 3);
	}

	if (ip) {
		return true;
	} else {
		printf("Unable to obtain this console's IP address.\n");
		return false;
	}
}

bool initialize_networking() {
	// Prepare libcurl's internals.
	curl_global_init(CURL_GLOBAL_ALL);

	// Attempt three times to initialize libwiisocket.
	int times = 0;
	int result = 0;
	while (times < 3) {
		result = wiisocket_init();
		if (result == 0) {
			// Success!
			break;
		}
		printf("Unable to initialize network, retrying...\n");
	}

	if (result != 0) {
		printf("Failed to initialize network. (wiisocket init error %d)\n", result);
	}

	// Ensure networking is available.
	return ensure_wifi();
}

/*
* ----------------------------------------------
* Storage/FAT filesystem
* ----------------------------------------------
*/

// Used to store things such as "sd:/" or "usb:/".
char rootdir[10];

bool sd_mounted = false;
bool usb_mounted = false;

// Determine whether we can open a folder on
// the given FAT media.
static bool can_open_root_fs() {
	DIR *root;
	root = opendir(rootdir);
	if (root) {
		closedir(root);
		return true;
	}
	return false;
}

// Ensure that we can read and write on the given FAT media.
bool test_fat() {
	// Try to open root filesystem - if we don't check here, mkdir crashes later
	if (!can_open_root_fs()) {
		printf("Unable to open root filesystem of %s\n", rootdir);
		return false;
	}

	// Change dir
	if (chdir(rootdir)) {
		printf("Could not change to root directory to %s\n", rootdir);
		return false;
	}

	// Create directories
	char dir_apps[50];
	char dir_hbb[150];
	char dir_hbbtemp[150];

	strcpy(dir_apps, rootdir);
	strcat(dir_apps, "apps");
	strcpy(dir_hbb, rootdir);
	strcat(dir_hbb, "apps/homebrew_browser");
	strcpy(dir_hbbtemp, rootdir);
	strcat(dir_hbbtemp, "apps/homebrew_browser/temp");

	if (!opendir(dir_apps)) {
		mkdir(dir_apps, 0777);
		if (!opendir(dir_apps)) {
			printf("Could not create %s directory.\n", dir_apps);
		}
	}


	if (!opendir(dir_hbb)) {
		mkdir(dir_hbb, 0777);
		if (!opendir(dir_hbb)) {
			printf("Could not create %s directory.\n", dir_hbb);
		}
	}

	if (!opendir(dir_hbbtemp)) {
		mkdir(dir_hbbtemp, 0777);
		if (!opendir(dir_hbbtemp)) {
			printf("Could not create %s directory.\n", dir_hbbtemp);
		}
	}

	return true;
}

const DISC_INTERFACE* sd = &__io_wiisd;
const DISC_INTERFACE* usb = &__io_usbstorage;

bool initialize_device(int method) {
	bool mounted = false;
	char name[10];
	const DISC_INTERFACE* disc = NULL;

	switch(method)
	{
			case 1:
					sprintf(name, "sd");
					disc = sd;
					break;
			case 2:
					sprintf(name, "usb");
					disc = usb;
					break;
	}

	if (disc->startup() && fatMountSimple(name, disc)) {
		mounted = true;
	} else {
		printf("Unable to mount %s...\n", name);
	}

	return mounted;
}

void initialize_fat() {
	// At least one FAT initialisation must be completed.
	printf("Attempting to mount SD card... ");
	if (initialize_device(1)) {  // 1 = METHOD_SD
		strcpy(rootdir, "sd:/");
		if (test_fat() == true) {
			sd_mounted = true;
			printf("SD card mounted.\n");
		} else {
			fatUnmount("sd:");
			sleep(1);
		}
	}
}

// --------------------------------------------------------------

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void initialize() {
    // Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

    // The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");
    
    initialize_networking();
	initialize_fat();
}