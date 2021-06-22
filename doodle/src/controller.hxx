#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller(Model&);



protected:
    void on_key(ge211::Key) override;
    void on_mouse_up(ge211::Mouse_button, ge211::Posn<int>) override;
    void draw(ge211::Sprite_set& set) override;
    void on_frame(double) override;
    ge211::Dims<int> initial_window_dimensions() const override;
    Model& model;
    View view;

private:
    int doodle_x_move {35};
//     Model model_;
//     View view_;
};
