#pragma once

#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <boidsECS/helpers.hpp>
#include <boidsECS/ECS.hpp>

namespace smash_game
{
    struct GameData
    {
        uint target_fps;
        int max_speed;
        AspectRatio aspect_ratio;
        muint border_size;
        int biggest_sprite;
    };

    static constexpr GameData game_data
    {
        120,
        603979776,
        AspectRatio{16, 9},
        (muint(1) << 23),
        500
    };

    struct PositionComponent : sf::Vector2u
    {
    };

    struct SpeedComponent : sf::Vector2i
    {
    };

    struct joytick_state
    {
        sf::Int32 x;
        sf::Int32 y;

        static constexpr sf::Int32 max_val = 203979776;
    };

    struct InputsComponent
    {
        std::array<sf::Uint32, 2> input_state;
        joytick_state movement_joystick;
    };

    struct InputSystem
    {
        InputSystem(sf::Window& window_)
        :window(window_)
        {}

        void pre_tick()
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                        window.close();
                        break;
                }
            }
        }

        void post_tick()
        {

        }

        void operator()(Id id, InputsComponent& inputsComponent, PositionComponent& position, SpeedComponent& speed, DeltaTime dt) const
        {
            inputsComponent.movement_joystick.x = 0;
            inputsComponent.movement_joystick.y = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                inputsComponent.movement_joystick.x -= joytick_state::max_val;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                inputsComponent.movement_joystick.x += joytick_state::max_val;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                inputsComponent.movement_joystick.y += joytick_state::max_val;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                inputsComponent.movement_joystick.y -= joytick_state::max_val;
            }

            //std::cout << "input: " << double(dt) <<std::endl;
        }

        //captures
        sf::Window& window;
    };

    struct PositionUpdateSystem
    {
        void pre_tick()
        {
        }
        void post_tick()
        {

        }
        void operator()(Id id, InputsComponent const& input, PositionComponent& position, SpeedComponent& speed, DeltaTime dt) const
        {
            speed.x += input.movement_joystick.x;
            speed.x = std::clamp(speed.x, -game_data.max_speed, game_data.max_speed);
            speed.y += input.movement_joystick.y;
            speed.y = std::clamp(speed.y, -game_data.max_speed, game_data.max_speed);

            position.x += speed.x*double(dt);
            position.y += speed.y*double(dt);

            //std::cout << "position" << std::endl;
            //std::cout << speed.x << ", " << speed.y << std::endl;
        }
    };

    struct RenderSystem
    {
        RenderSystem(sf::RenderWindow& window_, sf::RenderTexture& render_texture_)
        :window(window_), render_texture(render_texture_)
        {}
        
        void pre_tick()
        {
            render_texture.clear();
            window.clear();
        }

        void post_tick()
        {   
            render_texture.display();
            auto render_sprite = sf::Sprite(render_texture.getTexture());
            render_sprite.setPosition({0,0});
            window.draw(render_sprite);
            window.display();
        }

        //basically a linear transformation, could use linear algebra library to do that stuff
        static sf::Vector2u model_to_screen_pos(sf::Vector2i const& screen_dimentions, sf::Vector2u const& model_pos)
        {
            muint screen_w = screen_dimentions.x;
            muint screen_h = screen_dimentions.y;

            muint model_camera_w = ((~muint(0)) - (game_data.border_size << 1));
            muint model_camera_start_x = game_data.border_size;
            uint64_t b_x = (uint64_t(model_camera_start_x) * uint64_t(screen_w)) / uint64_t(model_camera_w);
            uint64_t result_x = uint64_t(screen_w) * uint64_t(model_pos.x) / uint64_t(model_camera_w) - b_x;

            uint64_t model_camera_h = (uint64_t(game_data.aspect_ratio.y) * uint64_t(model_camera_w)) / game_data.aspect_ratio.x;
            muint model_camera_start_y = ((~muint(0)) - model_camera_h) >> 1;
            uint64_t b_y = (uint64_t(model_camera_start_y) * uint64_t(screen_h)) / uint64_t(model_camera_h);
            uint64_t result_y = uint64_t(screen_h) * uint64_t(model_pos.y) / uint64_t(model_camera_h) - b_y;

            return sf::Vector2u(uint(result_x), screen_h - uint(result_y));
        }

        void operator()(Id id, PositionComponent const& position)
        {
            sf::RectangleShape box({float(50), float(50)});

            sf::Vector2u screen_pos = model_to_screen_pos(sf::Vector2i(window.getSize()), position);

            box.setPosition(screen_pos.x + game_data.biggest_sprite, screen_pos.y);
            box.setFillColor(sf::Color::White);
            render_texture.draw(box);
            //window.draw(box);   
        }

        //captures
        sf::RenderWindow& window;
        sf::RenderTexture& render_texture;
    };

    void game()
    {
        constexpr int window_w = 1600;
        constexpr int window_h = 900;

        constexpr int window_texture_w = window_w + game_data.biggest_sprite;
        constexpr int window_texture_h = window_h + game_data.biggest_sprite;

        constexpr sf::Int32 border_size_w = 2<<23;
        constexpr sf::Int64 stage_units_w = (static_cast<sf::Int64>(2) << 32) - 1 - 2*border_size_w;
        constexpr sf::Int64 stage_units_h = (9*stage_units_w)/16;
        constexpr sf::Int64 border_size_h_64 = ((static_cast<sf::Int64>(2) << 32) - 1 - (9*stage_units_w)/16)/2;
        constexpr sf::Int32 border_size_h = static_cast<sf::Int32>(border_size_h_64);
        constexpr sf::Int32 biggest_sprite_ever = 500;

        sf::RenderWindow window(sf::VideoMode(window_w, window_h), "smash_game", sf::Style::Titlebar | sf::Style::Close);
        sf::RenderTexture render_texture{};
        if (!render_texture.create(window_texture_w, window_texture_h))
        {
            std::cout << "Could not create render texture" << std::endl;
            abort();
        }

        //sf::View view;
        //view.reset(sf::FloatRect{static_cast<float>(border_size_w), float(border_size_h), float(stage_units_w), float(9*stage_units_w/16)});
        //view.setViewport(sf::FloatRect{0.f, 0.f, 1.f, 1.f});
        //window.setView(view);

        using gmeta::types_t;
        ECS<types_t<
            Archetype<
                1024, 
                InputsComponent, 
                PositionComponent, 
                SpeedComponent>
            >,
        types_t<
            InputSystem,
            PositionUpdateSystem,
            RenderSystem
        >> ecs{
            InputSystem{window}, 
            PositionUpdateSystem{},
            RenderSystem{window, render_texture}};

        ecs.AddEntity(InputsComponent{}, PositionComponent{{muint(1)<<31, muint(1)<<31}}, SpeedComponent{{0,0}});

        std::chrono::_V2::system_clock::time_point time_now;
        std::chrono::_V2::system_clock::time_point time_previous_frame = std::chrono::high_resolution_clock::now();
        constexpr int nanosecs_per_frame = 1000000000 / game_data.target_fps;
        while (window.isOpen())
        {
            time_previous_frame = std::chrono::high_resolution_clock::now();
            ecs.tick();

            time_now = std::chrono::high_resolution_clock::now();
            auto dt = time_now - time_previous_frame;
            
            if (dt.count() < nanosecs_per_frame)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds{nanosecs_per_frame - dt.count()});
            }
        }
    }
}