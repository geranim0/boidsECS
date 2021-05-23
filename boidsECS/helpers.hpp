#pragma once

#include <iostream>
#include <tuple>

//gereral stuff

template<typename Fn, typename... Args>
void foreach_tuple(Fn&& fn, std::tuple<Args...> const& tup)
{
	std::apply([fn = std::forward<Fn>(fn)](auto const&... elem)
	{
		(fn(elem), ...);
	}, tup);
}

//ECS boids specific

struct Position
{
    float x;
    float y;
    float z;
};

struct Speed
{
    float x;
    float y;
    float z;

	void print()
	{
		std::cout << z << std::endl;
	}
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