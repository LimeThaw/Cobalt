//
// Created by flocke on 05.10.15.
//

#include "texture_cache.h"

#include <algorithm>

texture_cache &texture_cache::get_instance() {
    static texture_cache cache;
    return cache;
}

texture_data_source texture_cache::get_texture_from_filename(std::string filename) {
    auto i = entries.find(filename);
    if(i != entries.end()) {
        return i->second;
    } else {
        auto rst = texture_data_source::load_from_file(filename);
        entries.insert(std::make_pair(filename, rst));
        return rst;
    }
}

std::string texture_cache::get_path(const texture_data_source &source) {
	for (auto it = entries.begin(); it != entries.end(); ++it ) {
		if (it->second == source)
			return it->first;
	}
	printf("! Could not find path of a texture. Returned empty string.\n");
	return "";	 
}
