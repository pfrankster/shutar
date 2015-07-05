#include "Shutar.h"

#include <c2d2\chien2d2.h>
#include <c2d2\chienaudio2.h>
#include <c2d2\chien2d2mapa.h>
#include <c2d2\ator.h>

#include "AtorManager.h"
#include "Nave.h"
#include "Tiro.h"

unsigned int musicas[3];
unsigned int logoPUC, jogorolando, mouseTX;

unsigned int mapa;

int mousePosX, mousePosY;

bool tocandomusica = false; 
bool shootOK;
unsigned int shootType = 1; 

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

	//inicia sistema de mapa da Chien
	C2D2M_Inicia(); 
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
	mouseTX = C2D2_CarregaSpriteSet("mira.png", 24, 24);

	//carrega atores do jogo
	bool cnave = Nave_Carrega(); 
	bool ctiro = Tiro_Carrega();

	//carrega mapa
	mapa = C2D2M_CarregaMapaMappy("mapa4.FMP", "sheetstar01.png");
	
	int numcamadas = 4;
	//define a marca inical dos tiles programados da chien
	C2D2M_CamadaMarcas(mapa, 3, 89);

	C2D2M_VelocidadeCamadaMapa(mapa, numcamadas - 1, 1);
	// Faz um for esotérico para atribuir as velocidades. Se pra você facilitar, use uma camada só que não dá nada
	for (int i = 0, vel = numcamadas - 1; i<numcamadas - 1; i++, vel--)
		C2D2M_VelocidadeCamadaMapa(mapa, i, vel);

		

	if (cnave)
	{
		int xinit = 0, yinit = 0;
		// cria o personagem
		C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_INICIO, &xinit, &yinit); 
		nave = ATOR_CriaAtor(NAVE, xinit, yinit, 0);
		//nave = ATOR_CriaAtor(NAVE, xinit, yinit, 0);
	}
	
	if (ctiro)
		shootOK = false; 

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

		
		mousePosX = mouse->x;
		mousePosY = mouse->y;

		Nave_ProcessaControle(nave);
			ATOR_AplicaEstado(nave, mapa, LARGURA_TELA, ALTURA_TELA);
			Nave_Atualiza(nave, mapa);


			if (shootOK)
			{
				ATOR_AplicaEstado(tiro, mapa, LARGURA_TELA, ALTURA_TELA);
				Tiro_Atualiza(tiro, mapa);
			}


		//eventos diretos da chien para o jogo 
			Evento ev;

			while (ATOR_ProximoEventoJogo(&ev))
			{
				switch (ev.tipoEvento)
				{

				case	EVT_PRESSIONOU_BAIXO:
					shootOK = false;
					break;

				case EVT_CRIA_PERSONAGEM:
					switch (ev.subtipo)
					{
					case TIRO_NAVE:
						// Se o tiro é nulo, pode criar um novo
						if (!shootOK)
						{
							printf("atirou!\n");
							tiro = ATOR_CriaAtor(TIRO_NAVE, ev.x, ev.y, ev.valor);
							//tiro = ATOR_CriaAtor(TIRO_NAVE, 0, 0, ev.valor);
							ATOR_TocaEfeitoTela(nave, 0, mapa);
							shootOK = true;
						}

						break;

					case REDBOSS:
						// Se o tiro é nulo, pode criar um novo
						if (redboss == 0)
							redboss = ATOR_CriaAtor(REDBOSS, ev.x, ev.y, ev.valor);
						break;


					}
					break;
				}
			}
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
		//C2D2_DesenhaSprite(jogorolando, 0, 0, 0);

		C2D2M_DesenhaCamadaMapa(mapa, 0, 0, 0, LARGURA_TELA, ALTURA_TELA); //desenha estrelas
		C2D2M_DesenhaCamadaMapa(mapa, 1, 0, 0, LARGURA_TELA, ALTURA_TELA);//desenha planetas
		C2D2M_DesenhaCamadaMapa(mapa, 2, 0, 0, LARGURA_TELA, ALTURA_TELA);//desenha nuvens


		
		ATOR_CentraMapa(nave, mapa, LARGURA_TELA, ALTURA_TELA);
		
		ATOR_Desenha(nave, mapa, 0, 0);
		if (shootOK)
			ATOR_Desenha(tiro, mapa, 0, 0);


		C2D2_DesenhaSprite(mouseTX, 0, mousePosX, mousePosY);
		break;
	case 3:
		//Desenha Menu Interno


		break;
	case 4:
		//Desenha GameOver

		break;

	}

	
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
				CA2_AjustaVolume(25,-1);
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

		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	}
}


void Shutar::Dispose()
{
	//Tudo que for iniciado precisar ser encerrado ... JANELA ATORES SONS ETC 
	ATOR_DescarregaAtor(NAVE);
	ATOR_DescarregaAtor(TIRO_NAVE);

	CA2_Encerra(); //audio
	ATOR_Encerra();//atores
	C2D2M_Encerra(); //mapas
	C2D2_Encerra();//chien
}


Shutar::~Shutar()
{
	Dispose();
}
