#include "model_loader.h"

// Based on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
bool loadObj(const char* path,
    std::vector<float>& out_vertices,
    std::vector<float>& out_tex_coords,
    std::vector<float>& out_normals) {

    std::vector<unsigned int> vertexIndices, texCoordsIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_tex_coords;
    std::vector<glm::vec3> temp_normals;

    FILE* file;
    fopen_s(&file, path, "r");
    if (file == NULL) {
        printf("Impossible to open the file!\n");
        return false;
    }

    while (1) {
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader));
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else: parse lineHeader
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 tex;
            fscanf_s(file, "%f %f\n", &tex.x, &tex.y);
            temp_tex_coords.push_back(tex);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];
            int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &texCoordIndex[0], &normalIndex[0], &vertexIndex[1], &texCoordIndex[1], &normalIndex[1], &vertexIndex[2], &texCoordIndex[2], &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser :( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            texCoordsIndices.push_back(texCoordIndex[0]);
            texCoordsIndices.push_back(texCoordIndex[1]);
            texCoordsIndices.push_back(texCoordIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        out_vertices.insert(out_vertices.end(), { vertex.x, vertex.y, vertex.z, 1.0 });
    }
    // For each texture coordinate of each triangle
    for (unsigned int i = 0; i < texCoordsIndices.size(); i++) {
        unsigned int texCoordIndex = texCoordsIndices[i];
        glm::vec2 tex = temp_tex_coords[texCoordIndex - 1];
        out_tex_coords.insert(out_tex_coords.end(), { tex.x, tex.y });
    }
    // For each vertex normal of each triangle
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        out_normals.insert(out_normals.end(), { normal.x, normal.y, normal.z, 0.0 });
    }

    return true;
}
