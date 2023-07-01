//
//  Utilities.cpp
//  texsyn
//
//  Created by Craig Reynolds on 12/16/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#include "Utilities.h"
#include "Vec2.h"

// Compute the inverse Möbius transformation of the complex plane. It is
// parameterized by four "points" (aka complex numbers). The Wikipedia
// article (https://en.wikipedia.org/wiki/Möbius_transformation) says the four
// points should satisfy: ad − bc ≠ 0.
// See Tim Hutton cool app: http://timhutton.github.io/mobius-transforms/
Complex inverse_mobius_transform(Complex z,
                                 Complex a, Complex b, Complex c, Complex d)
{
    Complex numerator = (d * z) - b;
    Complex denominator = a - (c * z);
    if (denominator == Complex(0, 0)) debugPrint(denominator);
    return numerator / denominator;
}

// Returns the scalar amplitude of a co-sinusoidal spot, for a given sample
// position, and given spot parameters (center, inner_radius, outer_radius)
// as in Spot::getColor(), etc. (TODO use it there?)
float spot_utility(Vec2 position,
                   Vec2 center,
                   float inner_radius,
                   float outer_radius)
{
    // Distance from sample position to spot center.
    float d = (position - center).length();
    // Fraction for interpolation: 0 inside, 1 outside, ramp between.
    float f = remapIntervalClip(d, inner_radius, outer_radius, 0, 1);
    // map interval [0, 1] to cosine curve.
    return 1 - sinusoid(f);
}

// TODO to be used by Gabor noise. Something like the Grating texture operator,
// but transform is via an angle and center point, assumes sinosoid (softness=1)
float grating_utility(Vec2 position, Vec2 center, float angle, float wavelength)
{
    Vec2 moved = position - center;
    Vec2 rotated = moved.rotate(angle);
    return 1 - sinusoid(rotated.x() / (wavelength * 2));
}
