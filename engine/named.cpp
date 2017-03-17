// Created 2016-11-01 by LimeThaw

#include "named.h"

named::named(std::string new_name) {
	name = name_manager::get_instance()->insert(new_name, this);
}

named::~named() {
	name_manager::get_instance()->remove(name);
}

void named::set_name(std::string new_name) {
	name_manager::get_instance()->remove(name);
	name = name_manager::get_instance()->insert(new_name, this);
}

std::string named::get_name() {
	return name;
}