#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include "Program.h"

/*
ISSUES:
    -Weird behaviour when resizing (ex. the title text gets all messed up,
    and also bad things happen when resizing on the pause screen)
*/

int main(int argc, char** argv) {
	Program program;

	//doctest related setup
	//For more information, see:
	//https://github.com/onqtam/doctest/blob/master/doc/markdown/main.md
	//------------------------------------------------------------
	doctest::Context context;
    context.applyCommandLine(argc, argv);

    //don't break in the debugger when assertions fail
    context.setOption("no-breaks", true);
    int res = context.run();
    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

	//your program - if the testing framework is integrated in your production code
    int client_stuff_return_code = 0;

    //the result from doctest is propagated here as well
    return res + client_stuff_return_code;
}
