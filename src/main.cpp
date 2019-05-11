#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "Program.h"

/*
ISSUES:
    -Weird behaviour when resizing (ex. the title text gets all messed up,
    and also bad things happen when resizing on the pause screen)
*/

int main(int argc, char** argv) {
	sf::RenderWindow window{sf::VideoMode(800, 600), "zemianadrezh"};
	window.setFramerateLimit(144);
	//doctest related setup
	//For more information, see:
	//https://github.com/onqtam/doctest/blob/master/doc/markdown/main.md
	//------------------------------------------------------------
	doctest::Context context;
    context.applyCommandLine(argc, argv);

    //don't break in the debugger when assertions fail
    context.setOption("no-breaks", true);
    int res = context.run();
    if(res != 0) {
		return res;
    }

	//After all tests are successfully run, we'll actually launch the Program
	Program program(window);
	return 0;
}
