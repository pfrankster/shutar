#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "MiniRed.h"
#include "Shutar.h"
#include <math.h>
		 //Animacao
		 // Vetor com as animações da nave (3 no total)
		 Animacao animaMiniRed[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
			 { 1, 1, { 0 } },
			 { 1, 1, { 1 } }

};


// A função que carrega o Player
//
bool MiniRed_Carrega()
{
	return ATOR_CarregaAtorEstatico(MINIRED, "minired2.png", 64, 64, 0, 0, 64, 64, animaMiniRed, true, 0, 0, &MiniRed_Atualiza);
}

// A função para fazer a lógica do Tiro
//
// Data: 06/06/2015
// Última atualização: 04/07/2015

bool MiniRed_Atualiza(Ator *a, unsigned int idMapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, MINIRED_PARADO, false);
		break;

	case MINIRED_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			// coloca a animação da nave parada
			ATOR_TrocaAnimacao(a, 0);
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}

		a->velocidade = 0;

		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{

			case EVT_POSICAO:
				if (
					(unsigned int)(a->y - ev.y) < 250 && (unsigned int)(a->x - ev.x) < 250 ||

					(unsigned int)(a->y - ev.y) < 250 && (unsigned int)(a->x - ev.x) *-1 < 250 ||

					(unsigned int)(a->y - ev.y) *-1 < 250 && (unsigned int)(a->x - ev.x) *-1 < 250 ||

					(unsigned int)(a->y - ev.y) *-1 < 250 && (unsigned int)(a->x - ev.x) < 250
					)
				{
					ATOR_TrocaEstado(a, MINIRED_DESLOCANDO, false);
				}

				break;

				//EVENTOU COLISAO
			case EVT_COLIDIU_PERSONAGEM:
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;

			}//FIM SWITCH EVENTOS

		}

		break; //FIM ESTADO 

	case MINIRED_DESLOCANDO:
	{

		Evento ev;

		ATOR_TrocaAnimacao(a, 1);
	
		a->velocidade = 2.1f;


		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
				//EVENTOU COLISAO
			case EVT_COLIDIU_PERSONAGEM:
			{
				if (ev.subtipo = TIRO_NAVE)
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;
			}


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
					if (ev.y > a->y)
					{
						angulo = 360 - angulo;
					}
				}
				// Segundo e terceiro quadrantes
				else
				{
					// Terceiro quadrante
					if (ev.y > a->y)
					{
						angulo += 180;
					}
					// Segundo quadrante
					else
					{
						angulo = 180 - angulo;
					}
				}
				a->olhandoPara = angulo;
				a->direcao = a->olhandoPara;

				if (

					(unsigned int)(a->y - ev.y) > 250 && (unsigned int)(a->x - ev.x) > 250 ||

					(unsigned int)(a->y - ev.y) > 250 && (unsigned int)(a->x - ev.x) *-1 > 250 ||

					(unsigned int)(a->y - ev.y) *-1 > 250 && (unsigned int)(a->x - ev.x) *-1 > 250 ||

					(unsigned int)(a->y - ev.y) *-1 > 250 && (unsigned int)(a->x - ev.x) > 250

					)

				{
					ATOR_TrocaEstado(a, MINIRED_PARADO, false);
				}

				break;
			}




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
