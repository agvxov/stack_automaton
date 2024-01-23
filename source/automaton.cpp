#include "automaton.hpp"

#include <stdlib.h>
#include <string.h>

using namespace std;

std::string to_string(const Rule &r) {
	return std::string() + "Rule{ translation: " + r.transition + ", irrel: " + r.irrel + " }";
}

namespace automaton {
	const char POP    = '\01';
	const char ACCEPT = '\02';

    stack<char> symbol_stack;
    const char * input_str;
	size_t ip;

	string irrelevant;

	void (*display_callback)(decltype(input_str),
							decltype(symbol_stack),
							decltype(irrelevant) ir,
                            const Rule &r) = NULL;

	map<char, map<char, Rule>> matrix;
}

bool automaton::run() {
    while (true) {
		// Preoperation calculations
		char n = input_str[ip];
		if (symbol_stack.empty()) {
            return false;
		}
        char m = symbol_stack.top();

        Rule r("");
        try {
             r = matrix.at(m)
                    .at(n);
        } catch(...) {
			return false;
		}

		// Render to display
		if (display_callback) {
			display_callback(input_str + ip, symbol_stack, irrelevant, r);
		}

		// Act on input
		symbol_stack.pop();
		irrelevant += r.irrel;

        if (r.transition == string("") + ACCEPT) {
            return true;
        } if (r.transition == string("") + POP) {
            if (not input_str[ip]) {
                return false;
            }
            ip++;
		} else {
			for (auto it = r.transition.rbegin(); it != r.transition.rend(); it++) {
				symbol_stack.push(*it);
			}
        }
    }
}

void automaton::reset(){
	free((char *)input_str);
	input_str = NULL;
    symbol_stack = stack<char>({'\0', 'E'});
	ip = 0;
	irrelevant = "";
    matrix.clear();
}
