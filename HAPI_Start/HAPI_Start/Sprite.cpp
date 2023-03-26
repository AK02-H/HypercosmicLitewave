#include "Sprite.h"
#include <HAPI_Types.h>
#include <math.h>


bool Sprite::LoadTexture(const std::string& filename)
{
	bool missingTexture = false;

	if (!HAPI.LoadTexture("Data/" + filename, &m_texture, m_width, m_height)) {

		HAPI.UserMessage("Could not load texture", "Error lol");

		missingTexture = true;

		if (!HAPI.LoadTexture("Data/texMissing.png", &m_texture, m_width, m_height)) {
			HAPI.UserMessage("why da hell is the error texture missing wallahi", "Error lol");
			return false;
		}
	}

	texRect = Rectangle(0, m_width, 0, m_height);	//default
	screenRect = Rectangle(0, screenWidth, 0, screenHeight);	//move to constructor later?


	
	return true;
}

//Mention that blackboxing made changing the animation system easier because
//I didn't need to change anything in the entity/animator component
bool Sprite::LoadAnimationSheetData(const std::string& filename)
{
	if (doAnimate)
	{

		CHapiXML xml("Data/" + filename);


		//First step: load all rectangle data
		m_allRects.clear();
		CHapiXMLNode* rectNodes = xml.GetFirstNodeWithName("Rectangles");
		std::vector<CHapiXMLNode*> rectangleChildren = rectNodes->GetChildren();
		for (auto node : rectangleChildren)
		{
			CHapiXMLAttribute tempAttL;
			CHapiXMLAttribute tempAttR;
			CHapiXMLAttribute tempAttT;
			CHapiXMLAttribute tempAttB;

			if (!node->GetAttributeWithName("left", tempAttL))
				std::cout << "xml attribute 'left' not found" << std::endl;

			if (!node->GetAttributeWithName("right", tempAttR))
				std::cout << "xml attribute 'right' not found" << std::endl;

			if (!node->GetAttributeWithName("top", tempAttT))
				std::cout << "xml attribute 'top' not found" << std::endl;

			if (!node->GetAttributeWithName("bottom", tempAttB))
				std::cout << "xml attribute 'bottom' not found" << std::endl;

			//be careful with the order values are passed into the rectangle constructor, the official class goes L R T B
			Rectangle newRect(tempAttL.AsInt(), tempAttR.AsInt(), tempAttT.AsInt(), tempAttB.AsInt());

			m_allRects.push_back(newRect);
		}


		//second step: load animations as int vectors, which determine which rect for which frame of animation

		//Currently no way to make sure this loads properly
		std::vector<CHapiXMLNode*> animNodes = xml.GetAllNodesWithName("Animation");

		for (auto node : animNodes)
		{
			CHapiXMLAttribute tempAttribute;

			std::string tempName;
			std::vector<int> tempAnimationFrames;

			if (!node->GetAttributeWithName("name", tempAttribute))
			{
				std::cout << "Something went wrong with getting animation name" << std::endl;
			}

			tempName = tempAttribute.AsString();

			std::vector<CHapiXMLNode*> animationFrameNodes = node->GetChildren();

			for (auto subNode : animationFrameNodes)
			{
				CHapiXMLAttribute newFrame;

				if (!subNode->GetAttributeWithName("frame", newFrame))
					std::cout << "xml attribute 'frame' not found" << std::endl;
				
				

				tempAnimationFrames.push_back(newFrame.AsInt());

			}

			AddNewAnimation(tempName, tempAnimationFrames);

		}

		//SetAnimation(std::string("test"));

	}

	//if texture is missing, do not load animation

	//set this bool as a parameter later
	//doAnimate can't be changed at runtime
	//if !doAnimate, texRect is used as the texture rectangle
	//if doAnimate, the animation functions get the texture rectangle from the current animation instead
	

	return true;
}



void Sprite::Draw(BYTE* screen, int x, int y, bool transparent, std::string animationName, int frameNumber, SpriteEffect sprEffect)
{
	assert(screen != nullptr);

	if (transparent)
	{
		//implement frame number later
		//BlitTransparent(screen, screenWidth, m_texture, m_width, m_height, x, y);
		//BlitTransparentRotNoRect(screen, screenWidth, m_texture, m_width, m_height, x, y, rotation);
		BlitTransparent(screen, m_texture, screenRect, texRect, x, y, animationName, frameNumber, sprEffect);
	}
	else
	{
		BlitFast(screen, m_texture, m_width, m_height, x, y);
	}
}

void Sprite::DrawRepeating(BYTE* screen, int x, int y, bool transparent, eScrollType scrollType, eScrollDirection scrollDir, int frameNumber)
{
	assert(screen != nullptr);

	BlitRepeating(screen, m_texture, screenRect, texRect, x, y, scrollType, scrollDir);
}

void Sprite::BlitTransparent(BYTE* screen, BYTE* texture, int texW, int texH, int posX, int posY) {

	int screenOffset = (posX + posY * screenWidth) * 4;
	int textureOffset = 0;

	int endOfLineOffset = (screenWidth - texW) * 4;

	for (size_t y = 0; y < texH; y++)
	{
		for (size_t x = 0; x < texW; x++)
		{

			if (x + posX < 0 || x + posX >= screenWidth || y + posY < 0 || y + posY >= SCR_HEIGHT) {//change hardcode later

			}
			else
			{



				BYTE texR = texture[0];
				BYTE texG = texture[1];
				BYTE texB = texture[2];
				BYTE texA = texture[3];

				if (texA == 255)
				{
					memcpy(screen + screenOffset, texture + textureOffset, 4);
				}
				else if (texA > 0) {

					float ratio = texA / 255.0f;

					BYTE screenR = screen[screenOffset];
					BYTE screenG = screen[screenOffset + 1];
					BYTE screenB = screen[screenOffset + 2];

					BYTE finalR = (BYTE)(ratio * texR + (screenR * (1 - ratio)));
					BYTE finalG = (BYTE)(ratio * texG + (screenG * (1 - ratio)));
					BYTE finalB = (BYTE)(ratio * texB + (screenB * (1 - ratio)));

					screen[screenOffset] = finalR;
					screen[screenOffset + 1] = finalG;
					screen[screenOffset + 2] = finalB;
				}

			}

			texture += 4;
			screenOffset += 4;

		}


		screenOffset += endOfLineOffset;




	}
}

//might ditch doing rotations despite the attempt due to it being
//hard to implement with all the other graphics systems like animation
//state in report that rotation functionality was sacrificed for animation,
//because you can do rotations with animation but you can't do animation with rotations


//EXISTING BUG: COLLIDERS NEAR NEGATIVE SIDES OF SCREEN MISALIGN WITH SPRITES. THIS MAY NOT BE FIXED UNLESS ABSOLUETLY NECESSARY
//This is a definite issue with the actual player position rather than the collider position, the issue exists in the clipping of rectangles,
//Because when the code clipping the texture rects to the screen is removed this issue leaves.
void Sprite::BlitTransparent(BYTE* screen, BYTE* texture, const Rectangle& screenRect, const Rectangle& texRect, int posX, int posY, std::string animationName, int frame, SpriteEffect sprEffect)
{

	Rectangle clippedRect;

	//When animated, there's a possible issue with how its checked whether the sprite rect is on screen,
	//as animated frames do not originate at 0
	//If this becomes an issue, translate the rect back to 0 before doing the check
	if (doAnimate)
	{
		//this copy cstr might be expensive, check with keith if it is later
		clippedRect = Rectangle(*GetAnimationRect(animationName, frame));

	}
	else
	{
		clippedRect = Rectangle(texRect);
	}

	
	//this code needs to be changed for animation to work

#pragma region stinky rect clipping code


	Rectangle placeHolder(clippedRect.left, clippedRect.right, clippedRect.top, clippedRect.bottom);

	placeHolder.Translate(-placeHolder.left + posX, -placeHolder.top + posY); //what the clart is this
	//placeHolder.Translate(posX, posY);

	placeHolder.ClipTo(screenRect);

	if (placeHolder.IsOutsideOf(screenRect)) {
		//std::cout << "OUTSIDE OF SCREEN" << std::endl;
		return;
	}

	//placeHolder.Translate(-posX, -posY);

#pragma region copy of original

	//Rectangle placeHolder(clippedRect);

	//placeHolder.Translate(posX, posY);

	//placeHolder.ClipTo(screenRect);

	//if (placeHolder.IsOutsideOf(screenRect)) {
	//	//std::cout << "OUTSIDE OF SCREEN" << std::endl;
	//	return;
	//}

	////placeHolder.Translate(-posX, -posY);

#pragma endregion


#pragma endregion
	//?
	/*if (posX < 0)
		posX = 0;
	if (posY < 0)
		posY = 0;*/


	int screenOffset{ 0 };
	int endOfLineOffset{ 0 };
	int endOfTexOffset{ 0 };


	InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);
	GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset, sprEffect);
	

}

//Code currently only works with sprites that are the exact size of the screen
void Sprite::BlitRepeating(BYTE* screen, BYTE* texture, const Rectangle& screenRect, const Rectangle& texRect, int posX, int posY, eScrollType scrollType, eScrollDirection scrollDir)
{


#pragma region new version


	int reverser = 1;
	int reverserA = 1;
	int reverserB = 1;

	//if scrolling in both directions, do the forward sprite first then the backwards one
	

	switch (scrollDir)
	{

	case eScrollDirection::Backward:
		reverser = -1;

		break;

	case eScrollDirection::HorizFOR_Vert_FOR:
		break;
	case eScrollDirection::HorizFOR_Vert_BACK:
		reverserB = -1;
		break;
	case eScrollDirection::HorizBACK_Vert_FOR:
		reverserA = -1;
		break;
	case eScrollDirection::HorizBACK_Vert_BACK:
		reverserA = -1;
		reverserB = -1;
		break;
	default:
		break;
	}


	//if (scrollDir == eScrollDirection::BothWays) reverser = -1;


	int initialValueOfPosX = posX;
	int initialValueOfPosY = posY;
	int factorsBiggerX{ 0 };
	int factorsBiggerY{ 0 };
	BYTE* initTexValue = texture;


	if (initialValueOfPosX < -2048)
	{
		posX = posX;
	}


	//if scrolls in both axis, do horizontal first then do vertical afterwards
	switch (scrollType)
	{
	case eScrollType::Horizontal:

		

		factorsBiggerX = (int) abs(posX) / screenWidth;
		posX -= screenWidth * (factorsBiggerX)*reverser;


		break;
	case eScrollType::Vertical:
		factorsBiggerY = (int) abs(posY) / screenHeight;
		posY -= screenHeight * (factorsBiggerY) * reverser;

		break;

	case eScrollType::HorizontalAndVertical:

		factorsBiggerX = (int)abs(posX) / screenWidth;
		posX -= screenWidth * (factorsBiggerX)*reverserA;

		factorsBiggerY = (int)abs(posY) / screenHeight;
		posY -= screenHeight * (factorsBiggerY)*reverserB;

		break;

	default:
		break;
	}


	Rectangle clippedRect(texRect);
	int screenOffset{ 0 };
	int endOfLineOffset{ 0 };
	int endOfTexOffset{ 0 };

	#pragma region possibly entirely unecessary


	//clippedRect.Translate(posX, posY);

	//clippedRect.ClipTo(screenRect);

	//if (clippedRect.IsOutsideOf(screenRect)) {
	//	//std::cout << "OUTSIDE OF SCREEN" << std::endl;
	//	return; //bring back or remove?
	//}

	//clippedRect.Translate(-posX, -posY);

	////?
	//if (posX < 0)
	//	posX = 0;
	//if (posY < 0)
	//	posY = 0;

#pragma endregion

	
	InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);	
	GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset);


	



	
	//
	if (scrollType != eScrollType::HorizontalAndVertical)
	{
		//twice


		texture = initTexValue;

		switch (scrollType)
		{
		case eScrollType::Horizontal:
			posX = initialValueOfPosX - screenWidth * (factorsBiggerX + 1) * reverser;
			break;
		case eScrollType::Vertical:
			posY = initialValueOfPosY - screenHeight * (factorsBiggerY + 1) * reverser;
			break;
		default:
			break;
		}



		InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);
		GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset);
	}
	else
	{
		int revX = 1;
		int revY = 1;

		switch (scrollDir)
		{
		case eScrollDirection::HorizFOR_Vert_FOR:

			revX = 1;
			revY = 1;

			break;
		case eScrollDirection::HorizFOR_Vert_BACK:

			revX = 1;
			revY = -1;

			break;
		case eScrollDirection::HorizBACK_Vert_FOR:

			revX = -1;
			revY = 1;

			break;
		case eScrollDirection::HorizBACK_Vert_BACK:

			revX = -1;
			revY = -1;

			break;
		default:
			break;
		}





		texture = initTexValue;
		posX = initialValueOfPosX - screenWidth * (factorsBiggerX + 1) * revX;
		posY = initialValueOfPosY - screenHeight * (factorsBiggerY) * revY;

		InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);
		GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset);

		texture = initTexValue;
		posX = initialValueOfPosX - screenWidth * (factorsBiggerX) * revX;
		posY = initialValueOfPosY - screenHeight * (factorsBiggerY + 1) * revY;

		InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);
		GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset);

		texture = initTexValue;
		posX = initialValueOfPosX - screenWidth * (factorsBiggerX + 1) * revX;
		posY = initialValueOfPosY - screenHeight * (factorsBiggerY + 1) * revY;

		InitBlitOffsets(screenOffset, endOfLineOffset, endOfTexOffset, texture, screenRect.Width(), clippedRect, texRect, posX, posY);
		GeneralBlitCode(screen, texture, clippedRect, screenRect.Width(), posX, posY, screenOffset, endOfLineOffset, endOfTexOffset);
	}

	

#pragma endregion



}


std::vector<int>* Sprite::GetAnimation(std::string& animationName)
{
	if (m_allAnimations.find(animationName) == m_allAnimations.end())
	{
		//animation not found, send message
		HAPI.UserMessage("Animation " + animationName + " not found in sprite.", "Warning");
		return nullptr;
	}
	else
	{
		return &m_allAnimations.find(animationName)->second;
	}
}

std::unordered_map<std::string, std::vector<int>>* Sprite::GetAllAnimations()
{
	return &m_allAnimations;
}

//Make an alternate blitfast for repeating?
void Sprite::BlitFast(BYTE* screen,
	BYTE* texture, int texW, int texH, int posX, int posY) {

	int screenOffset = (posX + posY * screenWidth) * 4;
	int textureOffset = 0;

	for (size_t y = 0; y < texH; y++)
	{
		memcpy(screen + screenOffset, texture + textureOffset, texW * 4);

		screenOffset += screenWidth * 4;
		textureOffset += texW * 4;
	}

}

//Mention in report: a lot of time was spent on implementing code that contributes to good game design, e.g gameplay feedback like flashes, hitstop and feedback
//Reduces line count of longer blit functions
void Sprite::GeneralBlitCode(BYTE*& screen, BYTE*& texture, const Rectangle& clippedRect, int screenRectWidth, int posX, int posY, int& screenOffset, int& endOfLineOffset, int& endOfTexOffset, SpriteEffect sprEffect)
{
	for (size_t y = 0; y < clippedRect.Height(); y++)
	{
		for (size_t x = 0; x < clippedRect.Width(); x++)
		{
			if (x + posX < 0 || x + posX >= screenRectWidth || y + posY < 0 || y + posY >= screenHeight) {//change hardcode later

			}
			else
			{

				//Alpha determines whether the pixel is drawn or not so it is checked first
				BYTE texA = texture[3];
			
				if (sprEffect == SpriteEffect::eHalfAlpha) texA /= 2;
				else if (sprEffect == SpriteEffect::eQuarterAlpha) texA /= 4;
				else if (sprEffect == SpriteEffect::eNegativeHalfTransparent)
				{
					texA /= 2;
				}

				if (texA == 255) {

					switch (sprEffect)
					{
					case SpriteEffect::eFlashWhiteFull:
					{

						screen[screenOffset] = 255;
						screen[screenOffset + 1] = 255;
						screen[screenOffset + 2] = 255;

						break;
					}
					case SpriteEffect::eFlashWhitePartial:
					{
						BYTE texR = texture[0];
						BYTE texG = texture[1];
						BYTE texB = texture[2];

						//Make proportin a global value?
						float propR = (255 - texR) * 0.5;
						float propG = (255 - texG) * 0.5;
						float propB = (255 - texB) * 0.5;

						screen[screenOffset] = (BYTE) texR + propR;
						screen[screenOffset + 1] = (BYTE)texG + propG;
						screen[screenOffset + 2] = (BYTE)texB + propB;

						break;
					}
					case SpriteEffect::eNegative:
					{


						BYTE texR = texture[0];
						BYTE texG = texture[1];
						BYTE texB = texture[2];

						/*screen[screenOffset] = texG;
						screen[screenOffset + 1] = texR;
						screen[screenOffset + 2] = texB;*/
						
						/*screen[screenOffset] = 255 - texG;
						screen[screenOffset + 1] = 255 - texR;
						screen[screenOffset + 2] = 255 - texB;*/

						screen[screenOffset] = 255 - texR;
						screen[screenOffset + 1] = 255 - texG;
						screen[screenOffset + 2] = 255 - texB;

						break;
					}

					default:
					{
						memcpy(screen + screenOffset, texture, 4);

						break;
					}
					}

				}
				else if (texA > 0) {


					BYTE texR = 0;
					BYTE texG = 0;
					BYTE texB = 0;

					//Implement the rest, otherwise effects won't work on transparent pixels
					//(low priority)
					switch (sprEffect)
					{
					case SpriteEffect::eFlashWhiteFull:
					{

						texR = 255;
						texG = 255;
						texB = 255;

						break;
					}
					case SpriteEffect::eFlashWhitePartial:
					{

						float propR = (255 - texR) * 0.5;
						float propG = (255 - texG) * 0.5;
						float propB = (255 - texB) * 0.5;


						texR += propR;
						texG += propG;
						texB += propB;

						break;
					}
					case SpriteEffect::eNegative:
					{
						texR = 255 - texR;
						texG = 255 - texG;
						texB = 255 - texB;

						break;
					}
					case SpriteEffect::eNegativeHalfTransparent:
					{
						texR = 255 - texR;
						texG = 255 - texG;
						texB = 255 - texB;

						break;
					}
					default:
					{

						texR = texture[0];
						texG = texture[1];
						texB = texture[2];

						break;
					}
					}




					float ratio = texA / 255.0f;

					BYTE screenR = screen[screenOffset];
					BYTE screenG = screen[screenOffset + 1];
					BYTE screenB = screen[screenOffset + 2];

					BYTE finalR = (BYTE)(ratio * texR + (screenR * (1 - ratio)));
					BYTE finalG = (BYTE)(ratio * texG + (screenG * (1 - ratio)));
					BYTE finalB = (BYTE)(ratio * texB + (screenB * (1 - ratio)));

					screen[screenOffset] = finalR;
					screen[screenOffset + 1] = finalG;
					screen[screenOffset + 2] = finalB;
				}

			}
			texture += 4;
			screenOffset += 4;
		}

		screenOffset += endOfLineOffset;
		texture += endOfTexOffset;
	}
}

void Sprite::InitBlitOffsets(int& screenOffset, int& endOfLineOffset, int& endOfTexOffset, BYTE* &texture, int screenRectWidth, Rectangle &clippedRect, const Rectangle& texRect, int posX, int posY)
{
	screenOffset = (posX + posY * screenRectWidth) * 4;
	endOfLineOffset = (screenRectWidth - clippedRect.Width()) * 4;
	endOfTexOffset = (texRect.Width() - clippedRect.Width()) * 4;
	texture += (clippedRect.left + clippedRect.top * texRect.Width()) * 4;
}

