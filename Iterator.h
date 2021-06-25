#pragma once
#include <iostream>
#include "Container.h"
#include "Exception.h"

template <class _DataType>
class List;

template <class _DataType>
class ListIterator {

private:

	friend class List <_DataType>;

	const List <_DataType>* ptr;

	typedef typename List <_DataType>::Node N;
	N* cur;

public:

	ListIterator() noexcept : cur(nullptr), ptr(nullptr)
	{}

	explicit ListIterator(const List<_DataType>& pt, N* pt_cur) noexcept : ptr(&pt), cur(pt_cur)
	{}

	ListIterator(const ListIterator& orig) noexcept : ptr(orig.ptr), cur(orig.cur) {}

	~ListIterator() noexcept  {
		ptr = nullptr;
		cur = nullptr;
	}

	ListIterator& operator = (const ListIterator& orig) noexcept {
		if (this == &orig)
			return *this;
		ptr = orig.ptr;
		cur = orig.cur;
		
		return *this; //!!! Ошибка - возврат по ссылке на локальную переменную (Исправлено)
	}

	ListIterator& operator ++ () { //!!! Лишнее копирование при возврате (Исправлено)
		if (cur == ptr->last->next)
			throw IteratorError("An attempt to go out of the list has been done");

		cur = cur->next;
		return *this;
	}

	ListIterator& operator -- () { //!!! Лишнее копирование при возврате (Исправлено)
		if (cur == ptr->first->prev)
			throw IteratorError("An attempt to go out of the list has been done");

		cur = cur->prev;
		return *this;
	}

	ListIterator operator ++ (int) {
		if (cur == ptr->last->next)
			throw IteratorError("An attempt to go out of the list has been done");

		ListIterator res(*this);
		cur = cur->next;
		return res;
	}

	ListIterator operator -- (int) {
		if (cur == ptr->first->prev)
			throw IteratorError("An attempt to go out of the list has been done");

		ListIterator res(*this);
		cur = cur->prev;
		return res;
	}

	_DataType& operator * () const noexcept {
		return cur->data;
	}
	
	_DataType* operator -> () const noexcept {
		return &cur->data;
	}

	bool operator == (const ListIterator& iter) const noexcept {
		return ptr == iter.ptr && cur == iter.cur;
	}
	
	bool operator != (const ListIterator& orig) const noexcept {
		return ptr != orig.ptr || cur != orig.cur;
	}

	operator _DataType* () const noexcept {
		return &cur->data;
	}

};