#ifndef _LVL1SCENE_INCLUDE
#define _LVL1SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"
#include "Block.h"
#include "PickUp.h"
#include "Enemy.h"

class Lvl1Scene
{

public:
	Lvl1Scene();
	~Lvl1Scene();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

private:
	void initShaders();
	void updateCamera();
	void prepareEntities();

	TileMap *background, *map, *entities;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::ivec2 posCamera;
	glm::mat4 projection, modelview;

	vector<Block*> blocks;
	vector<PickUp*> pickUps;
	vector<Enemy*> enemies;	
};
