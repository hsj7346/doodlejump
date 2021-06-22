//
// Created by Benedict Hee Seok Jung on 5/27/21.
//

#include "doodle.hxx"
#include "specification.hxx"

Doodle::Doodle(Spec const& spec)
        : live {false},
          score {0},
          velocity {spec.doodle_velocity},
          center {spec.starting_block.up_by(spec.block_dims.height*2)
                      .right_by(spec.block_dims.width/2)}
{ }

bool
Doodle::hits_bottom(Spec const& spec) const
{
    Position p {center.down_by(spec.doodle_dims.height/2)};
    return p.y > spec.background_dims.height;
}


bool
Doodle::hits_block_top(Spec const& spec, Block& block) const
{
    int x1 = center.x - spec.doodle_dims.width/2;
    int x2 = center.x + spec.doodle_dims.width/2;
    int y = center.y - spec.doodle_dims.height/2;
    if (x2 < block.top_left().x || x1 > block.top_right().x) {
        return false;
    } else {
        if (velocity.height > 0) {
            if (y >= (block.top_right().y-spec.doodle_dims.height) && y <= (block
            .center().y-spec.doodle_dims.height)) {
                return true;
            }
        }
        return false;
    }
}


bool
Doodle::hits_obstacle(Spec const& spec, Block& block) const
{
    int x1 = center.x - spec.doodle_dims.width/2;
    int x2 = center.x + spec.doodle_dims.width/2;
    int y1 = center.y - spec.doodle_dims.height/2;
    int y2 = center.y + spec.doodle_dims.height/2;
    if (block.top_right().x < x1) {
        return false;
    } else if (x2 < block.top_left().x) {
        return false;
    } else if (y1 < block.top_right().y) {
        return false;
    } else if (block.bottom_left().y < y2) {
        return false;
    } else {
        return true;
    }
}


Doodle
Doodle::next(double dt) const
{
    Doodle result(*this);
    result.center.x += dt * velocity.width;
    result.center.y += dt * velocity.height;
    return result;
}


