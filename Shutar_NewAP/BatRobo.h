#ifndef BATROBO_H
#define BATROBO_H

#include <c2d2/ator.h>

#define VELOCIDADEBOSS 0.5f


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	BATROBO_PARADO = ATOR_PROG_INI, BATROBO_DESLOCANDO
};

// A função que carrega o personagem atriz (pra facilitar).
//
bool BatRobo_Carrega();
// A função para fazer a lógica da atriz
bool BatRobo_Atualiza(Ator *a, unsigned int mapa);


#endif
