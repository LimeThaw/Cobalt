//
// Created by flocke on 05.10.15.
//

#ifndef COBALT_TEXTURE_CACHE_H
#define COBALT_TEXTURE_CACHE_H

#include <map>
#include <string>
#include "texture_data_source.h"

class texture_cache {
    private:
        std::map<std::string, texture_data_source> entries;
    public:
        static texture_cache &get_instance();

        texture_data_source get_texture_from_filename(std::string filename);
};


#endif //COBALT_TEXTURE_CACHE_H
