//
// Created by Benedict Hee Seok Jung on 5/28/21.
//

#pragma once
#include <ge211.hxx>

struct Spec
{
    Spec();
    ge211::Dims<int> doodle_velocity;
    int icon_center_x{40};
    int doodle_adj {15};
    int doodle_accel;
    ge211::Dims<int> block_dims;
    int block_spacing;
    ge211::Dims<int> background_dims;
    ge211::Dims<int> doodle_dims;
    ge211::Dims<int> monster_dims;
    ge211::Posn<int> starting_block;
};