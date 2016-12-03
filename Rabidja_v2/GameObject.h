#pragma once
//#include "SDL.h"


class GameObject
{
protected:
	// Points de vie/santé + chrono d'invicibilité
	int life, invincibleTimer;

	// Coordonnées du sprite
	int x, y;

	// Largeur, hauteur du sprite 
	int h, w;

	// Checkpoint pour le héros (actif ou non)
	int checkpointActif;
	// + coordonnées de respawn (réapparition)
	int respawnX, respawnY;


	// Variables utiles pour l'animation :
	// Numéro de la frame (= image) en cours + timer
	int frameNumber, frameTimer, frameMax;
	// Nombre max de frames, état du sprite et direction
	//   dans laquelle il se déplace (gauche / droite)
	int etat, direction;


	// Variables utiles pour la gestion des collisions :
	//Est-il sur le sol, chrono une fois mort
	int onGround, timerMort;
	//Vecteurs de déplacement temporaires avant détection
	//des collisions avec la map
	float dirX, dirY;
	//Sauvegarde des coordonnées de départ
	int saveX, saveY;

	//Variable pour le double saut
	int jump;

	//Variables pour gérer les plateformes flottantes
	//Coordonnées de départ
	int beginX, beginY;
	// Type de plateforme (horizontale ou verticale)
	// Le joueur est-il dessus ?
	int type, player_dessus;

	//Valeurs attribuées aux états/directions
	const int IDLE = 0;
	const int  WALK = 1;
	const int  JUMP1 = 2;
	const int  JUMP2 = 3;
	const int  DEAD = 4;

	const int  RIGHT = 1;
	const int  LEFT = 2;

	//Constante pour les limites de la caméra avant scrolling
	const int  LIMITE_X = 400;
	const int  LIMITE_Y = 220;
	const int  LIMITE_W = 100;
	const int  LIMITE_H = 80;

	const int TIME_BETWEEN_2_FRAMES_PLAYER = 4;

	const int PLAYER_WIDTH = 40;
	const int PLAYER_HEIGTH = 50;

	//Vitesse de déplacement en pixels du sprite
	const int PLAYER_SPEED = 4;

	//Constantes définissant la gravité et la vitesse max de chute
	const float GRAVITY_SPEED = 0.6;
	const int MAX_FALL_SPEED = 15;
	const int JUMP_HEIGHT = 10;

	const int TILE_SIZE = 32;

public:
	int getOnGround() const;
	void setOnGround(int value);

	int getH() const;

	int getDirX() const;
	void setDirX(int value);

	int getX() const;
	void setX(int value);

	int getDirY() const;
	void setDirY(int value);

	void setBeginX(int value);
	int getBeginX() const;
	void setBeginY(int value) ;
	int getBeginY() const;
	int getType() const;
	void setType(int value);
	void setDirection(int value);
	int getDirection() const;

	int getPlayerDessu() const;
	void setPlayerDessu(int value);

	int getJumpHeight() const;

	int getLife() const;
	void setLife(int value);

	void setTimerMort(int value);
	int getW() const;
	void setW(int value);
	void setH(int value);

	int getY() const;
	void setY(int value);

	int getCheckPoint() const;
	void setCheckPoint(int value);

	int getInvincibleTimer() const;
	void setInvincibleTimer(int value);

	void setRespawn(int rx, int ry);


	GameObject();
	~GameObject();
};

