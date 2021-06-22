#include "controller.hxx"
#include "model.hxx"

static double dt = 0.001;

Controller::Controller(Model& model1)
        : model(model1),
          view(model1)
{ }


void
Controller::on_key(ge211::Key key)
{
    if (key == ge211::Key::code('q')) {
        quit();
    }
    if (key == ge211::Key::code(' ')) {
        model.start_game();
    }
    if (model.doodle.live) {
        if (key == ge211::Key::left()) {
            if (model.doodle.center.x >= model.spec.doodle_velocity.width) {
                model.doodle.center.x -= doodle_x_move;
            } else {
                model.doodle.center.x = model.spec.background_dims.width;
                model.doodle.center.x -= doodle_x_move;
            }
        }
        if (key == ge211::Key::right()) {
            if (model.doodle.center.x <= model.spec.background_dims.width){
                model.doodle.center.x += doodle_x_move;
            } else {
                model.doodle.center.x = model.spec.doodle_velocity
                        .width-model.spec.block_dims.height;
                model.doodle.center.x += doodle_x_move;
            }
        }
    }
}



void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int>)
{
    if (!model.doodle.live) {
        model.start_game();
    }
}

void
Controller::draw(ge211::Sprite_set& set)
{
    view.draw(set);
}

void
Controller::on_frame(double)
{
    model.progress(dt);
}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return model.spec.background_dims;
}

