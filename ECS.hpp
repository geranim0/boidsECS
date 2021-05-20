#pragma once

#include <cstdint>
#include <array>
#include <tuple>
#include <forward_list>
#include <unordered_map>
#include <variant>
#include <type_traits>

#include "gmeta.hpp"
#include "helpers.hpp"

template<size_t ChunkSize, typename... Ts>
class ArchetypeChunk
{
public:

	ArchetypeChunk()
	{
		_size = 0;
	}

	template<typename T>
	std::array<std::remove_reference_t<T>, ChunkSize>& getComponent()
	{
		return std::get<std::array<std::remove_reference_t<T>, ChunkSize>>(_arrays);
	}

	template<size_t Index>
	auto& getComponent()
	{
		return std::get<Index>(_arrays);
	}

	template<typename Fn>
	void foreach(Fn&& fn)
	{
		foreach_inner<Fn, ChunkSize, std::tuple<std::array<size_t, ChunkSize>, std::array<std::remove_reference_t<Ts>, ChunkSize>...>, typename gmeta::fntraits_t<Fn>::Args_t>{}(std::forward<Fn>(fn), _arrays, _size);
	}

	template<typename Fn>
	void foreach_with_index(Fn&& fn)
	{
		for (size_t i = 0; i < _size; i++)
		{
			fn(i, std::forward<size_t>(getComponent<0>()[i]), std::forward<Ts>(getComponent<Ts>()[i])...);
		}
	}

	bool AddEntity(size_t id, Ts&&... args)
	{
		if (_size == ChunkSize)
		{
			return false;
		}
		else
		{
			getComponent<0>()[_size] = id;
			((getComponent<Ts>()[_size] = args) , ...);

			id_to_index_map.emplace(id, _size);
			
			_size += 1;
			return true;
		}
	}

	bool DeleteEntityById(size_t id)
	{
		if (!id_to_index_map.contains(id))
		{
			return false;
		}
		else
		{
			size_t index = id_to_index_map[id];
			id_to_index_map.erase(id);
			DeleteEntityByIndex(index);
			return true;
		}
	}

	size_t size()
	{
		return _size;
	}

private:

	template<typename Fn, size_t ChunkSize, typename ArraysT, typename Ts>
	struct foreach_inner{};

	template<typename Fn, size_t ChunkSize, typename ArraysT, typename... Ts>
	struct foreach_inner<Fn, ChunkSize, ArraysT, gmeta::types_t<Ts...>>
	{
		void operator()(Fn&& fn, ArraysT& arrays, size_t size)
		{
			for (size_t i = 0; i < size; i++)
			{
				fn(std::get<std::array<std::remove_reference_t<Ts>, ChunkSize>>(arrays)[i]...);
			}
		}
	};

	bool DeleteEntityByIndex(size_t index)
	{
		if (index >= _size)
		{
			return false;
		}
		else if (index == _size - 1)
		{
			_size -= 1;
			return true;
		}
		else
		{
			getComponent<0>()[index] = getComponent<0>()[_size - 1];
			((getComponent<Ts>()[index] = getComponent<Ts>()[_size - 1]), ...);
			id_to_index_map[getComponent<0>()[_size - 1]] = index;
			_size -= 1;
			return true;
		}
	}

	size_t _size;
	std::tuple<std::array<size_t, ChunkSize>, std::array<std::remove_reference_t<Ts>, ChunkSize>...> _arrays;
	std::unordered_map<size_t, size_t> id_to_index_map;
};


/*
With current implementation, removing entities in middle chunks will work,
but these chunks will stay incomplete forever since the new entities are always
appended to the front() chunk. 
TODO: address this problem.
*/


template<size_t ChunkSize, typename... Ts>
class Archetype
{
public:

	template<typename Enable = 
		std::enable_if_t<gmeta::gis_same<gmeta::true_t, gmeta::all_different_t<gmeta::types_t<Ts...>>>::value>>
	Archetype()
	{

	}

	bool AddEntity(size_t id, Ts&&... args)
	{
		if (chunks.empty())
		{
			chunks.push_front(ArchetypeChunk<ChunkSize, Ts...>{});
		}
		else if (chunks.front().size() == ChunkSize)
		{
			chunks.push_front(ArchetypeChunk<ChunkSize, Ts...>{});
		}

		bool success = chunks.front().AddEntity(id, std::forward<Ts>(args)...);

		if (success)
		{
			id_to_ptr_to_chunk_map.emplace(id, &chunks.front());
		}

		return success;
	}


	//problem right now: if we do a lot of deletes we will end up with a lot of empy chunks.
	//TODO: to some bookkeeping to keep things nice and compact in memory
	bool DeleteEntityById(size_t id)
	{
		if (!id_to_ptr_to_chunk_map.contains(id))
		{
			return false;
		}
		else
		{
			return id_to_ptr_to_chunk_map[id]->DeleteEntityById(id);
		}
	}

	template<typename Fn>
	void foreach(Fn&& fn)
	{
		for (auto& chunk : chunks)
		{
			chunk.foreach(std::forward<Fn>(fn));
		}
	}

private:
	std::forward_list<ArchetypeChunk<ChunkSize, Ts...>> chunks;
	std::unordered_map<size_t, ArchetypeChunk<ChunkSize, Ts...>*> id_to_ptr_to_chunk_map;
};

struct UpdatePositionSystem
{
	void operator()(Position pos) const
	{
		pos.x += 1;
	}
};

struct UpdateSpeedSystem
{
	void operator()(Speed& speed) const
	{
		speed.y += 1;
	}
};

struct UpdateCounterSystem
{
	void operator()(Counter& counter) const
	{
		counter += 1;
	}
};

//when instanciating ECS object, must specify all archetype types that are going to be used with ECS
//and also all systems
template<typename ArchetypeTs, typename SystemTs, typename Enable = void>
class ECS
{};

template<typename... ArchetypeTs, typename... SystemTs>
class ECS<gmeta::types_t<ArchetypeTs...>,
	gmeta::types_t<SystemTs...>, 
	std::enable_if_t<gmeta::gis_same<gmeta::true_t, gmeta::all_different_t<gmeta::types_t<ArchetypeTs...>>>::value>>
{
public:

	ECS()
	{
		nextId = 0;
	}

	template<typename... Ts>
	bool AddEntity(Ts&&... args)
	{
		using Archetype_t = gmeta::get_archetype_t<gmeta::types_t<ArchetypeTs...>, gmeta::types_t<Ts...>>;
		Archetype_t& archetype = std::get<Archetype_t>(archetypes);
		bool success = archetype.AddEntity(nextId, std::forward<Ts>(args)...);
		id_to_archetype_map.emplace(nextId, &archetype);
		nextId += 1;
		
		return success;
	}

	bool DeleteEntity(uint64_t id)
	{
		bool success = false;
		std::visit(
			[_id = id, &success](auto* ptr_to_archetype) {
				success = ptr_to_archetype->DeleteEntityById(_id);
			}, id_to_archetype_map[id]);
		
		id_to_archetype_map.erase(id);

		return success;
	}

	template<typename Fn>
	void foreach(Fn&& fn)
	{
		foreach_inner<Fn, std::tuple<ArchetypeTs...>, gmeta::get_archs_from_system_t<gmeta::types_t<ArchetypeTs...>, Fn>>{}(std::forward<Fn>(fn), archetypes);
	}

	template<typename System, typename Archetypes>
	struct apply_system_foreach_archetype
	{};

	template<typename System, typename... Archetypes>
	struct apply_system_foreach_archetype<System, gmeta::types_t<Archetypes...>>
	{
		void operator()(System const& system)
		{
			foreach2(system);
		}
	};

	void tick()
	{
		foreach_tuple(
			[&](auto const& system) {
				foreach(system);
			}, systems);
	}

private:

	template<typename Fn, typename ArchetypesT, typename MatchingArchetypes>
	struct foreach_inner {};

	template<typename Fn, typename ArchetypesT, typename...MatchingArchetypes>
	struct foreach_inner<Fn, ArchetypesT, gmeta::types_t<MatchingArchetypes...>>
	{
		void operator()(Fn&& fn, ArchetypesT& archetypes)
		{
			(std::get<MatchingArchetypes>(archetypes).foreach(std::forward<Fn>(fn)), ...);
		}
	};

	uint64_t nextId;
	std::tuple<ArchetypeTs...> archetypes;
	std::unordered_map<size_t, std::variant<ArchetypeTs*...>> id_to_archetype_map;

	std::tuple<SystemTs...> systems;
}; 