#pragma once

#include <ge211.hxx>
#include "specification.hxx"
#include "doodle.hxx"
#include <vector>

class Model
{
public:
    explicit Model(Spec const& spec = Spec());



    void start_game();

    void progress(double dt);

    std::vector<Block> next(std::vector<Block> blocks, int, double) const;

    Spec const spec;

    Doodle doodle;

    std::vector<Block> blocks;

    std::vector<Block> monsters;

    Block * current_;

    bool game_over;

    ge211::Random_source<int> random_block_x;

};
