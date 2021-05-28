#pragma once

#include <iostream>
#include <tuple>

//gereral stuff

template<typename Fn, typename... Args>
void foreach_tuple(Fn&& fn, std::tuple<Args...>& tup)
{
	std::apply([fn = std::forward<Fn>(fn)](auto&... elem)
	{
		(fn(elem), ...);
	}, tup);
}

template<typename Fn, typename... Args>
void foreach_tuple(Fn&& fn, std::tuple<Args...> const& tup)
{
	std::apply([fn = std::forward<Fn>(fn)](auto const&... elem)
	{
		(fn(elem), ...);
	}, tup);
}

//ECS boids specific

struct position3f
{
    float x;
    float y;
    float z;
};

struct position2i
{
	int x, y;
};

bool operator==(position2i const& lhs, position2i const& rhs);
bool operator<(position2i const& lhs, position2i const& rhs);

struct speed3f
{
    float x;
    float y;
    float z;

	void print()
	{
		std::cout << z << std::endl;
	}
};

struct speed2i
{
	int x, y;
};

struct Mass
{
	operator float() const
	{
		return m;
	}
    float m;
};

struct Counter
{
	operator size_t() const
	{
		return counter;
	}

	Counter& operator += (int increment)
	{
		this->counter += increment;
		return *this;
	}

	size_t counter;
};