#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "MiniShooter.h"
#include "Shutar.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
//Animacao
// Vetor com as animações da nave (3 no total)
Animacao animaMiniShooter[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	{ 1, 1, { 0 } }

};

// Estado do tiro
enum { TIRO_DESLOCANDO = ATOR_PROG_INI };
// A distância máxima que um tiro pode percorrer
#define MAX_DTIRO	600
// A velocidade de um tiro
#define VTIRO		5


// A função que carrega o Player
//
bool MiniShooter_Carrega()
{
	return ATOR_CarregaAtorEstatico(MINISHOOTER, "miniShooter.png", 48, 48, 0, 0, 48, 48, animaMiniShooter, true, 0, 0, &MiniShooter_Atualiza);
}

// A função para fazer a lógica do Tiro
//
// Data: 06/06/2015
// Última atualização: 04/07/2015

bool MiniShooter_Atualiza(Ator *a, unsigned int idMapa)
{

	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
	{	// Muda para o estado adequado
		ATOR_TrocaEstado(a, MINISHOOTER_PARADO, false);
		a->temporizadores[0] = 15;
		int v2 = rand() % 4;
		v2 < 1 ? v2 = 1 : v2 = v2; 
		//a->velocidade = 2.1f;
		a->velocidade = v2;

		break; 
	}

	case MINISHOOTER_PARADO:
	{
		Evento ev;
		if (a->estado.subestado == ESTADO_INICIO)
		{		// coloca a animação da nave parada
			ATOR_TrocaAnimacao(a, 0);
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
			//a->direcao = a->olhandoPara;

		}



		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_COLIDIU_PERSONAGEM:
				if (ev.subtipo = TIRO_NAVE)
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);

				break;

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
				a->direcao = a->olhandoPara;
				break;
			}
			case EVT_TEMPO:
				if (ev.subtipo == 0)
				{
					ev.tipoEvento = EVT_CRIA_PERSONAGEM;
					ev.subtipo = TIRO_INIMIGO;
					ev.x = a->x;
					ev.y = a->y;
					ev.valor = a->olhandoPara;
					ATOR_EnviaEventoJogo(&ev);
					
					a->temporizadores[0] = 120;
				}
				
				printf("\nMS ATIRANDO");
				break;
			

			}//fim switch
		}//FIM WHILE

		break;
	}

	case ATOR_ENCERRADO:
		return false;
		break; //ator encerrado
	}

	return true;
}