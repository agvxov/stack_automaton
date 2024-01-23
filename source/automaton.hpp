#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include <string.h>
#include <string>
#include <map>
#include <stack>

namespace automaton {
    extern const char POP;
    extern const char ACCEPT;
}

class Rule {
public:
	std::string transition;
	std::string irrel = "";

	Rule(const char c) {
		this->transition = strndup(&c, 1);
	}

    Rule(const char * const s) {
        if (not strcmp(s, "ACCEPT")) {
            this->transition = automaton::ACCEPT;
            this->irrel      = "";
            return;
        } else if (not strcmp(s, "POP")) {
            this->transition = automaton::POP;
            this->irrel      = "";
            return;
        }

		const char * split = strchr(s, ',');
		if (not split) {
			this->transition = s;
			return;
		}

		int i = split - s - 1;
		i < 0 ? i = 0 : 0;
		this->transition = std::string(s + 1, i);
		this->irrel      = std::string(split + 1);
		this->irrel      = this->irrel.substr(0, this->irrel.size()-1);
	}


	explicit operator bool() const {
		return not
			(transition == ""
			&& irrel    == "");
	}
};

extern std::string to_string(const Rule &r);

namespace automaton {
	extern std::stack<char> symbol_stack;
	extern const char * input_str;
	extern size_t ip;
    
	extern std::string irrelevant;

	extern void (*display_callback)(decltype(input_str),
									decltype(symbol_stack),
									decltype(irrelevant) ir,
									const Rule &r);

	extern std::map<char, std::map<char, Rule>> matrix;

	extern bool run();
	extern void reset();
}

#endif
