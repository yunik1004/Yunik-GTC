#include "model.hpp"
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>

namespace YUNIK_GTC {
    ObjectFile::ObjectFile () {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        //tinyobj::callback_t
        std::stringstream a;
        a << "444";
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &a);
    }

    ObjectFile::~ObjectFile () {
        //
    }
}