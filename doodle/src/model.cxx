#include "model.hxx"

Model::Model(Spec const& spec)
        : spec(spec),
          doodle(spec),
          game_over(false),
          random_block_x(0 ,spec.background_dims
          .width-spec
          .block_dims.width)
{
    for (int i = 0; i < 8; i++) {
        Block block;
        block.x = random_block_x.next();
        block.y = spec.starting_block.y - i * spec.block_spacing;
        block.width = spec.block_dims.width;
        block.height = spec.block_dims.height;
        blocks.push_back(block);
    }
    Block block;
    block.x = random_block_x.next();
    block.y = spec.starting_block.x;
    block.width = spec.monster_dims.width;
    block.height = spec.monster_dims.height;
    monsters.push_back(block);
    current_ = &blocks[0];


    // Block last {blocks.back()};
    doodle.center = {blocks[0].x+spec.icon_center_x,blocks[0]
                     .y-spec.block_dims.height};
}

void
Model::start_game()
{
    doodle.live = true;
}

std::vector<Block>
Model::next(std::vector<Block> nblocks, int speed, double dt) const
{
    std::vector<Block> result(nblocks);
    for (int i = 0; i < result.size(); i++) {
        result[i].y += dt * speed;
    }
    return result;
}

void
Model::progress(double dt)
{
    if (doodle.live) {
        for (int i = 0; i < blocks.size(); i++) {
            if (blocks[i].y > (spec.background_dims.height-spec.block_dims.height)) {
                Block last(blocks.back());
                blocks[i] = last;
                blocks.pop_back();
            }
        }
        for (int i = 0; i < monsters.size(); i++){
            if (monsters[i].y > (spec.background_dims.height-spec.block_dims.height)) {
                Block last(monsters.back());
                monsters[i] = last;
                monsters.pop_back();
            }
        }
        Doodle d(doodle.next(dt));
        int speed = 0;
        if (d.hits_bottom(spec)) {
            doodle.live = false;
            game_over = true;
        } else if (d.hits_obstacle(spec,monsters[0])) {
            doodle.live = false;
            game_over = true;
        } else {
            if (d.velocity.height < 0) {
                doodle.velocity.height += spec.doodle_accel;
                speed = doodle.velocity.height * -1 + 1500;
                Block last(blocks.back());
                if (last.y > 160) {
                    Block block;
                    block.x = random_block_x.next();
                    block.y = 0;
                    block.width = spec.block_dims.width;
                    block.height = spec.block_dims.height;
                    blocks.push_back(block);
                }
                if (monsters.empty()) {
                    Block block;
                    block.x = random_block_x.next();
                    block.y = 0;
                    block.width = spec.monster_dims.width;
                    block.height = spec.monster_dims.height;
                    monsters.push_back(block);
                }
            } else if (d.velocity.height == 0) {
                doodle.velocity.height = spec.doodle_velocity.height * -1;
            } else {
                for (int i = 0; i < blocks.size(); i++) {
                    if (d.hits_block_top(spec, blocks[i])) {
                        if (current_ != &blocks[i]) {
                            doodle.velocity.height = -3800;
                            doodle.score += 1;
                        } else {
                            doodle.velocity.height = spec.doodle_velocity
                                                         .height;
                        }
                        current_ = &blocks[i];
                    }
                }
            }
            Block temp (*current_);
            if (temp.y < (spec.background_dims.height-spec.block_spacing)) {
                std::vector<Block> b(next(blocks, speed, dt));
                blocks = b;
                std::vector<Block> m(next(monsters, speed, dt));
                monsters = m;
            }
        }
        Doodle result(doodle.next(dt));
        doodle = result;
    } else if (game_over) {
        int speed = -10000;
        std::vector<Block> b(next(blocks,speed, dt));
        blocks = b;
        std::vector<Block> m(next(monsters,speed, dt));
        monsters = m;
    }
}

            // if (doodle.velocity.height < 0) {
            //     doodle.center.y += doodle.velocity.height;
            //     for (int i = 0; i < blocks.size(); i ++) {
            //         blocks[i].y += doodle.velocity.height * -1;
            //     }
            //     doodle.velocity.height += spec.doodle_accel;
            //     Block last(blocks.back());
            //     if (last.y > 120) {
            //         Block block;
            //         block.x = random_block_x.next();
            //         block.y = 1;
            //         block.width = spec.block_dims.width;
            //         block.height = spec.block_dims.height;
            //         blocks.push_back(block);
            //     }
            // } else if (doodle.velocity.height == 0) {
            //     doodle.velocity.height = spec.doodle_fall_velocity.height;
            // } else {
            //     doodle.center.y += doodle.velocity.height;
            //     for (int i = 0; i < blocks.size(); i++){
            //         if (doodle.hits_block_top(spec,blocks[i])){
            //             doodle.velocity.height = spec.doodle_velocity
            //                                          .height;
            //         }
            //     }
            // }
//         }
//     }
// }

