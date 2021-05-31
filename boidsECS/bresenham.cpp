#include <boidsECS/bresenham.hpp>

//function that was used to generate unit tests
void bresenham::__internal_print_bresenham_for_tests()
{
    using std::vector;
    using std::cout;
    using std::endl;
    
    vector<position2i> positions_to_test_1{{0,0}, {0,0}, {0,0}, {0,0}, {-1, -4}, {-3, -1}, {-3, 0}, {-3, 1}, {-1, 3}, {0, 2}, {1,2}, {2,1}, {1, 0}, {3, -1}, {1, -3}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
    vector<position2i> positions_to_test_2{{0,0}, {0,1}, {1,0}, {1,1}, {1, 4}, {4, 1}, {0,0}, {2, -1}, {1, -2}, {0, -2}, {-1, -3}, {-3, -1}, {-2, 0}, {-3, 1}, {-1, 4}, {0,3}, {2,2}, {2, -2}, {-1,-1}, {-1, 1}};
    
    vector<vector<position2i>> results;

    for (std::size_t i = 0; i < positions_to_test_1.size(); i++)
    {
        auto brenres = bresenham::bresenham(positions_to_test_1[i], positions_to_test_2[i]);
        results.push_back(bresenham::bresenham(positions_to_test_1[i], positions_to_test_2[i]));
    }

    //print results for easy copy and paste in tests source
    for (std::size_t i = 0; i < results.size(); i++)
    {
        auto const& result = results[i];
        cout << "result = bresenham::bresenham(position2i{" << positions_to_test_1[i].x << ", " << positions_to_test_1[i].y << "}, position2i" << "{" << positions_to_test_2[i].x << ", " << positions_to_test_2[i].y << "});" << endl;
        cout << "eq = result == std::vector<position2i>{";
        for (auto const& pos : result)
        {
            cout << "{" << pos.x << ", " << pos.y << "}, ";
        }
        cout << "};" << endl;
        cout << "EXPECT_TRUE(eq);" << endl << endl;
    }
}