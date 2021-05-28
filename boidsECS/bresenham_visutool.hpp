#pragma once

#include <SFML/Graphics.hpp>
#include <boidsECS/bresenham.hpp>
#include <boidsECS/helpers.hpp>

namespace bresenham_visutool
{
    void draw_grid(sf::RenderWindow& window, int blocks_w, int blocks_h, int block_size)
    {
        //horiontal lines
        for (int i = 0; i <= blocks_h; i++)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(0, i*block_size)),
                sf::Vertex(sf::Vector2f(blocks_w*block_size, i*block_size))
            };
            window.draw(line, 2, sf::Lines);
        }

        //vertical lines
        for (int i = 0; i <= blocks_w; i++)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(i*block_size, 0)),
                sf::Vertex(sf::Vector2f(i*block_size, block_size*blocks_h))
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    void draw_bresenham(sf::RenderWindow& window, int blocks_w, int blocks_h, int block_size)
    {
        sf::Vector2i mouse_position = sf::Mouse::getPosition(window);

        int mouse_block_x = mouse_position.x / block_size;
        int mouse_block_y = blocks_h -  mouse_position.y / block_size;

        position2i center{blocks_w/2, blocks_h/2};
        position2i dst{mouse_block_x, mouse_block_y};

        std::vector<position2i> bresenham_result = bresenham::bresenham(center, dst);

        sf::RectangleShape block({(float)block_size, (float)block_size});
        for (auto const& pos : bresenham_result)
        {
            block.setPosition({(float) pos.x*block_size, (float) (blocks_h - pos.y)*block_size});
            window.draw(block);
        }
    }
}