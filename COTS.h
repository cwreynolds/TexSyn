//
//  COTS.h
//  texsyn
//
//  Created by Craig Reynolds on 6/15/20.
//  Copyright © 2020 Craig Reynolds. All rights reserved.
//
//------------------------------------------------------------------------------
//
//  Implementation of a COTS map, as described in:
//
//      Jarek Rossignac, 2020, “Corner-operated Tran-similar (COTS) Maps,
//      Patterns, and Lattices”, ACM Transactions on Graphics (TOG), Volume
//      39, Issue 1, Article 5, https://doi.org/10.1145/3267346
//
//      An interacive sketch, written in Processing, a self-guided demo of COTS
//      behavior and possibilities: https://github.com/jarek-rossignac/COTS
//
//  The c++ code below was derived from that Processing demo. Initially portions
//  were copied and converted to c++ and to use TexSyn's Vec2 class. Note that
//  this is used as "const" descriptions of maps that are not modified after
//  they are initially constructed. Functions exist to set corner positions, but
//  they are not well tested.
//
//------------------------------------------------------------------------------

#pragma once
#include "Vec2.h"

class COTS
{
public:
    COTS() {}
    COTS(Vec2 a, Vec2 b, Vec2 c, Vec2 d) : a_(a), b_(b), c_(c), d_(d)
    {
        updateParameters();
    }
    // get/set methods for corners (A, B, C, D)
    Vec2 getA() const { return a_; }
    Vec2 getB() const { return b_; }
    Vec2 getC() const { return c_; }
    Vec2 getD() const { return d_; }
    void setA(Vec2 a) { a_ = a; updateParameters(); }
    void setB(Vec2 b) { b_ = b; updateParameters(); }
    void setC(Vec2 c) { c_ = c; updateParameters(); }
    void setD(Vec2 d) { d_ = d; updateParameters(); }
    // Get fixed point of map (set after having updated corners and recomputed).
    Vec2 getF() const { return f_; }
    void setF(Vec2 f) { f_ = f; }

    // Recompute/re-cache: (1) spiral scalings, (2) spiral angles, and (3) fixed
    // point. To be called when the control points have been moved.
    void updateParameters()
    {
        // Spiral scaling (m) and spiral angle (a) for U parametric direction.
        au = spiralAngle(getA(), getB(), getD(), getC());
        mu = spiralScale(getA(), getB(), getD(), getC());
        // Spiral scaling (m) and spiral angle (a) for V parametric direction.
        av = spiralAngle(getA(), getD(), getB(), getC());
        mv = spiralScale(getA(), getD(), getB(), getC());
        // Fixed point of map.
        setF(SpiralCenter(au, mu, getA(), getD()));
    }

    // Computes inverse COTS mapping: from the position of a point in the range
    // (output) of the map, to the corresponding point in the domain (input) of
    // the map. So for example from a pixel on the screen to the parametric
    // coordinates of that point in the (input) space of the map.
    Vec2 Inverse(Vec2 M) const
    {
        Vec2 Mi;
        float x, y;
        float amin = std::min(std::min(0.0f, au + av), std::min(au, av));
        float amax = std::max(std::max(0.0f, au + av), std::max(au, av));
        float mmin = std::min(std::min(1.0f, mu * mv), std::min(mu, mv));
        float pm = distance(getF(), M) / distance(getF(), getA());
        float pa = angle(getA() - getF(), M - getF());
        auto MapInv = [&](float a, float m,
                          Vec2 F, Vec2 A,
                          float au, float mu,
                          float av, float mv)
        {
            float u = (log(m)*av-log(mv)*a) / (log(mu)*av-log(mv)*au);
            float v = (a-u*au)/av;
            return Vec2(u, v);
        };
        for (int i = -2; i <= 2; i++)
        {
            float ppa = pa + (2 * pi * i);
            Mi = MapInv(ppa, pm, getF(), getA(), au,  mu,  av,  mv);
            x = Mi.x(); y = Mi.y();
            if((amin < ppa) && (ppa < amax) && (mmin < pm) &&
               (0 <= x) && (x <= 1) && (0 <= y) && (y <= 1))
            {
                return Mi;
            }
        }
        return MapInv(pa, pm, getF(), getA(),  au,  mu,  av,  mv);
    }
        
    // NOTE: this is not called in TexSyn. It is used in the original Processing
    // Sketch during interactive adjustment. I converted it from Processing to
    // c++ while doing some testing. But in TexSyn these COTS map objects are
    // normally treated as "immutable", they are constructed then never modified
    // duing their lifetime.
    //
    // Recomputes COTS parameter, selecting angle branchings that minimize pops.
    void COTSupdateMap(Vec2 A1, Vec2 B1, Vec2 C1, Vec2 D1, int np)
    {
        n=np;
        setA(A1);
        setB(B1);
        setC(C1);
        setD(D1);
        mu = spiralScale(getA(), getB(), getD(), getC());
        mv = spiralScale(getA(), getD(), getB(), getC());
        float pau=au, pav=av; // previous angles
        // new values
        float aun = spiralAngle(getA(), getB(), getD(), getC());
        float avn = spiralAngle(getA(), getD(), getB(), getC());
        float aue = aun-au; // u difference
        if(abs(aue + 2 * pi) < abs(aue)) aue += 2 * pi;
        if(abs(aue - 2 * pi) < abs(aue)) aue -= 2 * pi;
        if(abs(aue + 2 * pi) < abs(aue)) aue += 2 * pi;
        if(abs(aue - 2 * pi) < abs(aue)) aue -= 2 * pi;
        float ave = avn-av; // v difference
        if(abs(ave + 2 * pi) < abs(ave)) ave += 2 * pi;
        if(abs(ave - 2 * pi) < abs(ave)) ave -= 2 * pi;
        if(abs(ave + 2 * pi) < abs(ave)) ave += 2 * pi;
        if(abs(ave - 2 * pi) < abs(ave)) ave -= 2 * pi;
        // TODO a global flag set in GUI.pde, for now assume it is cleared.
        bool resetBranching = false;
        if(resetBranching)
        {
            au=aun; av=avn; resetBranching=false;
        }
        else
        {
            au=pau+aue; av=pav+ave;
        }
        setF(SpiralCenter(au, mu, getA(), getD()));
    }
    
    // Spiral utilities.
    // Between Edge(A,B) and Edge(C,D), not (D,C) as in COTS
    float spiralAngle(Vec2 A0, Vec2 B0, Vec2 A1, Vec2 B1) const
    {
        return angle(B0 - A0, B1 - A1);
    }
    float spiralScale(Vec2 A0, Vec2 B0, Vec2 A1, Vec2 B1) const
    {
        return distance(A1, B1) / distance(A0, B0);
    }
    // Computes fixed-point of spiral that takes A0 to A1.
    Vec2 SpiralCenter(float a, float m, Vec2 A0, Vec2 A1) const
    {
        float c=cos(a), s=sin(a);
        Vec2 W(m * c - 1, m * s);
        float d = W.dot(W);
        Vec2 A1A0 = A0 - A1;
        Vec2 V(W.dot(A1A0), det(W, A1A0));
        return A0 + (V / d);
    }
    
    // utilities from pv.pde
    // measure ||AB|| (Distance)
    float distance(Vec2 P, Vec2 Q) const { return (Q - P).length(); }
    // det | U V | = scalar cross UxV
    float det(Vec2 U, Vec2 V) const {return U.rotate90degCCW().dot(V); }
    // angle <U,V> (between -PI and PI)
    float angle(Vec2 U, Vec2 V) const { return atan2(det(U,V), U.dot(V)); };

private:
    // Four corners of the the map's curvilinear "rectangle" region R
    Vec2 a_, b_, c_, d_;
    Vec2 f_;                                // fixed point of quad
    int n=6;                                // cell-count in each direction
    // Should these be Vec2 ?
    float mu=1, mv=1;                       // spiral scalings
    float au=0, av=0;                       // spiral angles
};
