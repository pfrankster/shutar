#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "RedBoss.h"
#include "Shutar.h"
#include <math.h>
//Animacao
// Vetor com as animações da nave (3 no total)
Animacao animaRedBoss[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros
	{ 8, 12, { 0, 1, 2, 3, 4, 3, 2, 1 } }

};



// A função que carrega o Player
//
bool RedBoss_Carrega()
{
	return ATOR_CarregaAtorEstatico(REDBOSS, "sheet_redboss.png", 384, 480, 96, 48, 288, 190, animaRedBoss, true, 0, 0, &RedBoss_Atualiza);
}

// A função para fazer a lógica do Tiro
//
// Data: 06/06/2015
// Última atualização: 04/07/2015

bool RedBoss_Atualiza(Ator *a, unsigned int idMapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	//  **-*-*-*-*-*-*-*-   NASCEU 
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, REDBOSS_PARADO, false);
		a->velocidade = 0.2f;
		
		break;

	case REDBOSS_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_TrocaAnimacao(a, 0);
			a->estado.subestado = ESTADO_RODANDO;
		}


		while (ATOR_ProximoEvento(a, &ev))

		{
			// se for um evento de movimentação
			switch (ev.tipoEvento)
			{


			case EVT_COLIDIU_PERSONAGEM:
				if (ev.subtipo == TIRO_NAVE)
				{
					if(a->vidas > 0)
						a->vidas--;
					else
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
				}
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

					//a->olhandoPara = angulo;
					a->direcao = angulo;

					break;
			}


			}//fim switch
		} //fim while

		break;


	case ATOR_ENCERRADO:
		return false;
		break;
	}

	return true;
}
