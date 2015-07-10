#ifndef NAVE_H
#define NAVE_H

#include <c2d2/ator.h>

#define VNAVE	6

//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	NAVE_PARADA = ATOR_PROG_INI, 
	NAVE_DESLOCANDO,
	NAVE_ESTABILIZANDO
};

// A função que carrega o personagem atriz (pra facilitar).
//
bool Nave_Carrega();
// A função para fazer a lógica da atriz
bool Nave_Atualiza(Ator *a, unsigned int mapa);
void Nave_ProcessaControle(Ator *a); 

#endif
