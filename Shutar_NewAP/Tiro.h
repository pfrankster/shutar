#ifndef TIRO_H
#define TIRO_H

#include <c2d2/ator.h>

#define VTIRO	21


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	TIRO_DESLOCANDO = ATOR_PROG_INI
	};

// A função que carrega o personagem atriz (pra facilitar).
//
bool Tiro_Carrega();
// A função para fazer a lógica da atriz
bool Tiro_Atualiza(Ator *a, unsigned int mapa);
//void Tiro_ProcessaControle(Ator *a);

#endif
