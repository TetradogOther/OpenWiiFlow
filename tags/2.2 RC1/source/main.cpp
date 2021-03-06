#include "video.hpp"
#include "menu/menu.hpp"
#include "loader/disc.h"
#include "loader/fs.h"
#include "loader/alt_ios.h"
#include "loader/sys.h"
#include "loader/wbfs.h"
#include "text.hpp"
#include <ogc/system.h>
#include <unistd.h>
#include <wiilight.h>
#include "homebrew.h"
#include "gecko.h"

extern "C"
{
    extern void __exception_setreload(int t);
	extern bool use_port1;
}

extern const u8 wait_hdd_png[];

extern bool geckoinit;
extern bool bootHB;
extern int mainIOS;
extern int mainIOSRev;
extern int mainIOSminRev;

CMenu *mainMenu;
extern "C" void ShowError(const wstringEx &error){mainMenu->error(error); }
extern "C" void HideWaitMessage() {mainMenu->_hideWaitMessage(); }

void parse_ios_arg(int arg, int *ios, int *min_rev)
{
	*ios = arg;
	gprintf("Passed IOS: %i\n", *ios);
	switch (arg)
	{
		case 222: *min_rev = IOS_222_MIN_REV; break;
		case 223: *min_rev = IOS_223_MIN_REV; break;
		case 224: *min_rev = IOS_224_MIN_REV; break;
		case 249: *min_rev = IOS_249_MIN_REV; break;
		case 250: *min_rev = IOS_250_MIN_REV; break;
		default:  *min_rev = IOS_ODD_MIN_REV; break;
	}
	gprintf("Passed IOS Minimum Rev: %i\n", *min_rev);
}


int old_main(int argc, char **argv)
{
	geckoinit = InitGecko();
	__exception_setreload(5);

	SYS_SetArena1Hi((void *)0x81200000);	// See loader/apploader.c
	CVideo vid;

	bool wbfsOK = false;
	char *gameid = NULL;
	
	for (int i = 0; i < argc; i++)
	{
		if (argv[i] != NULL && strcasestr(argv[i], "ios=") != NULL && strlen(argv[i]) > 4)
		{
			while(argv[i][0] && !isdigit(argv[i][0])) argv[i]++;
			if (atoi(argv[i]) < 254 && atoi(argv[i]) > 0)
				parse_ios_arg(atoi(argv[i]), &mainIOS, &mainIOSminRev);
		}
		else if (argv[i] != NULL && strcasestr(argv[i], "port=") != NULL)
		{
			while(argv[i][0] && !isdigit(argv[i][0])) argv[i]++;
			bool port = atoi(argv[i]);
			if (port <= 1 && port >= 0)
				use_port1 = port;
		}
		else if (strlen(argv[i]) == 6)
		{
			gameid = argv[i];
			for (int i=0; i<5; i++)
				if (!isalnum(gameid[i]))
					gameid = NULL;
		}
	}
	gprintf("Loading cIOS: %d, Port: %d\n", mainIOS, is_ios_type(IOS_TYPE_HERMES) ? use_port1 : 0);

	// Load Custom IOS
	bool iosOK = loadIOS(mainIOS, false, false);
	mainIOSRev = IOS_GetRevision();
	iosOK = iosOK && mainIOSRev >= mainIOSminRev;

	// MEM2 usage :
	// 36 MB for general purpose allocations
	// 12 MB for covers (and temporary buffers)
	// adds 15 MB from MEM1 to obtain 27 MB for covers (about 150 HQ covers on screen)
	MEM2_init(36, 12);	// Max ~48

	// Init video
	vid.init();
	WIILIGHT_Init();

	vid.waitMessage(0.2f);

	// Init
	Sys_Init();
	Sys_ExitTo(EXIT_TO_HBC);

	if (iosOK)
	{
		Mount_Devices();
		wbfsOK = WBFS_Init(WBFS_DEVICE_USB, 1) >= 0;

		//Try swapping here first to avoid HDD Wait screen.
		for(int tries = 0; tries < 4 && !wbfsOK; tries++)// ~4 seconds before HDD wait will pop
		{
			usleep(1000000);

			if (FS_Mount_USB())
				wbfsOK = WBFS_Init(WBFS_DEVICE_USB, 1) >= 0;

			if(wbfsOK) break;
			if (is_ios_type(IOS_TYPE_HERMES))
			{
				use_port1 = !use_port1;
				loadIOS(mainIOS, false, true); //Reload the EHC module.
			}
		}

		if (!wbfsOK)
		{
			s8 switch_port = 4;

			// Show HDD Wait Screen
			STexture texWaitHDD;
			texWaitHDD.fromPNG(wait_hdd_png, GX_TF_RGB565, ALLOC_MALLOC);
			vid.hideWaitMessage();
			vid.waitMessage(texWaitHDD);

			while(!wbfsOK)
			{
				while(!FS_Mount_USB()) //Wait indefinitely until HDD is there or exit requested, trying each device 4 times one second apart before switching port.
				{
					if (switch_port < 4) switch_port++;

					WPAD_ScanPads(); PAD_ScanPads();

					u32 wbtnsPressed = 0, gbtnsPressed = 0,
						wbtnsHeld = 0, gbtnsHeld = 0;

					for(int chan = WPAD_MAX_WIIMOTES-1; chan >= 0; chan--)
					{
						wbtnsPressed |= WPAD_ButtonsDown(chan);
						gbtnsPressed |= PAD_ButtonsDown(chan);

						wbtnsHeld |= WPAD_ButtonsHeld(chan);
						gbtnsHeld |= PAD_ButtonsHeld(chan);
					 }

					 if (Sys_Exiting() || (wbtnsPressed & WBTN_HOME) || (gbtnsPressed & GBTN_HOME))
						Sys_Exit(0);

					if (switch_port >= 4 && is_ios_type(IOS_TYPE_HERMES))
					{
						use_port1 = !use_port1;
						loadIOS(mainIOS, false, true);
						switch_port = 0;
					}
					VIDEO_WaitVSync();
					usleep(1000000);
				}
				wbfsOK = WBFS_Init(WBFS_DEVICE_USB, 1) >= 0;
			}
			vid.hideWaitMessage();
			vid.waitMessage(0.2f);
			SMART_FREE(texWaitHDD.data);
		}
	}

	bool dipOK = Disc_Init() >= 0;
	MEM2_takeBigOnes(true);
	int ret = 0;
	do
	{
		Open_Inputs();
		Mount_Devices();
		gprintf("USB Available: %d\n", FS_USBAvailable());
		gprintf("SD Available: %d\n", FS_SDAvailable());

		CMenu menu(vid);
		menu.init();
		mainMenu = &menu;
		if (!iosOK)
			menu.error(sfmt("IOS %i rev%i or later is required", mainIOS, mainIOSminRev));
		else if (!dipOK)
			menu.error(L"Could not initialize the DIP module!");
		else if (!wbfsOK)
			menu.error(L"WBFS_Init failed");
		else
		{
			if (gameid != NULL && strlen(gameid) == 6)
				menu._directlaunch(gameid);
			else
				ret = menu.main();
		}
		vid.cleanup();
		Unmount_All_Devices();
		if (bootHB)	BootHomebrew();
	} while (ret == 1);
	return ret;
};

int main(int argc, char **argv)
{
	Sys_Exit(old_main(argc, argv));
	return 0;
}

