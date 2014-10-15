/****************************************************************************
 Ryan Frappier
 Copyright
 ****************************************************************************/

#include "Program.h"

#define BOOST_ALL_NO_LIB

int main(int argc, char* argv[])
{
	Program game(640, 620, "Galaxian");
	
	game.execute();
    
	return 0;	
}
