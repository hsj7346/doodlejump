#include "view.hxx"
#include <string>

namespace {
using Color = ge211::Color;
Color Black{ge211::Color(0,0,0)};

}

View::View(Model const& model)
        : model_(model),
          doodle_sprite(ge211::Image_sprite("doodle2.png")),
          // doodle_sprite({10,10},doodle_color),
          background_sprite(ge211::Image_sprite("background.png")),
          platform_sprite(ge211::Image_sprite("platform1.png")),
          monster_sprite(ge211::Image_sprite("big_blue_monster.png")),
          point_sprite{ge211::Text_sprite(std::to_string(model.doodle.score),
                                          sans)}
{}

void
View::draw(ge211::Sprite_set& set)
{
    set.add_sprite(background_sprite, Posn{0,0}, 0);
    ge211::Text_sprite::Builder point_build(sans);
    ge211::Text_sprite::Builder game_build(sans);
    game_build.add_message("Game Over");
    game_build.color(Black);
    point_build.add_message(std::to_string(model_.doodle.score));
    point_build.color(Black);
    point_sprite.reconfigure(point_build);
    gameover_sprite.reconfigure(game_build);
    set.add_sprite(point_sprite, {model_.spec.block_spacing * 4,10}, 2);
    if (model_.game_over) {
        set.add_sprite(doodle_sprite, {model_.doodle.center.x-model_
        .spec.doodle_adj,
                                       model_.spec.background_dims.height}, 2);
        set.add_sprite(gameover_sprite,{125,model_.spec.block_spacing * 4},2);
    } else {
        Position dood {model_.doodle.center.x - model_.spec.doodle_adj, model_
        .doodle.center.y - model_.spec.icon_center_x};
        set.add_sprite(doodle_sprite, dood, 2);
    }
    for (int i = 0; i < model_.blocks.size(); i++) {
        set.add_sprite(platform_sprite,{model_.blocks[i].x, model_.blocks[i]
        .bottom_left().y},1);
    }
    for (int i = 0; i < model_.monsters.size(); i++) {
        set.add_sprite(monster_sprite,{model_.monsters[i].x, model_.monsters[i]
                .bottom_left().y},1);
    }
}

