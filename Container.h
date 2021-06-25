#pragma once
#include <iostream>
#include <initializer_list>
#include "Iterator.h" 
#include "Exception.h"

template <class _DataType>
class ListIterator;

template <class _DataType>
class List {

private:

	friend class ListIterator <_DataType>;

	struct Node { //!!! Переименовать в Node (Исправлено)
		_DataType data;
		Node* next;
		Node* prev;

		Node() noexcept 
			: data(_DataType())
			, next(nullptr)
			, prev(nullptr)
		{}
		
		explicit Node(
				const _DataType& data, 
				Node* ptr, 
				Node* ptr2) noexcept
			: data(data)
			, next(ptr)
			, prev(ptr2)
		{}
	};

	int size;
	Node* first;
	Node* last;

public:

	using iterator = ListIterator<_DataType>;

	List() noexcept : size(0), first(nullptr), last(nullptr) {}


    //!!! Почему данный конструкто не explicit (Исправлено)
	explicit List(int n) : size(n), first(nullptr), last(nullptr) {
		if (size <= 0)
			throw ContainerError("The size of the list must be a positive integer number");
	}

	explicit List(int n, const _DataType& data) : size(n), first(nullptr), last(nullptr) {
		if (size <= 0)
			throw ContainerError("The size of the list must be a positive integer number");
		for (int i = 0; i < size; ++i) {
			Node* cur = new Node(data, nullptr, last);
			if (last != nullptr)
				last->next = cur;
			last = cur;
			if (first == nullptr)
				first = cur;
		}
	}

	template <class IterType>
	List(const IterType& begin, const IterType& end) noexcept : first(nullptr), last(nullptr) {
		size = 0;
		for (auto i = begin; i != end; ++i) {
			Node* cur = new Node(*i, nullptr, last);
			if (last != nullptr)
				last->next = cur;
			last = cur;
			if (first == nullptr)
				first = cur;
			++size;
		}
	}

	List(const std::initializer_list <_DataType>& list) noexcept : size(list.size()), first(nullptr), last(nullptr) {
	    
		for (const auto& list_data : list) {                               //!!! const auto & (Исправлено)
		
			Node* cur = new Node(list_data, nullptr, last);
			if (last != nullptr)
				last->next = cur;
			last = cur;
			if (first == nullptr)
				first = cur;
		}
	}

	List(const List& list_data) noexcept : size(list_data.size), first(nullptr), last(nullptr) {
		Node* cur2 = list_data.first;
		
		for (int i = 0; i < size; ++i) {
			Node* cur = new Node(cur2->data, nullptr, last);
			if (last != nullptr)
				last->next = cur;
			last = cur;
			if (first == nullptr)
				first = cur;
			cur2 = cur2->next;
		}
	}

	List(List&& list_data) noexcept : size(list_data.size), first(list_data.first), last(list_data.last) {
		list_data.size = 0;
		list_data.first = nullptr;
		list_data.last = nullptr;
	}

	~List() noexcept {
		clear();
	}

	List& operator = (const List& list) noexcept {
		if (this == &list)
			return *this;

		this->clear(); //!!! Грубая ошибка (Исправлено)
		
		size = list.size;
		Node* cur2 = list.first;

		for (int i = 0; i < size; ++i) {
			Node* cur = new Node;
			cur->data = cur2->data;
			cur->next = nullptr;
			cur->prev = last;
			if (last != nullptr)
				last->next = cur;
			last = cur;
			if (first == nullptr)
				first = cur;
			cur2 = cur2->next;
		}

		return *this;
	}

	List& operator = (List&& data) noexcept {
	    
		size = data.size;
		first = data.first;
		last = data.last;

		data.size = 0;
		data.first = nullptr;
		data.last = nullptr;
		
		return *this;
	}

	iterator begin() const {
		if (first == nullptr)
			throw ContainerError("The list is empty");

		return iterator(*this, first);
	}

	iterator end() const {
		if (last == nullptr)
			throw ContainerError("The list is empty");

		return iterator(*this, last);
	}

	iterator middle() const {
		if (first == nullptr || last == nullptr)
			throw ContainerError("The list is empty");

		Node* curf = first;
		Node* curl = last;

		while (curf != curl && (curf->next != nullptr || curl->prev != nullptr)) {
			if (curf->next != nullptr)
				curf = curf->next;
			if (curf == curl)
				break;
			if (curl->prev != nullptr)
				curl = curl->prev;
			if (curf == curl)
				break;
		}

		return iterator(*this, curf);
	}

	void pushBack(const _DataType& elem) {
		Node* cur = new Node(elem, nullptr, last);
		if (last != nullptr)
			last->next = cur;
		last = cur;
		if (first == nullptr)
			first = cur;
		++size;
	}

	void popBack() {
		if (first == nullptr)
			throw ContainerError("The list was empty");

		if (first == last) {
			delete first;
			first = last = nullptr;
		}

		else {
			Node* cur = last->prev;
			delete last;
			cur->next = nullptr;
			last = cur;
		}
		--size;
	}

	void pushFront(const _DataType& elem) {
		Node* cur = new Node(elem, first, nullptr);
		if (first != nullptr)
			first->prev = cur;
		first = cur;
		if (last == nullptr)
			last = cur;
		++size;
	}

	void popFront() {
		if (first == nullptr)
			throw ContainerError("The list is empty");

		if (first == last) {
			delete first;
			first = last = nullptr;
		}

		else {
			Node* cur = first->next;
			delete first;
			cur->prev = nullptr;
			first = cur;
		}
		--size;
	}

	iterator insert(iterator pos, const _DataType& elem) {
		if (this != pos.ptr)
			throw ContainerError("The iterator doesn't belong to the list");

		Node* temp = pos.cur;

		if (temp == first) {
			pushFront(elem);
			return iterator(*this, first);
		}

		Node* cur = new Node(elem, temp, temp->prev);
		Node* temp2 = temp->prev;
		temp->prev = cur;
		temp2->next = cur;
		++size;

		return iterator(*this, pos.cur->prev);
	}

    //!!! Итераторы передают по значению (Исправлено)

	template <class IterType>
	iterator insert(iterator pos, const IterType& begin, const IterType& end) {
		if (this != pos.ptr)
			throw ContainerError("The iterator doesn't belong to the list");

		Node* temp = pos.cur;

		if (temp == first) {

			for (auto i = begin; i != end; ++i) {
				if (i == begin) {
					Node* cur = new Node(*i, first, nullptr);
					first->prev = cur;
					first = cur;
					++size;
				}
				else {
					Node* cur = new Node(*i, temp, temp->prev);
					Node* temp2 = temp->prev;
					temp->prev = cur;
					temp2->next = cur;
					++size;
				}
			}

			return iterator(*this, first);
		}

		iterator res(*this, temp->prev);
		for (auto i = begin; i != end; ++i) {
			Node* cur = new Node(*i, temp, temp->prev);
			Node* temp2 = temp->prev;
			temp->prev = cur;
			temp2->next = cur;
			++size;
		}
		
		++res;
		return res;
	}

	iterator erase(iterator pos) {
		if (this != pos.ptr)
			throw ContainerError("The iterator doesn't belong to the list");

		Node* temp = pos.cur;

		if (temp == first) {
			popFront();
			return iterator(*this, first);
		}

		if (temp == last) {
			popBack();
			return iterator(*this, last);
		}

		Node* temp2 = temp->next;
		temp2->prev = temp->prev;
		temp2 = temp2->prev;
		temp2->next = temp->next;

		--size;
		delete temp;

		return iterator(*this, temp2->next);
	}

	void clear() noexcept {
		if (first == nullptr)
			return;
		while (size > 0)
			popFront();
	}

	void resize(int num) {
		if (num < 0)
			throw ContainerError("The size of the list must be a positive integer number");

		if (num == 0)
			clear();

		while (num > size)
				pushBack(_DataType());

		while (num < size)
			popBack();
	}

	bool empty() const noexcept {
		return (size == 0);
	}

	int get_size() const noexcept {
		return size;
	}

};