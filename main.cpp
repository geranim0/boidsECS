//#include <SFML/Graphics.hpp>
#include <iostream>

#include "helpers.hpp"
#include "ECS.hpp"

using namespace std;



void testArchetypeChunk()
{
    cout << "testArchetypeChunk " << endl;
    cout << "test getComponent" << endl;
    auto ac = ArchetypeChunk<32, int, double, char>{};

    auto& intComponent = ac.getComponent<int>();

    int it = 0;
    for (auto& i : intComponent)
    {
        i = it++;
    }

    intComponent = ac.getComponent<int>();

    for (auto& i : intComponent)
    {
        cout << i << endl;
    }

    cout << "test addEntity" << endl;

    ac.AddEntity(0, 69, 3.1416, 'x');
    ac.AddEntity(1, 420, 1.1416, 'y');
    ac.AddEntity(2, 0, 0.1416, 'z');

    ac.foreach([](size_t id, int a, double b, char c) {
        cout << id << " "<< a << " "<< b << " "<< c << endl;
        });

    cout << "test addEntity againe" << endl;
    ac.AddEntity(3, 320, 5.1416, 'a');
    ac.foreach([](size_t id, int a, double b, char c) {
        cout << id << " " << a << " " << b << " " << c << endl;
        });

    cout << "test removeEntityById" << endl;
    bool removed = ac.DeleteEntityById(2);
    ac.foreach([](size_t id, int a, double b, char c) {
        cout << id << " " << a << " " << b << " " << c << endl;
        });

    cout << "removed: " << removed << endl;

    cout << "test foreach" << endl;

    auto fn = [](size_t id, int a, double b, char c) {
        cout <<"id: "<< id <<", a:" <<a << ", b:" << b << ", c:" << c << endl;
    };

    ac.foreach(fn);

    ac.foreach([](char c, int a) {
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

    arc.foreach([](char c, size_t id)
        {
            cout << c << " " << id << endl;
        }); 
    cout << "-------------" << endl;

    arc.foreach([](char c, size_t id)
        {
            cout << c << " " << id << endl;
        });

    cout << "-------------" << endl;
}

void testECS()
{
    cout << "testECS " << endl;

    using arcs_t = gmeta::types_t<
        Archetype<32, Position, Speed, Mass, Counter>,
        Archetype<2, int, double, Counter>>;

    ECS<arcs_t,
        gmeta::types_t<
            UpdatePositionSystem, 
            UpdateSpeedSystem, 
            UpdateCounterSystem>>  
        ecs;

    ecs.AddEntity(Position{ 1, 2, 3 }, Speed{ 4,5,1 }, Mass{ 5.1 }, Counter{ 0 });
    ecs.AddEntity<Position, Speed, Mass>(Position{ 2, 2, 3 }, Speed{ 4,5,2 }, Mass{ 5.2 }, Counter{ 0 });
    ecs.AddEntity<Position, Speed, Mass>(Position{ 3, 2, 3 }, Speed{ 4,5,3 }, Mass{ 5.3 }, Counter{ 0 });
    ecs.AddEntity<Position, Speed, Mass>(Position{ 4, 2, 3 }, Speed{ 4,5,4 }, Mass{ 5.4 }, Counter{ 0 });
    ecs.AddEntity<Position, Speed, Mass>(Position{ 5, 2, 3 }, Speed{ 4,5,5 }, Mass{ 5.5 }, Counter{ 0 });

    ecs.AddEntity(1, 1.1, Counter{ 0 });
    ecs.AddEntity(2, 2.2, Counter{ 0 });

    ecs.DeleteEntity(0);
    ecs.DeleteEntity(2);
    ecs.DeleteEntity(4);

    /*ecs.foreach<Position, Speed, Mass, Counter>(
        [](size_t id, Position pos, Speed speed, Mass mass, Counter counter)
        {
            cout << mass.m << endl;
        });*/

    auto fn = [](size_t id, Speed speed)
        {
            cout << "id: " << id << ", speed(x): " << speed.x << endl;
        };

    using fnT = decltype(fn);


    //works
    using test2 = gmeta::extract_parameter_types_from_system_call_operator_inner<gmeta::fntraits_t<fnT>>::type;

    //fails
    using test3 = gmeta::extract_parameter_types_from_system_call_operator_inner_t<fnT>;

    //fails
    //using test1 = gmeta::extract_parameter_types_from_system_call_operator_t<fnT>;

    using arc1 = Archetype<32, Position, Speed, Mass, Counter>;
    using arc2 = Archetype<2, int, double, Counter>;


    //test subset_of_t<types_t<Ts...>, remove_sizeT_from_archetype_t<FirstArchetype>
    //using test12 = gmeta::subset_of_t<types_t<size_t, >, remove_sizeT_from_archetype_t<FirstArchetype>

    //test get_archs_from_argument_types_inner_t
    using test11 = gmeta::get_archs_from_argument_types_inner_t<gmeta::types_t<arc1, arc2>, gmeta::types_t<size_t, Speed>, gmeta::types_t<>>;
    if constexpr (gmeta::gis_same<test11, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_argument_types_inner_t success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_argument_types_inner_t failure" << std::endl;
    }


    //test get_archs_from_argument_types_t
    using test0 = gmeta::get_archs_from_argument_types_t<gmeta::types_t<arc1, arc2>, gmeta::types_t<size_t, Speed>>;
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
            gmeta::types_t<arc1,arc2>, 
            fnT>;
    if constexpr (gmeta::gis_same<testMatchingArchetypes_t, gmeta::types_t<arc1>>::value)
    {
        std::cout << "get_archs_from_system_t success" << std::endl;
    }
    else
    {
        std::cout << "get_archs_from_system_t failure" << std::endl;
    }



    ecs.foreach([](size_t id, Speed& speed)
        {
            //cout << "id: " << id << ", speed(x): " << speed.x << endl;
            speed.x += 1;
            speed.print();
        });

    ecs.foreach([](size_t& id)
        {
            cout << "id: " << id++  << endl;
        });

    const auto countersys = UpdateCounterSystem{};
    const auto possys = UpdatePositionSystem{};
    const auto speedsys = UpdateSpeedSystem{};

    using matchingArchsPosSys = gmeta::get_archs_from_system_t<arcs_t, UpdatePositionSystem>;

    //using matchingArchsPosSysArgumentsTypeInner = gmeta::

    //Archetype<32, Position, Speed, Mass, Counter>
    //Archetype<2, int, double, Counter>
    if constexpr (gmeta::gis_same<matchingArchsPosSys, gmeta::types_t<Archetype<32, Position, Speed, Mass, Counter>>>::value)
    {
        cout << "success matchingArchsPosSys" << endl;
    }
    else
    {
        cout << "failure matchingArchsPosSys" << endl;
    }

    ecs.foreach(countersys);
    ecs.foreach(possys);
    ecs.foreach(speedsys);


    ecs.tick();

    cout << "-------------" << endl;
}

int main()
{
    //testArchetypeChunk();
    //testArchetype();
    testECS();

    using should_be_false_t = gmeta::all_different_t<gmeta::types_t<int, int, double>>;
    using should_be_true_t = gmeta::all_different_t<gmeta::types_t<int, char, double>>;


    /*if (false)
    {
        sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(shape);
            window.display();
        }
    }*/

    return 0;
}