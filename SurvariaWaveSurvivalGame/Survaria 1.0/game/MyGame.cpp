#include "stdafx.h"
#include "MyGame.h"

CMyGame::CMyGame(void) : menu(960, 540, "Menu.png", 0),
hotbarInv(960, 32.5,260,65, "hotbar.png",0),
invDirt(864,32.5, 0,0, 0),
invGrass(928, 32.5,.0,0,0),
invCobble(992,32.5,0,0,0)
{
	// TODO: add initialisation here
}

CMyGame::~CMyGame(void)
{
	// TODO: add destruction code here
}


void CMyGame::OnUpdate()
{
	if (IsMenuMode()) return;
	Uint32 t = GetTime();
	timer += GetDeltaTime();
	waveTimer += GetDeltaTime();
	time += GetDeltaTime();
	healthSaved = player.GetHealth();
	background.Update(t);
	deathScreen.Update(t);
	hotbarInv.Update(t);
	invDirt.Update(t);
	invGrass.Update(t);
	invCobble.Update(t);
	player.Update(t);
	skeletonEnemy.Update(t);
	playerControl();
	enemies();
	spawnEnemies();
	
	mouseX = GetMouseCoords().m_x;//setting the mouse X position, constantly updating.
	mouseY = GetMouseCoords().m_y;//setting the mouse Y position, constantly updating.
	for (CSprite* dirtBlock : dirt) { dirtBlock->Update(t); }
	for (CSprite* cobbleBlock : cobble) { cobbleBlock->Update(t); }
	for (CSprite* grassBlock : grass) { grassBlock->Update(t); }
	for (CSprite* copperOre : copper) { copperOre->Update(t); }
	for (CSprite* ironOre : iron) { ironOre->Update(t); }
	for (CSprite* tree : treeSmall) { tree->Update(t); }
	for (CSprite* tree : treeBig) { tree->Update(t); }
	for (CSprite* dirtBlockSpawned : dirtSpawned) { dirtBlockSpawned->Update(t); }
	for (CSprite* grassBlockSpawned : grassSpawned) { grassBlockSpawned->Update(t); }
	for (CSprite* cobbleBlockSpawned : cobbleSpawned) { cobbleBlockSpawned->Update(t); }
	for (CSprite* skeleton : skeletonEnemyList) { skeleton->Update(t); }
	for (CSprite* projectile : projectiles) { projectile->Update(t); }
	for (CSprite* skeltonProjectile : skeletonProjectiles) { skeltonProjectile->Update(t); }

	for (CSprite* projectile : projectiles)
	{
		// delete shot if reached the edge of the screen
		if (projectile->GetX() > 1920) projectile->Delete();
	}
	for (CSprite* projectile : projectiles)
	{
		for (CSprite* skeleton : skeletonEnemyList)
		{
			if (projectile->GetX() >= player.GetX() +100 || projectile->GetX() <= player.GetX() - 100)
			{
				projectile->Delete();
			}
			else {
				if (projectile->HitTest(skeleton, 1))
				{
					skeleton->SetHealth(skeleton->GetHealth() - 20);
					cout << skeleton->GetHealth() << endl;
					projectile->Delete();
				}
				if (skeleton->GetHealth() <= 0)
				{
					projectile->Delete();
					skeleton->Delete();
					skeletonsKilled++;
				}
			}
		}
	}
	
	
	projectiles.remove_if(deleted);
	skeletonEnemyList.remove_if(deleted);
	skeletonProjectiles.remove_if(deleted);

	if (IsGameOverMode())
	{
		if (IsKeyDown(SDLK_y))
		{
			
			NewGame();
			
		}
		if (IsKeyDown(SDLK_n))
		{

			StopGame();

		}
		backgroundMusic.FadeOut(10000);
		deathScreen.SetImage("grey");
		deathScreen.SetPosition(960, 540);
	}
	if (IsKeyDown(SDLK_F10))
	{
		GameOver();
	}
	
	if (play == false)
	{
		if (time > 9000)
		{
			backgroundMusic.Play("key1Quiet.wav", -1);
			play = true;
		}
	}
}


void CMyGame::BlockPlacement()//Spawns the blocks onto the screen.
{
	//Spawn Skeletons
	for (int i = 0; i <= setSpawnAmount; i++)
	{
		int x = rand() % 48;
		int setX = numberListX[x];
		CSprite* skeleton = new CSprite(setX, 483, 0, 0, GetTime());
		skeletonEnemyList.push_back(skeleton);
		skeletonsSpawned++;
		skeleton->SetHealth(setHealth);
	}
	//1 LAYER OF GRASS*******************************************************************************************
	CSprite* grassBlock = new CSprite(20, 420, "Grass_Block40.png", GetTime());//this spawns the first block
	grass.push_back(grassBlock);
	for (int i =0; i <= 48; i++)
	{
		int x = grassBlock->GetX() + blockSize;
		grassBlock = new CSprite(x, 420, "Grass_Block40.png", GetTime());//this then uses the x and y that we have just calculated for above to spawn a new block on the screen.
		grass.push_back(grassBlock);
	}
	//***********************************************************************************************************


	//DIRT BLOCK SPAWN*******************************************************************************************
	
	int y = 20;//*****    sets the y position *****
	CSprite* dirtBlock = new CSprite(20, 20, "Dirt_Block1.png", GetTime());//this spawns the first block
	dirt.push_back(dirtBlock);
	for (int i = 0; i <= blockAmount; i++) {//loop for how many blocks to spawn, remove 2 blocks.
		int x = dirtBlock->GetX() + blockSize;//because we spawn the first block at 20x20, we need to add 40(the block size) to the new position so that it does not over lap with the previous block.
		if (x >= 1920)//if the block is about to spawn off the screen run this code below.
		{
			x = x - 1920;//reset the block back to 0
			y = dirtBlock->GetY() + blockSize;//the y is already set to 20(as marked with the asterisks above^) so this code will get the current y position, and then add the blocksize(40) to it.
		//the code between this if statement is repeated at the end of every new line of blocks to start a new line, beginning on the left.
		}
		 dirtBlock = new CSprite(x, y, "Dirt_Block1.png", GetTime());//this then uses the x and y that we have just calculated for above to spawn a new block on the screen.
		dirt.push_back(dirtBlock);//this then pushes the spawned block to the list of blocks.
	}
	//***********************************************************************************************************

	//COBBLESTONE RANDOM SPAWN***********************************************************************************

	int cob = 1;//for debugging purposes.
	int  cobLost= 1;//for debugging purposes.
	for (int i = 0; i < 700; i++)// loop for how many blocks you want
	{
		int x =rand() % 48; //picks a random number out of 48
		cout << "X: " << x << endl;
		int y = rand() % 8;//picks a random number out of 10
		cout << "Y: " << y << endl;
		int setX = numberListX[x];//allocates the corresponding number from the list by using the random number generated
		int setY = numberListY[y];//allocates the corresponding number from the list by using the random number generated
		for (CSprite* cobbleBlock : cobble)//checking against every cobble block in the list
		{
			if (setX == cobbleBlock->GetX() && setY == cobbleBlock->GetY()) //NEEDS WORK, if both the x and y spots are the same do not spawn a block.
			{
				cobbleBlock->Delete();
				cobLost++;//for debuging purposes
				continue;//returns to begining of loop.
			}
		}
		CSprite* cobbleBlock = new CSprite(setX, setY, "Cobble_Block40.png", GetTime());// spawn the block using the setY and setX we calculated above.
			cobble.push_back(cobbleBlock);//add the newly spawned blocks to the list.
			for (CSprite* dirtBlock : dirt)//checking against every dirt block in the list.
			{
				if (setX == dirtBlock->GetX() && setY == dirtBlock->GetY())//checks to see if a cobble block has the same position as a dirt block.
				{
					dirtBlock->Delete();//deletes dirt block from screen if it is hidden behind a cobble block.
				}
			}
			cob++;//for debugging purposes.
	}
	cout << "Cobble On Screen: " << cob << endl;
	cout << "Cobble deleted due to overlapping with already spawned cobble: " << cobLost << endl;
	//***********************************************************************************************************

	//COPPER PLACEMENT*******************************************************************************************

	
	for (int i =0;i<= copperSpawnAmount; i++ )
	{
		int x = rand() % 48; //picks a random number out of 48
		int y = rand() % 8;//picks a random number out of 10
		int setX = numberListX[x];//allocates the corresponding number from the list by using the random number generated
		int setY = numberListY[y];
		CSprite* copperOre = new CSprite(setX, setY, "Copper_Ore40.png", GetTime());
		copper.push_back(copperOre);
	}


	//IRON PLACEMENT*********************************************************************************************
	
	for (int i = 0; i <= ironSpawnAmount; i++)
	{
		int x = rand() % 48; //picks a random number out of 48
		int y = rand() % 5;//picks a random number out of 10
		int setX = numberListX[x];//allocates the corresponding number from the list by using the random number generated
		int setY = numberListY[y];
		CSprite* ironOre = new CSprite(setX, setY, "Iron_Ore40.png", GetTime());
		iron.push_back(ironOre);
	}

	//TREE PLACEMENT*********************************************************************************************
	

	for (int i = 0; i <= 1; i++)
	{
		int x = rand() % 48;
		int setX = numberListX[x];
		CSprite* tree = new CSprite(setX, 788, 700, 700, "Tree140.png", GetTime());
		treeBig.push_back(tree);
	}
	for (int i = 0; i <= 3; i++)
	{
		int x = rand() % 48;
		int setX = numberListX[x];
		CSprite* tree = new CSprite(setX, 688, 500, 500, "Tree140.png", GetTime());
		treeSmall.push_back(tree);
	}
	grass.remove_if(deleted);
	dirt.remove_if(deleted);
	cobble.remove_if(deleted);
	copper.remove_if(deleted);
	iron.remove_if(deleted);
	treeSmall.remove_if(deleted);
	treeBig.remove_if(deleted);
}
void CMyGame::playerControl()
{
	float jumpSpeed = 500;
	float slowDown = 15;
	float walkSpeedLeft = -150;
	float wlakSpeedRight = 150;
	CVector oldPos = player.GetPosition();

	enum { NONE, STANDLEFT, STANDRIGHT, WALKLEFT, WALKRIGHT, JUMPLEFT, JUMPRIGHT } p_state;
	//walk right
	if (player.GetStatus() > 4) player.SetYVelocity(player.GetYVelocity() - slowDown);
	if(IsKeyDown(SDLK_d) && player.GetStatus() != WALKRIGHT && player.GetStatus() <WALKRIGHT)
	{
		player.SetMotion(wlakSpeedRight, 0);
		player.SetAnimation("walkright", 10);
		player.SetStatus(WALKRIGHT);
		sounds.Play("footstep2.wav",-1);
	}
	//stand right
	else if (!IsKeyDown(SDLK_d)&& player.GetStatus() == WALKRIGHT)
	{
		player.SetMotion(0, 0);
		player.SetImage("standright");
		player.SetStatus(STANDRIGHT);
		sounds.Play("footstep2.wav");
	}
	//walk left
	if (IsKeyDown(SDLK_a) && player.GetStatus() != WALKLEFT && player.GetStatus() < WALKLEFT)
	{
		player.SetMotion(walkSpeedLeft, 0);
		player.SetAnimation("walkleft", 10);
		player.SetStatus(WALKLEFT);
		sounds.Play("footstep2.wav",-1);
	}
	//stand left
	else if (!IsKeyDown(SDLK_a) && player.GetStatus() == WALKLEFT)
	{
		player.SetMotion(0, 0);
		player.SetImage("standleft");
		player.SetStatus(STANDLEFT);
		sounds.Play("footstep2.wav");
	}
	//jump 
	if (IsKeyDown(SDLK_SPACE) && player.GetStatus() != JUMPLEFT && player.GetStatus() != JUMPRIGHT)
	{
		//jump left
		if (player.GetStatus() == STANDLEFT || player.GetStatus() == WALKLEFT)
		{
			player.SetYVelocity(jumpSpeed);
			player.SetImage("standleft");
			player.SetStatus(JUMPLEFT);
			sounds.Play("jump.wav");
		}

		//jump right
		if (player.GetStatus() == STANDRIGHT || player.GetStatus() == WALKRIGHT)
		{
			player.SetYVelocity(jumpSpeed);
			player.SetImage("standright");
			player.SetStatus(JUMPRIGHT);
			sounds.Play("jump.wav");
		}
	}

	//sets floor
	for (CSprite* grassBlock :grass)
	{
		if (player.HitTest(grassBlock))
		{
			
				player.SetY(470);
				player.SetMotion(0, 0);
				player.SetImage("standleft");
				player.SetStatus(STANDLEFT);
		}
	}
	player.Update(GetTime());
}
void CMyGame::enemies()
{
	CVector playerPos = player.GetPos();
	for (CSprite* skeleton : skeletonEnemyList)
	{
		//move skeleton right
		if (skeleton->GetX() < playerPos.GetX())
		{
			skeleton->SetStatus(RIGHT);
				skeleton->SetXVelocity(20);
				skeleton->SetAnimation("runRight", 20);
				skeletonWalk.Play("80% Cut.wav" - 1);
				walkRight = false;
		}
		//move skeleton left
		if (skeleton->GetX() > playerPos.GetX())
		{
			skeleton->SetStatus(LEFT);
			skeleton->SetAnimation("runLeft", 20);
			skeletonWalk.Play("80% Cut.wav" - 1);

			skeleton->SetXVelocity(-20);
			walkLeft = false;
		}
			//shoot the player while facing right
			if (skeleton->GetStatus() == RIGHT && skeleton->GetX() >= player.GetX() - 50)
			{
				if (timer > 1000)
				{
					skeleton->SetAnimation("hitright");
					CSprite* skeletonProjectile = new CSprite(skeleton->GetX(), skeleton->GetY(), "Grass_Block40.png", CColor::White(), GetTime());
					skeletonProjectile->SetSize(1, 1);
					skeletonProjectile->SetXVelocity(1000);
					skeletonProjectiles.push_back(skeletonProjectile);
					timer = 0;
					skeletonHit.Play("weapondraw.wav");

				}
			}
			//shoot the player while facing left
			if (skeleton->GetStatus() == LEFT && skeleton->GetX() <= player.GetX() + 50)
			{
				if (timer > 1000)
				{
					skeleton->SetAnimation("hitleft", 20);
					CSprite* skeletonProjectile = new CSprite(skeleton->GetX(), skeleton->GetY(), "Grass_Block40.png", CColor::White(), GetTime());
					skeletonProjectile->SetSize(1, 1);
					skeletonProjectile->SetXVelocity(-1000);
					skeletonProjectiles.push_back(skeletonProjectile);
					cout << "multiple" << endl;
					timer = 0;
					skeletonHit.Play("weapondraw.wav");
				}
			}
			for (CSprite* skeletonProjectile : skeletonProjectiles) {
				
				
					if (skeletonProjectile->HitTest(&player, 1))
					{
						//remove 5 health from player
						player.SetHealth(healthSaved -5);
						sounds.Play("playerhit");
						cout << player.GetHealth() << endl;
						//player knockback
						if (player.GetX() < skeleton->GetX())
						{
							player.SetX(player.GetX() - 10);
						}
						if (player.GetX() > skeleton->GetX())
						{
							player.SetX(player.GetX() + 10);
						}
						skeletonProjectile->Delete();
					}
					if (player.GetHealth() <= 0)
					{
						//if player health <0 remove from screen and game over
						skeletonProjectile->Delete();
						player.SetPosition(-100, -100);
						GameOver();
					}
			}
	}
	skeletonEnemyList.remove_if(deleted);
	skeletonProjectiles.remove_if(deleted);
}
void CMyGame::spawnEnemies()
{
	if (skeletonsKilled == skeletonsSpawned)
	{
		wave = true;
		waveNumber++;
		roundsSurvived++;
		spawnIncrease = setSpawnAmount + increaseSpawnAmount;
		for (int i = 0; i <= spawnIncrease; i++)
		{
			//spawns the skeleton on alternate sides
			if (counter %2 ==0)
			{
				CSprite* skeleton = new CSprite(rand() % 480, 483, 0, 0, GetTime());
				skeletonEnemyList.push_back(skeleton);
				skeletonsSpawned++;
				skeleton->SetHealth(setHealth + increaseHealth);
				increaseHealth = increaseHealth + 10;
				skeleton->SetXVelocity(skeleton->GetXVelocity() + setSpeed);
			}
			else
			{
				CSprite* skeleton = new CSprite(rand() % 480 +1440, 483, 0, 0, GetTime());
				skeletonEnemyList.push_back(skeleton);
				skeletonsSpawned++;
				skeleton->SetHealth(setHealth + increaseHealth);
				increaseHealth = increaseHealth + 10;
				skeleton->SetXVelocity(skeleton->GetXVelocity() + setSpeed);
			}
		}
		wave == true;
		counter++;
		setupLevel2();
	}
	cout << "spawned: " << skeletonsSpawned << endl;//debug
	cout << "killed" << skeletonsKilled << endl;//debug
}

void CMyGame::OnDraw(CGraphics* g)
{
	background.Draw(g);
	for (CSprite* dirtBlock : dirt) { dirtBlock->Draw(g); }
	for (CSprite* cobbleBlock : cobble) { cobbleBlock->Draw(g); }
	for (CSprite* grassBlock : grass) { grassBlock->Draw(g); }
	for (CSprite* copperOre : copper) { copperOre->Draw(g); }
	for (CSprite* ironOre : iron) { ironOre->Draw(g); }
	for (CSprite* tree : treeSmall) { tree->Draw(g); }
	for (CSprite* tree : treeBig) { tree->Draw(g); }
	for (CSprite* dirtBlockSpawned : dirtSpawned) { dirtBlockSpawned->Draw(g); }
	for (CSprite* grassBlockSpawned : grassSpawned) { grassBlockSpawned->Draw(g); }
	for (CSprite* cobbleBlockSpawned : cobbleSpawned) { cobbleBlockSpawned->Draw(g); }
	for (CSprite* skeleton : skeletonEnemyList) { skeleton->Draw(g); }
	for (CSprite* projectile : projectiles) { projectile->Draw(g); }
	for (CSprite* skeletonProjectile : skeletonProjectiles) { skeletonProjectile->Draw(g); }

	if (IsMenuMode())
	{
		menu.Draw(g);
		return;
	}
	


	hotbarInv.Draw(g);
	invDirt.Draw(g);
	invGrass.Draw(g);
	invCobble.Draw(g);
	player.Draw(g);
	skeletonEnemy.Draw(g);
	deathScreen.Draw(g);
	//displays block amounts on hotbar
	*g <<  xy(870, 34) << font(10) << dirtAmount;
	*g << xy(934, 34) << font(10) << grassAmount;
	*g << xy(998, 34) << font(10) << cobbleAmount;
	//draws player health in green, yellow and red
	*g << xy(20, 20) << font(50) << color(CColor::Green()) << "Health: " << player.GetHealth();
	if (player.GetHealth() <= 50)
	{
		*g << xy(20, 20) << font(50) << color(CColor::Yellow()) << "Health: " << player.GetHealth();

	}
	if (player.GetHealth() <= 20)
	{
		*g << xy(20, 20) << font(50) << color(CColor::Red()) << "Health: " << player.GetHealth();

	}


	for (CSprite* skeleton : skeletonEnemyList)
	{
		
		*g << xy(skeleton->GetX() -10, skeleton->GetY() + 30) << color(CColor::Green())<< font(20) << skeleton->GetHealth();

		if (skeleton->GetHealth() <= 50)
		{
			*g << xy(skeleton->GetX() - 10, skeleton->GetY() + 30) << color(CColor::Yellow()) << font(20) << skeleton->GetHealth();
		}
		if (skeleton->GetHealth() <= 20)
		{
			*g << xy(skeleton->GetX() - 10, skeleton->GetY() + 30) << color(CColor::Red()) << font(20) << skeleton->GetHealth();
		}
	}
	//Time
		calculateSeconds++;
		seconds = calculateSeconds / 10;
		*g << xy(1720, 1000) << font(10) << color(CColor::Black()) << seconds;
		*g << xy(1400, 20) << font(50) << color(CColor::Green()) << "Skeletons Killed: " << skeletonsKilled;

	//displays wave number on screen	
	if (skeletonsKilled == skeletonsSpawned/2)
	{
		*g << xy(850, 1000) << color(CColor::DarkGreen()) << font(50) << "Wave " << waveNumber + 1 << "Approaches!";

	}
	else
	{
		*g << xy(930, 1000) << color(CColor::Green()) << font(50) << "Wave " << waveNumber << "!";
	}
	if (IsGameOverMode())
	{
		*g << center << vcenter << color(CColor::Red()) << font(100) << "You have perished!";
		*g << xy(880,190)<< color(CColor::Green()) << font(50) << skeletonsKilled;
		*g << xy(1260, 190) << color(CColor::Green()) << font(50) << roundsSurvived;

	}



}
void CMyGame::OnInitialize()
{
	
	backgroundMusic.Volume(1);
	menuMusic.Volume(1);
	skeletonWalk.Volume(1);
	playerHit.Volume(100);
	skeletonHit.Volume(1);
	sounds.Volume(1);
	play = false;
	waveNumber = 1;
	roundsSurvived = 0;
	waitingTime = 3;
	setSpawnAmount = 5;//starting spawn amount
	setHealth = 50;//startng health amount
	setSpeed = 5;//speed increase of skeleton per wave
	increaseHealth = 20;//health increase per round
	increaseSpawnAmount = 3;//skeleton spawn amoutn per round
	counter = 1;
	seconds = 0;
	skeletonsSpawned = 0;
	calculateSeconds = 0;
	skeletonsKilled = 0;
	player.SetHealth(100);
	blockAmount = 478;
	blockSize = 40;
	width = 48;
	height = 27;
	gridSize = width * height;
	copperSpawnAmount = 30;
	ironSpawnAmount = 20;
	invGrass.LoadImage("Grass_Block64.png", "grass");
	invDirt.LoadImage("Dirt_Block64.png", "dirt");
	invCobble.LoadImage("Cobble_Block64.png", "cobble");
	//player images
	player.AddImage("Walk_Right.png", "walkright", 4,1,0,0,3,0,CColor::Black());
	player.AddImage("Walk_Left.png", "walkleft", 4,1,0,0,3,0, CColor::Black());
	player.AddImage("Punch_Left.png", "punchleft", 9, 1, 0, 0, 8, 0, CColor::Black());
	player.AddImage("Punch_Right.png", "punchright", 9, 1, 0, 0, 8, 0, CColor::Black());
	player.LoadImage("Walk_Right.png", "standright", 4, 1, 0, 0, CColor::Black());
	player.LoadImage("Walk_Left.png", "standleft", 4, 1, 0, 0, CColor::Black());

	//skeleton images



	//player.SetAnimation("punchleft", 30);
	player.SetImage("standleft");
	player.SetPos(500, 466);

	background.LoadImage("background.png", "world");
	background.SetImage("world");
	background.SetPosition(960, 540);
	deathScreen.LoadImage("DeathScreen.png", "grey");

}
// called when a new game is requested (e.g. when F2 pressed)
// use this function to prepare a menu or a welcome screen
void CMyGame::OnDisplayMenu()
{
	play = false;
	skeletonEnemyList.clear();
	grass.clear();
	cobble.clear();
	dirt.clear();
	copper.clear();
	iron.clear();
	treeSmall.clear();
	treeBig.clear();
	seconds = 0;
	skeletonsSpawned = 0;
	calculateSeconds = 0;
	skeletonsKilled = 0;
	player.SetHealth(100);
	player.SetPos(500, 466);
	deathScreen.SetPosition(-960, -540);
	setupLevel1();
	menuMusic.Play("menumusic.wav",-1);
	waveNumber = 1;


}
void CMyGame::setupLevel1()
{
	BlockPlacement();
	for (CSprite* skeleton : skeletonEnemyList)
	{
		skeleton->LoadImage("Skele_Left.png", "skelestandleft", 11, 1, 0, 0, CColor::Black());
		skeleton->AddImage("Skele_Right.png", "runRight", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_Left.png", "runLeft", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_Swing.png", "hitright", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_SwingLeft.png", "hitleft", 11, 1, 0, 0, 10, 0, CColor::Black());
	}
}
void CMyGame::setupLevel2()
{
	setSpawnAmount = setSpawnAmount + increaseSpawnAmount;
	for (CSprite* skeleton : skeletonEnemyList)
	{
		skeleton->LoadImage("Skele_Left.png", "skelestandleft", 11, 1, 0, 0, CColor::Black());
		skeleton->AddImage("Skele_Right.png", "runRight", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_Left.png", "runLeft", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_Swing.png", "hitright", 11, 1, 0, 0, 10, 0, CColor::Black());
		skeleton->AddImage("Skele_SwingLeft.png", "hitleft", 11, 1, 0, 0, 10, 0, CColor::Black());
	}
}


// called when a new game is started
// as a second phase after a menu or a welcome screen
void CMyGame::OnStartGame()
{

}

// called when a new level started - first call for nLevel = 1
void CMyGame::OnStartLevel(Sint16 nLevel)
{
	
	
}

// called when the game is over
void CMyGame::OnGameOver()
{
	skeletonEnemyList.clear();
	sounds.Play("death.wav");
}

// one time termination code
void CMyGame::OnTerminate()
{
}


/////////////////////////////////////////////////////
// Keyboard Event Handlers

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_p)
	{
		if (IsMenuMode()) StartGame();
		menuMusic.FadeOut(10000);
	}
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();
	
	if (sym == SDLK_F2)
	{
		NewGame();
		
	}
	if (sym == SDLK_1)
	{
		slotOne = true;
		slotTwo = false;
		slotThree = false;
		slotFour = false;
	}
	if (sym == SDLK_2)
	{
		slotOne = false;
		slotTwo = true;
		slotThree = false;
		slotFour = false;

	}
	if (sym == SDLK_3)
	{
		slotOne = false;
		slotTwo = false;
		slotThree = true;
		slotFour = false;
	}
	if (sym == SDLK_4)
	{
		slotOne = false;
		slotTwo = false;
		slotThree = false;
		slotFour = true;
	}
	if (sym == SDLK_LSHIFT)
	{
		if (slotOne == true)
		{
			damage = 20;

			if (player.GetStatus() == STANDLEFT || player.GetStatus() == WALKLEFT)
			{
				player.SetAnimation("punchleft", 25);
				CSprite* projectile = new CSprite(player.GetX(), player.GetY(), "Grass_Block40.png", CColor::White(), GetTime());
				projectile->SetSize(1, 1);
				projectile->SetXVelocity(-1000);
				projectiles.push_back(projectile);
				playerHit.Play("impact2.wav");
			}
			if (player.GetStatus() == STANDRIGHT || player.GetStatus() == WALKRIGHT)
			{
				player.SetAnimation("punchright", 25);

				CSprite* projectile = new CSprite(player.GetX(), player.GetY(), "Grass_Block40.png", CColor::White(), GetTime());
				projectile->SetSize(1, 1);
				projectile->SetXVelocity(1000);
				projectiles.push_back(projectile);
				playerHit.Play("impact2.wav");
			}
		}
	}
}
	


void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
}


/////////////////////////////////////////////////////
// Mouse Events Handlers

void CMyGame::OnMouseMove(Uint16 x, Uint16 y, Sint16 relx, Sint16 rely, bool bLeft, bool bRight, bool bMiddle)
{

}

void CMyGame::OnLButtonDown(Uint16 x, Uint16 y)
{

	float gridPosX = mouseX / blockSize;//get the X mouse coord in pixels, then divide by the size of the block.
	int gridPosXR = ceil(gridPosX);//round the number given by the previous calculation to the ceiling to give the grid number for x.
	float gridPosY = mouseY / blockSize;//get the Y mouse coord in pixels, then divide by the size of the block.
	int gridPosYR = ceil(gridPosY);//round the number given by the previous calculation to the ceiling to give the grid number for Y.
	
	
	if (slotOne == true)
	{
		//DELETES THE BLOCK FROM SCREEN
		if (IsLButtonDown())//when clicking on the block, it will be removed.
		{
			for (CSprite* dirtBlock : dirt)
			{
				//mouse

				//block
				float dirtBlockgridPosX = (dirtBlock->GetX() - 20) / blockSize;//Get the block X pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int dirtBlockgridPosXR = ceil(dirtBlockgridPosX);//round the number given by the previous calculation to the ceiling to give the grid number for Block grid X.
				float dirtBlockgridPosY = (dirtBlock->GetY() - 20) / blockSize;//Get the block Y pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int dirtBlockgridPosYR = ceil(dirtBlockgridPosY);//round the number given by the previous calculation to the ceiling to give the grid number for Block grid Y.

				CVector mousePos;//creates a vector to store the Mouse X and Y positions.
				mousePos.SetX(gridPosXR);//setting the X grid position of the Mouse
				mousePos.SetY(gridPosYR);//setting the Y grid position of the Mouse
				CVector dirtBlockPos;//creates a vector to store the Block X and Y positions.
				dirtBlockPos.SetX(dirtBlockgridPosXR);//setting the X grid position of the Block
				dirtBlockPos.SetY(dirtBlockgridPosYR);//setting the Y grid position of the Block


				if (mousePos.GetX() == dirtBlockPos.GetX() && mousePos.GetY() == dirtBlockPos.GetY())//checking if the mouse grid position is the same as the block grid position;
				{
					invDirt.SetImage("dirt");
					dirtBlock->Delete();//Deleting the block if the grid positions are the same;
					int random = rand() % 7 + 1;
					dirtAmount +=random;
					mouseClicks++;
				}
			}
			for (CSprite* grassBlock : grass)
			{
				float grassBlockgridPosX = (grassBlock->GetX() - 20) / blockSize;//Get the block X pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int grassBlockgridPosXR = ceil(grassBlockgridPosX);//round the number given by the previous calculation to the ceiling to give the grid number for Block grid X.
				float grassBlockgridPosY = (grassBlock->GetY() - 20) / blockSize;//Get the block Y pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int grassBlockgridPosYR = ceil(grassBlockgridPosY);
				CVector mousePos;//creates a vector to store the Mouse X and Y positions.
				mousePos.SetX(gridPosXR);//setting the X grid position of the Mouse
				mousePos.SetY(gridPosYR);//setting the Y grid position of the Mouse
				CVector grassBlockPos;//creates a vector to store the Block X and Y positions.
				grassBlockPos.SetX(grassBlockgridPosXR);//setting the X grid position of the Block
				grassBlockPos.SetY(grassBlockgridPosYR);//setting the Y grid position of the Block
				if (mousePos.GetX() == grassBlockPos.GetX() && mousePos.GetY() == grassBlockPos.GetY())//checking if the mouse grid position is the same as the block grid position;
				{
					invGrass.SetImage("grass");
					grassBlock->Delete();//Deleting the block if the grid positions are the same;
					int random = rand() % 7 + 1;
					grassAmount +=random;
					mouseClicks++;
				}
			}
			for (CSprite* cobbleBlock : cobble)
			{
				float cobbleBlockgridPosX = (cobbleBlock->GetX() - 20) / blockSize;//Get the block X pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int cobbleBlockgridPosXR = ceil(cobbleBlockgridPosX);//round the number given by the previous calculation to the ceiling to give the grid number for Block grid X.
				float cobbleBlockgridPosY = (cobbleBlock->GetY() - 20) / blockSize;//Get the block Y pixel position, then take 20 of as we had to add 20 pixels when spawning them so that they did not overlap, and then divide by the size of the block.
				int cobbleBlockgridPosYR = ceil(cobbleBlockgridPosY);
				CVector mousePos;//creates a vector to store the Mouse X and Y positions.
				mousePos.SetX(gridPosXR);//setting the X grid position of the Mouse
				mousePos.SetY(gridPosYR);//setting the Y grid position of the Mouse
				CVector cobbleBlockPos;//creates a vector to store the Block X and Y positions.
				cobbleBlockPos.SetX(cobbleBlockgridPosXR);//setting the X grid position of the Block
				cobbleBlockPos.SetY(cobbleBlockgridPosYR);//setting the Y grid position of the Block
				if (mousePos.GetX() == cobbleBlockPos.GetX() && mousePos.GetY() == cobbleBlockPos.GetY())//checking if the mouse grid position is the same as the block grid position;
				{
					invCobble.SetImage("cobble");
					cobbleBlock->Delete();//Deleting the block if the grid positions are the same;
					cobbleAmount++;
					mouseClicks++;
				}
			}
		}
		dirt.remove_if(deleted);//removing the block from the list
		grass.remove_if(deleted);
		cobble.remove_if(deleted);
	}
	//SPAWNS BLOCKS
	if (slotTwo == true)
	{
		if (dirtAmount > 0)
		{
			int dirtSaved = dirtAmount;
			dirtAmount--;
			for (int i = 0; i <= 1; i++) {
				CSprite* dirtBlockSpawned = new CSprite(-20, -20, 0, 0, GetTime());
				dirtSpawned.push_back(dirtBlockSpawned);
				if (IsLButtonDown())
				{
					int roundedPosX = gridPosXR * 40;
					int	 roundedPosY = gridPosYR * 40;
					CVector combined;
					combined.SetX(roundedPosX);
					combined.SetY(roundedPosY);
					for (CSprite* dirtBlockSpawned : dirtSpawned)
					{
						if (roundedPosX == dirtBlockSpawned->GetX() && roundedPosY == dirtBlockSpawned->GetY())
						{
							dirtAmount = dirtSaved + 1;
							dirtBlockSpawned->Delete();
						}
					}
					dirtBlockSpawned = new CSprite(0, 0, 40, 40, "Dirt_Block64.png", GetTime());
					dirtBlockSpawned->SetX(roundedPosX + 20);
					dirtBlockSpawned->SetY(roundedPosY + 20);

					dirtSpawned.push_back(dirtBlockSpawned);
				}
			}
		}
		else { dirtAmount = 0; }
	}
	if (slotThree == true)
	{
		if (grassAmount > 0)
		{
			int grassSaved = grassAmount;
			grassAmount--;
			for (int i = 0; i <= 1; i++) {
				CSprite* grassBlockSpawned = new CSprite(-20, -20, 0, 0, GetTime());
				grassSpawned.push_back(grassBlockSpawned);
				if (IsLButtonDown())
				{
					int roundedPosX = gridPosXR * 40;
					int	 roundedPosY = gridPosYR * 40;
					CVector combined;
					combined.SetX(roundedPosX);
					combined.SetY(roundedPosY);
					for (CSprite* grassBlockSpawned : dirtSpawned)
					{
						if (roundedPosX == grassBlockSpawned->GetX() && roundedPosY == grassBlockSpawned->GetY())
						{
							grassAmount = grassSaved + 1;
							grassBlockSpawned->Delete();
						}
					}
					grassBlockSpawned = new CSprite(0, 0, 40, 40, "Grass_Block64.png", GetTime());
					grassBlockSpawned->SetX(roundedPosX + 20);
					grassBlockSpawned->SetY(roundedPosY + 20);
					grassSpawned.push_back(grassBlockSpawned);
				}
			}
		}
		else { grassAmount = 0; }
	}
	if (slotFour == true)
	{
		if (cobbleAmount > 0)
		{
			int cobbleSaved = cobbleAmount;
			cobbleAmount--;
			for (int i = 0; i <= 1; i++) {
				CSprite* cobbleBlockSpawned = new CSprite(-20, -20, 0, 0, GetTime());
				cobbleSpawned.push_back(cobbleBlockSpawned);
				if (IsLButtonDown())
				{
					int roundedPosX = gridPosXR * 40;
					int	 roundedPosY = gridPosYR * 40;
					CVector combined;
					combined.SetX(roundedPosX);
					combined.SetY(roundedPosY);
					for (CSprite* grassBlockSpawned : cobbleSpawned)
					{
						if (roundedPosX == cobbleBlockSpawned->GetX() && roundedPosY == cobbleBlockSpawned->GetY())
						{
							cobbleAmount= cobbleSaved +1;
							grassBlockSpawned->Delete();
						}

					}
					cobbleBlockSpawned = new CSprite(0, 0, 40, 40, "Cobble_Block64.png", GetTime());
					cobbleBlockSpawned->SetX(roundedPosX + 20);
					cobbleBlockSpawned->SetY(roundedPosY + 20);
					cobbleSpawned.push_back(cobbleBlockSpawned);
				}
			}
		}
		else { cobbleAmount = 0; }
	}
}

void CMyGame::OnLButtonUp(Uint16 x, Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x, Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x, Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x, Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x, Uint16 y)
{
}
