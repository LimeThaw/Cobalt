//
// Created by flocke on 05.10.15.
//

#include "texture_cache.h"

#include <algorithm>

/**
 *  Singleton getter function. Keeps a static instance of the cache and returns it on request.
 *  This means that there is never more than one cache object in a program.
 */
texture_cache &texture_cache::get_instance() {
    static texture_cache cache;
    return cache;
}

/**
 *  Returns the data source for a specified file. If the file has been loaded before, the cache
 *  will look up the data and return it. Otherwise the image is loaded and registered in the cache.
 *  @param	filename	The path of the file you want to load.
 */
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

/**
 *  Returns the file path for a loaded texture source. If the source is not present in the cache
 *  iit will print a warning and return an empty string.
 *  @param	source	The texture_data_source object that you want to know the file path for.
 */
std::string texture_cache::get_path(const texture_data_source &source) {
	for (auto it = entries.begin(); it != entries.end(); ++it ) {
		if (it->second == source)
			return it->first;
	}
	printf("%s! Could not find path of a texture. Returned empty string.\n", indent::get().c_str());
	return "";
}
