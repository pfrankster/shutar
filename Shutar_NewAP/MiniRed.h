#ifndef MINIRED_H
#define MINIRED_H

#include <c2d2/ator.h>

#define VELOCIDADEBOSS 0.5f


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	MINIRED_PARADO = ATOR_PROG_INI, MINIRED_DESLOCANDO
};

// A função que carrega o personagem atriz (pra facilitar).
//
bool MiniRed_Carrega();
// A função para fazer a lógica da atriz
bool MiniRed_Atualiza(Ator *a, unsigned int mapa);


#endif
