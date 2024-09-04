//A simple game of Pong versus a computer opponent
//Controls: UP and DOWN arrow keys

#include <iostream>
#include <raylib.h>

using namespace std;

int playerScore = 0;
int cpuScore = 0;

class Ball {
public:
	float x, y;
	int speedX, speedY, radius;

	void Draw()
	{
		DrawCircle(x, y, radius, LIME);
	}

	void Update()
	{
		x += speedX;
		y += speedY;
		if (y + radius >= GetScreenHeight() || y - radius <= 0)
		{
			speedY *= -1;
			speedX*=1.3;	//Every time the ball hits a wall it becomes faster until it resets
		}
		
		if (x - radius <= 0) {
			cpuScore++;
			ResetBall();
		}
		if (x + radius >= GetScreenWidth())
		{
				playerScore++;
				ResetBall();
		}
	}
	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;
		speedX = pow(-1, GetRandomValue(0, 1)) * 4;	//Randomizer for the ball's starting direction
		speedY = pow(-1, GetRandomValue(0, 1)) * 4;
	}
};
Ball ball;

class Paddle {
protected:
	void MoveLimit()	//Keeping the paddles from going off-screen
	{
		if (y < 0)
			y = 0;
		if (y + height >= GetScreenHeight())
			y = GetScreenHeight() - height;
	}
public: float x, y;
	  float width, height;
	  int speed;
	  void Draw()
	  {
		  DrawRectangle(x, y, width, height, BLUE);
	  }
	  void Update()
	  {
		  if (IsKeyDown(KEY_UP))
		  {
			  y = y - speed;
		  }
		  if (IsKeyDown(KEY_DOWN))
		  {
			  y = y + speed;
		  }
		  MoveLimit();
	  }
};
Paddle player;

class CpuPaddle : public Paddle {
public:
	void Update()
	{
		if (y> ball.y)
		{
			y = y - speed;
		}
		if (y + height< ball.y)
		{
			y = y + speed;
		}
		MoveLimit();
	}

};
CpuPaddle cpu;

int main()
{
	const int scrWidth = 1280; //Screen Width
	const int scrHeight = 800; //Screen Height
	InitWindow(scrWidth, scrHeight, "MyPong");
	SetTargetFPS(60);
	ball.radius = 15;
	ball.x = scrWidth / 2;
	ball.y = scrHeight / 2;
	ball.speedX = 5;
	ball.speedY = 5;

	player.width = 15;
	player.height = 120;
	player.x = 5;
	player.y = scrHeight / 2 - player.height / 2;
	player.speed = 5;

	cpu.width = 15;
	cpu.height = 120;
	cpu.x = scrWidth - player.width - 5;
	cpu.y = scrHeight / 2 - player.height / 2;
	cpu.speed = 5;

	while (!WindowShouldClose())
	{
		BeginDrawing();
		
		ball.Update();
		player.Update();
		cpu.Update();

		// Drawing
		ClearBackground(BLACK);
		DrawLine(scrWidth / 2, 0, scrWidth / 2, scrHeight, DARKBLUE);
		DrawRing({ scrWidth / 2, scrHeight / 2 }, 108, 110, 0, 360, 80, GOLD);
		DrawText(TextFormat("P1: %d", playerScore), 0, 0, 70, SKYBLUE);
		DrawText(TextFormat("P2: %d", cpuScore), scrWidth / 2, 0, 70, SKYBLUE);
		ball.Draw();
		player.Draw();
		cpu.Draw();

		//Collission detection between the ball and the two paddles
		if (CheckCollisionCircleRec({ ball.x,ball.y }, ball.radius, { player.x,player.y,player.width,player.height }))
		{
			ball.speedX = ball.speedX * -1;
		}
		if (CheckCollisionCircleRec({ ball.x,ball.y }, ball.radius, { cpu.x,cpu.y,cpu.width,cpu.height }))
		{
			ball.speedX = ball.speedX * -1;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}