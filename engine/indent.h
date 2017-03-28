#ifndef INDENT_H_INCLUDED
#define INDENT_H_INCLUDED

#include <string>

/**
 *  A little helper class used to prettify output. It carries the state of the current indentation,
 *  which helps to make the runtime output way clearer and more informative. Increase the indent when
 *  entering a seperate section and decrease it when exiting it. To ensure nice output over the
 *  whole program this is a global, static singleton object.
 */
class indent {
	public:
		static const std::string get() { return get_instance()->ret(); } ///< Returns the current indentation as a string. Can be conveniently printed before the output.
		static void increase() { get_instance()->inc(); } ///< Decreases the global indentation level
		static void decrease() { get_instance()->dec(); } ///< Increases the global indentation level
	private:
		static indent* get_instance() { if(instance == nullptr) instance = new indent(); return instance; }
		indent() { indentation = ""; }
		const std::string &ret() { return indentation; } // I think these should be pretty clear...
		void inc() { indentation += "  "; }
		void dec() { indentation = indentation.substr(0, indentation.size() - 2); }
		static indent* instance; /// < The singleton instance used to keep track of the state
		std::string indentation; ///< The current indentation. Can conveniently be printed before any output.
};

#endif