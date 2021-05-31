#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glxext.h>

#include <iostream>

#include <boidsECS/ECS.hpp>
#include <boidsECS/bresenham_visutool.hpp>
#include <boidsECS/flocon_generator.hpp>

using std::cout,
    std::endl;

void testArchetypeChunk()
{
    cout << "testArchetypeChunk " << endl;
    cout << "test getComponent" << endl;
    auto ac = ArchetypeChunk<32, int, double, char>{};

    auto &intComponent = ac.getComponent<int>();

    int it = 0;
    for (auto &i : intComponent)
    {
        i = it++;
    }

    intComponent = ac.getComponent<int>();

    for (auto &i : intComponent)
    {
        cout << i << endl;
    }

    cout << "test addEntity" << endl;

    ac.AddEntity(0, 69, 3.1416, 'x');
    ac.AddEntity(1, 420, 1.1416, 'y');
    ac.AddEntity(2, 0, 0.1416, 'z');

    ac.foreach ([](size_t id, int a, double b, char c) {
        cout << id << " " << a << " " << b << " " << c << endl;
    });

    cout << "test addEntity againe" << endl;
    ac.AddEntity(3, 320, 5.1416, 'a');
    ac.foreach ([](size_t id, int a, double b, char c) {
        cout << id << " " << a << " " << b << " " << c << endl;
    });

    cout << "test removeEntityById" << endl;
    bool removed = ac.DeleteEntityById(2);
    ac.foreach ([](size_t id, int a, double b, char c) {
        cout << id << " " << a << " " << b << " " << c << endl;
    });

    cout << "removed: " << removed << endl;

    cout << "test foreach" << endl;

    auto fn = [](size_t id, int a, double b, char c) {
        cout << "id: " << id << ", a:" << a << ", b:" << b << ", c:" << c << endl;
    };

    ac.foreach (fn);

    ac.foreach ([](char c, int a) {
        cout << c << a;
    });

    cout << "-------------" << endl;
}

void testArchetype()
{
    cout << "testArchetype " << endl;

    auto arc = Archetype<2, int, double, char>{};

    //should not work, 2 of same type in same archetype is not permitted
    //auto arc_failure = Archetype<32, int, double, char, int>{};

    arc.AddEntity(0, 1, 1.1, 'a');
    arc.AddEntity(1, 3, 1.2, 'b');
    arc.AddEntity(2, 5, 1.3, 'c');

    arc.foreach ([](char c, size_t id) {
        cout << c << " " << id << endl;
    });
    cout << "-------------" << endl;

    arc.foreach ([](char c, size_t id) {
        cout << c << " " << id << endl;
    });

    cout << "-------------" << endl;
}

void testECS()
{
    cout << "testECS " << endl;

    using arcs_t = gmeta::types_t<
        Archetype<32, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>;

    ECS<arcs_t,
        gmeta::types_t<
            UpdatePositionSystem,
            UpdateSpeedSystem,
            UpdateCounterSystem>>
        ecs;

    ecs.AddEntity(position3f{1, 2, 3}, speed3f{4, 5, 1}, Mass{5.1}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{2, 2, 3}, speed3f{4, 5, 2}, Mass{5.2}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{3, 2, 3}, speed3f{4, 5, 3}, Mass{5.3}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{4, 2, 3}, speed3f{4, 5, 4}, Mass{5.4}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{5, 2, 3}, speed3f{4, 5, 5}, Mass{5.5}, Counter{0});

    ecs.AddEntity(1, 1.1, Counter{0});
    ecs.AddEntity(2, 2.2, Counter{0});

    ecs.DeleteEntity(0);
    ecs.DeleteEntity(2);
    ecs.DeleteEntity(4);

    /*ecs.foreach<Position, Speed, Mass, Counter>(
        [](size_t id, Position pos, Speed speed, Mass mass, Counter counter)
        {
            cout << mass.m << endl;
        });*/

    auto fn = [](size_t id, speed3f speed) {
        cout << "id: " << id << ", speed(x): " << speed.x << endl;
    };

    using fnT = decltype(fn);

    using arc1 = Archetype<32, position3f, speed3f, Mass, Counter>;
    using arc2 = Archetype<2, int, double, Counter>;

    //test subset_of_t<types_t<Ts...>, remove_sizeT_from_archetype_t<FirstArchetype>
    //using test12 = gmeta::subset_of_t<types_t<size_t, >, remove_sizeT_from_archetype_t<FirstArchetype>

    //test get_archs_from_argument_types_inner_t
    using test11 = gmeta::get_archs_from_argument_types_inner_t<gmeta::types_t<arc1, arc2>, gmeta::types_t<size_t, speed3f>, gmeta::types_t<>>;
    if constexpr (gmeta::gis_same<test11, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_argument_types_inner_t success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_argument_types_inner_t failure" << std::endl;
    }

    //test get_archs_from_argument_types_t
    using test0 = gmeta::get_archs_from_argument_types_t<gmeta::types_t<arc1, arc2>, gmeta::types_t<size_t, speed3f>>;
    if constexpr (gmeta::gis_same<test0, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_argument_types_t success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_argument_types_t failure" << std::endl;
    }

    // test get_archs_from_system
    using testMatchingArchetypes = gmeta::get_archs_from_system<
        gmeta::types_t<arc1, arc2>,
        fnT>::type;
    if constexpr (gmeta::gis_same<testMatchingArchetypes, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_system success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_system failure" << std::endl;
    }

    // test get_archs_from_system_t
    using testMatchingArchetypes_t = gmeta::get_archs_from_system_t<
        gmeta::types_t<arc1, arc2>,
        fnT>;
    if constexpr (gmeta::gis_same<testMatchingArchetypes_t, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_system_t success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_system_t failure" << std::endl;
    }

    ecs.foreach ([](speed3f &speed) {
        //cout << "id: " << id << ", speed(x): " << speed.x << endl;
        speed.x += 1;
        speed.print();
    });

    ecs.foreach ([](size_t &id) {
        cout << "id: " << id++ << endl;
    });

    const auto countersys = UpdateCounterSystem{};
    const auto possys = UpdatePositionSystem{};
    const auto speedsys = UpdateSpeedSystem{};

    using matchingArchsPosSys = gmeta::get_archs_from_system_t<arcs_t, UpdatePositionSystem>;

    //using matchingArchsPosSysArgumentsTypeInner = gmeta::

    //Archetype<32, Position, Speed, Mass, Counter>
    //Archetype<2, int, double, Counter>
    if constexpr (gmeta::gis_same<matchingArchsPosSys, gmeta::types_t<Archetype<32, position3f, speed3f, Mass, Counter>>>::value)
    {
        cout << "success matchingArchsPosSys" << endl;
    }
    else
    {
        cout << "failure matchingArchsPosSys" << endl;
    }

    ecs.foreach (countersys);
    ecs.foreach (possys);
    ecs.foreach (speedsys);

    ecs.tick();

    cout << "-------------" << endl;
}

constexpr int BLOCKS_W = 1001;
constexpr int BLOCKS_H = 1001;
constexpr int BLOCK_PIXELS = 1;

int main()
{
    if (true)
    {
        sf::RenderWindow window(sf::VideoMode(BLOCKS_W* BLOCK_PIXELS, BLOCKS_H* BLOCK_PIXELS), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        GLuint vbo;

        glGenBuffers(1, &vbo);

        /*while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            //window.draw(shape);
            
            //bresenham_visutool::draw_grid(window, BLOCKS_W, BLOCKS_H, BLOCK_PIXELS);
            //bresenham_visutool::draw_bresenham(window, BLOCKS_W, BLOCKS_H, BLOCK_PIXELS);
            
            window.display();
        }*/

        //flocon_generator::flocon_generator(window, BLOCKS_W, BLOCKS_H, BLOCK_PIXELS);
        flocon_generator::flocon_generator_ECS<(std::size_t)BLOCKS_W, (std::size_t)BLOCKS_H>(window, BLOCK_PIXELS);
    }

    return 0;
}