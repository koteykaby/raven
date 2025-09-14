# Dedicated Server for Ghost Recon Future Soldier

This project is a community effort to restore the online features of *Tom Clancy's Ghost Recon: Future Soldier*  
and make the game playable again.

> [!NOTE]  
> The project is still under development and requires time to become fully functional.  
> Stability is not guaranteed!

## Usage (Windows)

### Build

To build this project, you need [MSVC Build Tools](https://visualstudio.microsoft.com/downloads/?q=build+tools)  
and [premake5](https://premake.github.io/download/) to generate build scripts.  

Once the tools are installed, run the `scripts/build_all_windows.bat` script from the source folder.  
The compiled binaries will be located in the `bin` folder.

### Installation

After building the project, copy the modded `ubiorbitapi_r2_loader.dll` and the configuration file  
(from the `config` folder) into the game directory where the executables are located.  

The game folder should look like this:

```text
Future Soldier.exe
Future Soldier DX9.exe
Future Soldier DX11.exe
Launcher.exe
uplay_r1_loader.dll
ubiorbitapi_r2_loader.dll       (modded)
ubiorbitapi_r2_loader.dll.orig  (original backup)
raven.ini
```

Now run server.exe, then launch the game to connect to the services.

> [!NOTE]
> Currently, only the DX9 binary is supported for patching.
> Using DX11 or other executables may cause crashes.
