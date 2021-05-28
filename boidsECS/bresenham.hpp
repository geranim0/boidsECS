#pragma once

#include <vector>
#include <boidsECS/helpers.hpp>

namespace bresenham
{
    using std::vector;

struct line
{
	position2i p1, p2;
};

template<typename PosT, typename DecodeFn>
vector<PosT> bresenham_inner(PosT p1, PosT p2, DecodeFn&& decodeFn)
{
	vector<PosT> ret;
	int x = p1.x;
	int y = p1.y;

	int m = 2 * (p2.y - p1.y);
	int error = 0;

	while (x <= p2.x)
	{
		ret.push_back(decodeFn(PosT{ x, y }));
		error += m;
		if (error > 0)
		{
			error -= 2 * (p2.x - p1.x);
			y += 1;
		}
		x += 1;
	}
	return ret;
}

//here, dx and dy are positive. third thing to do: check if slope is vertical, horizontal, (m < 1) or (m>1) or (m==1)
template<typename PosT, typename DecodeFn>
vector<PosT> bresenham_check_slope(PosT p1, PosT p2, DecodeFn&& decodeFn)
{
	auto mirrorDiag = [](PosT p) -> PosT {
		return PosT{ p.y, p.x };
	};

	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	if (dx == 0) //vertical
	{
		vector<PosT> ret;
		for (int i = p1.y; i <= p2.y; i++)
		{
			ret.push_back(decodeFn({ p1.x, i }));
		}
		return ret;
	}
	else if (dy == 0) //horizontal
	{
		vector<PosT> ret;
		for (int i = p1.x; i <= p2.x; i++)
		{
			ret.push_back(decodeFn({ i, p1.y }));
		}
		return ret;
	}
	else if (dx == dy) // m == 1
	{
		vector<PosT> ret;
		for (int i = 0 ; i <= dx; i++)
		{
			ret.push_back(decodeFn({ p1.x + i, p1.y + i }));
		}
		return ret;
	}
	else if (dy > dx) // m > 1
	{
		return bresenham_inner(mirrorDiag(p1), mirrorDiag(p2),
			[decodeFn, mirrorDiag](PosT p)
			{
				return decodeFn(mirrorDiag(p));
			});
	}
	else //m < 1, all good
	{
		return bresenham_inner(p1, p2, decodeFn);
	}
}

//second thing to do
template<typename PosT, typename DecodeFn>
vector<PosT> bresenham_check_dy(PosT p1, PosT p2, DecodeFn&& decodeFn)
{
	auto mirrorX = [](PosT p) -> PosT {
		return PosT{ p.x, -p.y };
	};

	if (p2.y < p1.y)
	{
		return bresenham_check_slope(mirrorX(p1), mirrorX(p2),
			[decodeFn, mirrorX](PosT p)
			{
				return decodeFn(mirrorX(p));
			});
	}
	else
	{
		return bresenham_check_slope(p1, p2, decodeFn);
	}
}

//first thing to do
template<typename PosT>
vector<PosT> bresenham_check_dx(PosT p1, PosT p2)
{
	auto mirrorY = [](PosT p) -> PosT {
		return PosT{ -p.x, p.y };
	};
	auto id = [](PosT p) -> PosT {
		return p;
	};

	if (p2.x < p1.x)
	{
		return bresenham_check_dy(mirrorY(p1), mirrorY(p2), mirrorY);
	}
	else
	{
		return bresenham_check_dy(p1, p2, id);
	}
}

//entry point.
//DO NOT MODIFY: USED AS A SOURCE OF TRUTH for TESTS
template<typename PosT>
vector<PosT> bresenham(PosT p1, PosT p2)
{
	return bresenham_check_dx(p1, p2);
}

//function that was used to generate unit tests
void __internal_print_bresenham_for_tests()
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

}//end namespace