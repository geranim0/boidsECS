#include <gtest/gtest.h>
#include <boidsECS/smash_game.hpp>

TEST(smash_game_render_system, model_to_screen_pos)
{
    int screen_w = 1600;
    int screen_h = 900;

    muint model_camera_w = ((~muint(0)) - (smash_game::game_data.border_size << 1));
    int model_camera_start_x = smash_game::game_data.border_size;
    int b_x = -(model_camera_start_x * screen_w) / model_camera_w;

    int model_camera_h = (smash_game::game_data.aspect_ratio.y * model_camera_w) / smash_game::game_data.aspect_ratio.x;
    int model_camera_start_y = ((~muint(0)) - model_camera_h) >> 1;
    int b_y = -(model_camera_start_y * screen_h) / model_camera_h;

    auto left_middle = smash_game::RenderSystem::model_to_screen_pos(
        sf::Vector2i{screen_w, screen_h}, sf::Vector2u{muint(1) << 23, (~muint(0)) >> 1});


    auto right_middle = smash_game::RenderSystem::model_to_screen_pos(
        sf::Vector2i{screen_w, screen_h}, 
        sf::Vector2u{~(muint(0)) - (muint(1) << 23), (~muint(0)) >> 1});

    EXPECT_EQ(left_middle.x, 0);
    EXPECT_EQ(left_middle.y, 450);

    EXPECT_EQ(right_middle.x, 1600);
    EXPECT_EQ(right_middle.y, 450);
}