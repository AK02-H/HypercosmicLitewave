/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file
	
	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI please see the Reference folder in the HAPI sub-directory
*/

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>
#include <cassert>
#include "World.h"
#include "IGraphics.h"
#include "Visualisation.h"
#include "Rectangle.h"


#include <HAPI_Types.h>


// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down
void HAPI_Main()
{
	#pragma region dummy code for creating quick animation xml sheets


	/*CHapiXMLNode* rootNode = new CHapiXMLNode("AnimSheet");

	CHapiXMLNode* rectangles = new CHapiXMLNode("Rectangles");
	CHapiXMLNode* anim = new CHapiXMLNode("Animation");
	
	CHapiXMLAttribute attrName("name", "animationName");
	anim->AddAttribute(attrName);

	int left = 0;
	int right = 91;
	int top = 0;
	int bottom = 343;

	int frame = 0;
	for (size_t y = 0; y < 4; y++)
	{
		for (size_t x = 0; x < 5; x++)
		{
			CHapiXMLNode* rect = new CHapiXMLNode("Rect");
			CHapiXMLAttribute attrFRAME("FRAMEID", frame);

			CHapiXMLAttribute attrLEFT("left", (int)left);
			CHapiXMLAttribute attrRIGHT("right", right);
			CHapiXMLAttribute attrTOP("top", top);
			CHapiXMLAttribute attrBOTTOM("bottom", bottom);

			rect->AddAttribute(attrFRAME);

			rect->AddAttribute(attrLEFT);
			rect->AddAttribute(attrRIGHT);
			rect->AddAttribute(attrTOP);
			rect->AddAttribute(attrBOTTOM);

			rectangles->AddChild(rect);

			CHapiXMLNode* frem = new CHapiXMLNode("Frame");

			CHapiXMLAttribute attrFrem("frame", frame);
			frem->AddAttribute(attrFrem);
			anim->AddChild(frem);


			frame++;
			left += 91;
			right += 91;
		}

		left = 0;
		right = 91;
		top += 343;
		bottom += 343;
	}
	
	rootNode->AddChild(rectangles);
	rootNode->AddChild(anim);
	
	CHapiXML xml(rootNode);
	xml.Save("animSheetTest.xml");*/



	#pragma endregion



	
	World world;
	world.LoadMainMenu();
	//world.Run();

	




	
}




