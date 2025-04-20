#ifndef WORLD_CPP_
#define WORLD_CPP_
#include "raylib.h"
#include "raymath.h"
#include "FastNoiseLite.h"

#define WRLDW 1200
#define WRLDH 1000
#define TLS 1.0f

class World {
	private:
	FastNoiseLite noiseGenerator;
	float nvalue;
	public:
	int seed;
	int tiles[WRLDW][WRLDH];

	void GenerateStone(int seedi) {
		int seed = seedi;
		noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Value);
		noiseGenerator.SetSeed(seed);
		noiseGenerator.SetFrequency(0.05f);
		for (int i = 0;i < WRLDW;i++) {
			for (int l = 0;l < WRLDH;l++) {
				nvalue = noiseGenerator.GetNoise(i*1.0f,l*1.0f);
				if (nvalue<0) {
					tiles[i][l] = 1;
				}
			}
		}
	}
	// Set exc to any negative value to not use it. Replaces ONLY exc tiles
	void AddNoiseClumps(FastNoiseLite::NoiseType noisetype,int idi,int freq,int exc) {
		noiseGenerator.SetNoiseType(noisetype);
		noiseGenerator.SetSeed(seed*2);
		noiseGenerator.SetFrequency(freq*0.01f);
		for (int i = 0;i < WRLDW;i++) {
			for (int l = 0;l < WRLDH;l++) {
				nvalue = noiseGenerator.GetNoise(i*1.0f,l*1.0f);
				if (nvalue<0) {
					if (exc>0) {
						if (tiles[i][l]==exc) {
							tiles[i][l] = idi;
						}
					} else {
						tiles[i][l] = idi;
					}
				}
			}
		}
	}
	void CreateSurface() {
		noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
		noiseGenerator.SetSeed(seed/2);
		noiseGenerator.SetFrequency(0.01f);
		for (int i = 0;i < WRLDW;i++) {
			nvalue = noiseGenerator.GetNoise(i*1.0f,1.0f);
			for (int l = 0;l < nvalue*80;l++) {
				tiles[i][l]=0;
			}
		}
	}

	void Draw(Rectangle viewportIndex) {
		Vector2 strt = {viewportIndex.x,viewportIndex.y};
		Vector2 end = {strt.x+viewportIndex.width,strt.y+viewportIndex.height};
		strt.x = Clamp(strt.x,0,WRLDW);
		strt.y = Clamp(strt.y,0,WRLDH);
		end.x = Clamp(end.x,0,WRLDW);
		end.y = Clamp(end.y,0,WRLDH);

		for (int i = strt.x;i < end.x;i++) {
			for (int l = strt.y;l < end.y;l++) {
				if (tiles[i][l]==1){DrawRectangleV({i*TLS,l*TLS},{TLS,TLS},GRAY);} else
				if (tiles[i][l]==2){DrawRectangleV({i*TLS,l*TLS},{TLS,TLS},BROWN);} else
				if (tiles[i][l]==3){DrawRectangleV({i*TLS,l*TLS},{TLS,TLS},GREEN);}
			}
		}
	}
};

#endif