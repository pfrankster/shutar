#include <c2d2/chien2d2.h>

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
	{ 7, 2, { 8, 9, 10, 11, 10, 9, 8 } },

};

char *sons[] = { "laser1.ogg", "propulsao.ogg" };


// A função que carrega o Player
//
bool Nave_Carrega()
{
	return ATOR_CarregaAtorEstatico(NAVE, "nave_sheet.png", 96, 96, 5, 5, 90, 90, animaNave, true, sons, 2, &Nave_Atualiza);
}

// A função para fazer a lógica do DarkPhoenix
//
// Data: 04/09/2008
// Última atualização: 04/09/2008
//
bool Nave_Atualiza(Ator *a, unsigned int mapa)
{

	Evento ev;
	switch (a->estado.estado)
	{

	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, NAVE_PARADA, false);
		// Muda o temporizador para criar o tiro
		// A escolha da posição 0 no vetor é arbitrária
		a->temporizadores[0] = 1;
		break;
	case NAVE_PARADA:
		a->velocidade = VNAVE - 1;
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

				break;

			case EVT_LIBEROU_BOTAO2:
				a->velocidade = 0;
				break;


			case EVT_PRESSIONOU_BOTAO3:

				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = TIRO_NAVE;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);


				break;

				/************************************************************************/
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

				/*	case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

				break;*/

				/*
				// se pressionou para cima
				case EVT_LIBEROU_CIMA:
				// Se só estava indo para cima, pára
				if (a->direcao == 90)
				{
				// Indica a velocidade
				a->velocidade = 0;
				// Muda o estado
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				}
				// Se estava indo em diagonal, fica só na horizontal
				else if (a->direcao == 45)
				a->direcao = a->olhandoPara = 0;
				else if (a->direcao == 135)
				a->direcao = a->olhandoPara = 180;
				break;
				// se pressionou para cima
				case EVT_LIBEROU_ESQ:
				if (a->direcao == 180)
				{
				// Indica a velocidade
				a->velocidade = 0;
				// Muda o estado
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				}
				// Se estava indo em diagonal, fica só na vertical
				else if (a->direcao == 225)
				a->direcao = a->olhandoPara = 270;
				else if (a->direcao == 135)
				a->direcao = a->olhandoPara = 90;
				break;
				// se pressionou para cima
				case EVT_LIBEROU_DIR:
				if (a->direcao == 0)
				{
				// Indica a velocidade
				a->velocidade = 0;
				// Muda o estado
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				}
				// Se estava indo em diagonal, fica só na vertical
				else if (a->direcao == 45)
				a->direcao = a->olhandoPara = 90;
				else if (a->direcao == 315)
				a->direcao = a->olhandoPara = 270;
				break;
				// se pressionou para cima
				case EVT_LIBEROU_BAIXO:
				if (a->direcao == 270)
				{
				a->velocidade = 0;
				// Muda o estado
				ATOR_TrocaEstado(a, NAVE_PARADA, false);
				}
				// Se estava indo em diagonal, fica só na horizontal
				else if (a->direcao == 225)
				a->direcao = a->olhandoPara = 180;
				else if (a->direcao == 315)
				a->direcao = a->olhandoPara = 0;
				break;
				// Se pressiona alguma outra direção, pode entrar em diagonal
				case EVT_PRESSIONOU_CIMA:
				// Se está indo na horizontal, pode ir em diagonal
				if (a->direcao == 0)
				a->direcao = a->olhandoPara = 45;
				else if (a->direcao == 180)
				a->direcao = a->olhandoPara = 135;
				break;
				case EVT_PRESSIONOU_BAIXO:
				// Se está indo na horizontal, pode ir em diagonal
				if (a->direcao == 0)
				a->direcao = a->olhandoPara = 315;
				else if (a->direcao == 180)
				a->direcao = a->olhandoPara = 225;
				break;
				case EVT_PRESSIONOU_ESQ:
				// Se está indo na vertical, pode ir em diagonal
				if (a->direcao == 90)
				a->direcao = a->olhandoPara = 135;
				else if (a->direcao == 270)
				a->direcao = a->olhandoPara = 225;
				break;
				case EVT_PRESSIONOU_DIR:
				// Se está indo na vertical, pode ir em diagonal
				if (a->direcao == 90)
				a->direcao = a->olhandoPara = 45;
				else if (a->direcao == 270)
				a->direcao = a->olhandoPara = 315;
				break;

				*/
				/*------------------*/

			case EVT_PRESSIONOU_BOTAO3:


				ev.tipoEvento = EVT_CRIA_PERSONAGEM;
				ev.subtipo = TIRO_NAVE;

				ev.x = a->x;
				ev.y = a->y;

				ev.valor = (int)a->olhandoPara;
				ATOR_EnviaEventoJogo(&ev);
				//a->temporizadores[0] = 30;
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

				break;


			case EVT_LIBEROU_BOTAO1:
				//a->velocidade = 0; 
				ATOR_TrocaEstado(a, NAVE_PARADA, false);

				//a->velocidade = 0; 

				break;


			case EVT_PRESSIONOU_BOTAO2:
				a->velocidade = 0;
				ATOR_TrocaEstado(a, NAVE_PARADA, false);

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
