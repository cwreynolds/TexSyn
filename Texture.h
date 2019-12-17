//
//  Texture.h
//  texsyn
//
//  Created by Craig Reynolds on 12/15/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Color.h"

class AbstractTexture
{
    AbstractTexture(){}
    virtual Color pixel(float x, float y) = 0;
};
