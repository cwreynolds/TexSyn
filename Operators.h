//
//  Operators.h
//  texsyn
//
//  Created by Craig Reynolds on 12/31/19.
//  Copyright © 2019 Craig Reynolds. All rights reserved.
//

#pragma once
#include "Texture.h"

class Operator : public Texture {};

// Use a given "matte" texture to combine two other textures. The luminance of
// the matte at each color sample is used as the interpolation fraction between
// the corresponding samples of the other textures.
class SoftMatte : public Operator
{
public:
    SoftMatte(const Texture& _matte,
              const Texture& _texture0,
              const Texture& _texture1)
        : matte(_matte), texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return interpolate(matte.getColor(position).luminance(),
                           texture0.getColor(position),
                           texture1.getColor(position));
    }
private:
    const Texture& matte;
    const Texture& texture0;
    const Texture& texture1;
};

// Add two textures.
class Add : public Operator
{
public:
    Add(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) + texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Subtract two textures. (texture0 - texture1)
class Subtract : public Operator
{
public:
    Subtract(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) - texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// TODO TEMP FOR TESTING? I kept going back and forth about needing this,
//      instead of, or in addition to Subtract. But now I want it for texture
//      comparison while working on StretchSpot

// Names AbsDiff Distance, Norm?

// Absolute value of the difference of two textures. abs(texture0 - texture1)
class AbsDiff : public Operator
{
public:
    AbsDiff(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color diff = texture0.getColor(position) - texture1.getColor(position);
        // TODO define overloat of std::abs() for Color?
        return Color(std::abs(diff.r()),
                     std::abs(diff.g()),
                     std::abs(diff.b()));
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Multiply two textures: for each color sample, multiply component-wise the
// corresponding color samples of the two textures.
class Multiply : public Operator
{
public:
    Multiply(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        return texture0.getColor(position) * texture1.getColor(position);
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Select between two textures, sample by sample, by taking the one whose
// luminance is greater.
class Max : public Operator
{
public:
    Max(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color c0 = texture0.getColor(position);
        Color c1 = texture1.getColor(position);
        return (c0.luminance() > c1.luminance()) ? c0 : c1;
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Select between two textures, sample by sample, by taking the one whose
// luminance is less.
class Min : public Operator
{
public:
    Min(const Texture& _texture0, const Texture& _texture1)
        : texture0(_texture0), texture1(_texture1) {}
    Color getColor(Vec2 position) const override
    {
        Color c0 = texture0.getColor(position);
        Color c1 = texture1.getColor(position);
        return (c0.luminance() < c1.luminance()) ? c0 : c1;
    }
private:
    const Texture& texture0;
    const Texture& texture1;
};

// Scale rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Scale : public Operator
{
public:
    Scale(float _scale, const Texture& _texture)
        : scale(_scale), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position / scale);
    }
private:
    const float scale;
    const Texture& texture;
};

// Rotate rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Rotate : public Operator
{
public:
    Rotate(float _angle, const Texture& _texture)
        : angle(_angle), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position.rotate(-angle));
    }
private:
    const float angle;
    const Texture& texture;
};

// Translate rigid geometric transformation. Mostly for hand-written code.
// TODO should this have a "center" parameter? For GP it would make sense,
// but if this is just for hand-written code it can be handled by ordering.
class Translate : public Operator
{
public:
    Translate(Vec2 _translation, const Texture& _texture)
        : translation(_translation), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position - translation);
    }
private:
    const Vec2 translation;
    const Texture& texture;
};


// Maps the brightness of a sample of the given Texture to a pure hue (full
// brightness, full saturation). The hue transform is offset by a given phase.
class BrightnessToHue : public Operator
{
public:
    BrightnessToHue (float _huePhase, const Texture& _texture)
        : huePhase (_huePhase), texture (_texture) {}
    Color getColor(Vec2 position) const override
    {
        float luminance = texture.getColor(position).luminance();
        float red, green, blue;
        Color::convertHSVtoRGB(luminance + huePhase, 1.0f, 1.0f,
                               red, green, blue);
        return Color(red, green, blue);
    }
private:
    const float huePhase;
    const Texture& texture;
};

// Wrap a given portion of a half-plane radially around a given center point,
// keeping the texture along a given ray unchanged. Related to a rectangular-
// (Cartesian)-to-polar transform.
//
// The parameters are the "width" of a vertical slice of the half-plane which
// will be wrapped, the "center" of rotation, a "fixed_ray" vector indicating
// the orientation of the fixed point ray extending from the center, and the
// source "texture".
//
// Consider the "fixed_ray" to be the +Y axis of a space with "center" at its
// origin. For any point on the X axis from [-width, width] consider the ray
// parallel to +Y. The X coordinate is mapped to an angle of rotation from +Y.
//
class Wrap : public Operator
{
public:
    Wrap(float _width, Vec2 _center, Vec2 _fixed_ray, const Texture& _texture)
      : width(_width),
        center(_center),
        fixed_ray(_fixed_ray),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        // Position relative to "center".
        Vec2 p = position - center;
        // X and Y basis vectors of transformed space.
        Vec2 new_y = fixed_ray.normalize();
        Vec2 new_x = new_y.rotate90degCW();
        // Measure angle (0 parallel to new_y axis, pi/2 parallel to new_x).
        float angle = atan2 (p.dot(new_x), p.dot(new_y));
        // Distance from "center".
        float radius = p.length();
        Vec2 new_point = (center +
                          new_x * remapInterval(angle, -pi, pi, -width, width) +
                          new_y * radius);
        return texture.getColor(new_point);
    }
private:
    const float width;
    const Vec2 center;
    const Vec2 fixed_ray;
    const Texture& texture;
};

// TODO updated 2009 version
// TODO remember to remove this after the blog post.
//
// scale a Texture along the X axis leaving the Y axis unchanged
// parameters: scale factor, rotation angle, translation vector
class Stretch2009 : public Operator
{
public:
    Stretch2009(float _scale, float _angle, Vec2 _center, const Texture& _texture)
      : scale(_scale),
        angle(_angle),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 p = position;
        p = p - center;
        p = p.rotate(-angle);
        p = Vec2(p.x() / scale, p.y());
        p = p.rotate(angle);
        p = p + center;
        return texture.getColor(p);
    }
private:
    const float scale;
    const float angle;
    const Vec2 center;
    const Texture& texture;
};

// Stretch (scale in one dimension) the given input Texture. The stretching is
// defined by the first Vec2 argument "stretch". Its magnitude is the scale
// factor, and its direction is the axis scaling. The "center" argument is the
// origin of the scaling. It is unchanged by the transform, as is the line
// through it perpendicular to the "stretch" vector.
class Stretch : public Operator
{
public:
    Stretch(Vec2 _stretch, Vec2 _center, const Texture& _texture)
      : scale(_stretch.length()),
        main_basis(_stretch / scale),
        perp_basis(main_basis.rotate90degCW()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float main_distance = offset.dot(main_basis);
        float perp_distance = offset.dot(perp_basis);
        float stretched = main_distance / scale;
        return texture.getColor(center +
                                main_basis * stretched +
                                perp_basis * perp_distance);
    }
private:
    const float scale;
    const Vec2 main_basis;
    const Vec2 perp_basis;
    const Vec2 center;
    const Texture& texture;
};

// Modifies the given texture within a disk of "radius" around "center", doing a
// "fisheye" expansion of the center of the disk (when center_magnification > 1)
// or a contraction (when center_magnification < 1).
class StretchSpot : public Operator
{
public:
    StretchSpot(float _center_magnification,
                float radius,
                Vec2 _center,
                const Texture& _texture) :
        center_magnification(_center_magnification),
        outer_radius(radius),
        center(_center),
        texture(_texture)
    {
        ifNeededInitializeInverseLUT();
    }
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float radius = offset.length();
        float relative_radius = radius / outer_radius;
        float s = (relative_radius == 0 ? 0 :
                   ((relative_radius > 1) ? 1 :
                    ((center_magnification < 1) ?
                     inverseRemapper (relative_radius) :
                     remapper (relative_radius))));
        return texture.getColor((offset * s) + center);
    }
    // maps from relative radius to magnification multiplier between 0 and 1
    float remapper(float rr) const
    {
        return interpolate(interpolate (rr, rr, sinusoid (rr)),
                           ((center_magnification > 1) ?
                            (1 / center_magnification) :
                            center_magnification),
                           1.0f);
    }
    float inverseRemapper(float rr) const
    {
        int i = int(std::round(rr * (lutSize() - 1)));
        return inverse_lut->at(i) / rr;
    }
    void ifNeededInitializeInverseLUT()
    {
        if (center_magnification < 1)
        {
            inverse_lut = std::make_shared<std::vector<float>>(lutSize());
            float lastQ = 0;
            int i = 0;
            inverse_lut->at(0) = 0;
            float inverseRes = 1.0 / lutSize();
            for (float r = 0; r <= 1; r = r + (inverseRes * 0.01))
            {
                float q = r * remapper (r);
                float d = q - lastQ;
                if (d > inverseRes)
                {
                    i++;
                    if (i == lutSize()) break;
                    inverse_lut->at(i) = r;
                    lastQ = q;
                }
            }
            // Fill out LUT in case of numerical error.
            for (; i<lutSize() - 1; ) { inverse_lut->at(++i) = 1; }
        }
    }
private:
    const float center_magnification;
    const float outer_radius;
    const Vec2 center;
    const Texture& texture;
    std::shared_ptr<std::vector<float>> inverse_lut;
    
    // Note that this is massive overkill. The current design looks up a single
    // value in the inverse LUT. This could be much smaller if it interpolated
    // between adjacent values in the LUT. Perhaps it make sense to trade memory
    // for code simplicity. This value was chosen by drawing 511 pixel diameter
    // test textures using this code and visually inspecting for "smooth" edges.
    //     Grating vert_stripes(Vec2(0, 0), Color(1, 1, 1),
    //                          Vec2(0.1, 0), Color(0, 0, 0), 0.2);
    //     StretchSpot(0.5, 10, Vec2(-5, 0), vert_stripes).displayInWindow();
    int lutSize() const { return 10000; }
};

// Creates a Color grating by taking a slice(/transit/1d texture/waveform) from
// a given Texture, then sweeping that pattern in a perpendicular direction. The
// slice is defined by a tangent vector and a center. The tangent basis vector
// give the direction of the slice and its length is a scaling factor along the
// slice. (Nearly identical to a very large first arg to Stretch.)
class SliceGrating : public Operator
{
public:
    SliceGrating(Vec2 _slice_tangent, Vec2 _center, const Texture& _texture)
      : slice_tangent(_slice_tangent),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float projection = offset.dot(slice_tangent);
        return texture.getColor(center + (slice_tangent * projection));
    }
private:
    const Vec2 slice_tangent;
    const Vec2 center;
    const Texture& texture;
};

// Creates a radial color pattern based on a slice(/transit/1d texture/waveform)
// from another Texture. The slice is defined by a "center" point on it and a
// tangent vector indicating its orientation and scaling. The radial pattern
// consists of rays of colors read from a portion of the slice, addressed by
// angle relative to the tangent direction.
class SliceToRadial : public Operator
{
public:
    SliceToRadial(Vec2 _slice_tangent, Vec2 _center, const Texture& _texture)
      : slice_tangent(_slice_tangent),
        perpendicular(slice_tangent.rotate90degCW()),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float local_y = offset.dot(slice_tangent);
        float local_x = offset.dot(perpendicular);
        float angle = atan2(local_x, local_y);
        return texture.getColor(center + (slice_tangent * angle));
    }
private:
    const Vec2 slice_tangent;
    const Vec2 perpendicular;
    const Vec2 center;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Shear one Texture, along a given axis, by an amount based on luminance values
// from a slice of another Texture. Parameters are the two Textures, along with
// an axis for each, defined as a center point and tangent vector.
class SliceShear : public Operator
{
public:
    SliceShear(Vec2 _slice_tangent,
               Vec2 _slice_center,
               const Texture& _texture_for_slice,
               Vec2 _shear_tangent,
               Vec2 _shear_center,
               const Texture& _texture_to_shear)
      : slice_tangent(_slice_tangent),
        slice_center(_slice_center),
        texture_for_slice(_texture_for_slice),
        shear_tangent(_shear_tangent),
        shear_center(_shear_center),
        texture_to_shear(_texture_to_shear),
        perpendicular(shear_tangent.rotate90degCCW()) {}
    Color getColor(Vec2 position) const override
    {
        // Look up position on slice, measure its color's luminance.
        Vec2 slice_offset = position - slice_center;
        float slice_projection = slice_offset.dot(slice_tangent);
        Vec2 point_on_slice = slice_center + (slice_tangent * slice_projection);
        Color slice_color = texture_for_slice.getColor(point_on_slice);
        float luminance = slice_color.luminance();
        
//        // TODO TEMP
//        luminance = fmod_floor(slice_projection, 0.1);
        
        
        // Find point on texture_to_shear: decompose into x,y in shear space,
        // offset x by luminince from slice sample, recombine to new position.
        Vec2 shear_offset = position - shear_center;
        float local_x = shear_offset.dot(shear_tangent);
        float local_y = shear_offset.dot(perpendicular);
        return texture_to_shear.getColor(shear_center +
                                         shear_tangent * (local_x + luminance) +
                                         perpendicular * local_y);

//        return slice_color;

    }
private:
    const Vec2 slice_tangent;
    const Vec2 slice_center;
    const Texture& texture_for_slice;
    const Vec2 shear_tangent;
    const Vec2 shear_center;
    const Texture& texture_to_shear;
    const Vec2 perpendicular;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Use the Möbius transformation of the complex plane as a Texture Operator by
// considering it isomorphic to the Cartesian Texture plane. The transformation
// is parameterized by four "points" (aka complex numbers). The Wikipedia
// article (https://en.wikipedia.org/wiki/Möbius_transformation) says the four
// points should satisfy: ad − bc ≠ 0.
// See Tim Hutton cool app: http://timhutton.github.io/mobius-transforms/
class MobiusTransform : public Operator
{
public:
    MobiusTransform(Vec2 _a, Vec2 _b, Vec2 _c, Vec2 _d, const Texture& _texture)
      : a(Vec2ToComplex(_a)),
        b(Vec2ToComplex(_b)),
        c(Vec2ToComplex(_c)),
        d(Vec2ToComplex(_d)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Complex z = Vec2ToComplex(position);
        Complex imt = inverse_mobius_transform(z, a, b, c, d);
        return texture.getColor(ComplexToVec2(imt));
    }
    static Vec2 ComplexToVec2(Complex z) { return {z.real(), z.imag()}; }
    static Complex Vec2ToComplex(Vec2 v) { return {v.x(), v.y()}; }
private:
    const Complex a;
    const Complex b;
    const Complex c;
    const Complex d;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// TODO trying "distributed sampling" approach. Takes some fixed number of
// random samples within the convolution kernel, looks up input color, weights
// it, and averages the result.
//
// TODO maybe try jiggled grid approach to get better sample coverage

//    class Blur : public Operator
//    {
//    public:
//        Blur(const float _width, const Texture& _texture)
//            : width(_width), texture(_texture) {}
//        Color getColor(Vec2 position) const override
//        {
//            // TODO VERY PROTOTYPE
//            float radius = width / 2;
//    //        int subsamples = 10;
//            int subsamples = 50;
//    //        int subsamples = 100;
//    //        int subsamples = 500;
//            Color weighted_sum(0, 0, 0);
//            for (int i = 0; i < subsamples; i++)
//            {
//    //            Vec2 offset = Vec2::randomPointInUnitDiameterCircle() * radius;
//    //            float length = offset.length();
//
//                float length = frandom2(0, radius);
//                Vec2 offset = Vec2::randomUnitVector() * length;
//
//
//                float weight = 1 - sinusoid(length / radius);
//                Color color_at_offset = texture.getColor(position + offset);
//                weighted_sum += color_at_offset * weight;
//
//    //            std::cout << "---------------------------------------" << std::endl;
//    //            debugPrint(radius);
//    //            debugPrint(offset);
//    //            debugPrint(position + offset);
//    //            debugPrint(length / radius);
//    //            debugPrint(weight);
//    //            debugPrint(color_at_offset);
//    //            debugPrint(weighted_sum);
//            }
//            return weighted_sum / subsamples;
//        }
//    private:
//        const float width;
//        const Texture& texture;
//    };

//    class Blur : public Operator
//    {
//    public:
//        Blur(const float _width, const Texture& _texture)
//            : width(_width), texture(_texture) {}
//        Color getColor(Vec2 position) const override
//        {
//            // TODO VERY PROTOTYPE
//            float radius = width / 2;
//    //        int subsamples = 50;
//    //        int subsamples = 100;
//    //        int subsamples = 1000;
//            int subsamples = 15 * 15;
//    //        Color weighted_sum(0, 0, 0);
//            Color sum_of_weighted_colors(0, 0, 0);
//            float sum_of_weights = 0;
//            for (int i = 0; i < subsamples; i++)
//            {
//    //            float length = frandom2(0, radius);
//    //            Vec2 offset = Vec2::randomUnitVector() * length;
//
//                Vec2 offset = Vec2::randomPointInUnitDiameterCircle() * radius;
//                float length = offset.length();
//
//
//                float weight = 1 - sinusoid(length / radius);
//                Color color_at_offset = texture.getColor(position + offset);
//    //            weighted_sum += color_at_offset * weight;
//                sum_of_weighted_colors += color_at_offset * weight;
//                sum_of_weights += weight;
//
//
//    //            std::cout << "---------------------------------------" << std::endl;
//    //            debugPrint(radius);
//    //            debugPrint(offset);
//    //            debugPrint(position + offset);
//    //            debugPrint(length / radius);
//    //            debugPrint(weight);
//    //            debugPrint(color_at_offset);
//    //            debugPrint(weighted_sum);
//            }
//    //        return weighted_sum / subsamples;
//            return sum_of_weighted_colors / sum_of_weights;
//        }
//    private:
//        const float width;
//        const Texture& texture;
//    };

// TODO this version uses an NxN grid of samples
class Blur : public Operator
{
public:
    Blur(const float _width, const Texture& _texture)
        : width(_width), texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        // TODO VERY PROTOTYPE
        float radius = width / 2;
        int n = 15;  // number of subsamples is n²
        float cell_width = width / n;
        std::vector<Vec2> offsets;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                offsets.push_back(Vec2((i * cell_width) - radius, // cell corner
                                       (j * cell_width) - radius) +
                                  Vec2(frandom01() * cell_width,  // jiggle
                                       frandom01() * cell_width));
        Color sum_of_weighted_colors(0, 0, 0);
        float sum_of_weights = 0;
        for (Vec2 offset : offsets)
        {
            float length = offset.length();
            if (length <= radius)
            {
                float weight = 1 - sinusoid(length / radius);
                Color color_at_offset = texture.getColor(position + offset);
                sum_of_weighted_colors += color_at_offset * weight;
                sum_of_weights += weight;
            }
        }
        return sum_of_weighted_colors / sum_of_weights;
    }
private:
    const float width;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Colorize one texture by mapping its luminance to the sequence of colors
// along a slice(/transit/1d texture/waveform) of another texture.
class Colorize : public Operator
{
public:
    Colorize(Vec2 _slice_tangent,
             Vec2 _center,
             const Texture& _texture_for_slice,
             const Texture& _texture_to_color)
      : slice_tangent(_slice_tangent),
        center(_center),
        texture_for_slice(_texture_for_slice),
        texture_to_color(_texture_to_color) {}
    Color getColor(Vec2 position) const override
    {
        // Look up color to be tinted/colorized an get its luminance.
        Color original = texture_to_color.getColor(position);
        float luminance = original.luminance();
        // Look up color by luminance along slice in texture_for_slice.
        Vec2 on_slice = center + (slice_tangent * luminance);
        return texture_for_slice.getColor(on_slice);
    }
private:
    const Vec2 slice_tangent;
    const Vec2 center;
    const Texture& texture_for_slice;
    const Texture& texture_to_color;
};

// Given two intensity thresholds and an input texture, remap the texture colors
// between those thresholds to "full intensity range". Converts the texture to
// hue-saturation-value color space. Areas darker than the lower threshold get
// value=0, areas brighter than the upper threshold get value=1, areas between
// the two thresholds are mapped to values on [0, 1]. The hue and saturation
// components remain unchanged.
class SoftThreshold : public Operator
{
public:
    SoftThreshold (float _intensity0, float _intensity1, const Texture& _texture)
      : intensity0(std::min(_intensity0, _intensity1)),
        intensity1(std::max(_intensity0, _intensity1)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        float new_v = remapIntervalClip(value, intensity0, intensity1, 0, 1);
        color.setHSV(hue, saturation, new_v);
        return color;
    }
private:
    const float intensity0;
    const float intensity1;
    const Texture& texture;
};

// Finds (colored) edges based on Blur and "unsharp masking" -- subtracting the
// blurred texture from the original.
//
// TODO this is a "modernized" version of the old August 2009 version. It finds
// the edges, a signal which is symmetric around zero brightness, then biases
// that up by adding middle gray. That last part seems questionable, but
// definitely produces a result more likely to be visable.
class EdgeDetect : public Operator
{
public:
    EdgeDetect(const float width, const Texture& texture)
      : blur(width, texture),
        edges(texture, blur) {}
    Color getColor(Vec2 position) const override
    {
        return edges.getColor(position) + Color::gray(0.5);
    }
private:
    Blur blur;
    Subtract edges;
};

class EdgeEnhance : public Operator
{
public:
    EdgeEnhance(const float width,
//                 const float strength,
                const Texture& texture)
    : blur(width, texture),
    edges(texture, blur),
//    scaled (*new Multiply (Pixel::gray (strength), edges)),
    enhanced(texture, edges) {}
    Color getColor(Vec2 getColor) const override
    {
        return enhanced.getColor(getColor);
    }
private:
    Blur blur;
    Subtract edges;
//    Multiply scaled;
    Add enhanced;
};

// In HSV space, rotates the hue by the given "offset". "H" is on [0, 1] so
// H+offset is taken "f-modulo 1.0". Only the fractional part of offset is
// meaningful.
class AdjustHue : public Operator
{
public:
    AdjustHue(float _offset, const Texture& _texture)
      : offset(_offset),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        color.setHSV(std::fmod(hue + offset, 1), saturation, value);
        return color;
    }
private:
    const float offset;
    const Texture& texture;
};

// Adjust saturation: in HSV space, scale (and clip) saturation.
class AdjustSaturation : public Operator
{
public:
    AdjustSaturation (float _factor, const Texture& _texture)
      : factor(_factor),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color color = texture.getColor(position);
        float hue, saturation, value;
        color.getHSV(hue, saturation, value);
        color.setHSV(hue, clip(saturation * factor, 0, 1), value);
        return color;
    }
private:
    const float factor;
    const Texture& texture;
};

// Adjust brightness: scale all colors by a given factor. RGB components are
// multiplied by the factor. See also Multiply which forms the product of two
// textures. In the previous version of this library there was a Tint operator
// that multiplied a Texture by a Color.
class AdjustBrightness : public Operator
{
public:
    AdjustBrightness (float _factor, const Texture& _texture)
      : factor(_factor),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        return texture.getColor(position) * factor;
    }
private:
    const float factor;
    const Texture& texture;
};

// Twist an input texture around a given "center". The twist has infinite
// extent but falls off as 1/r. This creates a spiral tightly curved near
// "center" and asymptotically approaching zero curvature for increasing radius.
// The Twist is parameterized by an "angle_scale" (bigger values mean more
// twisting) and a "radius_scale" which adjusts the rate of falloff (bigger
// values pull the twisting closed to "center"). The twist angle is:
// angle = angle_scale / ((radius * radius_scale) + 1)
class Twist : public Operator
{
public:
    Twist (float _angle_scale,
           float _radius_scale,
           const Vec2 _center,
           const Texture&
           _texture)
      : angle_scale(_angle_scale),
        radius_scale(_radius_scale),
        center(_center),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Vec2 offset = position - center;
        float radius = offset.length();
        float angle = angle_scale / ((radius * radius_scale) + 1);
        Vec2 rotated_offset = offset.rotate(angle);
        return texture.getColor(center + rotated_offset);
    }
private:
    const float angle_scale;
    const float radius_scale;
    const Vec2 center;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class BrightnessWrap: public Operator
{
public:
    BrightnessWrap(const float _intensity0,
                   const float _intensity1,
                   const Texture& _texture)
      : intensity0(std::min(_intensity0, _intensity1)),
        intensity1(std::max(_intensity0, _intensity1)),
        texture(_texture) {}
    Color getColor(Vec2 position) const override
    {
        Color result;
        if (intensity0 == intensity1)
        {
            result = Color::gray(intensity0);
        }
        else
        {
            Color input = texture.getColor(position);
            float brightnessBefore = input.luminance();
            float intensity_interval = intensity1 - intensity0;

            // QQQ XXX really bad code, needs to be rewritten XXX QQQ
            float foo = brightnessBefore;
            foo -= intensity0;
            foo /= intensity_interval;
            foo = foo - floor (foo);
            foo *= intensity_interval;
            foo += intensity0;
            const float brightnessAfter = foo;
            
            if (brightnessAfter == 0)
            {
                result = Color::gray(0);
            }
            else
            {
                float brightnessRatio = brightnessAfter / brightnessBefore;
                result = input * brightnessRatio;
            }
        }
        return result;
    }
private:
    const float intensity0;
    const float intensity1;
    const Texture& texture;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
