#include "Shutar.h"

#include <c2d2\chien2d2.h>
#include <c2d2\chienaudio2.h>
#include <c2d2\ator.h>

#include "AtorManager.h"
#include "Nave.h"

#define LARGURA_TELA 1024
#define ALTURA_TELA 578


unsigned int musicas[3];
unsigned int logoPUC, jogorolando;

bool tocandomusica = false; 


//Personagens do jogo
Ator *nave;
Ator *tiro;
Ator *prop;
Ator **inimigos;
Ator **ocorrencias;
Ator *redboss;


Shutar::Shutar()
{

	//carrega a chien
	if (C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Shutar - GameProject TIJ"))
		printf("carregou a Chien");
	else
		printf("erro ao carregar a chien");

	//carrega audio chien
	CA2_Inicia();

	//inicia atores da chien
	ATOR_Inicia(); 
}

void Shutar::Setup()
{
	//musicas do jogo
	musicas[0] = CA2_CarregaMusica("");
	musicas[1] = CA2_CarregaMusica("splashloop.wav");
	musicas[2] = CA2_CarregaMusica("bgloop.wav");


	//carrega sprites
	logoPUC = C2D2_CarregaSpriteSet("splashprojeto.png", 0, 0);
	jogorolando = C2D2_CarregaSpriteSet("jogorolando.png", 0, 0);

	//carrega atores do jogo

	bool cnave = Nave_Carrega(); 
		

	if (cnave)
	{
		int xinit = 0, yinit = 0;
		// cria o personagem
		//--->>>>>>>>>>>>>>>>>>>>>>>> C2D2M_PrimeiroBlocoMarca(0, C2D2M_INICIO, &xinit, &yinit); 
		//--->nave = ATOR_CriaAtor(NAVE, xinit, yinit, 0);
		
		nave = ATOR_CriaAtor(NAVE, 0, xinit, yinit);

		
	}
	

}


void Shutar::Update(int gamestate)
{
	C2D2_Botao* teclas = C2D2_PegaTeclas();
	C2D2_Mouse* mouse = C2D2_PegaMouse();

	if (gamestate == 1)
	{
		if (teclas[C2D2_ENTER].pressionado){ GameState = 2; CA2_FadeMusica(2); tocandomusica = false; }
		if (teclas[C2D2_ESC].pressionado){ GameState = 10; }
	}


	if (gamestate == 2)
	{
		if (teclas[C2D2_ESC].pressionado){ GameState = 1; tocandomusica = false; }
		Nave_ProcessaControle(nave);
			ATOR_AplicaEstado(nave, 0, LARGURA_TELA, ALTURA_TELA);
			Nave_Atualiza(nave, 0);
	}


}




void Shutar::Draw()
{
	C2D2_LimpaTela(); 

	switch (GameState)
	{
	case 1:
		//Desenha Splash
		C2D2_DesenhaSprite(logoPUC, 0, 0, 0); 

		
		break;
	case 2:
		//Desenha Splash
		C2D2_DesenhaSprite(jogorolando, 0, 0, 0);
		ATOR_Desenha(nave, 0, LARGURA_TELA / 2, ALTURA_TELA / 2);


		break;
	case 3:
		//Desenha Splash


		break;
	case 4:
		//Desenha Splash

		break;

	}

	C2D2_Sincroniza(C2D2_FPS_PADRAO);
}

void Shutar::Run()
{
	Setup();
	GameLoop();
	Dispose(); 
}

void Shutar::GameLoop()
{

	//Tudo que for iniciado precisar ser encerrado ... JANELA ATORES SONS ETC 
	while (GameState < 4)
	{
		//se musica esta tocando não toca de novo
		if (!tocandomusica)
		{
			switch (GameState)
			{
			case 1:
				CA2_AjustaVolume(15,-1);
				CA2_TocaMusica(GameState, -1);
				tocandomusica = true;
				break;
			case 2:
				CA2_AjustaVolume(50, 100);

				CA2_TocaMusica(GameState, 100);
				tocandomusica = true;
				break;
			case 3:
				CA2_AjustaVolume(30, 100);
				CA2_TocaMusica(GameState, -1);
				tocandomusica = true;
				break;
			}

		}


		Update(GameState);
		Draw();

		
	}
}


void Shutar::Dispose()
{
	//Tudo que for iniciado precisar ser encerrado ... JANELA ATORES SONS ETC 
	CA2_Encerra();
	ATOR_Encerra();
	C2D2_Encerra();
}


Shutar::~Shutar()
{
	Dispose();
}
