========================================================================
HAPI Library

(c) School of Computing, Engineering & Digital Technologies, Teesside University 2020
Created by Keith Ditchburn (k.j.ditchburn@tees.ac.uk)
========================================================================

For details on using the library please see the documentation available in the HAPI / Reference sub-directory 

Requirements
============
- Visual Studio 2019
- Windows 10

Files
=====

HAPI_lib.h - All the includes required to use the Hardware API (HAPI).	
HAPI_Types.h - HAPI types used in HAPI calls
HAPI_InputCodes.h - Contains macros to determine key presses

Your app must link with one of the HAPI static libraries:

For PC Builds:

HAPI64.lib			- 64 bit release version
HAPI_debug64.lib	- 64 bit debug version

DLLs for dependant libraries are in HAPI\Libs64

HAPI also needs the Viz 2019 runtime libraries installed. A lot of machines will have them already
but if not point the user at: https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads
	
Version Change History
======================

Version 2.55 22/08/21
=====================
- Rebuilt for new year with latest libraries (Visual Studio 2019, Version: 16.11.1).

Version 2.54 06/10/20
=====================
- Changes so if the initialisation of performance counters fails HAPI can still be used (minus the ram reporting etc.)

Version 2.53 16/09/20
=====================
- Rebuilt for new teaching year
- Added eBorderless start up flag to allow for running in a window but without an app bar
- Added eTopLeft start up flag to move the window to 0,0 on screen when running (rather than using Windows default position)

Version 2.52 21/09/19
=====================
- added VSync option to initialisation flags to allow vertical sync to be set
- added LimitFrameRate function
- Changed to debug symbol generation (Zi to Z7) means symbols are now embedded in debug lib making it bigger
but should get rid of the pdb warnings.

Version 2.50 20/08/18
=====================
- Removed video playback. It was not being used much and added a lot of dependant dlls. This means it is
much easier to distribute your game. The streamed media functions remain but now only support music playback.
- Removed other dependencies. This means HAPI now only needs OpenAL32.dll (and the Viz libs of course)
- Reworked streaming audio playback due to the above changes
- Rebuilt for latest Viz ready for new year
- Additional additions for HAPI Sprites support (mainly under the hood)

Version 2.35 05/02/18
=====================
- Completely altered the way text rendering is handled meaning it now renders direct to the surface
(previously it queued up the text and rendered it as the last thing before displaying)
This means that text is drawn in the correct order. It also means text can be drawn to any memory block
and a new RenderText function has been added to support this.
- Surface text rendering now takes a flag to determine if it writes alpha or not
- Added function: GetWorkingDirectory
- Added some further helper XML functions
- Includes fix for lab / home version mismatch for release mode (the other failed)
- Open dialog now uses result as initial filename (if not empty)

Version 2.31 27/09/17
=====================
- Possible fix for a Viz versioning problem with release builds
(Turned off /LTCG and /GL)
- Stopped distributing 32 bit versions as no student is using 32 bits

Version 2.30 06/09/17
=====================
- Rebuilt for Visual Studio 2017
- Updated all dependencies to latest versions
- Removed unsigned int where int would do throughout
- Deprecated DebugText, users should use cout and cerr instead
- Removed performance profiler as the one in Visual Studio does the same
- Fixed a memory leak when rendering text with an outline
- Added predefined colours
- Added some colour operators including insertion for cout
- Added the ability to get fps
- Added the ability to change the colour of the FPS text
- Added user text entry dialog
- Added ability to turn key repeat on and off
- Removed internal indirection where possible
- Windows 7 is no longer supported so removed specific dinput dll
- FileDialog now allows initial directory to be set as well as a flag to allow it to be usedto browse for folders not files

Version 2.24 - 04/04/17
- Added new function GetEnteredText to get text input without having to translate key pressed
- More fixes to cout
- Added ToString to HAPI_TColour
- Fixes to some key input

Version 2.23 - 17/03/17
- Reworked GetTextSize as SFML text can be offset when rendered so need to return full rectangle
- When errors occur HAPI now immediately writes the log to disk
- You can also force the log to be written early via the new call: WriteLogSoFar()

Version 2.22 - 26/02/2017
- Added a * operator for HAPI_TColour to allow modulation of one colour by another
- added XML GetByteStream and a byte stream constructor
- Fixed some logging output that was splitting lines incorrectly
- New function: GetExecutableDirectory - does what it says!	

Version 2.20 - 12/01/2017
- New functions added:
	- LoadTextureAligned to align texture data on a 128 bit boundary	
	- PauseStreamedMedia to avoid confusion when using StopStreamedMedia
	- Added by request:
		- UserTextEntry to allow for input from the user via a modal dialog box
		- GetTextSize to work out the size of the text prior to rendering
		- GetFPS, returns the current frame rate as a number
- Renamed ChangeFontDirect to ChangeFontFromFile to avoid confusion
- Added equivalence operators for HAPI_TColour
- Changed some project settings for optimisation reasons
- Changed some XML calls
- Changed CHapiXMLNode GetValue call so it returns an attribute that can be value queried
- Small internal changes to allow HAPI to be a subsystem for another library (HAPI Sprites)
- Other minor changes & optimisations

Version 2.10 - 19/11/2016
- Added XML loading, creating and saving via a seperate CHapiXML type	
	- See examples for how to use
	- Note: built using pugixml (http://pugixml.org/)

Version 2.03 - 12/11/2016
- FileDialog call now starts in the working directory (rather than Documents)
- FileDialog now adds default extension to filename
- Removed the maximum limit set on text entries - now dynamically resises as required
- ALT key now correctly releases

Version 2.02 - 30/10/2016
- screen pointer is now guaranteed to be aligned to a 16 byte boundary (128 bit)

Version 2.01 - 19/10/2016
- linked to Windows 7 version of XInput so HAPI can be used on Windows 7

Version 2.00 - 17/09/16
- Rebuilt for Visual Studio 2015 and updated to C++11 across the board
- Many additions to sound and music / video capabilities
- A number of interface changes meaning this is no longer instantly backward compatible
- Updated dependencies
- Video playback is now available again
- Many additions to sound and music / video capabilities
- Rebuilt for Visual Studio 2015 and updated to C++11 across the board
- Added: GetSoundRawData to allow access to the actual sample data of a sound (for advanced uses)
- Added: SetSoundRawData to change actual sample data
- Added: SWORD is an alias for signed short
- Fixed: mouse wheel was not being recorded correctly

Version 1.57 27/04/16
- Work Around: Still issues with performance counters so now if they fail HAPI will not exit but just continue without them
- Fixed: key input of comma was wrongly mapped to semi colon
- Fixed: added a wait to music stopping to help improve missed playback

Version 1.56 23/10/15
- Rebuilt with latest SDL 2 build (still beta!) due to current release breaking the rumble
- Changes so HAPI now supports Steam overlay (when launched from Steam)
- Changed internal performance query to use language agnostic lookup
(Basically this could mean HAPI may fail on a non-English OS)

Version 1.55 21/10/15
- Fixed a bug causing a crash when HAPI->Close was called
- Improved error checking for internal performance counters

Version 1.54 06/10/15
- Accidently left some debug code enabled in 1.53

Version 1.53 05/10/15
- Changed the GetFont call to allow the specifying of antialiasing and turned it off by default
- Added a function GetFontDirect to load a font from a filename rather than using one installed in the OS
- Font is now released correctly once the texture atlas is created

Version 1.52 17/09/15
- Minor modifications to text rendering

Version 1.51 11/08/15
- Added a function SaveImage
- Removed some debug output text no longer needed

Version 1.50 Beta 30/07/15
- Implemented SDL versions of all the DirectX functionality
- Added SDL2 as a renderer platform and made it the default for PC instead of Direct3D 9
- Changed to using DLL versions of C++ libraries
- Combined additional libraries into the HAPI library so there is now no need to link to anything else
	- However a number of DLLs are now needed (provided)
- Now only supports Windows 7 or later
- Music is now played back via SDL_Mixer and supports volume.
- Using FreeImage now to load textures so more types supported and power of 2 issues has gone
- Using FreeType for font support
	- Font names are looked up in the registry to get a filename
- HAPI no longer pauses when losing focus
- Supports SDL joystick, controller and haptic subsystems meaning the 360 is supported as before. If required many more
capabilities could be added to this for other devices. Something for the future.
- could not change enums to scoped C++11 as it would break legacy code
- could not use constexpr as not supported until Visual Studio 2015
- typedefs converted to c++11 alias (using)
- Added memory usage when FPS is shown
- HAPI internal text now uses own system font
- Added debug performance overlay

Version 1.40 18/05/14
- Rebuilt for Visual Studio 2013

Version 1.31 26/01/14
- Added support for running under Linux with SDL

Version 1.30 15/11/12
- Reworked XBox controller code to avoid large slow downs
- User must now implement deadzone checking so added values to HAPI_InputCodes.h:
	#define HK_GAMEPAD_LEFT_THUMB_DEADZONE  7849
	#define HK_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
	#define HK_GAMEPAD_TRIGGER_THRESHOLD    30

Version 1.25 01/10/12
- Recompiled with Visual Studio 2012 for updated libraries
- Added 64 bit versions
- Window title now indicates if you are running the 32 or 64 bit version of HAPI

Version 1.24 12/09/11
- Fixed the ALT-TAB error so the app now goes inactive (your loop will not update and time is paused) if the 
user switches from your full screen application
- Fixed the crash running in full screen when using a dual monitor setup
- the above fixes also mean that if your windowed app loses focus it will pause until it gains focus again

Version 1.23 03/10/10
- Rebuilt for Visual Studio 2010
- Rebuilt for Microsoft DirectX June 2010 Release

Version 1.22 16/12/09
- Improved the accuracy of the internal timer

Version 1.21 11/11/09
- Removed the exception throwing that had been added in 1.19

Version 1.20 24/09/09
- Recompiled for the August 2009 DirectX SDK

Version 1.19 08/09/09
- Recompiled for the March 2009 DirectX SDK
- Added exceptions for errors. When an error occurs HAPI now throws the HAPI_ErrorCodes value.

Version 1.18 - 08/09/08
- Recompiled for Visual Studio 2008 and the August 2008 DirectX SDK
- Added a ChangeFont call allowing the font type, size and weight to be specified

Version 1.17 - 28/09/07

- Fixed a bug when running under Vista causing video not to show correctly
- Changed the way the time is calculated so if the game is minimised the clock stops. Hence when it resumes any simulation linked to the time will continue correctly

Known issues
	- using ALT-TAB	in full screen mode leads to a HAPI Error. This is something that I aim to fix in the future.

Version 1.16 - 15/09/07

- Added two new functions: PlayStreamedMedia and StopStreamedMedia. In the PC version this allows
video or music to be streamed from disk. PC supported formats are .avi, .asf, .mpeg, .mp3 and .wav.
- If a texture is stretched on load HAPI now reports this as a warning in the output pane of Visual Studio (and in the log)
- You can no longer pass in a flag at init time to hide the pointer. Instead use the SetShowCursor function
- Removed the video capture. It is much easier just to use a third party tool
- The internal menu has been removed
- Updated for the latest version of DirectX (August 2007 Update)
- Added logging. Every time you run your game a log named HAPI_Log.html is generated. This can be used to help track down bugs.
- The window can no longer be resized

Version 1.15 - 23/02/07
 - recompiled to use the static Multithreaded Runtime Libraries instead of the DLL version. 
 I have done this to solve an issue people have had with the new Platform SDK and distributing their games.
 It means however that the HAPI lib files are much bigger. When linking you should now use: /MT and /MTd
 - added another video option that uses a 512MB buffer to improve the frame rate when recording video

Version 1.14 - 05/01/07
 - Bug fix: sometimes when minimising your window or debugging the HAPI internal menu would appear. This has been fixed.
 - Added a header with all the key and controller codes: HAPI_InputCodes.h
 - Added support for the GP2X platform
	- Added a SDL sound type: H_SSDL
	- Added a GP2X input type: H_IGP2X
	- Added a SDL renderer type: H_RSDL 
	- Limitations: the frame rate is not brilliant. I will work on improving it for a later version.
 - Added volume and pan optional parameters to the PlayASound call.
 - Due to the addition of an SDL platform the user main must now be named HAPI_main
 - Rebuilt for DirectX (December 2006 Update)
 
Version 1.13 - 16/11/06
- Added middle mouse button, mouse wheel and x1 and x2 button values to the mouse structure
- Fixed a major bug with GetLastError that caused a stack overflow
- Corrected the order of colours in HAPI_TColour to match texture and screen order
Version 1.12 - 05/11/06
- Fixed a problem with GetTime() on multi-core or multi-processor systems. Very occasionally
innacurate results were occuring.
- Changed from using stringstream to ostringstream as there is a memory leak in the Microsoft runtime library for stringstream (anything derived from iostream, see: http://connect.microsoft.com/VisualStudio/feedback/Workaround.aspx?FeedbackID=98861)
- In release mode the version text is no longer displayed. Instead the version number is appended
to the window title text
- Added some further error checking
- Made a few small optimisations
- Altered the order of the colours in HAPI_TColour to match texture and screen order
- Added a WORD typedef
- Added H_DEVICE_NOT_CONNECTED error code
- Added SetControllerRumble function to control the rumble motor speed for the Xbox 360 controller
Version 1.11 - 24/09/06
Fixed a build problem where the HAPI_debug library was being built with unicode
Added a default button parameter to the UserMessage function
Version 1.10 - 20/09/06
Added support for controllers and for the Xbox 360 controller specifically
Two new functions: GetMaxControllers and GetControllerData
Version 1.09 - 11/08/06
Changed to class based interface
Changed to use of STL string over char strings
Stopped keyboard messages being received when the window did not have the focus
Version 1.08 - 14/01/06
Added a function to toggle the cursor on or off: SetShowCursor
Version 1.07 - 04/01/06
Fixed a bug where the window close button was active in fullscreen mode
Added wrapper code for GAT version of HAPI - should not cause problems with full version
Version 1.06 - 19/12/05
Added HAPI_CreateVideo call to create an avi from the rendered frames
Added HAPI_MessageBox call to put up a message box
Version 1.05 - 05/12/05
Added a flags parameter to HAPI_Initialise
Added flags for specifying fullscreen mode and for specifying no mouse pointer
Version 1.04 - 01/12/05
Fixed a memory leak in Error.cpp
Fixed a couple of texture related bugs in PocketPC code
Improved error messages and information output to the Viz output pane
including actual loaded texture size.
Version 1.03 - 12/11/05
Fixed a bug in HAPI_DrawText where variables were not being displayed correctly
Added H_UNSUPPORTED error code
Improved error messages and information output to the Viz output pane
Version 1.02 - 30/10/05
HAPI_GetTime() call added
Renamed HAPI_TColour functions to avoid confusion (added setX)
Added HAPI_Close function so game can be shut down without relying on x button
Version 1.01 - 23/10/05		
PocketPC Platform support
Added Input subsystem
Subsystem enums altered
HAPI_Initialise call changed to take input subsystem and now uses default enums
Version 1.0 - 06/10/05
Tested OK in labs
Finalised the documentation
Version 0.9 - 30/10/05
Fixed some bugs in the DirectSound sound system
FPS text position made optional
Unicode build fixes
Version 0.8 - 29/09/05
Added the ability to position the FPS text
Version 0.7 - 22/09/05
Added the ability to hide/show the FPS
HAPI version text now disappears after 5 seconds
Version 0.6 - 21/09/05
Improved error handling
Version 0.5 - 20/09/05
Added PC sound support using DirectSound

Version 0.1 - 0.5
Preliminary construction builds
			
