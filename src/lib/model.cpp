#include "model.hpp"
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include <tiny_obj_loader.h>
#include "filesys.hpp"

namespace YUNIK_GTC {
    ModelOBJ::ModelOBJ (void) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;
        ifstream obj("model/box.obj");
        
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &obj);
        if (!err.empty()) {
            std::cerr << err << std::endl;
        }
        if (!ret) {
            std::cerr << "Error: cannot load model" << std::endl;
        }
        std::cout << "Load success" << std::endl;

        printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
        printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
        printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
        printf("# of materials = %d\n", (int)materials.size());
        printf("# of shapes    = %d\n", (int)shapes.size());
    }

    ModelOBJ::~ModelOBJ (void) {
        //
    }
}
