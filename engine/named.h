// Created 2016-11-01 by LimeThaw

#ifndef NAMED_H_INCLUDED
#define NAMED_H_INCLUDED

#include <string>
#include "name_manager.h"

/**
 *  A class offering basic functionality for object names. Inherited by all classes that have a
 *  name. Works with the name_manager class to avoid diplicate names.
 */

class named {
	public:
		named(std::string new_name = "");
		~named();
		void set_name(std::string new_name);
		std::string get_name();
	private:
		std::string name;
};

#endif // NAMED_H_INCLUDED