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
//      An interacive demo, written in Processing, a self-guided demo of COTS
//      behavior and possibilities: https://github.com/jarek-rossignac/COTS
//
//  The c++ code below was derived from that Processing demo. Initially portions
//  were copied and conversted to c++ and the TexSyn's Vec2 class. Note that
//  this is used as "const" descriptions of maps that are not modified after
//  they are initially constructed. There are functions for setting corner
//  positions, but those are not well tested.
//
//------------------------------------------------------------------------------

#pragma once

#include "Vec2.h"

class COTS
{
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
    // Get fixed point of map.
    Vec2 getF() const { return f_; }

//  COTSMap(pt A1, pt B1, pt C1, pt D1) // creates map and sets its parameters
//    {
//    A=P(A1);  B=P(B1);  C=P(C1);  D=P(D1);
//    au = spiralAngle(A,B,D,C);
//    mu = spiralScale(A,B,D,C);
//    F = SpiralCenter(au,mu,A,D);
//    av = spiralAngle(A,D,B,C);
//    mv = spiralScale(A,D,B,C);
//    }

    // TODO compare with COTSupdateMap() in original Processing code
    void updateParameters()
    {
        au = spiralAngle(getA(), getB(), getD(), getC());
        mu = spiralScale(getA(), getB(), getD(), getC());
        f_ = SpiralCenter(au, mu, getA(), getD());
        av = spiralAngle(getA(), getD(), getB(), getC());
        mv = spiralScale(getA(), getD(), getB(), getC());
    }
    
    //---------------------------------------------------------------
    // utilities from pv.pde
    
    // dot(U,V): U*V (dot product U*V)
//    float dot(vec U, vec V) {return U.x*V.x+U.y*V.y; }
    float dot(Vec2 U, Vec2 V) {return U.dot(V); }

    // det | U V | = scalar cross UxV
//    float det(vec U, vec V) {return dot(R(U),V); }
    float det(Vec2 U, Vec2 V) {return dot(R(U),V); }

    // angle <U,V> (between -PI and PI)
//    float angle (vec U, vec V) {return atan2(det(U,V),dot(U,V)); };
    float angle(Vec2 U, Vec2 V) { return atan2(det(U,V),dot(U,V)); };
    
    // V turned right 90 degrees (as seen on screen)
    //    vec R(vec V) {return new vec(-V.y,V.x);};
    Vec2 R(Vec2 V) { return V.rotate90degCCW(); }
    
//    // measure
//    // ||AB|| (Distance)
//    float d(pt P, pt Q) {return sqrt(d2(P,Q));  };
//    // AB*AB (Distance squared)
//    float d2(pt P, pt Q) {return sq(Q.x-P.x)+sq(Q.y-P.y); };

    // measure
    // ||AB|| (Distance)
    float d(Vec2 P, Vec2 Q) { return (Q - P).length(); }

    //---------------------------------------------------------------

    //************************* SPIRAL *****************
    // Between Edge(A,B) and Edge(C,D), not (D,C) as in COTS
//    float spiralAngle(pt A0, pt B0, pt A1, pt B1)
//    {return angle(V(A0,B0),V(A1,B1));}
    float spiralAngle(Vec2 A0, Vec2 B0, Vec2 A1, Vec2 B1)
    {
//        return angle(V(A0,B0),V(A1,B1));
//        return angle(Vec2(A0, B0), Vec2(A1, B1));
        return angle(B0 - A0, B1 - A1);
    }

//    float spiralScale(pt A0, pt B0, pt A1, pt B1)
//    {return d(A1,B1)/d(A0,B0);}
    float spiralScale(Vec2 A0, Vec2 B0, Vec2 A1, Vec2 B1)
    {
        return d(A1, B1) / d(A0, B0);
    }

//    // computes fixed-point of spiral that takes A0 to A1
//    pt SpiralCenter(float a, float m, pt A0, pt A1)
//    {
//        float c=cos(a), s=sin(a);
//        vec W = V(m*c-1,m*s);
//        float d = dot(W,W);
//        vec A1A0 = V(A1,A0);
//        vec V = V(dot(W,A1A0),det(W,A1A0));
//        return P(A0,1./d,V);
//    }
    // computes fixed-point of spiral that takes A0 to A1
    Vec2 SpiralCenter(float a, float m, Vec2 A0, Vec2 A1)
    {
        float c=cos(a), s=sin(a);
        Vec2 W(m * c - 1, m * s);
        float d = dot(W, W);
        Vec2 A1A0 = A0 - A1;
        Vec2 V(dot(W, A1A0), det(W, A1A0));
//        return P(A0, 1./d, V);
        return A0 + (V * 1./d);
    }

//    // sV
//    vec W(float s,vec V) {return V(s*V.x,s*V.y);}
//    pt P(pt P, float s, vec V) {return P(P,W(s,V)); }
//    //  P+sV (P transalted by sV)
//
//    vec V(pt P, pt Q) {return new vec(Q.x-P.x,Q.y-P.y);};
//    // PQ (make vector Q-P from P to Q
    
private:
    
    // Four corners of the the map's curved "rectangle" region R
    Vec2 a_;
    Vec2 b_;
    Vec2 c_;
    Vec2 d_;
    
    Vec2 f_;                               // fixed point of quad


    int n=6;                                // cell-count in each direction
    
    // Should these be Vec2 ?
    float mu=1, mv=1;                       // spiral scalings
    float au=0, av=0;                       // spiral angles
    
    // TODO needed?
    float cx=0.5, cy=0.5, cr = 0.5;         // control-circle center coordinates
                                            //    and radius wrt cell [0,0]

    
    //    int n=6;                                // cell-count in each direction
    //    pt A=P(), B=P(), C=P(), D=P();          // corners
    //    float mu=1, mv=1;                       // spiral scalings
    //    float au=0, av=0;                       // spiral angles
    //    float cx=0.5, cy=0.5, cr = 0.5;         // control-circle center coordinates
    //                                            //    and radius wrt cell [0,0]
    //    pt F=P();                               // fixed point of quad

};
