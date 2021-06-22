#pragma once

#include <ge211.hxx>
#include "specification.hxx"

using Position = ge211::Posn<int>;
using Velocity = ge211::Dims<int>;
using Block = ge211::Rect<int>;

struct Doodle
{
    Doodle(Spec const&);

    bool hits_bottom(Spec const&) const;


    bool hits_block_top(Spec const&, Block&) const;


    bool hits_obstacle(Spec const&, Block&) const;


    Doodle next(double dt) const;

    bool live;

    int score;

    Velocity velocity;

    Position center;
};