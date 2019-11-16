#define DOCTEST_CONFIG_IMPLEMENT
#include "debug.h"

#include "Program.h"

int main(int argc, char** argv) {
	sf::RenderWindow window{sf::VideoMode(1600, 900), "zemianadrezh"};
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
	if (res != 0) {
		return res;
	}

	//After all tests are successfully run, we'll actually launch the Program
	Program program(window);
	return 0;
}
