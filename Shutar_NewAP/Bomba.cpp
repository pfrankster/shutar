#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "Bomba.h"
#include "Shutar.h"
#include <math.h>
//Animacao
// Vetor com as animações da nave (3 no total)
Animacao animaBomba[] = {
	// Ordem: número de quadros, tempo entre os quadros, vetor com a seqüência de quadros

	{ 8, 8, { 0,1,2,3,4,5,6,7 } }

};



// A função que carrega o Player
//
bool Bomba_Carrega()
{
	return ATOR_CarregaAtorEstatico(BOMBA, "bomba.png", 48,48, 0, 0, 48,48, animaBomba, false, 0, 0, &Bomba_Atualiza);

}

// A função para fazer a lógica do Tiro
//
// Data: 06/06/2015
// Última atualização: 04/07/2015


bool Bomba_Atualiza(Ator *a, unsigned int idMapa)
{
	Evento ev;
	switch (a->estado.estado)
	{
	case ATOR_NASCENDO:
		// Muda para o estado adequado
		ATOR_TrocaEstado(a, BOMBA_DESLOCANDO, false);
		// Usa valores auxiliares para guardar a posição inicial do tiro


		a->aux_real[0] = a->x;
		a->aux_real[1] = a->y;

		// Dá a velocidade e muda a direção em que está olhando
		a->velocidade = VBOMBA;

		a->olhandoPara = a->direcao;
		break;


	case BOMBA_DESLOCANDO:
	{
		if (a->estado.subestado == ESTADO_INICIO)
		{
			ATOR_TrocaAnimacao(a, 0);

			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
			// O tiro existe por 1 segundo
			a->temporizadores[0] = 180;
		}
		// Enquanto houverem eventos
		while (ATOR_ProximoEvento(a, &ev))
		{
			switch (ev.tipoEvento)
			{


			case EVT_TEMPO:
				if (ev.subtipo == 0)
				{
					// Se passou a distância, encerra o ator
					ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					return false;
				}
				break;


			}//fuim switch
		}//fim while

		break;
	}

	case ATOR_ENCERRADO:
		ev.tipoEvento = EVT_PRESSIONOU_CIMA;
		ATOR_EnviaEventoJogo(&ev);
		return false;
	}
	return true;
}


