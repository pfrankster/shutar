#include "Shutar.h"
#include <stdlib.h>
#include  <time.h>
using namespace std;


int main(int nargs, char* vargs[])
{
	srand(time(NULL));

	Shutar game;
	game.Run();
	
		
	return 0;
}
