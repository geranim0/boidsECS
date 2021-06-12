#pragma once

#include <cstdint>
#include <array>
#include <tuple>
#include <list>
#include <unordered_map>
#include <variant>
#include <type_traits>
#include <limits>
#include <cstring>
#include <chrono>

#include <boidsECS/usings.hpp>
#include <boidsECS/gmeta.hpp>
#include <boidsECS/helpers.hpp>

template <std::size_t ChunkSize, typename... Ts>
class ArchetypeChunk
{
public:
	ArchetypeChunk()
	{
		_size = 0;
	}

	template <typename T>
	std::array<std::remove_cvref_t<T>, ChunkSize> &getComponent()
	{
		return std::get<std::array<std::remove_cvref_t<T>, ChunkSize>>(_arrays);
	}

	template <std::size_t Index>
	auto &getComponent()
	{
		return std::get<Index>(_arrays);
	}

	template <typename Fn>
	void foreach (Fn &&fn, DeltaTime dt)
	{
		using FnTsWithoutDeltaTime = gmeta::remove_first_encountered_T_from_pack_t<DeltaTime ,typename gmeta::fntraits_t<Fn>::Args_t>;
		foreach_inner<Fn, ChunkSize, std::tuple<std::array<Id, ChunkSize>, std::array<std::remove_cvref_t<Ts>, ChunkSize>...>, FnTsWithoutDeltaTime>{}(std::forward<Fn>(fn), _arrays, _size, dt);
	}

	template <typename Fn>
	void foreach (Fn &&fn)
	{
		using FnTsWithoutDeltaTime = gmeta::remove_first_encountered_T_from_pack_t<DeltaTime ,typename gmeta::fntraits_t<Fn>::Args_t>;
		foreach_inner<Fn, ChunkSize, std::tuple<std::array<Id, ChunkSize>, std::array<std::remove_cvref_t<Ts>, ChunkSize>...>, FnTsWithoutDeltaTime>{}(std::forward<Fn>(fn), _arrays, _size);
	}

	bool AddEntity(Id id, Ts... args)
	{
		if (_size == ChunkSize || id_to_index_map.contains(id))
		{
			return false;
		}
		else
		{
			getComponent<0>()[_size] = id;
			((getComponent<Ts>()[_size] = std::move(args)), ...);

			id_to_index_map.emplace(id, _size);

			_size += 1;
			return true;
		}
	}

	bool DeleteEntityById(Id id)
	{
		if (!id_to_index_map.contains(id))
		{
			return false;
		}
		else
		{
			std::size_t index = id_to_index_map[id];
			id_to_index_map.erase(id);
			DeleteEntityByIndex(index);
			return true;
		}
	}

	bool ContainsEntity(Id id) const
	{
		return id_to_index_map.contains(id);
	}

	std::size_t Size() const
	{
		return _size;
	}

	bool Full() const
	{
		return _size == ChunkSize;
	}

	bool Empty() const
	{
		return _size == 0;
	}

	std::size_t RemainingSpace() const
	{
		return ChunkSize - _size;
	}

private:
	template <typename Fn, std::size_t _ChunkSize, typename ArraysT, typename _Ts>
	struct foreach_inner
	{
	};

	template <typename Fn, std::size_t _ChunkSize, typename ArraysT, typename... _Ts>
	struct foreach_inner<Fn, _ChunkSize, ArraysT, gmeta::types_t<_Ts...>>
	{
		void operator()(Fn &&fn, ArraysT &arrays, std::size_t size, DeltaTime dt)
		{
			for (std::size_t i = 0; i < size; i++)
			{	
				fn(std::get<std::array<std::remove_cvref_t<_Ts>, _ChunkSize>>(arrays)[i]..., dt);
			}
		}
		
		void operator()(Fn &&fn, ArraysT &arrays, std::size_t size)
		{
			for (std::size_t i = 0; i < size; i++)
			{	
				fn(std::get<std::array<std::remove_cvref_t<_Ts>, _ChunkSize>>(arrays)[i]...);
			}
		}
	};

	bool DeleteEntityByIndex(std::size_t index)
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

	std::size_t _size;
	std::tuple<std::array<Id, ChunkSize>, std::array<std::remove_cvref_t<Ts>, ChunkSize>...> _arrays;
	std::unordered_map<Id, std::size_t> id_to_index_map;
};

/*
With current implementation, removing entities in middle chunks will work,
but these chunks will stay incomplete forever since the new entities are always
appended to the front() chunk. 
TODO: address this problem.
*/
template <std::size_t ChunkSize, typename... Ts>
class Archetype
{
public:
	template <typename Enable =
				  std::enable_if_t<gmeta::gis_same<gmeta::true_t, gmeta::all_different_t<gmeta::types_t<Id, Ts...>>>::value>>
	Archetype()
	{
		_size = 0;
	}

	bool AddEntity(Id id, Ts... args)
	{
		if (_chunks.empty() || _chunks.front().Size() == ChunkSize)
		{
			_chunks.push_front(ArchetypeChunk<ChunkSize, Ts...>{});
		}

		bool success = _chunks.front().AddEntity(id, std::move(args)...);

		if (success)
		{
			_size += 1;
			_id_to_ptr_to_chunk_map.emplace(id, &_chunks.front());
		}

		return success;
	}

	std::size_t Size() const
	{
		return _size;
	}

	//problem right now: if we do a lot of deletes we will end up with a lot of empy chunks.
	//TODO: to some bookkeeping to keep things nice and compact in memory
	bool DeleteEntityById(Id id)
	{
		if (!_id_to_ptr_to_chunk_map.contains(id))
		{
			return false;
		}
		else
		{
			bool success = _id_to_ptr_to_chunk_map[id]->DeleteEntityById(id);
			if (success)
			{
				_size -= 1;
				_id_to_ptr_to_chunk_map.erase(id);
			}
			return success;
		}
	}

	//Uses normal chunk API, inefficient
	template<size_t N = std::numeric_limits<std::size_t>::max()>
	std::size_t Compress_Dummy()
	{
		std::size_t moved = 0;

		auto move_from = _chunks.begin();
		auto move_to = --_chunks.end();

		while (move_from != move_to && moved < N)
		{
			if (move_to->Full())
			{
				--move_to;
				continue;
			}
			else if(move_from->Empty())
			{
				move_from = _chunks.erase(move_from);
				continue;
			}
			
			std::size_t num_entities_to_move = std::min({N - moved, move_from->Size(), move_to->RemainingSpace()});
			std::array<Id, ChunkSize> ids_to_delete;
			std::size_t ids_to_delete_idx = 0;
			move_from->foreach([num_entities_to_move, _this = this, &move_to, &moved, &ids_to_delete, &ids_to_delete_idx](Id id, Ts&... ts)
			{
				if (ids_to_delete_idx < num_entities_to_move)
				{
					move_to->AddEntity(id, std::move(ts)...);
					_this->_id_to_ptr_to_chunk_map[id] = &(*move_to);
					moved += 1;
					ids_to_delete[ids_to_delete_idx++] = id;
				}
			});

			for (std::size_t i = 0; i < num_entities_to_move; i++)
			{
				move_from->DeleteEntityById(ids_to_delete[i]);
			}
		}
		return moved;
	}

	/*
	Template argument: Max number of elements to compress
	Will compress elements of all chunks to the end of the list,
	moving elements from incomplete chunks near the front() to the end.
	Returns: number of elements compressed
	*/
	/*template<size_t N = std::numeric_limits<std::size_t>::max()>
	std::size_t Compress_memcpy()
	{
		std::size_t moved = 0;

		auto move_from = _chunks.begin();
		auto move_to = --_chunks.end();

		while (move_from != move_to && moved < N)
		{
			if (move_to->Full())
			{
				--move_to;
				continue;
			}
			else if(move_from->Empty())
			{
				move_from = _chunks.erase(move_from);
				continue;
			}

			while(moved < N)
			{
				//Get maximum N of elements movable from move_from to move_to
				std::size_t num_entities_to_move = std::min({N - moved, move_from->Size(), move_to->RemainingSpace()});
				chunk_memcpy<Id, Ts...>(move_to, move_from, num_entities_to_move);
				moved += num_entities_to_move;
			}
		}

		return moved;
	}*/

	bool Contains(Id id) const
	{
		return _id_to_ptr_to_chunk_map.contains(id);
	}

	std::size_t CountUnfilledChunks() const
	{
		size_t unfilled = 0;
		for (auto const& chunk : _chunks)
		{
			if (!chunk.Full())
			{
				unfilled += 1;
			}
		}
		return unfilled;
	}

	template <typename Fn>
	void foreach (Fn &&fn, DeltaTime dt)
	{
		for (auto &chunk : _chunks)
		{
			chunk.foreach (std::forward<Fn>(fn), dt);
		}
	}

	template <typename Fn>
	void foreach (Fn &&fn)
	{
		for (auto &chunk : _chunks)
		{
			chunk.foreach (std::forward<Fn>(fn));
		}
	}

private:

	/*template<typename T>
	void chunk_memcpy_inner(auto& dst, auto& src, std::size_t count)
	{					
		std::size_t move_to_start_idx = dst->Size() - 1;
		std::size_t move_from_start_idx = src->Size() - 1;
		
		void* dest_ptr = &dst.getComponent<std::array<T, ChunkSize>>()[move_to_start_idx];
		void* src_ptr = &src.getComponent<std::array<T, ChunkSize>>()[move_from_start_idx];
		
		std::size_t num_bytes = count*sizeof(T);

		std::memcpy(dest_ptr, src_ptr, num_bytes);
	}

	template<typename... Ts>
	void chunk_memcpy(auto& dst, auto& src, std::size_t count)
	{		
		(chunk_memcpy_inner<Ts>(dst, src, count), ...);
	}*/

	std::list<ArchetypeChunk<ChunkSize, Ts...>> _chunks;
	std::unordered_map<size_t, ArchetypeChunk<ChunkSize, Ts...> *> _id_to_ptr_to_chunk_map;
	size_t _size;
};

//when instanciating ECS object, must specify all archetype types that are going to be used with ECS
//and also all systems
template <typename ArchetypeTs, typename SystemTs, typename Enable = void>
class ECS
{
};

template <typename... ArchetypeTs, typename... SystemTs>
class ECS<gmeta::types_t<ArchetypeTs...>,
		  gmeta::types_t<SystemTs...>,
		  std::enable_if_t<gmeta::gis_same<gmeta::true_t, gmeta::all_different_t<gmeta::types_t<ArchetypeTs...>>>::value>>
{
public:
	ECS()
	{
		nextId = 0;
		size = 0;
		time_previous_frame = std::chrono::high_resolution_clock::now();
	}

	ECS(SystemTs&&... systems_) : systems{std::tuple<SystemTs...>{systems_...}}
	{
		nextId = 0;
		size = 0;
		time_previous_frame = std::chrono::high_resolution_clock::now();
	}

	template <typename... Ts>
	bool AddEntity(Ts... args)
	{
		using Archetype_t = gmeta::get_archetype_t<gmeta::types_t<ArchetypeTs...>, gmeta::types_t<Ts...>>;
		Archetype_t &archetype = std::get<Archetype_t>(archetypes);
		bool success = archetype.AddEntity(nextId, std::move(args)...);

		if (success)
		{
			id_to_archetype_map.emplace(nextId, &archetype);
			nextId += 1;
			size += 1;
		}

		return success;
	}

	bool DeleteEntity(Id id)
	{
		if (!id_to_archetype_map.contains(id))
		{
			return false;
		}
		else
		{
			bool success = false;

			std::visit(
				[_id = id, &success](auto *ptr_to_archetype) {
					success = ptr_to_archetype->DeleteEntityById(_id);
				},
				id_to_archetype_map[id]);

			if (success)
			{
				id_to_archetype_map.erase(id);
				size -= 1;
			}

			return success;
		}
	}

	std::size_t Size()
	{
		return size;
	}

	template<size_t N = std::numeric_limits<std::size_t>::max()>
	std::size_t Compress()
	{
		std::size_t compressed = 0; 
		foreach_tuple(
			[&compressed](auto& archetype)
			{
				compressed += archetype.Compress_Dummy();
			},
		archetypes);

		return compressed;
	}

	template <typename Fn>
	void foreach (Fn &&fn, DeltaTime dt)
	{
		foreach_inner<Fn, std::tuple<ArchetypeTs...>, gmeta::get_archs_from_system_t<gmeta::types_t<ArchetypeTs...>, Fn>>{}(std::forward<Fn>(fn), archetypes, dt);
	}

	void tick()
	{
		time_now = std::chrono::high_resolution_clock::now();
		auto dt = time_now - time_previous_frame;
		DeltaTime dt_seconds{static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(dt).count()) / 1000000.0};
		time_previous_frame = time_now;


		foreach_tuple(
			[&](auto &system) {
				system.pre_tick();
				foreach (system, dt_seconds);
				system.post_tick();
			},
			systems);
	}

private:
	template <typename Fn, typename ArchetypesT, typename MatchingArchetypes>
	struct foreach_inner
	{
	};

	template <typename Fn, typename ArchetypesT, typename... MatchingArchetypes>
	struct foreach_inner<Fn, ArchetypesT, gmeta::types_t<MatchingArchetypes...>>
	{
		void operator()(Fn &&fn, ArchetypesT &archetypes, DeltaTime dt)
		{
			if constexpr (gmeta::types_t_pack_contains_v<DeltaTime, typename gmeta::fntraits_t<Fn>::Args_t>)
			{
				(std::get<MatchingArchetypes>(archetypes).foreach (std::forward<Fn>(fn), dt), ...);
			}
			else
			{
				(std::get<MatchingArchetypes>(archetypes).foreach (std::forward<Fn>(fn)), ...);
			}
		}
	};

	Id nextId;
	std::tuple<ArchetypeTs...> archetypes;
	std::unordered_map<std::size_t, std::variant<ArchetypeTs *...>> id_to_archetype_map;
	std::tuple<SystemTs...> systems;
	std::size_t size;

	//time
	std::chrono::_V2::system_clock::time_point time_now;
	std::chrono::_V2::system_clock::time_point time_previous_frame;
};