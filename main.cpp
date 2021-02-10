#include "include/raylib.h"
#include <iostream>
#include <cmath>
#include <math.h>
#include <string>
#include <vector>

#define NUMBER_OF_RAYS 540
#define FOV 60
#define WIDTHMAP 17
#define HEIGHTMAP 17
#define SCALE 10

typedef struct segment
{
	Vector2 firstPoint;
	Vector2 secondPoint;

} segment;

typedef struct ray
{
	Vector2 pos;
	Vector2 dir;

} ray;

typedef struct myRectangle
{
	Rectangle rec;
	Color col;
	bool isWall;


} myRec;

static inline float scale(float A, float A1, float A2, float Min, float Max)
{
	long double percentage = (A-A1)/(A1-A2);
	return (percentage) * (Min-Max)+Min;
}

static inline bool lineLine(ray &r, segment &s, Vector2 &intersecPoint)
{
	float x1 = r.pos.x;
	float y1 = r.pos.y;
	float x2 = r.dir.x;
	float y2 = r.dir.y;
	float x3 = s.firstPoint.x;
	float y3 = s.firstPoint.y;
	float x4 = s.secondPoint.x;
	float y4 = s.secondPoint.y;

	// calculate the direction of the lines
	float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		// optionally, draw a circle where the lines meet
		intersecPoint.x = x1 + (uA * (x2-x1));
		intersecPoint.y = y1 + (uA * (y2-y1));
		return true;
	}
	return false;
}

// static inline Vector2 closestVec(Vector2 ori, Vector2 v2, Vector2 intersec)
// {
// 	int cmp1 = 0;
// 	int cmp2 = 0;

// 	cmp1 = std::abs(ori.x - v2.x) + std::abs(ori.y - v2.y);
// 	cmp2 = std::abs(ori.x - intersec.x) + std::abs(ori.y - intersec.y);

// 	return ((cmp1 >= cmp2) ? intersec : v2);
// }

int main()
{
	int w = 1920;
	int h = 720;
	std::string name;

	Vector2 intersecPoint;
	bool inter = false;
	float fovInRadian = (FOV * PI) / 180;
	float angleBetweenRays = fovInRadian / NUMBER_OF_RAYS;


	Vector2 tmp;

	ray player;

	int lineWidth = w / NUMBER_OF_RAYS;
	if (lineWidth < 1) lineWidth = 1;

	std::vector<segment> seg;

	std::vector<float> dist;
	dist.resize(NUMBER_OF_RAYS);

	float temp;

	ray r[NUMBER_OF_RAYS];

	myRec map[HEIGHTMAP][WIDTHMAP];

	for (int i = 0; i < HEIGHTMAP; i++)
	{
		for (int j = 0; j < WIDTHMAP; j++)
		{
			if (i == 0 || j == 0 || i == HEIGHTMAP - 1 ||j == WIDTHMAP - 1)
			{
				map[i][j].col = RED;
				map[i][j].isWall = true;
				map[i][j].rec.x = j * SCALE;
				map[i][j].rec.y = i * SCALE;
				map[i][j].rec.height = SCALE;
				map[i][j].rec.width = SCALE;
			}
			else
			{
				map[i][j].col = {50, 50, 50, 255};
				map[i][j].isWall = false;
				map[i][j].rec.x = j * SCALE;
				map[i][j].rec.y = i * SCALE;
				map[i][j].rec.height = SCALE;
				map[i][j].rec.width = SCALE;
			}
		}
	}

	seg.push_back({{SCALE, SCALE}, {SCALE * WIDTHMAP - SCALE, SCALE}});													// NORTH
	seg.push_back({{SCALE, SCALE * HEIGHTMAP - SCALE}, {SCALE * WIDTHMAP - SCALE, SCALE * HEIGHTMAP - SCALE}});			// SOUTH
	seg.push_back({{SCALE * WIDTHMAP - SCALE, SCALE}, {SCALE * WIDTHMAP - SCALE, SCALE * HEIGHTMAP - SCALE}});			// EAST
	seg.push_back({{SCALE, SCALE}, {SCALE, SCALE * HEIGHTMAP - SCALE}});												// WEST

	player.pos = {(HEIGHTMAP * SCALE) / 2, (WIDTHMAP * SCALE) / 2};
	player.dir.x = player.pos.x + std::cos(PI * 2) * ((WIDTHMAP * SCALE) * 2);
	player.dir.y = player.pos.y + std::sin(PI * 2) * ((HEIGHTMAP * SCALE) * 2);

	for (unsigned int i = 0; i < NUMBER_OF_RAYS; i++)
		r[i].pos = player.pos;

	InitWindow(w, h, "Raycasting 3D");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_LEFT))
		{
			
			tmp.x =			  ((player.dir.x - player.pos.x) * std::cos(angleBetweenRays + 0.03f)) + ((player.dir.y - player.pos.y) * std::sin(angleBetweenRays) + player.pos.x);
			player.dir.y = ((-(player.dir.x - player.pos.x)) * std::sin(angleBetweenRays + 0.03f)) + ((player.dir.y - player.pos.y) * std::cos(angleBetweenRays) + player.pos.y);
			player.dir.x = tmp.x;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			
			tmp.x =			  ((player.dir.x - player.pos.x) * std::cos(-angleBetweenRays - 0.03f)) + ((player.dir.y - player.pos.y) * std::sin(-angleBetweenRays) + player.pos.x);
			player.dir.y = ((-(player.dir.x - player.pos.x)) * std::sin(-angleBetweenRays - 0.03f)) + ((player.dir.y - player.pos.y) * std::cos(-angleBetweenRays) + player.pos.y);
			player.dir.x = tmp.x;
		}
		if (IsKeyDown(KEY_UP))
		{
			tmp = player.pos;
			player.pos.x -= ((player.pos.x - player.dir.x) * SCALE) /  10000;
			player.pos.y -= ((player.pos.y - player.dir.y) * SCALE) /  10000;
			player.dir.x -= 		((tmp.x - player.dir.x) * SCALE) / 10000;
			player.dir.y -= 		((tmp.y - player.dir.y) * SCALE) / 10000;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			tmp = player.pos;
			player.pos.x += ((player.pos.x - player.dir.x) * SCALE) /  10000;
			player.pos.y += ((player.pos.y - player.dir.y) * SCALE) /  10000;
			player.dir.x += 		((tmp.x - player.dir.x) * SCALE) / 10000;
			player.dir.y += 		((tmp.y - player.dir.y) * SCALE) / 10000;
		}

		// Xr =  (X-Xc)*cos(ang) + (Y-Yc)*sin(ang) + Xc;
		// Yr = -(X-Xc)*sin(ang) + (Y-Yc)*cos(ang) + Yc;

		r[0].pos = player.pos;

		r[0].dir.x = ((player.dir.x - player.pos.x) * std::cos(fovInRadian / 2)) + ((player.dir.y - player.pos.y) * std::sin(fovInRadian / 2) + player.pos.x);
		r[0].dir.y = ((-(player.dir.x - player.pos.x)) * std::sin(fovInRadian / 2)) + ((player.dir.y - player.pos.y) * std::cos(fovInRadian / 2) + player.pos.y);

		for (unsigned int i = 1; i < NUMBER_OF_RAYS; i++)
		{
			r[i].pos = player.pos;
			r[i].dir.x =    ((r[i - 1].dir.x - player.pos.x) * std::cos(-angleBetweenRays)) + ((r[i - 1].dir.y - player.pos.y) * std::sin(-angleBetweenRays) + player.pos.x);
			r[i].dir.y = ((-(r[i - 1].dir.x - player.pos.x)) * std::sin(-angleBetweenRays)) + ((r[i - 1].dir.y - player.pos.y) * std::cos(-angleBetweenRays) + player.pos.y);
		}

		for (unsigned int i = 0; i < NUMBER_OF_RAYS; i++)
		{
			for (unsigned int j = 0; j < seg.size(); j++)
			{
				if (lineLine(r[i], seg[j], intersecPoint))
				{
					r[i].dir = intersecPoint;
					dist[i] = sqrtf((float)(((r[i].pos.x - r[i].dir.x) * (r[i].pos.x - r[i].dir.x)) + ((r[i].pos.y - r[i].dir.y) * (r[i].pos.y - r[i].dir.y))));			//distance between two points
				}
			}
		}
		name = "Raycasting 3D    FPS: " + std::to_string(GetFPS());
		SetWindowTitle(name.c_str());

		BeginDrawing();

			ClearBackground(BLACK);

			for (unsigned int i = 0; i < HEIGHTMAP; i++)
			{
				for (unsigned int j = 0; j < WIDTHMAP; j++)
				{
					DrawRectangle(map[i][j].rec.x, map[i][j].rec.y, SCALE, SCALE, map[i][j].col);
				}
			}
			for (int i = 0; i < NUMBER_OF_RAYS; i++)
				DrawLine(r[i].pos.x, r[i].pos.y, r[i].dir.x, r[i].dir.y, GOLD);

			for (int i = 0; i < NUMBER_OF_RAYS; i++)
			{
				temp = scale(dist[i], WIDTHMAP * SCALE, 0, 1, h);
				DrawRectangle(200 + (i * lineWidth), (h / 2) - (temp / 2), lineWidth, temp, GOLD);

			}

			// DrawCircle(player.pos.x, player.pos.y, 30, GOLD);
			// DrawCircle(player.dir.x, player.dir.y, 30, PURPLE);
			// for (int i = 0; i < seg.size(); i++)
			// 	DrawLine(seg[i].firstPoint.x, seg[i].firstPoint.y, seg[i].secondPoint.x, seg[i].secondPoint.y, GOLD);

		EndDrawing();
	}
}

// si dist == 0 				alors height == h
// si dist == widthmap*scale	alors h == 1
