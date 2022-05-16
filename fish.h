#ifndef FISH_H
#define FISH_H

#include "object.h"
#include<vector>

namespace Objects {
    class Fish : public Object {
    protected:
        std::vector<float> vertices;
        std::vector<float> texture_coords;
        std::vector<float> normals;
        GLuint texture;

        float turnStart = -1;
        float turnEnd = -1;
        float direction = 0;
        float rotateTarget = 0;

    public:
        Fish(GLuint texture);
        virtual void draw(ShaderProgram* sp) override;
        virtual void performMove(float time, float deltaTime) override;
    };
}

#endif
