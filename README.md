# MarioCubeDownloader
WIP Wii homebrew application for downloading files from the [MarioCube](https://mariocube.com) preservation repository.

# Disclaimer
The MarioCube repository is focused on **preservation**, not **piracy**. With that in mind, use this tool responsibly. I'm not developing this for you to brag about how you have access to nearly every Wii/GC game or whatever makes you happy. It is tempting, but remember, that game developers deserve some love too.

Additionally, at the moment, SSL signature support isn't fully implemented, so the SSL check is currently disabled. YOU RUN THIS TOOL AT YOUR OWN RISK.

One more thing - the Wii's WiFi cards are **slow**. Don't expect blazing fast speeds, and don't expect some random 5 GB file to download for you in a few minutes.

# (I'll make a real README soon)
The general gist is, an application using libwiigui that is close to an authentic Nintendo-developed app feel for browsing and downloading files from the drive, using curl.

A few notes:
- Some code is borrowed from the Open Shop Channel's Homebrew Browser code
- Full-transprency: Some other code I'm using is generated from Bing Chat
    
# Building
FIXME: actually make this better, but assuming you have devkitpro and devkitppc installed, you'll need to install the portlibs for wii-curl and a few other libraries, then you can build as normal.

# FAQ

## Is the MarioCube team okay with this?
Yes! (4/3/2023)

## What is the point of this?
My Wii is downstairs in the basement with my Roku, and my computer is on the second floor of my house. It would be nice to just have automatic downloading of things.

## What can I download?
Pretty much anything from the repository (hopefully), BUT:

I will place restrictions on exactly what from the Wii point can be downloaded. I still need to figure out how user-choice item selection is going to work, but I'll probably prevent downloading IOS WADs, Wii Menu/NDEV WADs, and unnecessary files that can't be used with meaningfully on the console.

This isn't because I don't want to develop pointless features, but I don't want anyone to try installing IOS or the System Menu on top of the pre-existing ones they have. System-related WADs will be blocked from downloading, period. If you really want to, don't download it using my homebrew.

### What about WADs for services that can be restored?
This brings up a RiiConnect24 or WiiLink24 patcher app, but it would probably be safer and more effective to run on a personal computer than trying to patch on a Wii. Also, I don't think patched WADs for RC24/WL24 can be a drop-in replacement, since some require region info.

### What about Videos or Press Kits?
Videos, definitely, because WiiMC. Press Kits, I'm not sure what's in them since I've never downloaded the files.

### The repository has some DSiWare, can I download those?
Those won't help you on a Wii. Maybe another day some equivalent app can be made for *DS systems for this purpose.

## How can I help with development?
PR requests or translations are always welcome! I also want to try to have a WAD of this, so eventually some banner/music would be good.

# Also See
- [Open Shop Channel Homebrew Browser](https://github.com/OpenShopChannel/OSCHomebrewBrowser) for downloading Wii homebrew
- [NUSspli](https://github.com/V10lator/NUSspli) for the Wii U
