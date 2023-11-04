#include "Brick.h"

#include <stdlib.h>
#include <time.h>

#define SIZE_X 64
#define SIZE_Y 64

#define BRICK_JUMP_ANGLE_STEP 32
#define BRICK_JUMP_HEIGHT 32
#define BREAKING_DURATION 100

enum States
{
	NORMAL, MOVING, BREAKING, BREAKED
};

enum Type {
    NOCOIN, ONECOIN, MULTICOIN
};

void Brick::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
    Block::init(tileMapPos, shaderProgram);
    srand(time(NULL));
    breakingTime = 0;
    numCoins = 5;
    int num = rand() % 101;
    if (num < 10) type = MULTICOIN;
    else if (num < 50) type = NOCOIN;
    else type = ONECOIN;

    spritesheet.loadFromFile("images/spriteBlockLv1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(3);

    sprite->setAnimationSpeed(NORMAL, 8);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(MOVING, 8);
	sprite->addKeyframe(MOVING, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(BREAKING, 8);
	sprite->addKeyframe(BREAKING, glm::vec2(0.5, 0.f));

    sprite->setAnimationSpeed(BREAKED, 8);
	sprite->addKeyframe(BREAKED, glm::vec2(0.f, 0.5));
    sprite->changeAnimation(NORMAL);
}

void Brick::update(int deltaTime) {
    sprite->update(deltaTime);

    if (sprite -> animation() == MOVING) {
        jumpAngle += BRICK_JUMP_ANGLE_STEP;
		if(jumpAngle >= 180) {
            setPosition(glm::ivec2(position.x,originalY));
            sprite->changeAnimation(NORMAL);
        }
		else {
            int y = int(originalY - BRICK_JUMP_ANGLE_STEP * sin(3.14159f * jumpAngle / 180.f));
            setPosition(glm::ivec2(position.x,y));
        }
    }
    else if (sprite -> animation() == BREAKING) {
        breakingTime += deltaTime;
        isActive =  !(breakingTime > BREAKING_DURATION);
    }

    if (isHit) {
        sprite->changeAnimation(MOVING);
        jumpAngle = 0;
        originalY = position.y;
        isHit = false;
    }
    else if (isDestroyed && sprite -> animation() != BREAKING) {
        if (type != MULTICOIN) {
            breakingTime = 0;
            sprite->changeAnimation(BREAKING);
        }
        else if (numCoins <= 0) sprite->changeAnimation(BREAKED);
        isDestroyed = false;  
    }


}

void Brick::hit() {
    isHit = true;
}

void Brick::destroy() {
    isDestroyed = true;
    --numCoins;
}

bool Brick::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) 
{
    if (collision(position, glm::ivec2(SIZE_X,SIZE_Y), pos, size)) {
        return true;
    }
    return false;
}

bool Brick::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) 
{
    if (collision(position, glm::ivec2(SIZE_X,SIZE_Y), pos, size)) {
        *posX = position.x - SIZE_X;
        return true;
    }
    return false;
}

bool Brick::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) 
{
    if (collision(position, glm::ivec2(SIZE_X,SIZE_Y), pos, size)) {
        *posY = position.y - SIZE_Y;
        return true;
    }
    return false;
}

bool Brick::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) 
{
    if (collision(position, glm::ivec2(SIZE_X,SIZE_Y), pos, size)) { 
        *posY = position.y + SIZE_Y;
        return true;
    }
    return false;
}