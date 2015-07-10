#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "BatRobo.h"
#include "Shutar.h"
#include <math.h>
#include <stdlib.h>
//Animacao
// Vetor com as animações da nave (3 no total)
Animacao animaBatRobo[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	{ 1, 1, { 0 } }

};


// A função que carrega o Player
//
bool BatRobo_Carrega()
{
	return ATOR_CarregaAtorEstatico(BATROBO, "batwing2.png", 48, 48, 0, 0, 48, 48, animaBatRobo, true, 0, 0, &BatRobo_Atualiza);
}
// A função para fazer a lógica do Tiro
//
// Data: 06/06/2015
// Última atualização: 04/07/2015

bool BatRobo_Atualiza(Ator *a, unsigned int idMapa)
{

	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, BATROBO_PARADO, false);
		// Usa valores auxiliares para guardar a posição inicial do tiro
		a->aux_real[0] = a->x;
		a->aux_real[1] = a->y;
		a->velocidade = 0; 
		a->temporizadores[0] = 15;

		break;

	case BATROBO_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			// coloca a animação da nave parada
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}
		ATOR_TrocaAnimacao(a, 0);
		a->velocidade = 0; 

		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{
			case EVT_COLIDIU_PERSONAGEM:
				//if (ev.subtipo = TIRO_NAVE)
				ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				break;


			case EVT_TEMPO:
			{	if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, BATROBO_DESLOCANDO, false);
				int v1 = rand() % 120;
				//a->temporizadores[0] = 60;
				a->temporizadores[0] = v1;

				break;
			}
			}
		}


		break; 


	case BATROBO_DESLOCANDO:
		
		Evento ev;

			while (ATOR_ProximoEvento(a, &ev))
			{
				switch (ev.tipoEvento)
				{
				case EVT_COLIDIU_PERSONAGEM:
					//if (ev.subtipo = TIRO_NAVE)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					break;


				case EVT_TEMPO:
				{	if (ev.subtipo == 0)
					ATOR_TrocaEstado(a, BATROBO_PARADO, false);
				/*a->temporizadores[0] = 135;*/
				int v1 = rand() % 210;
				a->temporizadores[0] = v1;

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
					//a->olhandoPara = angulo;
					//a->direcao = a->olhandoPara;


					a->olhandoPara = ++a->olhandoPara%360 +36;
					a->direcao = angulo;
					a->velocidade = 4.1f;

					break;
				}



				}
			} //fim while eventos
			
		break;



	case ATOR_ENCERRADO:
		return false;
		break;
	}

	return true;
}
