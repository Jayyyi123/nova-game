#include "raylib.h"
#include "raymath.h"
#include "world.cpp"

int main(void)
{
	// INITIALIZE
	const int scw = 960;
	const int sch = 640;
	InitWindow(scw, sch, "Novala");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	// Camera
	Camera2D camera;
	camera.target = {0,0};
	camera.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	Rectangle viewportTiles;

	// World
	World world;
	world.GenerateStone(GetRandomValue(0,9999));
	world.AddNoiseClumps(FastNoiseLite::NoiseType_Perlin,2,20,1);
	world.AddNoiseClumps(FastNoiseLite::NoiseType_Value,0,10,-1);
	world.AddNoiseClumps(FastNoiseLite::NoiseType_Value,1,20,0);
	world.ErodeSurface();
	world.GrowGrass();
	world.AddUnderWorld();

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		// UPDATE
		camera.offset = {GetScreenWidth()/2.0f,GetScreenHeight()/2.0f};
		viewportTiles.x = (camera.target.x-camera.offset.x)/TLS;
		viewportTiles.y = (camera.target.y-camera.offset.y)/TLS;
		viewportTiles.width = ceil(GetScreenWidth()/TLS);
		viewportTiles.height = ceil(GetScreenHeight()/TLS);

		if (IsKeyDown(KEY_D)){camera.target.x+= 8;}
		if (IsKeyDown(KEY_A)){camera.target.x+=-8;}
		if (IsKeyDown(KEY_W)){camera.target.y+=-8;}
		if (IsKeyDown(KEY_S)){camera.target.y+= 8;}


		BeginDrawing(); // Start drawing
		ClearBackground(BLACK); // Clear the screen
		BeginMode2D(camera); // Use camera for renderings

		world.Draw(viewportTiles);

		DrawRectangleLinesEx({camera.target.x-camera.offset.x
			,camera.target.y-camera.offset.y,
			(float)GetScreenWidth(),
			(float)GetScreenHeight()},4.0f,RED);

		EndMode2D(); // Stop using camera for renderings
		EndDrawing(); // Stop drawing
	}
	// CLEANUP
	CloseWindow();
	return(0);
}