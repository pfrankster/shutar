#include <c2d2/chien2d2.h>
#include <c2d2\chien2d2mapa.h>
#include "AtorManager.h"
#include "BatRobo.h"
#include "Shutar.h"
#include <math.h>
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
		break;

	case BATROBO_PARADO:
		if (a->estado.subestado == ESTADO_INICIO)
		{
			// coloca a animação da nave parada
			ATOR_TrocaAnimacao(a, 0);
			// Troca o sub-estado
			a->estado.subestado = ESTADO_RODANDO;
		}

			//a->olhandoPara = a->olhandoPara%360 +50;
		a->olhandoPara = a->direcao;
			//a->temporizadores[0] = 100;

			while (ATOR_ProximoEvento(a, &ev))
			{
				switch (ev.tipoEvento)
				{
				case EVT_COLIDIU_PERSONAGEM:
					//if (ev.subtipo = TIRO_NAVE)
						ATOR_TrocaEstado(a, ATOR_ENCERRADO, false);
					break;
				}
			} //fim while eventos
			
		break;



	case ATOR_ENCERRADO:
		return false;
		break;
	}

	return true;
}
