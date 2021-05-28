#pragma once

#include <chrono>
#include <cmath>
#include <random>
#include <set>
#include <SFML/Graphics.hpp>
#include <boidsECS/bresenham.hpp>
#include <boidsECS/helpers.hpp>
#include <boidsECS/ECS.hpp>

namespace flocon_generator
{
    struct particle
    {
        position2i position;
        position2i speed;
    };

    template<size_t N>
    void remove_particle(std::array<particle, N>& particles, size_t& size, size_t index)
    {
        if (index == size - 1)
        {
            size -= 1;
        } 
        else
        {
            particles[index] = particles[size - 1];
            size -=1 ;
        }
    }

    template<size_t N, typename RngX, typename RngY, typename RngSpeed, typename RngSide>
    void replace_particle(std::array<particle, N>& particles, size_t& size, size_t index, RngX& rngX, RngY& rngY, RngSpeed& rngSpeed, RngSide& rngSide, int blocks_w, int blocks_h)
    {
        particle new_part;
        new_part.position = {rngX(), rngY()};

        position2i speed;
        while(speed.x == 0 || speed.y == 0)
        {
            speed = {rngSpeed(), rngSpeed()};
        }

        new_part.speed = speed;
        

        if (rngSide() == 0)
        {
            new_part.position.x = 0;
        }
        else if (rngSide() == 1)
        {
            new_part.position.x = blocks_w - 1;
        }
        else if (rngSide() == 2)
        {
            new_part.position.y = 0;
        }
        else
        {
            new_part.position.y = blocks_h - 1;
        }

        particles[index] = new_part;
    }

    template<typename Rng>
    void add_pos_to_structure(std::set<position2i>& structure, position2i const& pos, Rng& rng)
    {
        position2i randadd{0,0};
        /*if (rng() == 0)
        {
            randadd.x = 1;
        }
        else if (rng() == 1)
        {
            randadd.x = -1;
        }
        else if (rng() == 2)
        {
            randadd.y = 1;
        }
        else
        {
            randadd.y = -1;
        }*/

        structure.insert({pos.x + randadd.x, pos.y + randadd.y});
    }

    void move_particle(particle& particle, int blocks_w, int blocks_h)
    {
        position2i wanted_pos = {particle.position.x +particle.speed.x, particle.position.y + particle.speed.y};

        if (wanted_pos.x < 0)
        {
            particle.position.x = 0;
            particle.speed.x = -particle.speed.x;
        }
        else if (wanted_pos.x >= blocks_w)
        {
            particle.position.x = blocks_w - 1;
            particle.speed.x = -particle.speed.x;
        }
        else if (wanted_pos.y < 0)
        {
            particle.position.y = 0;
            particle.speed.y = -particle.speed.y;
        }
        else if (wanted_pos.y >= blocks_h)
        {
            particle.position.y = blocks_h - 1;
            particle.speed.y = -particle.speed.y;
        }
        else
        {
            particle.position = wanted_pos;
        }
    }

    void flocon_generator(sf::RenderWindow& window, int blocks_w, int blocks_h, int block_size)
    {
        using std::set;
        using std::size_t;
        using std::array;
        using std::mt19937_64;

        static const size_t num_particles = 5000;
        static const int max_speed = 200;
        static const int initial_structure_size = 5;
        static const int border_size = 100;

        set<position2i> structure;

        size_t particles_size = 0;
        array<particle, num_particles> particles;

        //fill particles with random shit
        mt19937_64 mt_rand(time(0));
        mt19937_64::result_type seed = time(0);
        auto speed_rand = std::bind(std::uniform_int_distribution<int>(-max_speed, max_speed),
                                mt19937_64(seed++));
        
        auto pos_rand_x = std::bind(std::uniform_int_distribution<int>(0, blocks_w),
                        mt19937_64(seed++));
        auto pos_rand_y = std::bind(std::uniform_int_distribution<int>(0, blocks_h),
                        mt19937_64(seed++));

        auto grow_rand = std::bind(std::uniform_int_distribution<int>(0, 3),
                        mt19937_64(seed++));
        for (size_t i = 0; i < num_particles; i++)
        {
            particle particle;
            particle.position = {pos_rand_x(), pos_rand_y()};

            position2i speed{0,0};
            while(speed.x == 0 || speed.y == 0)
            {
                speed = {speed_rand(), speed_rand()};
            }
            particle.speed = speed;
            particles[particles_size++] = particle;
        }

        //fill initial structure

        //structure.insert({blocks_w/2, blocks_h/2});
        //structure.insert({blocks_w/2 - 50, blocks_h/2 - 50});
        //structure.insert({blocks_w/2 + 50, blocks_h/2 - 50});
        //structure.insert({blocks_w/2, blocks_h/2 + 50});
        for (int i = 0; i < initial_structure_size; i++)
        {
            structure.insert({blocks_w/2, blocks_h/2 + i});
        }

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            //move and detect collision and update structure if collision
            //or bounce if we go into a wall
            std::vector<size_t> to_delete;
            for (size_t i = 0; i < particles_size; i++)
            {
                auto bresen_result = bresenham::bresenham(particles[i].position, {particles[i].position.x + particles[i].speed.x, particles[i].position.y + particles[i].speed.y});

                bool next = false;
                for (size_t j = 1; j < bresen_result.size(); j++)
                {
                    position2i pos = bresen_result[j];

                    if (structure.contains(pos) && 
                        pos.x < blocks_w - border_size &&
                        pos.x > border_size &&
                        pos.y > border_size &&
                        pos.y < blocks_h - border_size)
                    {
                        add_pos_to_structure(structure, bresen_result[j - 1], grow_rand);
                        to_delete.push_back(i);
                        next = true;
                        break;
                    }
                }

                if (next)
                {
                    continue;
                }
                else
                {
                    //move the particle
                    move_particle(particles[i], blocks_w, blocks_h);
                }
            }

            for (auto i : to_delete)
            {
                replace_particle(particles, particles_size, i, pos_rand_x, pos_rand_y, speed_rand, grow_rand, blocks_w, blocks_h);
            }

            sf::RectangleShape block({(float)block_size, (float)block_size});
            for(auto const& pos : structure)
            {
                block.setPosition({(float) pos.x*block_size, (float) (blocks_h - pos.y)*block_size});
                window.draw(block);
            }

            
            window.display();
        }
    }

    struct config
    {
        const int blocks_w;
        const int blocks_h;
        const int block_size;
        const int border_size;
        const int max_speed;
    };

    struct PositionUpdateSystem
    {
        PositionUpdateSystem(config const& cfg_) : cfg{cfg_}
        {}

        void operator()(position2i& pos, speed2i& speed)
        {
            pos = {pos.x + speed.x, pos.y + speed.y};

            if (pos.x < 0)
            {
                pos.x = 0;
                speed.x = -speed.x;
            }
            else if (pos.x >= cfg.blocks_w)
            {
                pos.x = cfg.blocks_w - 1;
                speed.x = -speed.x;
            }
            else if (pos.y < 0)
            {
                pos.y = 0;
                speed.y = -speed.y;
            }
            else if (pos.y >= cfg.blocks_h)
            {
                pos.y = cfg.blocks_h - 1;
                speed.y = -speed.y;
            }
        }

        config const& cfg;
    };

    class gstructure
    {

    };

    //TODO: change the seed mechanism. right now: hardcoded
    class CollisionSystem
    {

    public:
        CollisionSystem(std::set<position2i>& structure_, config const& cfg_) 
        : structure{structure_}, 
            cfg{cfg_}, 
            speed_rand{std::bind(std::uniform_int_distribution<int>(-cfg_.max_speed, cfg_.max_speed),
                    std::mt19937_64(0))},
            pos_rand_x{std::bind(std::uniform_int_distribution<int>(0, cfg_.blocks_w),
                    std::mt19937_64(1))},
            pos_rand_y{std::bind(std::uniform_int_distribution<int>(0, cfg_.blocks_h),
                    std::mt19937_64(2))},
            side_rand{std::bind(std::uniform_int_distribution<int>(0, 3),
                    std::mt19937_64(3))}           
        {}

        void reinitialize_particle(position2i& position, speed2i& speed)
        {
            position = {pos_rand_x(), pos_rand_y()};
            speed = {0, 0};

            while (speed.x == 0 || speed.y == 0)
            {
                speed = {speed_rand(), speed_rand()};
            }

            int side = side_rand();
            if (side == 0)
            {
                position.x = 0;
            }
            else if (side == 1)
            {
                position.x = cfg.blocks_w - 1;
            }
            else if (side == 2)
            {
                position.y = 0;
            }
            else
            {
                position.y = cfg.blocks_h - 1;
            }
        }

        void operator()(position2i& pos, speed2i& speed)
        {
            //opti: maybe use a generator here with coroutines
            auto bresen_result = bresenham::bresenham(pos, {pos.x + speed.x, pos.y + speed.y});
            
            for (std::size_t i = 1; i < bresen_result.size(); i++)
            {
                position2i bresen_pos = bresen_result[i];

                if (structure.contains(bresen_pos) && 
                        bresen_pos.x < cfg.blocks_w - cfg.border_size &&
                        bresen_pos.x > cfg.border_size &&
                        bresen_pos.y > cfg.border_size &&
                        bresen_pos.y < cfg.blocks_h - cfg.border_size)
                    {
                        for (int j = 1; (int)i - j >= 0 && j < 2; j++)
                        {
                            structure.insert(bresen_result[i - j]);
                        }
                        reinitialize_particle(pos, speed);
                        break;
                    }
            }
        }
    private:
        std::_Bind<std::uniform_int_distribution<int> (std::mt19937_64)>  speed_rand;
        std::_Bind<std::uniform_int_distribution<int> (std::mt19937_64)>  pos_rand_x;
        std::_Bind<std::uniform_int_distribution<int> (std::mt19937_64)>  pos_rand_y;
        std::_Bind<std::uniform_int_distribution<int> (std::mt19937_64)>  side_rand;
        
        //captured externals
        std::set<position2i>& structure;
        config const& cfg;
    };

    struct RenderSystem
    {
        RenderSystem(sf::RenderWindow& window_, std::set<position2i>& structure_, config const& cfg_) : 
            window{window_},
            structure{structure_},
            cfg{cfg_},
            block{{(float)cfg_.block_size, (float)cfg_.block_size}}
        {}

        void operator()()
        {
            window.clear();
            
            for(auto const& pos : structure)
            {
                block.setPosition({(float) pos.x*cfg.block_size, (float) (cfg.blocks_h - pos.y)*cfg.block_size});
                window.draw(block);
            }

            window.display();
        }

        sf::RectangleShape block;
        
        //captured externals
        config const& cfg;
        std::set<position2i>& structure;
        sf::RenderWindow& window;
    };

    void flocon_generator_ECS(sf::RenderWindow& window, int blocks_w, int blocks_h, int block_size)
    {
        using std::set;
        using std::size_t;
        using std::array;
        using std::mt19937_64;


        set<position2i> structure;
        config const cfg{blocks_w, blocks_h, block_size, 100, 10};
        const int num_particles = 100000;
        const int initial_structure_size = 10;

        mt19937_64::result_type seed = time(0);
        auto speed_rand = std::bind(std::uniform_int_distribution<int>(-cfg.max_speed, cfg.max_speed),
                        mt19937_64(seed++));
        auto pos_rand_x = std::bind(std::uniform_int_distribution<int>(0, blocks_w),
                        mt19937_64(seed++));
        auto pos_rand_y = std::bind(std::uniform_int_distribution<int>(0, blocks_h),
                        mt19937_64(seed++));

        using gmeta::types_t;
        ECS<types_t<
            Archetype<1024, position2i, speed2i>>,
        types_t<
            CollisionSystem,
            PositionUpdateSystem
        >> ecs{
            CollisionSystem{structure, cfg}, 
            PositionUpdateSystem{cfg}};

            //init
            for (int i = 0; i < num_particles; i++)
            {
                position2i pos = {pos_rand_x(), pos_rand_y()};
                speed2i speed = {0,0};
                while(speed.x == 0 || speed.y == 0)
                {
                    speed = {speed_rand(), speed_rand()};
                }
                ecs.AddEntity(pos, speed);
            }
            for (int i = 0; i < initial_structure_size; i++)
            {
                structure.insert({blocks_w/2, blocks_h/2 + i});
            }


            using Clock = std::chrono::high_resolution_clock;
            auto time_start = Clock::now();
            size_t frame_counter = 0;
            sf::RectangleShape block({(float)block_size, (float)block_size});
            while(window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                ecs.tick();

                window.clear();
                for(auto const& pos : structure)
                {
                    block.setPosition({(float) pos.x*cfg.block_size, (float) (cfg.blocks_h - pos.y)*cfg.block_size});
                    window.draw(block);
                }
                window.display();
                
                //FPS shit
                const size_t report_each_N_frames = 50;
                frame_counter++;
                if (frame_counter == report_each_N_frames)
                {
                    auto now = Clock::now();
                    auto elapsed = time_start - now;
                    double fps = report_each_N_frames * 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
                    std::cout << "FPS = " << fps << std::endl;
                    frame_counter = 0;
                    time_start = now;
                }
            }
    }
}