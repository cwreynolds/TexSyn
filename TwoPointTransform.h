//
//  TwoPointTransform.h
//  texsyn
//
//  Created by Craig Reynolds on 7/29/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//
//  A 2d affine transformation providing scaling, rotation, and translation.
//  Normally constructed with "two point" specification (like a two finger
//  pinch-twist-drag interface on a touch screen). The first point is the origin
//  of the local space (0, 0), the second point is (1, 0) in the local space.
//  localize() transforms a point specified in global space to the corresponding
//  point specified in local space. globalize() goes from local to global.

#pragma once
#include "Vec2.h"

class TwoPointTransform
{
public:
    TwoPointTransform() : TwoPointTransform(Vec2(0, 0), Vec2(1, 0)) {}
    TwoPointTransform(Vec2 point_0, Vec2 point_1)
      : scale_((point_1 - point_0).length()),
        origin_(point_0),
        x_basis_(point_1 - point_0),
        y_basis_(xBasis().rotate90degCCW()),
        x_basis_unit_(xBasis().normalize()),
        y_basis_unit_(xBasisUnit().rotate90degCCW()) {}
    float scale() const { return scale_; }
    Vec2 origin() const { return origin_; }
    Vec2 xBasis() const { return x_basis_; }
    Vec2 yBasis() const { return y_basis_; }
    Vec2 xBasisUnit() const { return x_basis_unit_; }
    Vec2 yBasisUnit() const { return y_basis_unit_; }
    // "Localize" a Vec2 from global space into this transform's "local space".
    Vec2 localize(Vec2 v) const
    {
        return Vec2((v - origin()).dot(xBasis() / sq(scale())),
                    (v - origin()).dot(yBasis() / sq(scale())));
    }
    // "Globalize" a Vec2 from this transform's "local space" to "global space".
    Vec2 globalize(Vec2 v) const
    {
        return origin() + (xBasis() * v.x()) + (yBasis() * v.y());
    }
private:
    const float scale_;
    const Vec2 origin_;
    const Vec2 x_basis_;
    const Vec2 y_basis_;
    const Vec2 x_basis_unit_;
    const Vec2 y_basis_unit_;
};
