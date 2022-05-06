#ifndef FISH_H
#define FISH_H

#include "object.h"

namespace Objects {
    class Fish : public Object {
    protected:
        float turnStart = -1;
        float turnEnd = -1;
        float direction = 0;
        float rotateTarget = 0;

    public:
        virtual void draw(ShaderProgram* sp) override;
        virtual void performMove(float time, float deltaTime) override;
    };
}

#endif
