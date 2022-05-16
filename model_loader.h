#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include<vector>
#include<glm/glm.hpp>

bool loadObj(const char* path,
    std::vector<float>& out_vertices,
    std::vector<float>& out_tex_coords,
    std::vector<float>& out_normals);

#endif
