#pragma once

#include "model.hxx"
#include <ge211.hxx>

class View
{
public:
    // using Color = ge211::Color;
    // using Dims = ge211::Dims<int>;
    // using Sprite = ge211::Sprite;
    // using Sprite_set = ge211::Sprite_set;
    // using Transform = ge211::Transform;
    // using Rect = ge211::Rect<int>;
    using Posn = ge211::Posn<int>;

    explicit View(Model const& model);
    void draw(ge211::Sprite_set& set);


private:
    Model const& model_;
    ge211::Image_sprite const doodle_sprite;
    // ge211::Rectangle_sprite const doodle_sprite;
    ge211::Image_sprite const background_sprite;
    ge211::Image_sprite const platform_sprite;
    ge211::Image_sprite const monster_sprite;
    ge211::Font sans{"sans.ttf", 50};
    ge211::Text_sprite gameover_sprite{"Game Over",sans};
    ge211::Text_sprite point_sprite;

};
