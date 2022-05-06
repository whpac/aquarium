#ifndef FISH_H
#define FISH_H

#include "object.h"

namespace Objects {
    class Fish : public Object {

        virtual void draw(ShaderProgram* sp) override;
    };
}

#endif
