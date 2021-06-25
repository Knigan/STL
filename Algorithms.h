#pragma once
#include <iostream>


//!!! Задание на защиту
//!!! Написать концепции, зщадающие основные ограничения шаблонов алгоритмов (Сделано)

template <class IterType>
concept MustHave = requires(IterType i, IterType j) { //!!! Нужно не везде писать писать requires, а заранее создать concept и их использовать (Исправлено)
	{++i} -> std::same_as<IterType&>;
	{--i} -> std::same_as<IterType&>;
	{i++} -> std::same_as<IterType>;
	{i--} -> std::same_as<IterType>;
	i = j;
	{i == j} -> std::same_as<bool>;
	{i != j} -> std::same_as<bool>;
	{*i} -> std::same_as<std::iter_reference_t<IterType>>;
	*i = *j;
	{*i == *j} -> std::same_as<bool>;
	{*i != *j} -> std::same_as<bool>;
	{*i < *j} -> std::same_as<bool>;
	{*i > *j} -> std::same_as<bool>;

	//!! Где !=, разыменование (Исправлено)
};

template <class IterType, class Type>
requires MustHave<IterType>
IterType findIf(const IterType& begin, const IterType& end, const Type& predicate) noexcept {
	IterType res = end;
	++res;
	for (IterType i = begin; i != end; ++i) {
		if (predicate(*i))
			return i;
	}
	return res;
}

template <class IterType>
requires MustHave<IterType>
IterType minElement(const IterType& begin, const IterType& end) noexcept {
	IterType res = end;
	++res;
	IterType j = res;
	for (IterType i = begin; i != end; ++i) {
		if (res == j || *i < *res)
			res = i;
	}

	return res;
}

template <class IterType>
requires MustHave<IterType>
IterType maxElement(const IterType& begin, const IterType& end) noexcept {
	IterType res = end;
	++res;
	IterType j = res;

	for (IterType i = begin; i != end; ++i) {
		if (res == j || *i > *res)
			res = i;
	}

	return res;
}

template <class IterType, class Type>
requires MustHave<IterType>
void forEach(const IterType& begin, const IterType& end, const Type& operation) noexcept {
	for (IterType i = begin; i != end; ++i)
		*i = operation(*i);
}

template <class IterType>
requires MustHave<IterType>
void sort(const IterType& begin, const IterType& end) noexcept {
	IterType e = end;
	--e;
	for (IterType i = begin; i != end; ++i) {
		for (IterType j = begin; j != e; ++j) {
			IterType k = j;
			++k;
			if (k != nullptr && *j < *k)
				std::swap(*j, *k);
		}
	}
}

template <class IterType, class Type>
requires MustHave<IterType>
void copyIf(const IterType& sourceBegin, const IterType& sourceEnd, const IterType& destBegin, const Type& predicate) noexcept {
	IterType j = destBegin;
	for (IterType i = sourceBegin; i != sourceEnd && j != nullptr; ++i) {
		if (predicate(*i)) {
			*j = *i;
			++j;
		}
	}

}
