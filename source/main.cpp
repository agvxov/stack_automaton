#include "automaton.hpp"
#include "automaton_gui.h"

#include <QApplication>

using namespace std;
using namespace automaton;

AutomatonGUI * g;

void print_state(decltype(input_str) is,
					decltype(symbol_stack) ss,
					decltype(irrelevant) ir,
                    const Rule &r) {
	string ss_ = "";
    while (not ss.empty()) {
        if (ss.top() == '\0') {
            ss_ += '#';
        } else {
            ss_ += ss.top();
        }
		ss.pop();
	}

    char * state;
    char * transition;

#if 0
    asprintf(&state, "\033[33;1m('%s', '%s', '%s')\033[0m\n",
			is,
			ss_.c_str(),
			ir.c_str()
		);
    asprintf(&transition, "\033[32;1m[%c,%c]\033[0m -> \033[36;1m(%s,%s)\033[0m\n",
			*input_str,
			symbol_stack.top(),
			r.transition.c_str(),
			r.irrel.c_str()
        );
#endif
    asprintf(&state, "('%s', '%s', '%s')",
            is,
            ss_.c_str(),
            ir.c_str()
        );
    asprintf(&transition, "[%c,%c] -> (%s,%s)",
            *input_str,
            symbol_stack.top() ? symbol_stack.top() : '#',
            r.transition.c_str(),
            r.irrel.c_str()
        );

    fputs(state, stdout);
    fputs(transition, stdout);
    g->display_state(state);
    g->display_state(transition);

    free(state);
    free(transition);

	for (auto it = ss_.rbegin(); it != ss_.rend(); it++) {
        if (*it == '#') {
            ss.push('\0');
        } else {
            ss.push(*it);
        }
	}
}

int main(int argc, char *argv[]) {
	// load
    reset();

#if 0
    input_str = strdup("i+i*(i+i)");
    matrix =
        #include "table.inc"
    ;
#endif

#if 0
    input_str = strdup("ii");
    matrix =
        #include "small.inc"
    ;
#endif

    display_callback = print_state;

	// IoC
    QApplication a(argc, argv);
    AutomatonGUI w;
    g = &w;
    w.show();
    return a.exec();
}
