#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "BatRobo.h"
#include "Shutar.h"
#include <math.h>
//Animacao
// Vetor com as anima��es da nave (3 no total)
Animacao animaBatRobo[] = {
	// Ordem: n�mero de quadros, tempo entre os quadros, vetor com a seq��ncia de quadros
	{ 1, 1, { 0 } }

};


// A fun��o que carrega o Player
//
bool BatRobo_Carrega()
{
	return ATOR_CarregaAtorEstatico(BATROBO, "batwing2.png", 48, 48, 0, 0, 48, 48, animaBatRobo, true, 0, 0, &BatRobo_Atualiza);
}
// A fun��o para fazer a l�gica do Tiro
//
// Data: 06/06/2015
// �ltima atualiza��o: 04/07/2015

bool BatRobo_Atualiza(Ator *a, unsigned int idMapa)
{

	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, BATROBO_PARADO, false);
		// Usa valores auxiliares para guardar a posi��o inicial do tiro
		a->aux_real[0] = a->x;
		a->aux_real[1] = a->y;
		a->velocidade = 0.2f; 
		break;

	case BATROBO_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			// coloca a anima��o da nave parada
			ATOR_TrocaAnimacao(a, 0);
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}
		

			//a->olhandoPara = a->olhandoPara%360 +50;
		//a->olhandoPara = a->direcao;
			//a->temporizadores[0] = 100;

			while (ATOR_ProximoEvento(a, &ev))
			{
				switch (ev.tipoEvento)
				{
				case EVT_COLIDIU_PERSONAGEM:
					//if (ev.subtipo = TIRO_NAVE)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					break;

				case EVT_POSICAO:
				{


					// Muda a dira��o na qual o personagem est� olhando
					// Calcula o cateto adjacente e oposto
					double ca = fabs(a->x - ev.x);
					double co = fabs(a->y - ev.y);
					double angulo = 90;
					// Se o cateto oposto � zero, o angulo � 90�
					// Sen�o, calcula
					if (ca != 0)
						angulo = atan(co / ca)*RAD_ANG;
					// Ajusta o quadrante
					// Primeiro e quarto quadrantes
					if (ev.x>a->x)
					{
						// Est� no quarto?
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



				}
			} //fim while eventos
			
		break;



	case ATOR_ENCERRADO:
		return false;
		break;
	}

	return true;
}
