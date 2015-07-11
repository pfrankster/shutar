#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "Nave.h"
#include <math.h>
//Animacao
// Vetor com as animações da nave (3 no total)
Animacao animaNave[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	// Vetor com a nave L1 parada
	{ 1, 1, { 0 } },
	// Vetor com a nave L1 andandobot
	{ 5, 3, { 1, 2, 3, 2, 1 } },
	// Vetor com a nave L2 PARADA
	{ 1, 1, { 4 } },
	// Vetor com a nave L2 DESLOCANDO
	{ 5, 3, { 5, 6, 7, 6, 5 } },
	// Vetor com a nave L2 ESTABILIZANDO
	{ 12, 16, { 8, 9, 4, 10, 8, 4, 11, 9, 10, 4, 9, 8 } }

};

char *sons[] = { "laser1.ogg", "propulsao.ogg", "bombtimer.ogg" };


// A função que carrega o Player
//
bool Nave_Carrega()
{
	return ATOR_CarregaAtorEstatico(NAVE, "nave_sheet.png", 96, 96, 5, 5, 90, 90, animaNave, true, sons, 3, &Nave_Atualiza);
}

// A função para fazer a lógica da Nave
//
// Data: 06/06/2015
// Última atualização: 04/07/2015
//
bool Nave_Atualiza(Ator *a, unsigned int mapa)
{

	Evento ev;
	switch (a->estado.estado)
	{

	case ATOR_ENCERRADO:
		Evento ev; 
		ev.tipoEvento = SUBEVT_FIM_FASE_DERROTA;
		ATOR_EnviaEventoJogo(&ev); 
		return false; 
	break; 

	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, NAVE_PARADA, false);
		// Muda o temporizador para criar o tiro
		// A escolha da posição 0 no vetor é arbitrária
		
		a->aux_int[3] = 1000; 
		a->vidas = 10; 
		
		a->temporizadores[0] = 1; //??
		break;

	case NAVE_PARADA:
		a->velocidade = VNAVE *0.5f;;
		if (a->aux_int[3] < 1000)
		a->aux_int[3]++;
		
		if (a->estado.subestado == ESTADO_INICIO)
		{

			// coloca a animação da nave parada
			ATOR_TrocaAnimacao(a, 2);
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}
		// Pega os eventos e processa
		while (ATOR_ProximoEvento(a, &ev))
		{
			// se for um evento de movimentação
			switch (ev.tipoEvento)
			{

			case EVT_COLIDIU_PERSONAGEM:
			{
				//se vida chegou ao fim encerra o ator
				if (a->vidas <= 0)
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);


				if (ev.subtipo == TIRO_INIMIGO)
				{

					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

					//ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					a->vidas--;


				}

				if (ev.subtipo == MINIRED || ev.subtipo == BATROBO || ev.subtipo == MINISHOOTER || ev.subtipo == REDBOSS)
				{
					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

					a->vidas = a->vidas - 3;


				}



			}

			if (a->vidas <= 0)
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;


				// Caso tenha movido o mouse
			case EVT_POSICAO:
			{
				// Muda a diração na qual o personagem está olhando
				// Calcula o cateto adjacente e oposto
				double ca = fabs(a->x - ev.x);
				double co = fabs(a->y - ev.y);
				double angulo = 90;
				// Se o cateto oposto é zero, o angulo é 90º
				// Senão, calcula
				if (ca != 0)
					angulo = atan(co / ca)*RAD_ANG;
				// Ajusta o quadrante
				// Primeiro e quarto quadrantes
				if (ev.x>a->x)
				{
					// Está no quarto?
					if (ev.y>a->y)
						angulo = 360 - angulo;
				}
				// Segundo e terceiro quadrantes
				else
				{
					// Terceiro quadrante
					if (ev.y>a->y)
						angulo += 180;
					// Segundo quadrante
					else
						angulo = 180 - angulo;
				}
				a->olhandoPara = angulo;


				break;
			}
			/***************************************/
			case EVT_PRESSIONOU_BOTAO1:
				a->direcao = a->olhandoPara;
				//a->velocidade = VNAVE;
				// Muda o estado
					ATOR_TrocaEstado(a, NAVE_DESLOCANDO, false);

				break;

			case EVT_PRESSIONOU_BOTAO2:
				a->velocidade = 0;
				ATOR_TrocaEstado(a, NAVE_ESTABILIZANDO, false);

				break;

			case EVT_LIBEROU_BOTAO2:
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				break;


			case EVT_PRESSIONOU_BOTAO3:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = TIRO_NAVE;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);
    
				a->aux_int[3] -= 50; //reduz energia ao atirar 

				break;

			case EVT_PRESSIONOU_BOTAO4:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = BOMBA;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);

				a->aux_int[3] -= 150; //reduz energia ao atirar 

				break;



				/**********************ACHOU CHECKPOINTS **************************************************/
			case EVT_CHECKPOINT:

				/*ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = REDBOSS;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);*/
				break;
				/************************************************************************/
			}
		}

		break;

	case NAVE_ESTABILIZANDO:
			// coloca a animação da nave estabilizando
			ATOR_TrocaAnimacao(a, 4);


		while (ATOR_ProximoEvento(a, &ev))
		{
			// se for um evento de movimentação
			switch (ev.tipoEvento)
			{

			case EVT_COLIDIU_PERSONAGEM:
			{
				if (a->vidas <= 0)
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);


				if (ev.subtipo == TIRO_INIMIGO)
				{
					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

					//ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					a->vidas--;


				}

				if (ev.subtipo == MINIRED || ev.subtipo == BATROBO || ev.subtipo == MINISHOOTER || ev.subtipo == REDBOSS)
				{
					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

					a->vidas = a->vidas - 3;


				}


				//se vida chegou ao fim encerra o ator
			}
			if (a->vidas <= 0)
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

				break;

				// Caso tenha movido o mouse
			case EVT_POSICAO:
			{
				// Muda a diração na qual o personagem está olhando
				// Calcula o cateto adjacente e oposto
				double ca = fabs(a->x - ev.x);
				double co = fabs(a->y - ev.y);
				double angulo = 90;
				// Se o cateto oposto é zero, o angulo é 90º
				// Senão, calcula
				if (ca != 0)
					angulo = atan(co / ca)*RAD_ANG;
				// Ajusta o quadrante
				// Primeiro e quarto quadrantes
				if (ev.x>a->x)
				{
					// Está no quarto?
					if (ev.y>a->y)
						angulo = 360 - angulo;
				}
				// Segundo e terceiro quadrantes
				else
				{
					// Terceiro quadrante
					if (ev.y>a->y)
						angulo += 180;
					// Segundo quadrante
					else
						angulo = 180 - angulo;
				}
				a->olhandoPara = angulo;


				break;
			}
			/***************************************/
			case EVT_PRESSIONOU_BOTAO1:
				a->direcao = a->olhandoPara;
				//a->velocidade = VNAVE;
				// Muda o estado
				ATOR_TrocaEstado(a, NAVE_DESLOCANDO, false);
				if (a->aux_int[3] > 0)
					a->aux_int[3]--;

				break;

			case EVT_PRESSIONOU_BOTAO2:
				a->velocidade = 0;
				ATOR_TrocaEstado(a, NAVE_ESTABILIZANDO, false);
				a->aux_int[3] -= .5f;
				break;

			case EVT_LIBEROU_BOTAO2:
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				break;


			case EVT_PRESSIONOU_BOTAO3:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = TIRO_NAVE;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);

				a->aux_int[3] -= 50;
				break;

			case EVT_PRESSIONOU_BOTAO4:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = BOMBA;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);

				a->aux_int[3] -= 150; //reduz energia ao atirar 

				break;


				/**********************ACHOU CHECKPOINTS **************************************************/
			case EVT_CHECKPOINT:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = REDBOSS;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);
				break;
				/************************************************************************/
			}
		}

		break;


	case NAVE_DESLOCANDO:

		if (a->estado.subestado == ESTADO_INICIO)
		{
			// coloca a animação da nave parada
			ATOR_TrocaAnimacao(a, 3);
			ATOR_TocaEfeitoTela(a, 1, mapa);

			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}
		// Pega os eventos e processa
		while (ATOR_ProximoEvento(a, &ev))
		{
			// se for um evento de movimentação
			switch (ev.tipoEvento)
			{

			case EVT_COLIDIU_PERSONAGEM:
			{
				if (a->vidas <= 0)
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);


				if (ev.subtipo == TIRO_INIMIGO)
				{
					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

					//ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					a->vidas--;


				}

				if (ev.subtipo == MINIRED || ev.subtipo == BATROBO || ev.subtipo == MINISHOOTER || ev.subtipo == REDBOSS)
				{
					if (a->vidas <= 0)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);


					a->vidas = a->vidas - 3;


				}


			//se vida chegou ao fim encerra o ator
			}
			if (a->vidas <= 0)
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;



			case EVT_PRESSIONOU_BOTAO3:
				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = TIRO_NAVE;
				ev.x = a->x;
				ev.y = a->y;
				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);

				a->aux_int[3] -= 50;
				//a->temporizadores[0] = 30;
				break;

			case EVT_PRESSIONOU_BOTAO4:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = BOMBA;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);

				a->aux_int[3] -= 150; //reduz energia ao atirar 

				break;



				/***********************************************************************************************/
			case EVT_CHECKPOINT:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = REDBOSS;
				ev.x = a->x;
				ev.y = a->y;
				ev.valor = (int)a->olhandoPara;

				ATOR_EnviaEventoJogo(&ev);

				break;

				/**********************************************************************************************/

			case EVT_PRESSIONOU_BOTAO1:

				a->direcao = a->olhandoPara;
				a->velocidade = VNAVE;
				if (a->aux_int[3] > 0)
					a->aux_int[3]--;

				break;


			case EVT_LIBEROU_BOTAO1:
				//a->velocidade = 0; 
				ATOR_TrocaEstado(a, NAVE_PARADA, false);

				//a->velocidade = 0; 

				break;


			case EVT_PRESSIONOU_BOTAO2:
				a->velocidade = 0;
				ATOR_TrocaEstado(a, NAVE_ESTABILIZANDO, false);

				break;

			case EVT_LIBEROU_BOTAO2:

				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				break;

				/*------------------*/

				
				case EVT_PERDE_FOCO_TELA:
				
					break;
				

				// Caso tenha movido o mouse
			case EVT_POSICAO:
			{
				// Muda a diração na qual o personagem está olhando
				// Calcula o cateto adjacente e oposto 
				double ca = fabs(a->x - ev.x);
				double co = fabs(a->y - ev.y);
				double angulo = 90;
				// Se o cateto oposto é zero, o angulo é 90º
				// Senão, calcula
				if (ca != 0)
					angulo = atan(co / ca)*RAD_ANG;
				// Ajusta o quadrante
				// Primeiro e quarto quadrantes
				if (ev.x>a->x)
				{
					// Está no quarto?
					if (ev.y>a->y)
						angulo = 360 - angulo;
				}
				// Segundo e terceiro quadrantes
				else
				{
					// Terceiro quadrante
					if (ev.y>a->y)
						angulo += 180;
					// Segundo quadrante
					else
						angulo = 180 - angulo;
				}
				a->olhandoPara = angulo;
				break;
			}

			//TENTAR COLOCAR COMBUSTIVEL NO TEMPO A SER REDUZIDO E COLETADO
			case EVT_TEMPO:
				if (ev.subtipo == 0)
				{
					//
				}
				break;

			}
		}

	}

	
	return true;
}


void Nave_ProcessaControle(Ator *a)
{
	int VTIRO = 3;
	int tiroType = 1;

	static Evento ev;


	C2D2_Botao *teclado = C2D2_PegaTeclas();
	C2D2_Mouse *mouse = C2D2_PegaMouse();

	
	if (teclado[C2D2_1].pressionado)
	{
		tiroType = 1; //substituir
		VTIRO = 6; //substituir 
	}
	if (teclado[C2D2_2].pressionado)
	{
		tiroType = 2;
		VTIRO = 12;
	}
	if (teclado[C2D2_3].pressionado)
	{
		tiroType = 3;
		VTIRO = 16;
	}
	if (teclado[C2D2_4].pressionado)
	{
		tiroType = 4;
		VTIRO = 30;
	}


	if (teclado[C2D2_ESPACO].ativo)
	{
		if (a->aux_int[3] > 100 )
		{
			ev.tipoEvento = EVT_PRESSIONOU_BOTAO1;
			ATOR_EnviaEvento(a, &ev);
		}
		else{
			ev.tipoEvento = EVT_LIBEROU_BOTAO1;
			ATOR_EnviaEvento(a, &ev);

		}

	}

	if (teclado[C2D2_ESPACO].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BOTAO1;
		ATOR_EnviaEvento(a, &ev);
	}



	//GERA TIRO 
	if (mouse->botoes[0].pressionado)
	{
		if (a->aux_int[3] > 200)
		{
			ev.tipoEvento = EVT_PRESSIONOU_BOTAO3;
			ATOR_EnviaEvento(a, &ev);
		}
	}



	//GERA BOMBA
	//if (mouse->botoes[1].pressionado)
		if (teclado[C2D2_B].pressionado)
		{
		if (a->aux_int[3] > 200)
		{
			ev.tipoEvento = EVT_PRESSIONOU_BOTAO4;
			ATOR_EnviaEvento(a, &ev);
		}
	}


	/*freio nave */
	if (mouse->botoes[1].pressionado)
	//if (teclado[C2D2_LSHIFT].pressionado)


	{
		ev.tipoEvento = EVT_PRESSIONOU_BOTAO2;
		ATOR_EnviaEvento(a, &ev);

	}

	if (mouse->botoes[1].liberado)
	//if (teclado[C2D2_LSHIFT].liberado)
	{
		ev.tipoEvento = EVT_LIBEROU_BOTAO2;
		ATOR_EnviaEvento(a, &ev);
		
	}



	// Manda a posição do mouse
	int x1 = 0;
	int y1 = 0;
	C2D2M_PosicaoXY(1, &x1, &y1);
	ev.tipoEvento = EVT_POSICAO;
	ev.x = x1 + mouse->x + 12;
	ev.y = y1 + mouse->y + 12;
	ATOR_EnviaEvento(a, &ev);



}

