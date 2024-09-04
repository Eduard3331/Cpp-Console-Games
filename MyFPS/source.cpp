//This is a FPS room simulator


//Controls: WASD for movement, QE for camera rotation, M to show the map

#include <iostream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <algorithm>
using namespace std;


int nScreenWidth = 120;	//The width and height have to match with the command prompt window's
int nScreenHeight = 40;

float fPlayerX = 1.0f;	//Player starting position and angle
float fPlayerY = 1.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;	//Map height and width
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;	//Player field of view and depth
float fDepth = 16.0f;

int main()
{
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); //Getting direct access to the console
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;	//Declaring and initializing the map (16x16 in this case)

	map += L"################";	//# - wall
	map += L"#..............#";	//. - empty space
	map += L"#...########...#";
	map += L"#...#.#..#.#...#";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#.....####.....#";
	map += L"#.....####.....#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"#.###......###.#";
	map += L"#.#.#......#.#.#";
	map += L"#..............#";
	map += L"################";

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	while (1)
	{

		auto tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;	
		tp1 = tp2;	
		float fElapsedTime = elapsedTime.count(); //This is to make the character move at a constant and fixed framerate

		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)	//Left rotation
			fPlayerA -= (0.9f) * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000)	//Right rotation
			fPlayerA += (0.9f) * fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)	//Forward movement
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;	//sinf and cosf are used to determine the angle in which the player is facing
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;	//and moving them towards that direction
		}
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')	//Collision detection
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)	//Backward movement
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
		}
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)	//Right movement
		{
			fPlayerX += sinf(fPlayerA + 3.14159/2.0) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA + 3.14159/2.0) * 5.0f * fElapsedTime;
		}
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX -= sinf(fPlayerA + 3.14159/2.0) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA + 3.14159 / 2.0) * 5.0f * fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)	//Left movement
		{
			fPlayerX += sinf(fPlayerA - 3.14159 / 2.0) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA - 3.14159 / 2.0) * 5.0f * fElapsedTime;
		}
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
		{
				fPlayerX -= sinf(fPlayerA - 3.14159 / 2.0) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA - 3.14159 / 2.0) * 5.0f * fElapsedTime;
		
		}
		for (int x = 0; x < nScreenWidth; x++)	//"Scanning" the player's screen using the width parameter
		{
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV; //Casting a ray from the player to each point
			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += 0.1f;	//Incrementing the distance from the player towards said direction

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);	//Testing if the ray has hit a wall
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)	//Checking if we went out of the map's bounds
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{
					if (map[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;

						vector<pair<float, float>> p;	//Checking if the ray has hit a boundary as well as a wall

						for(int cx=0; cx<2; cx++)
							for (int cy = 0; cy < 2; cy++)
							{
								float py = (float)nTestY + cy - fPlayerY; //Perfect corner coordinates
								float px = (float)nTestX + cx - fPlayerX;
								float d = sqrt(px * px + py * py); //Calculating the distance between perfect corner and player
								float dot = (fEyeX * px / d) + (fEyeY * py / d);	//Calculating the dot product
								p.push_back(make_pair(d, dot));
							}
						sort(p.begin(), p.end(), [](const pair<float, float>& left, const pair<float, float>& right) {return left.first < right.first; });
						//Since each square of wall has four perfect corners, we need to know which corners are the closest (and thus visible)
						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
					}
				}
				
			}
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			short nShade = ' ';
			short mShade = ' ';

			if (fDistanceToWall <= fDepth / 4.0f)		nShade = 0x2588;	//Shading the walls to give the impression of depth
			else if (fDistanceToWall < fDepth / 3.0f)	nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)	nShade = 0x2592;
			else if (fDistanceToWall < fDepth)			nShade = 0x2591;
			else										nShade = ' ';

			if (bBoundary)		nShade = ' ';	//Boundary shading
			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
					screen[y * nScreenWidth + x] = ' ';	//The ceiling is "empty"
				else if(y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nShade;	//Applying the shading to the walls
				else
				{				//Shading the floor
					float d = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (d < 0.2)		mShade = '0';
					else if (d < 0.4)	mShade = 'O';
					else if (d < 0.6)		mShade = '-';
					else if (d < 0.8)		mShade = '.';
					else					mShade = ' ';
					screen[y * nScreenWidth + x] = mShade;
				}
			}
		}
		//Showing the stats
		swprintf_s(screen,40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f",fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
		
		//Showing the map while the M key is pressed
		if (GetAsyncKeyState((unsigned short)'M') & 0x8000)
		{
			for (int nx = 0; nx < nMapHeight; nx++)
				for (int ny = 0; ny < nMapWidth; ny++)
				{
					screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];
				}

			screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'P';	//Player position
		}
	screen[nScreenWidth * nScreenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
}
	return 0;
}