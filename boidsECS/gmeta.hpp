#pragma once

#include <variant>
#include <array>
#include <iostream>

namespace gmeta
{
	using namespace std;

	template<typename Ret, typename... Args>
	using Sig = Ret(*)(Args...);

	template<typename... Ts>
	struct first_of_pack;

	template<typename First, typename... Rest>
	struct first_of_pack<First, Rest...>
	{
		using type = First;
	};

	template<typename... Ts>
	using first_of_pack_t = typename first_of_pack<Ts...>::type;


	template<size_t... Is>
	struct size_t_pack
	{
		constexpr size_t_pack() {};
	};

	template<typename... Ts>
	struct types_t
	{
		constexpr types_t() {};
	};

	template<int... vals>
	struct size_t_pack_t
	{
		constexpr size_t_pack_t() {};
	};

	template<template<typename...> typename Z, typename Types>
	struct apply_types;

	template<template<typename...> typename Z, typename... Ts>
	struct apply_types<Z, types_t<Ts...>>
	{
		using type = Z<Ts...>;
	};

	template<template<typename...> typename Z, class Types>
	using apply_types_t = typename apply_types<Z, Types>::type;

	template<typename Fn, typename Ret, typename... Args>
	struct fntraits_more
	{
		using Ret_t = Ret;
		using Args_t = types_t<Args...>;
		using Fn_t = Fn;
	};

	template<typename Fn>
	struct fntraits
	{
		template<typename Ret, typename Class, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(Class::*)(Args...));

		template<typename Ret, typename Class, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(Class::*)(Args...) const);

		template<typename Ret, typename Class, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(Class::*)(Args...) const&);

		template<typename Ret, typename Class, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(Class::*)(Args...)&);

		template<typename Ret, typename Class, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(Class::*)(Args...)&&);

		template<typename Ret, typename... Args>
		static fntraits_more<Fn, Ret, Args...> get_more(Ret(*)(Args...));

		template<typename Functor>
		static decltype(get_more(&Functor::operator())) get_more(Functor);

		using type = decltype(get_more(std::declval<Fn>()));
	};

	template<typename Fn>
	using fntraits_t = typename fntraits<Fn>::type;

	template<typename T>
	struct remove_first_T_from_pack;

	template<typename T, typename... Args>
	struct remove_first_T_from_pack<types_t<T, Args...>>
	{
		using type = types_t<Args...>;
	};

	template<typename Args>
	using remove_first_T_from_pack_t = typename remove_first_T_from_pack<Args>::type;

	template<typename Ret, typename T>
	struct Sig_from_pack;

	template<typename Ret, typename... Args>
	struct Sig_from_pack<Ret, types_t<Args...>>
	{
		using type = Ret(*)(Args...);
	};

	template<typename Ret, typename Args>
	using Sig_from_pack_t = typename Sig_from_pack<Ret, Args>::type;

	struct false_t {};
	struct true_t {};
	struct error_t {};

	template<typename T, typename U>
	struct gis_same
	{
		using type = false_t;
		static constexpr bool value = false;
	};

	template<typename T>
	struct gis_same<T, T>
	{
		using type = true_t;
		static constexpr bool value = true;
	};

	template<typename T, typename U>
	using gis_same_t = typename gis_same<T, U>::type;

	template<typename T, typename U>
	bool gis_same_v = gis_same<T, U>::value;

	template<typename cond, typename True, typename False>
	struct gcond {};

	template<typename True, typename False>
	struct gcond<true_t, True, False>
	{
		using type = True;
	};

	template<typename True, typename False>
	struct gcond<false_t, True, False>
	{
		using type = False;
	};

	template<typename cond, typename True, typename False>
	using gcond_t = typename gcond<cond, True, False>::type;

	template<typename T, typename... Rest>
	struct pack_contains {};

	template<typename T>
	struct pack_contains<T>
	{
		using type = false_t;
	};

	template<typename T, typename First, typename... Rest>
	struct pack_contains<T, First, Rest...>
	{
		using type = typename gcond<gis_same_t<T, First>, true_t, typename pack_contains<T, Rest...>::type>::type;
	};

	template<typename T>
	struct pack_contains<T, T>
	{
		using type = true_t;
		static constexpr bool value = true;
	};

	template<typename T, typename Last>
	struct pack_contains<T, Last>
	{
		using type = false_t;
		static constexpr bool value = false;
	};

	template<typename T, typename... Ts>
	using pack_contains_t = typename pack_contains<T, Ts...>::type;

	template<typename T, typename... Ts>
	bool pack_contains_v = gis_same_v<true_t, pack_contains_t<T, Ts...>>;

	template<typename Ts>
	struct first_of_types_t_pack {};

	template<typename First, typename... Rest>
	struct first_of_types_t_pack<types_t<First, Rest...>>
	{
		using type = First;
	};

	template<typename Ts>
	using first_of_types_t_pack_t = typename first_of_types_t_pack<Ts>::type;

	template<typename First, typename Rest>
	struct all_different_inner2
	{
	};

	template<typename First>
	struct all_different_inner2<First, types_t<>>
	{
		using type = true_t;
	};

	template<typename First, typename... Rest>
	struct all_different_inner2<First, types_t<Rest...>>
	{
		using pack = types_t<Rest...>;
		using _first_of_pack = first_of_types_t_pack_t<pack>;
		using pack_without_first = remove_first_T_from_pack_t<pack>;

		using type = gcond_t<
			gis_same_t<First, _first_of_pack>,
			false_t,
			typename all_different_inner2<First, pack_without_first>::type>;
	};

	template<typename First, typename Rest>
	using all_different_inner2_t = typename all_different_inner2<First, Rest>::type;

	//tests if all arguments passed are different from one another
	template<typename First, typename Ts>
	struct all_different_inner
	{};

	template<typename First>
	struct all_different_inner<First, types_t<>>
	{
		using type = true_t;
	};

	template<typename First, typename... Ts>
	struct all_different_inner<First, types_t<Ts...>>
	{
		using pack = types_t<Ts...>;
		using _first_of_pack = first_of_types_t_pack_t<pack>;
		using pack_without_first = remove_first_T_from_pack_t<pack>;

		using type = gcond_t<all_different_inner2_t<First, pack>,
			typename all_different_inner<_first_of_pack, pack_without_first>::type,
			false_t>;
	};

	template<typename First, typename... Ts>
	using all_different_inner_t = typename all_different_inner<First, types_t<Ts...>>::type;

	template<typename Ts>
	struct all_different
	{};

	template<>
	struct all_different<types_t<>>
	{
		using type = true_t;
	};

	template<typename First, typename... Rest>
	struct all_different<types_t<First, Rest...>>
	{
		using type = all_different_inner_t<First, Rest...>;
	};

	template<typename Ts>
	using all_different_t = typename all_different<Ts>::type;

	//fusions 2 types_t into one
	template<typename Ts, typename Us>
	struct fusion_2_typest
	{};

	template<typename... Ts, typename... Us>
	struct fusion_2_typest<types_t<Ts...>, types_t<Us...>>
	{
		using type = types_t<Ts..., Us...>;
	};

	template<typename Ts, typename Us>
	using fusion_2_typest_t = typename fusion_2_typest<Ts, Us>::type;

	//removes first encountered type T in pack
	template<typename ToRemoveT, typename Ts>
	struct remove_first_encountered_T_from_pack
	{};

	template<typename ToRemoveT>
	struct remove_first_encountered_T_from_pack<ToRemoveT, types_t<>>
	{
		using type = types_t<>;
	};

	template<typename ToRemoveT, typename FirstTs, typename... RestTs>
	struct remove_first_encountered_T_from_pack<ToRemoveT, types_t<FirstTs, RestTs...>>
	{
		using type = gcond_t<gis_same_t<ToRemoveT, FirstTs>,
			types_t<RestTs...>,
			fusion_2_typest_t<
			types_t<FirstTs>,
			typename remove_first_encountered_T_from_pack<ToRemoveT, types_t<RestTs...>>::type>>;
	};

	template<typename ToRemoveT, typename Ts>
	using remove_first_encountered_T_from_pack_t = typename remove_first_encountered_T_from_pack<ToRemoveT, Ts>::type;

	//tests if Ts are a subset of Us
	template<typename Ts, typename Us>
	struct subset_of {};

	template<typename... Us>
	struct subset_of<types_t<>, types_t<Us...>>
	{
		using type = true_t;
	};

	template<typename FirstT, typename... RestTs, typename... Us>
	struct subset_of<types_t<FirstT, RestTs...>, types_t<Us...>>
	{
		using type = gcond_t<pack_contains_t<FirstT, Us...>,
			typename subset_of<types_t<RestTs...>, remove_first_encountered_T_from_pack_t<FirstT, types_t<Us...>>>::type,
			false_t>;
	};

	template<typename Ts, typename Us>
	using subset_of_t = typename subset_of<Ts, Us>::type;


	//---------------------------------------------------------------------------//
	//                               ECS specific                                //
	//---------------------------------------------------------------------------//
	template<typename Tmplt, typename... Ts>
	struct last_N_template_parameters_are_same_as_Ts
	{
		using type = error_t;
	};

	template<template<size_t, typename...> typename Tmplt, size_t N, typename... RestTmplt, typename... Ts>
	struct last_N_template_parameters_are_same_as_Ts<Tmplt<N, RestTmplt...>, Ts...>
	{
		using type = gis_same_t<types_t<RestTmplt...>, types_t<Ts...>>;
	};

	template<typename Tmplt, typename... Ts>
	using last_N_template_parameters_are_same_as_Ts_t = typename last_N_template_parameters_are_same_as_Ts<Tmplt, Ts...>::type;

	template<typename  ArchetypesTs, typename Ts>
	struct get_archetype
	{
		struct ERROR_USING_INVALID_TYPES {};
		using type = ERROR_USING_INVALID_TYPES;
	};

	template<typename FirstArchetypeT, typename... RestArchetypesTs, typename... Ts>
	struct get_archetype<types_t<FirstArchetypeT, RestArchetypesTs...>, types_t<Ts...>>
	{
		using type = gcond_t<last_N_template_parameters_are_same_as_Ts_t<FirstArchetypeT, Ts...>,
			FirstArchetypeT,
			typename get_archetype<types_t<RestArchetypesTs...>, types_t<Ts...>>::type>;
	};

	template<typename ArchetypesTs, typename Ts>
	using get_archetype_t = typename get_archetype<ArchetypesTs, Ts>::type;

	//strips an archetype from its first nontype template parameter
	template<typename Archetype>
	struct remove_sizeT_from_archetype
	{};

	template<template<size_t, typename...> typename Archetype, size_t N, typename... Ts>
	struct remove_sizeT_from_archetype<Archetype<N, Ts...>>
	{
		using type = types_t<size_t, remove_reference_t<Ts>...>;
	};

	template<typename Archetype>
	using remove_sizeT_from_archetype_t = typename remove_sizeT_from_archetype<Archetype>::type;

	template<typename Archetypes, typename Ts, typename ArchetypesBuilderTs>
	struct get_archs_from_argument_types_inner
	{};

	template<typename... Ts, typename... ArchetypesBuilderTs>
	struct get_archs_from_argument_types_inner<types_t<>, types_t<Ts...>, types_t<ArchetypesBuilderTs...>>
	{
		using type = types_t<ArchetypesBuilderTs...>;
	};

	//Here I strip references generously. Might have to change that if parallelizing and need to know
	//what is readonly and what is read write to be able to parallelize systems correctly.
	template<typename FirstArchetype, typename... RestArchetypes, typename... Ts, typename... ArchetypesBuilderTs>
	struct get_archs_from_argument_types_inner<types_t<FirstArchetype, RestArchetypes...>, types_t<Ts...>, types_t<ArchetypesBuilderTs...>>
	{
		using type = gcond_t<subset_of_t<types_t<remove_reference_t<Ts>...>, remove_sizeT_from_archetype_t<FirstArchetype>>,
			typename get_archs_from_argument_types_inner<types_t<RestArchetypes...>, types_t<Ts...>, types_t<FirstArchetype, ArchetypesBuilderTs...>>::type,
			typename get_archs_from_argument_types_inner<types_t<RestArchetypes...>, types_t<Ts...>, types_t<ArchetypesBuilderTs...>>::type>;
	};

	template<typename Archetypes, typename Ts, typename ArchetypesBuilderTs>
	using get_archs_from_argument_types_inner_t = typename get_archs_from_argument_types_inner<Archetypes, Ts, ArchetypesBuilderTs>::type;

	template<typename Archetypes, typename Ts>
	struct get_archs_from_argument_types
	{};

	template<typename... Archetypes, typename... Ts>
	struct get_archs_from_argument_types<types_t<Archetypes...>, types_t<Ts...>>
	{
		using type = get_archs_from_argument_types_inner_t<types_t<Archetypes...>, types_t<Ts...>, types_t<>>;
	};

	template<typename Archetypes, typename Ts>
	using get_archs_from_argument_types_t = typename get_archs_from_argument_types<Archetypes, Ts>::type;

	template<typename>
	struct extract_parameter_types_from_system_call_operator_inner
	{
		struct ERROR_IN_extract_parameter_types_from_system_call_operator_inner {};
		using type = ERROR_IN_extract_parameter_types_from_system_call_operator_inner;
	};

	template<typename Ret, typename Class, typename... Args>
	struct extract_parameter_types_from_system_call_operator_inner<fntraits_more<Ret, Class, Args...>>
	{
		using type = types_t<Args...>;
	};

	template<typename System>
	using extract_parameter_types_from_system_call_operator_inner_t = typename extract_parameter_types_from_system_call_operator_inner<fntraits_t<System>>::type;

	template<typename Archetypes, typename System>
	struct get_archs_from_system
	{
		struct ERROR_IN_GET_ARCHS_FROM_SYSTEM {};
		using type = ERROR_IN_GET_ARCHS_FROM_SYSTEM;
	};

	template<typename... Archetypes, typename System>
	struct get_archs_from_system<types_t<Archetypes...>, System>
	{
		using type = get_archs_from_argument_types_t<types_t<Archetypes...>, extract_parameter_types_from_system_call_operator_inner_t<System>>;
	};

	template<typename Archetypes, typename System>
	using get_archs_from_system_t = typename get_archs_from_system<Archetypes, System>::type;
}