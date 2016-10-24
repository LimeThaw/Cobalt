#ifndef INDENT_H_INCLUDED
#define INDENT_H_INCLUDED

#include <string>

class indent {
	public:
		static const std::string get() { return get_instance()->ret(); }
		static void increase() { get_instance()->inc(); }
		static void decrease() { get_instance()->dec(); }
	private:
		static indent* get_instance() { if(instance == nullptr) instance = new indent(); return instance; }
		indent() { indentation = ""; }
		const std::string &ret() { return indentation; }
		void inc() { indentation += "  "; }
		void dec() { indentation = indentation.substr(0, indentation.size() - 2); }
		static indent* instance;
		std::string indentation;
};

#endif