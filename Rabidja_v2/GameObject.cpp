#include "GameObject.h"



int GameObject::getOnGround() const
{
	return this->onGround;
}

void GameObject::setOnGround(int value)
{
	this->onGround = value;
}

int GameObject::getH() const
{
	return this->h;
}

int GameObject::getDirX() const
{
	return this->dirX;
}

void GameObject::setDirX(int value)
{
	this->dirX = value;
}

int GameObject::getX() const
{
	return this->x;
}

void GameObject::setX(int value)
{
	this->x = value;
}

int GameObject::getDirY() const
{
	return this->dirY;
}

void GameObject::setDirY(int value)
{
	this->dirY = value;
}

void GameObject::setBeginX(int value)
{
	this->beginX = value;
}

int GameObject::getBeginX() const
{
	return this->beginX;
}

void GameObject::setBeginY(int value)
{
	this->beginY = value;
}

int GameObject::getBeginY() const
{
	return this->beginY;
}

int GameObject::getType() const
{
	return this->type;
}

void GameObject::setType(int value)
{
	this->type = value;
}

void GameObject::setDirection(int value)
{
	this->direction = value;
}

int GameObject::getDirection() const
{
	return this->direction;
}

int GameObject::getPlayerDessu() const
{
	return this->player_dessus;
}

void GameObject::setPlayerDessu(int value)
{
	this->player_dessus = value;
}

int GameObject::getJumpHeight() const
{
	return this->JUMP_HEIGHT;
}

int GameObject::getLife() const
{
	return this->life;
}

void GameObject::setLife(int value)
{
	this->life = value;
}

void GameObject::setTimerMort(int value)
{
	this->timerMort = value;
}

int GameObject::getW() const
{
	return this->w;
}

void GameObject::setW(int value)
{
	this->w = value;
}

void GameObject::setH(int value)
{
	this->h = value;
}

int GameObject::getY() const
{
	return this->y;
}

void GameObject::setY(int value)
{
	this->y = value;
}

int GameObject::getCheckPoint() const
{
	return this->checkpointActif;
}

void GameObject::setCheckPoint(int value)
{
	this->checkpointActif = value;
}

int GameObject::getInvincibleTimer() const
{
	return this->invincibleTimer;
}

void GameObject::setInvincibleTimer(int value)
{
	this->invincibleTimer = value;
}

void GameObject::setRespawn(int rx, int ry)
{
	this->respawnX = rx;
	this->respawnY = ry;
}



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}
