#pragma once



class Shutar
{
	/*Estados Possíveis 
		1 - Splash
		2 - Menu
		3 - Game
		4 - GameOver

	*/

public:
	Shutar();
	~Shutar();
	void Run();
	int GameState = 2;

	void GameLoop();
	void Setup();
	void Update(int gamestate);
	void Draw();
	void Dispose();

};

