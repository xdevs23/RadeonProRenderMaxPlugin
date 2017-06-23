# RadeonProRenderMaxPlugin

## Build Environment

Projects expect to find Max SDKs and binaries via environment variables (if they are not already present on your dev machine, 
set them to appropriate values)

Samples:
	ADSK_3DSMAX_SDK_2016=C:\Program Files\Autodesk\3ds Max 2016 SDK\maxsdk
	ADSK_3DSMAX_SDK_2017=C:\Program Files\Autodesk\3ds Max 2017 SDK\maxsdk
	ADSK_3DSMAX_x64_2016=C:\Program Files\Autodesk\3ds Max 2016\
	ADSK_3DSMAX_x64_2017=C:\Program Files\Autodesk\3ds Max 2017\


## To compile a release

1. Run the build.  At the end of the build a batch file will be run to create the dist directory which contains the binaries required for the max plugin.  If
Visual Studio is run in elevated mode, the binaries will be copied into the 3dsMax install directory.

## Debugging

Once project is building (recommend starting with Debug-2017)
	Set the paths in your Project->Debugging settings. Note that these values are stored in user files and are NOT retained 
	in source control. Set appropriate paths for different configurations, eg for Debug-2017 you will probably need:
		Debugging->Command = C:\Program Files\Autodesk\3ds Max 2017\3dsmax.exe
		Debugging->Working directory = C:\Program Files\Autodesk\3ds Max 2017

Now run using F5, and Max should launch, hopefully loading the plugin which should be copied in a post-build script to 
appropriate location. To test if plugin binary is being detected, you can put a breakpoint on LibInitialize() function in 
main.cpp, and it should be hit during Max startup.

Starting Developer Studio in Administrator(or elevated) mode will allow the plugin binaries contained in the 3dsMax directory to update in the post build step.

## To run autotests

1. Prepare test scenes. Each one goes into a separate folder, and needs to be named "scene.max".
2. In 3ds Max, go to command panel -> utilities -> more -> Autotesting tool
3. Set up parameters (how many passes to render in each scene, how many times to repeat tests (for memory leak hunting), ...) and click go. Select folder with the tests on HDD. 

## Outdated notes

The following are mostly wrong or out-of-date, to be removed with a more thorough revision of this doc.

### MSVS project & compilation notes

- We use a post build event that copies the correct DLL (debug or release) into 3ds max plugins directory. The script assumes 
  3ds Max is installed in the usual location
- "Multithreaded DLL" runtime library must be used, even in debug configuration. 3ds Max randomly segfaults otherwise. This 
  means STL containers will not perform any safety checks in debug mode, and are therefore unsafe to use. We use our own 
  containers isntead
- We use Level 4 warnings for maximum compiler-enforced safety
- 3ds Max <= 2012 uses multi-byte strings, 3ds Max >= 2013 uses unicode. This requires some clever typedefing
- We add this to the environment _NO_DEBUG_HEAP=1 - should we not do it, memory allocations/deallocations in debug mode with 
  debugger present would be EXTREMELY slow

### To run

1. Copy over all Radeon ProRender DLLs from folders:
     - Radeon ProRender plugin Debug build for max 2015
   into 3ds max folder (so the DLLs in root folder get copied to the same directory 
   where 3dsmax.exe is, and the content of the "plugins" folder gets copied to the 3dsmax "plugins" folder.
2. Compile FireMax. There is a post build script that will copy the compiled DLL into 3ds Max plugins folder. If the 3ds Max 
   is installed in unusual directory, this script (in "etc" folder) needs to be updated.
3. Run 3ds Max. There will be "Radeon ProRender" option in "Assign Renderer" rollout in render menu (opens after pressing F10).

