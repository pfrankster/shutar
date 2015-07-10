#ifndef BOMBA_H
#define BOMBA_H

#include <c2d2/ator.h>

#define VBOMBA	1


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	BOMBA_DESLOCANDO = ATOR_PROG_INI
	};

// A função que carrega o personagem atriz (pra facilitar).
//
bool Bomba_Carrega();
// A função para fazer a lógica da atriz
bool Bomba_Atualiza(Ator *a, unsigned int mapa);
//void Bomba_ProcessaControle(Ator *a);

#endif
