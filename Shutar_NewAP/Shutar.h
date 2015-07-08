#pragma once


#define LARGURA_TELA 1024
#define ALTURA_TELA 578


class Shutar
{

public:
	Shutar();
	~Shutar();
	void Run();

	void GameLoop();
	void Setup();
	void Update(int gamestate);
	void Draw();
	void Dispose();
	void CollisionHandler();

	int GameState = 1; 	/*Estados Possíveis  	1 - Splash  		2 - Menu		3 - Game		4 - GameOver	*/


};

