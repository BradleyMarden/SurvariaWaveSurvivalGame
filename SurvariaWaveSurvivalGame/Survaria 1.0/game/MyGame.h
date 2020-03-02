#pragma once

class CMyGame : public CGame
{
	// Define sprites and other instance variables here
	CSprite background;
	CSprite deathScreen;
	CSprite menu;
	CSprite hotbarInv;
	CSprite invDirt;
	CSprite invGrass;
	CSprite invCobble;
	
	CSprite player;
	CSprite skeletonEnemy;
	CSprite eyeEnemy;

	//sprite lists
	CSpriteList grass;
	CSpriteList dirtSpawned;
	CSpriteList grassSpawned;
	CSpriteList cobbleSpawned;
	CSpriteList dirt;
	CSpriteList cobble;
	CSpriteList iron;
	CSpriteList copper;
	CSpriteList treeSmall;
	CSpriteList treeBig;
	CSpriteList skeletonEnemyList;
	CSpriteList projectiles;
	CSpriteList skeletonProjectiles;
	int numberListX[49]{ 20,60,100,140,180,220,260,300,340,380,420,460,500,540,580,620,660,700,740,780,820,860,900,940,980,1020,1060,1100,1140,1180,1220,1260,1300,1340,1380,1420,1460,1500,1540,1580,1620,1660,1700,1740,1780,1820,1860,1900,1940 };//each point where a block can spawn on the x axis
	int numberListY[27]{ 20,60,100,140,180,220,260,300,340,380,420,460,500,540,580,620,660,700,740,780,820,860,900,940,980,1020,1060 };
	enum { NONE, STANDLEFT, STANDRIGHT, WALKLEFT, WALKRIGHT, JUMPLEFT, JUMPRIGHT }p_state;
	enum {LEFT, RIGHT, HITLEFT,HITRIGHT };

	CSoundPlayer sounds;
	CSoundPlayer skeletonHit;
	CSoundPlayer playerHit;
	CSoundPlayer skeletonWalk;
	CSoundPlayer menuMusic;
	CSoundPlayer backgroundMusic;
	bool play;
	//grid variable
	int time;
	bool wave = false;
	bool walkLeft = false;
	bool walkRight = false;
	int waveNumber;
	int waveTimer;
	int width;
	int height;
	int gridSize;
	int modulusX;
	int modulusY;
	CVector gridVector;
	int roundsSurvived;
	int y;
	int blockAmount;
	int blockSize;
	int damage;
	int health;
	int calculateSeconds;
	int seconds;
	int setSpawnAmount;
	int setHealth;
	int setSpeed;
	bool hit;
	int skeletonsKilled;
	int skeletonsSpawned;
	int skeleH;
	int increaseHealth;
	int increaseSpawnAmount;
	int spawnIncrease;
	int counter;
	//vector
	CVector coordPos;
	//mouse control
	int mouseX;
	int mouseY;
	//ore amount
	int copperSpawnAmount;
	int ironSpawnAmount;

	//Inventory Keep
	int grassAmount;
	int dirtAmount;
	int cobbleAmount;
	bool slotOne;
	bool slotTwo;
	bool slotThree;
	bool slotFour;

	float timer;
	int waitingTime;
	int healthSaved;


	int mouseClicks;
	CSoundPlayer effect;
public:
	CMyGame(void);
	~CMyGame(void);

	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void OnGameOver();
	virtual void OnTerminate();
	virtual void BlockPlacement();
	virtual void playerControl();
	virtual void enemies();
	virtual void spawnEnemies();
	virtual void setupLevel1();
	virtual void setupLevel2();


	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
