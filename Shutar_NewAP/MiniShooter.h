#ifndef MINISHOOTER_H
#define MINISHOOTER_H

#include <c2d2/ator.h>

#define VELOCIDADEBOSS 0.5f


//ALGUMAS PROPRIEDADES DA NAVE



// Enumeração dos estados da atriz do jogo
enum {
	MINISHOOTER_PARADO = ATOR_PROG_INI
};

// A função que carrega o personagem atriz (pra facilitar).
//
bool MiniShooter_Carrega();
// A função para fazer a lógica da atriz
bool MiniShooter_Atualiza(Ator *a, unsigned int mapa);
// A função para fazer o ator atirar
bool MiniShooter_Atira(Ator *a, unsigned int idMapa);


#endif