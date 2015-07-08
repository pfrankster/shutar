#ifndef TIRO_INIMIGO_H
#define TIRO_INIMIGO_H

#include <c2d2/ator.h>

#define VTIRO	5


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	TIRO_INIMIGO_DESLOCANDO = ATOR_PROG_INI
};

// A função que carrega o personagem atriz (pra facilitar).
//
bool TiroInimigo_Carrega();
// A função para fazer a lógica da atriz
bool TiroInimigo_Atualiza(Ator *a, unsigned int mapa);
//void Tiro_ProcessaControle(Ator *a);

#endif
