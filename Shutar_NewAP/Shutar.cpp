#include "Shutar.h"

#include <c2d2\chien2d2.h>
#include <c2d2\chienaudio2.h>
#include <c2d2\chien2d2mapa.h>
#include <c2d2\chien2d2primitivas.h>
#include <c2d2\ator.h>

#include "AtorManager.h"
#include "Nave.h"
#include "Tiro.h"
#include "Bomba.h"
#include "TiroInimigo.h"

#include "RedBoss.h"
#include "MiniRed.h"
#include "MiniShooter.h"

#include "BatRobo.h"
#include "Ocorrencia.h"

unsigned int musicas[3];
unsigned int menu, logoPUC, jogorolando, mouseTX, hud_nergy ; //tx simples hub e afins

unsigned int mapa;

int mousePosX, mousePosY;

int energytank = 100;

int indO = 0;

int numOcorrencias = 0;  
int numinimigosBT = 0;
int numinimigosMR = 0;
int numinimigosSH = 0;
int numinimigosRB = 0;
int numTirosInimigos = 20; 


int fuel = 100; 
bool tocandomusica = false; 
bool shootOK;
bool BombaOK;

unsigned int shootType = 1; 


//Personagens do jogo
Ator *nave;
Ator *tiro;//tironave
Ator *bomba;

Ator **inimigos; //bats
Ator **inimigosMR;//minired
Ator **inimigosSH;//minired
Ator **ocorrencias;//alertas
Ator **inimigosRB; //redBoss
Ator **tirosInimigos;

Ator *redboss;
Ator *minired;
Ator *batrobo;
Ator *ocorrencia; 
Ator *miniredAgenger;




Shutar::Shutar()
{

	//carrega a chien
	if (C2D2_Inicia(LARGURA_TELA, ALTURA_TELA, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Shutar - GameProject TIJ"))
		printf("carregou a Chien");
	else
		printf("erro ao carregar a chien");

	//carrega audio chien
	CA2_Inicia();

	//inicia primitivas
	C2D2P_Inicia();
	
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


	//carrega sprites simples
	logoPUC = C2D2_CarregaSpriteSet("splashprojeto.png", 0, 0);
	menu = C2D2_CarregaSpriteSet("screenmenu.jpg", 0, 0);
	jogorolando = C2D2_CarregaSpriteSet("jogorolando.png", 0, 0);
	mouseTX = C2D2_CarregaSpriteSet("mira.png", 24, 24);
	hud_nergy = C2D2_CarregaSpriteSet("hud_energy.png", 120, 24);


	//carrega atores do jogo
	bool cnave = Nave_Carrega(); 
	bool ctiro = Tiro_Carrega();
	bool cbomba = Bomba_Carrega();
	bool credboss = RedBoss_Carrega();
	bool cminired = MiniRed_Carrega();
	bool cbatrobo = BatRobo_Carrega();
	bool cocorrencia = Ocorrencia_Carrega();
	

//inimigo MiniShooter
	bool cminishooter = MiniShooter_Carrega();
	bool ctiroInimigo = TiroInimigo_Carrega();



	//carrega mapa
	mapa = C2D2M_CarregaMapaMappy("mapa5.FMP", "sheetstar01.png");
	
	int numcamadas = 4;
	//define a marca inical dos tiles programados da chien
	C2D2M_CamadaMarcas(mapa, 3, 89);

	C2D2M_VelocidadeCamadaMapa(mapa, numcamadas - 1, 1);
	// Faz um for esotérico para atribuir as velocidades. Se pra você facilitar, use uma camada só que não dá nada
	for (int i = 0, vel = numcamadas - 1; i<numcamadas - 1; i++, vel--)
		C2D2M_VelocidadeCamadaMapa(mapa, i, vel);

	int tpX = 0, tpY = 0; //apenas referencia para coord serem trazidas  

	//PROCURA SABER NUMERO TOTAL DE INIMIGOS TIPO BAT 
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO_BT, &tpX, &tpY))
	{
		numinimigosBT++;
		while (C2D2M_ProximoBlocoMarca(mapa, &tpX, &tpY))
			numinimigosBT++;
	}

	tpX = 0;
	tpY = 0;

	//PROCURA SABER NUMERO TOTAL DE INIMIGOS TIPO MINIRED
	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGOS_MINIRED, &tpX, &tpY))
	{
		numinimigosMR++;
		while (C2D2M_ProximoBlocoMarca(mapa, &tpX, &tpY))
			numinimigosMR++;
	}


	tpX = 0;
	tpY = 0;

	//PROCURA SABER NUMERO TOTAL DE INIMIGOS TIPO MINISHOOTER
	if (C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_FIM, &tpX, &tpY))
	{
		numinimigosSH++;
		while (C2D2M_ProximoBlocoMarca(mapa, &tpX, &tpY))
			numinimigosSH++;
	}



	tpX = 0;
	tpY = 0;

	if (C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_CHECK, &tpX, &tpY))
	{
		numOcorrencias++;
		while (C2D2M_ProximoBlocoMarca(mapa, &tpX, &tpY))
			numOcorrencias++;
	}

	tpX = 0;
	tpY = 0;

	if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_REDBOSS, &tpX, &tpY))
	{
		numinimigosRB++;
		while (C2D2M_ProximoBlocoMarca(mapa, &tpX, &tpY))
			numinimigosRB++;
	}




	if (cnave)
	{
		int xinit = 0, yinit = 0;
		// cria o personagem na posicao escolhida dentro do mapa com a tag INICIO
		C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_INICIO, &xinit, &yinit); 
		nave = ATOR_CriaAtor(NAVE, xinit, yinit, 0);
		//nave = ATOR_CriaAtor(NAVE, xinit, yinit, 0);
	}
	
	if (ctiro)
	{

		shootOK = false;
	}

	if (cbomba)
	{

		BombaOK = false;
	}


	if (credboss)
	{

		inimigosRB = (Ator**)malloc(sizeof(Ator*)*numinimigosRB);
		memset(inimigosRB, 0, numinimigosRB*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		int indO = 0;

		if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_REDBOSS, &xini, &yini))
		{
			inimigosRB[indO++] = ATOR_CriaAtor(REDBOSS, xini, yini, 0);
			while (C2D2M_ProximoBlocoMarca(mapa, &xini, &yini))
				inimigosRB[indO++] = ATOR_CriaAtor(REDBOSS, xini, yini, 0);
		}
	}
		//redboss = ATOR_CriaAtor(REDBOSS, -5, 5, 0);


	if (ctiroInimigo)
	{

		tirosInimigos = (Ator**)malloc(sizeof(Ator*)*numTirosInimigos);
		memset(tirosInimigos, 0, numTirosInimigos*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		int indO = 0;

		tirosInimigos[indO++] = 0;
		while (indO < numTirosInimigos)
			tirosInimigos[indO++] = 0;

	}

	if (cminishooter)
	{


		inimigosSH = (Ator**)malloc(sizeof(Ator*)*numinimigosSH);
		memset(inimigosSH, 0, numinimigosSH*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		int indO = 0;

		if (C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_FIM, &xini, &yini))
		{
			inimigosSH[indO++] = ATOR_CriaAtor(MINISHOOTER, xini, yini, 0);
			while (C2D2M_ProximoBlocoMarca(mapa, &xini, &yini))
				inimigosSH[indO++] = ATOR_CriaAtor(MINISHOOTER, xini, yini, 0);
		}
	}

	if (cminired)
	{

		inimigosMR = (Ator**)malloc(sizeof(Ator*)*numinimigosMR);
		memset(inimigosMR, 0, numinimigosMR*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		/*int indO = 0;*/

		if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGOS_MINIRED, &xini, &yini))
		{
			inimigosMR[indO++] = ATOR_CriaAtor(MINIRED, xini, yini, 0);
			while (C2D2M_ProximoBlocoMarca(mapa, &xini, &yini))
				inimigosMR[indO++] = ATOR_CriaAtor(MINIRED, xini, yini, 0);
		}
	}
	

	if (cbatrobo)
	{

		inimigos = (Ator**)malloc(sizeof(Ator*)*numinimigosBT);
		memset(inimigos, 0, numinimigosBT*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		int indO = 0;

		if (C2D2M_PrimeiroBlocoMarca(mapa, MARCA_INIMIGO_BT, &xini, &yini))
		{
			inimigos[indO++] = ATOR_CriaAtor(BATROBO, xini, yini, 0);
			while (C2D2M_ProximoBlocoMarca(mapa, &xini, &yini))
				inimigos[indO++] = ATOR_CriaAtor(BATROBO, xini, yini, 0);
		}

	}

	if (cocorrencia)
	{
		ocorrencias = (Ator**)malloc(sizeof(Ator*)*numOcorrencias);
		memset(ocorrencias, 0, numOcorrencias*sizeof(Ator*));

		//posicao temporaria para referencia
		int xini, yini = 0;
		int indO = 0;

		if (C2D2M_PrimeiroBlocoMarca(mapa, C2D2M_CHECK, &xini, &yini))
		{
			ocorrencias[indO++] = ATOR_CriaAtor(OCORRENCIA, xini, yini, 0);
			while (C2D2M_ProximoBlocoMarca(mapa, &xini, &yini))
				ocorrencias[indO++] = ATOR_CriaAtor(OCORRENCIA, xini, yini, 0);
		}

	}


}


void Shutar::Update(int gamestate)
{
	//AQUI FICAM AS REGRAS DO JOGO
	C2D2_Botao* teclas = C2D2_PegaTeclas();
	C2D2_Mouse* mouse = C2D2_PegaMouse();

	if (gamestate == 1)
	{
		if (teclas[C2D2_ENTER].pressionado){ GameState = 2; CA2_FadeMusica(2); tocandomusica = false; }
		if (teclas[C2D2_C].pressionado){ GameState = 3; CA2_FadeMusica(2); tocandomusica = false; }

		if (teclas[C2D2_ESC].pressionado){ GameState = 10; }
	}

	if (gamestate == 3)
	{
		if (teclas[C2D2_ESC].pressionado){ GameState = 1; }
	}


	if (gamestate == 2)
	{
		if (teclas[C2D2_ESC].pressionado){ GameState = 1; tocandomusica = false; }


		mousePosX = mouse->x;
		mousePosY = mouse->y;

		//REPASSA A POSICAO DA NAVE PARA TODOS OS INIMIGOS *-*-*-*-*-*
		int posXNave, posYNave;
		posXNave = nave->x;
		posYNave = nave->y;

		for (int i = 0; i < numinimigosSH; i++)
		{
			Evento evt;
			evt.tipoEvento = EVT_POSICAO;
			evt.x = posXNave;
			evt.y = posYNave;
			ATOR_EnviaEvento(inimigosSH[i], &evt);
			//printf("passou coordenada");
		}


			for (int i = 0; i < numinimigosMR; i++)
			{
				Evento evt;
				evt.tipoEvento = EVT_POSICAO;
				evt.x = posXNave;
				evt.y = posYNave;
				ATOR_EnviaEvento(inimigosMR[i], &evt);
			//printf("passou coordenada");
			}

			for (int i = 0; i < numinimigosBT; i++)
			{
				Evento evt;
				evt.tipoEvento = EVT_POSICAO;
				evt.x = posXNave;
				evt.y = posYNave;
				ATOR_EnviaEvento(inimigos[i], &evt);
				//printf("passou coordenada");
			}


			for (int i = 0; i < numinimigosRB; i++)
			{
				Evento evt;
				evt.tipoEvento = EVT_POSICAO;
				evt.x = posXNave;
				evt.y = posYNave;
				ATOR_EnviaEvento(inimigosRB[i], &evt);
				//printf("passou coordenada");
			}




		 //-*-*--*-*-*-*-*-*-*-*-*-*-*-*-


		//controle jogador

			Nave_ProcessaControle(nave);

		//atualiza nave 
			ATOR_AplicaEstado(nave, mapa, LARGURA_TELA, ALTURA_TELA);
			Nave_Atualiza(nave, mapa);


			// atualiza  as inimigos RB
			for (int i = 0; i < numinimigosRB; i++)
		{	
				ATOR_AplicaEstado(inimigosRB[i], mapa, LARGURA_TELA, ALTURA_TELA);
				// Aplica o estado da propulsao
				ATOR_Atualiza(inimigosRB[i], mapa);
			}

			// atualiza  as inimigos 
			for (int i = 0; i < numinimigosBT; i++)
			{
				ATOR_AplicaEstado(inimigos[i], mapa, LARGURA_TELA, ALTURA_TELA);
				ATOR_Atualiza(inimigos[i], mapa);
			}

			// atualiza  as inimigos MSHOOTER
			for (int i = 0; i < numinimigosSH; i++)
			{
				ATOR_AplicaEstado(inimigosSH[i], mapa, LARGURA_TELA, ALTURA_TELA);
				ATOR_Atualiza(inimigosSH[i], mapa);
			}



			for (int i = 0; i < numTirosInimigos; i++){
				if (tirosInimigos[i] != 0)
				{
					ATOR_AplicaEstado(tirosInimigos[i], mapa, LARGURA_TELA, ALTURA_TELA);
					ATOR_Atualiza(tirosInimigos[i], mapa);
				}
				

					if (!ATOR_Atualiza(tirosInimigos[i], mapa))
					{
						free(tirosInimigos[i]);
						tirosInimigos[i] = 0;
					}
			}


			// atualiza  as inimigos 
			for (int i = 0; i < numinimigosMR; i++)
			{
				ATOR_AplicaEstado(inimigosMR[i], mapa, LARGURA_TELA, ALTURA_TELA);
				// Aplica o estado da propulsao
				ATOR_Atualiza(inimigosMR[i], mapa);
			}


			// atualiza  as ocorrencias 
			for (int i = 0; i < numOcorrencias; i++)
			{
				ATOR_AplicaEstado(ocorrencias[i], mapa, LARGURA_TELA, ALTURA_TELA);
				// Aplica o estado da propulsao
				ATOR_Atualiza(ocorrencias[i], mapa);
			}


			//atualiza bomba 
			if (bomba != 0)
			{
				ATOR_AplicaEstado(bomba, 1, LARGURA_TELA, ALTURA_TELA);
				Tiro_Atualiza(bomba, 1);

			}
			if (!ATOR_Atualiza(bomba, 1))
			{
				free(bomba);
				bomba = 0; 

			}



			CollisionHandler(); 

			
		//eventos diretos da chien para o jogo 
			Evento ev;

			while (ATOR_ProximoEventoJogo(&ev))
			{
				switch (ev.tipoEvento)
				{

				case	EVT_PRESSIONOU_BAIXO:
					shootOK = false;
					tiro = 0;
					free(tiro);
					break;

				case	EVT_PRESSIONOU_CIMA:
					BombaOK = false;
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
								ATOR_TocaEfeitoTela(nave, 0, mapa);
								shootOK = true;
							}

							break;

						case BOMBA:
							// Se o tiro é nulo, pode criar um novo
							if (!BombaOK)
							{
								printf("BOMBA!\n");
								bomba = ATOR_CriaAtor(BOMBA, ev.x, ev.y, ev.valor);
								ATOR_TocaEfeitoTela(nave, 2, mapa);
								BombaOK = true;
							}

							break;


						case REDBOSS:
							// Se o tiro é nulo, pode criar um novo
							if (redboss == 0)
								redboss = ATOR_CriaAtor(REDBOSS, ev.x, ev.y, ev.valor);
							break;

						case MINIRED:
							// Se o tiro é nulo, pode criar um novo
							printf("\ngerador recebeu a info e vai gerar um MiniRED");
							miniredAgenger = ATOR_CriaAtor(MINIRED, ev.x, ev.y, ev.valor);
					
							break;

						case TIRO_INIMIGO:
						{
							// Se o tiro é nulo, pode criar um novo


							for (int i = 0; i < numTirosInimigos; i++)
							{
								if (tirosInimigos[i] == 0)
								{
									printf("atirou!\n");
										tirosInimigos[i] = ATOR_CriaAtor(TIRO_INIMIGO, ev.x, ev.y, ev.valor);
									ATOR_TocaEfeitoTela(nave, 0, mapa);
									break;

								}
							} //fim for

						}


						}
						break;
					}
			}
	} //fim gamestate 2


}




void Shutar::Draw()
{
	C2D2_LimpaTela(); 

	switch (GameState)
	{
	case 1:
		//Desenha Splash
		C2D2_DesenhaSprite(menu, 0, 0, 0); 

		
		break;

	case 2:
	{//Desenha GAME
		//C2D2_DesenhaSprite(jogorolando, 0, 0, 0);

		C2D2M_DesenhaCamadaMapa(mapa, 0, 0, 0, LARGURA_TELA, ALTURA_TELA); //desenha estrelas
		C2D2M_DesenhaCamadaMapa(mapa, 1, 0, 0, LARGURA_TELA, ALTURA_TELA);//desenha planetas
		C2D2M_DesenhaCamadaMapa(mapa, 2, 0, 0, LARGURA_TELA, ALTURA_TELA);//desenha nuvens





		ATOR_CentraMapa(nave, mapa, LARGURA_TELA, ALTURA_TELA);


		// Aplica as ocorrencias 
		for (int i = 0; i < numOcorrencias; i++)
		{
			ATOR_Desenha(ocorrencias[i], mapa, 0, 0);

		}

		// Aplica as inimigos 
		for (int i = 0; i < numinimigosBT; i++)
		{
			ATOR_Desenha(inimigos[i], mapa, 0, 0);
		}

		// Aplica as inimigos RB
		for (int i = 0; i < numinimigosRB; i++)
		{
			ATOR_Desenha(inimigosRB[i], mapa, 0, 0);

		}

		// Aplica as inimigos MR
		for (int i = 0; i < numinimigosMR; i++)
		{
			ATOR_Desenha(inimigosMR[i], mapa, 0, 0);

		}

		// Aplica as inimigos SH
		for (int i = 0; i < numinimigosSH; i++)
		{
			ATOR_Desenha(inimigosSH[i], mapa, 0, 0);

		}


		// Aplica as inimigos SH
		for (int i = 0; i < numinimigosSH; i++)
		{
			if (tirosInimigos[i] != 0)
				ATOR_Desenha(tirosInimigos[i], mapa, 0, 0);
		}



		if (tiro)
			ATOR_Desenha(tiro, mapa, 0, 0);

		if (bomba)
			ATOR_Desenha(bomba, mapa, 0, 0);

		ATOR_Desenha(nave, mapa, 0, 0);

		C2D2_DesenhaSprite(mouseTX, 0, mousePosX, mousePosY);

		C2D2_DesenhaSprite(hud_nergy, 0, LARGURA_TELA - 140, ALTURA_TELA - 34);

		//HUD 
		int coordXbarra = 894;
		int coordYbarra = 556;
		
		//int ent = coordXbarra + energytank;
		
		int ent = coordXbarra + nave->aux_int[3]/10;
		//static_cast<int>(coordXbarra)

		C2D2P_RetanguloPintado(coordXbarra, coordYbarra, ent, coordYbarra + 8, 0, 255, 0);




		break; 
	}
	case 3:
		//Desenha Splash
		C2D2_DesenhaSprite(logoPUC, 0, 0, 0);


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
				CA2_AjustaVolume(25, 100);

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


void Shutar::CollisionHandler(){

	//trata das colisoes do tiro
	if (shootOK)
	{
		ATOR_AplicaEstado(tiro, mapa, LARGURA_TELA, ALTURA_TELA);
		Tiro_Atualiza(tiro, mapa);


		//checa colisao com bats 
		for (int i = 0; i < numinimigosBT; i++)
		{
			if (ATOR_ColidiuAtores(tiro, inimigos[i]))
			{
				Evento ev;
				ATOR_EnviaEvento(inimigos[i], &ev);
				printf("\nacertou um bat");
			}

		}

		//checa colisao com MiniShooter
		for (int i = 0; i < numinimigosSH; i++)
		{
			if (ATOR_ColidiuAtores(tiro, inimigosSH[i]))
			{
				Evento ev;
				ATOR_EnviaEvento(inimigosSH[i], &ev);
				printf("\nacertou um mshooter");
			}

		}

		//checa colisao com MiniShooter
		for (int i = 0; i < numinimigosMR; i++)
		{
			if (ATOR_ColidiuAtores(tiro, inimigosMR[i]))
			{
				Evento ev;
				ev.subtipo = TIRO_NAVE;
				ATOR_EnviaEvento(inimigosMR[i], &ev);
				printf("\nacertou um miniRed");
			}

		}




		//checa colisao tiro reboss 
		for (int i = 0; i < numinimigosRB; i++)
		{
			if (ATOR_ColidiuAtores(tiro, inimigosRB[i]))
			{
				Evento ev;
				ATOR_EnviaEvento(inimigosRB[i], &ev);
				printf("\nacertou um redboss");
			}

		}

	}//fim if tiro


	//CONFERE SE NAVE ESTA SOB AREAS DE REPARO
	for (int i = 0; i < numOcorrencias; i++)
	{
		if (ATOR_ColidiuAtores(nave, ocorrencias[i]))
		{
			Evento ev;
			nave->aux_int[3] += 150; 
		}
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
