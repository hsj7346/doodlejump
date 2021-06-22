#include "model.hxx"
#include <catch.hxx>

Spec const spec;


TEST_CASE("doodle simulates gravity"){
    ///The doodle will simulate gravity by moving up for a set amount of time
    /// with a vertical velocity that gets smaller and smaller before falling
    /// back down with a negative vertical velocity indefinitely until it either
    /// hits the bottom of the screen or it hits a ‘block’.

    Model m(spec);

    int blocksize = m.blocks.size() - 1;
    //clear out all platforms except starting platform
    for (int i = 0; i < blocksize; i++) {
        m.blocks[blocksize - i] = m.blocks.back();
        m.blocks.pop_back();
    }
    m.doodle.live = true;

    Position expected_center = m.doodle.center;
    Velocity expected_velocity = m.doodle.velocity;

    //velocity when first launched
    CHECK(m.doodle.velocity.height == -5400);
    CHECK(expected_velocity.height == -5400);
    CHECK(m.doodle.center == expected_center);
    CHECK(m.doodle.center.y == 680); //we cant check the x coord bc the platform
    //is randomly assigned an x coord

    double dt = 0.01;
    m.progress(dt);
    expected_velocity.height += spec.doodle_accel;
    CHECK(m.doodle.velocity.height == expected_velocity.height);
    CHECK(m.doodle.velocity.height == -5300);
    expected_center.y = 627; ///goes up by 53
    CHECK(m.doodle.center == expected_center);

    m.progress(dt);
    expected_velocity.height += spec.doodle_accel;
    CHECK(m.doodle.velocity.height == expected_velocity.height);
    expected_center.y = 575;
    CHECK(m.doodle.center == expected_center); ///goes up by 52;

    expected_center.y += ceil(dt * m.doodle.velocity.height);
    m.progress(dt);
    expected_velocity.height += spec.doodle_accel;
    CHECK(m.doodle.velocity.height == expected_velocity.height);
    expected_center.y = 524;
    CHECK(m.doodle.center == expected_center);///goes up by 51;

    dt = 0.05;
    m.progress(dt);
    CHECK(m.doodle.center.y == 274); ///goes up by 250
    CHECK(m.doodle.velocity.height == -5000);

    dt = 0.0001;
    for (int i = 0; i < 50; i++) {
        m.progress(dt);
    }

    CHECK(m.doodle.velocity.height == 0); //top of its arc

    m.progress(dt);
    CHECK(m.doodle.velocity.height == 5400); //going down at doodle fall
    // velocity
    CHECK(m.doodle.velocity.height == spec.doodle_velocity.height*-1);

}

TEST_CASE("blocks move down") {
    ///5. The blocks will move down the screen as the game progresses in order
    /// to simulate the doodle moving upward. The background will move down
    /// every time the doodle hits a new block and jumps. //model test

    Model m(spec);
    m.doodle.live = true;

    Position posn{m.blocks[1].center().x, (m.blocks[1].top_left().y-16)};
    m.doodle.velocity.height = 110;
    m.doodle.center = posn;
    CHECK(m.doodle.hits_block_top(spec, m.blocks[1]) == false); // hits a block

    Position oldblockposn{m.blocks[1].top_left()};
    CHECK(oldblockposn.y == 600);

    Position BLOCK2POSN{m.blocks[2].top_left()};
    Position BLOCK3POSN{m.blocks[3].top_left()};

    CHECK(m.doodle.next(0.1).hits_block_top(spec, m.blocks[1]) == true); //
    // hits a block

    m.progress(0.1);
    m.progress(0.01);
    int speed = m.doodle.velocity.height * -1 + 1500;
    oldblockposn.y += 0.01 * speed;
    CHECK(m.blocks[1].top_left().y == oldblockposn.y);
    CHECK(m.blocks[1].top_left().y == 652); ///moves down by 52
    BLOCK2POSN.y += 52;
    BLOCK3POSN.y += 52;

    CHECK(m.blocks[2].top_left().y == BLOCK2POSN.y);
    CHECK(m.blocks[3].top_left().y == BLOCK3POSN.y); ///all the blocks move down


}

TEST_CASE("doodle hit block and bounce") {
    Model m(spec);
    m.doodle.live = true;

    Position posn{m.blocks[1].center().x, (m.blocks[1].top_left().y-16)};
    m.doodle.velocity.height = 110;
    m.doodle.center = posn;
    CHECK(m.doodle.hits_block_top(spec, m.blocks[1]) == false);

    CHECK(m.doodle.next(0.1).hits_block_top(spec, m.blocks[1]) == true);// hits a block

    m.doodle.velocity.height = -100; //doodle is now going up not down
    m.doodle.center = Position{m.blocks[1].center().x, (m.blocks[1].top_left
                                                                           ().y-10)};
    CHECK(m.doodle.next(0.01).hits_block_top(spec, m.blocks[1]) == false);

    m.doodle.velocity.height = 110; // back to going down
    m.doodle.center = posn;
    CHECK(posn.y == 584);
    m.progress(0.1);
    CHECK(m.doodle.center.y == 204); //after hitting the block, the doodle
    //started going up and now is 380 pixels
    // above its position before hitting the
    // block!
    CHECK(m.doodle.velocity.height == -3800);
    ///in our specification we said the doodle's velocity is changed to
    /// positive after it bounces. by positive, we meant it would go up,
    /// and forgot that in this game, the y axis starts with a 0 at the
    /// top and gets bigger going down. so really, the specs should say
    /// that the doodle's velocity is changed to NEGATIVE

}

TEST_CASE("doodle falls") {
    Model m(spec);
    m.blocks.clear();
    m.doodle.center = {250, 400}; ///place doodle in center of screen
    m.doodle.velocity.height = 200; //doodle is falling
    m.doodle.live = true;

    m.progress(1);
    CHECK(m.doodle.center.y == 600);
    CHECK(m.game_over == false);
    m.progress(1.1);
    CHECK(m.doodle.center.y == 820);
    CHECK(m.game_over == true);

}

TEST_CASE("doodle hit obstacle") {
    Model m(spec);
    m.blocks.clear();
    m.monsters.clear();
    Block block{235,385,30,30};
    m.monsters.push_back(block);
    CHECK(m.monsters.size() == 1);
    m.doodle.live = true;

    m.doodle.center = {250,400}; // from bottom
    CHECK(m.doodle.hits_obstacle(spec, m.monsters[0]) == true);
    CHECK(m.doodle.next(0.001).hits_obstacle(spec, m.monsters[0]) == true);
    m.progress(0.001);
    CHECK(m.game_over == true);

    Model m2(spec); //from left side
    m2.blocks.clear();
    m2.monsters.clear();
    Block block2{235,385,30,30};
    m2.monsters.push_back(block);
    CHECK(m2.monsters.size() == 1);
    m2.doodle.live = true;

    m2.doodle.velocity = {1000, 0};
    m2.doodle.center = {231, 400};
    CHECK(m2.doodle.hits_obstacle(spec, m2.monsters[0]) == true);
    CHECK(m2.doodle.next(0.001).hits_obstacle(spec, m2.monsters[0]) == true);
    m2.progress(0.001);
    CHECK(m2.game_over == true);

    Model m3(spec); //from right side
    m3.blocks.clear();
    m3.monsters.clear();
    Block block3{235,385,30,30};
    m3.monsters.push_back(block);
    CHECK(m3.monsters.size() == 1);
    m3.doodle.live = true;

    m3.doodle.center = {269, 400};
    m3.doodle.velocity = {-1000, 0};
    CHECK(m3.doodle.hits_obstacle(spec, m3.monsters[0]) == true);
    CHECK(m3.doodle.next(0.001).hits_obstacle(spec, m3.monsters[0]) == true);
    m3.progress(0.001);
    CHECK(m3.game_over == true);

    Model m4(spec); //from top
    m4.blocks.clear();
    m4.monsters.clear();
    Block block4{235,385,30,30};
    m4.monsters.push_back(block);
    CHECK(m4.monsters.size() == 1);
    m4.doodle.live = true;

    m4.doodle.center = {250, 391};
    m4.doodle.velocity = {0, 1000};
    CHECK(m4.doodle.hits_obstacle(spec, m4.monsters[0]) == true);
    CHECK(m4.doodle.next(0.001).hits_obstacle(spec, m4.monsters[0]) == true);
    m4.progress(0.001);
    CHECK(m4.game_over == true);
}