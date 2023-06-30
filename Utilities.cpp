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

// Utility for randomized subsampling in a square 2d region. Generates 2d
// offsets from the center of the square for an NxN jittered grid. Parameters:
//     n: square root of the number of grid cells (and offsets returned).
//     square_side_length: dimension of the 2d region covered by grid.
//     rs: a RandomSequence for generating random jitter.
//     offsets: vector to be filled with Vec2d offsets
// (FYI See https://www.redblobgames.com/x/1830-jittered-grid/)
// September 28, 2020: if n=1 offset will be in center of cell (square)
void jittered_grid_NxN_in_square(int n,
                                 float square_side_length,
                                 RandomSequence& rs,
                                 std::vector<Vec2>& offsets)
{
    offsets.clear();
    float radius = square_side_length / 2;
    float cell_width = square_side_length / n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            offsets.push_back(n == 1 ?
                              Vec2() :
                              Vec2((i * cell_width) - radius, // cell corner
                                   (j * cell_width) - radius) +
                              Vec2(rs.frandom01() * cell_width,  // jitter
                                   rs.frandom01() * cell_width));
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
