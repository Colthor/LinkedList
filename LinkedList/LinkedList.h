#pragma once
#include<memory>
#include<utility>
#include<cassert>
#include<stdexcept>



template <class T>
class LinkedList
{
template <class T>
class ListIterator;

public:
	LinkedList() = default;
	LinkedList(const LinkedList<T>& other) { *this = other; }
	LinkedList(LinkedList<T>&& other) { *this = other; }
	LinkedList(const std::initializer_list<T>& il);

	~LinkedList() { empty_list(); }

	void push_front(T in);
	T pop_front();
	void push_back(T in);
	T pop_back();
	size_t size() { return m_size; }
	void empty_list();

	ListIterator<T> begin() { return ListIterator<T>(this->m_first.get()); };
	ListIterator<T> end() { return ListIterator<T>(nullptr); };
	ListIterator<T> rbegin() { return ListIterator<T>(this->m_last); };
	ListIterator<T> rend() { return ListIterator<T>(nullptr); };

	inline bool operator==(const LinkedList<T>& other)
	{
		if (m_size != other.m_size) return false;
		Node<T>* lPtr = m_last;
		Node<T>* rPtr = other.m_last;
		while (lPtr != nullptr && rPtr != nullptr)
		{
			if (lPtr->data != rPtr->data) return false;
			lPtr = lPtr->prev;
			rPtr = rPtr->prev;
		}
		return true;
	}
	inline bool operator!=(const LinkedList<T>& other) { return !(*this == other); };

	inline LinkedList<T>& operator=(const LinkedList<T>& other)
	{
		if (*this != other)
		{
			empty_list();
			auto pEnd = other.m_last;
			while (nullptr != pEnd)
			{
				push_front(pEnd->data);
				pEnd = pEnd->prev;
			}
		}
		return *this;
	}
	inline LinkedList<T>& operator=(LinkedList<T>&& other) noexcept
	{
		if (*this != other)
		{
			empty_list();
			m_first = std::move(other.m_first);
			m_last = other.m_last;
			m_size = other.m_size;
			assert(other.m_first == nullptr);
			other.m_last = nullptr;
			other.m_size = 0;
		}
		return *this;
	}

	

private:
	
	template<class T>
	struct Node
	{
		T data{};
		std::unique_ptr<Node<T>> next {nullptr};
		Node<T>* prev{nullptr};

		inline bool operator==(const Node<T>& other) { return data == other.data; }
		inline bool operator!=(const Node<T>& other) { return data != other.data; };

		friend class ListIterator<T>;
	};

	template<class T>
	class ListIterator
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

	public:
		ListIterator() = default;
		ListIterator(LinkedList::Node<T>* start) :current(start) {}
		ListIterator(const ListIterator<T>& other) :current(other->current) {}

		inline T& operator*()
		{
			if (nullptr != current)
			{
				return (current->data);
			}
			else
			{
				auto err = std::range_error("Dereferenced LinkedList::end()");
				throw err;
			}
		}

		inline bool operator==(const ListIterator<T>& other) { return current == other.current; }
		inline bool operator!=(const ListIterator<T>& other) { return current != other.current; }

		inline ListIterator<T>& operator++()
		{
			if (nullptr != current)
			{
				current = current->next.get();
			}
			return *this;
		}
		inline ListIterator<T>& operator--()
		{
			if (nullptr != current)
			{
				current = current->prev;
			}
			return *this;
		}
		ListIterator<T> operator++(int) { auto tmp = *this; ++(*this); return tmp; }
		ListIterator<T> operator--(int) { auto tmp = *this; --(*this); return tmp; }

	private:
		LinkedList::Node<T>* current = nullptr;
	};

	std::unique_ptr<Node<T>> m_first{ nullptr };
	Node<T>* m_last{ nullptr };
	size_t m_size{ 0 };

	
};



template <class T>
LinkedList<T>::LinkedList(const std::initializer_list<T>& il)
{
	for (auto i : il)
	{
		push_back(i);
	}
}


template <class T>
void LinkedList<T>::push_front(T in)
{
	auto tmp = std::move(m_first);
	m_first = std::make_unique<Node<T>>();
	if (nullptr != tmp)
	{
		tmp->prev = m_first.get();
	}
	else
	{
		m_last = m_first.get();
	}
	m_first->next = std::move(tmp);
	m_first->data = in;
	++m_size;
	
}

template <class T>
T LinkedList<T>::pop_front()
{
	if (m_size > 0)
	{
		T td = m_first->data;
		m_first = std::move(m_first->next);
		if (nullptr != m_first)
		{
			m_first->prev = nullptr;
		}
		else
		{
			m_last = nullptr;
		}
		--m_size;
		return td;
	}
	else
	{
		std::length_error err("pop_back() called on empty list");
		throw err;
	}
}

template <class T>
void LinkedList<T>::push_back(T in)
{
	std::unique_ptr<Node<T>> tmp;
	tmp = std::make_unique<Node<T>>();
	tmp->data = in;

	if (nullptr != m_last)
	{
		tmp->prev = m_last;
		m_last->next = std::move(tmp);
		m_last = m_last->next.get();
	}
	else
	{
		m_last = tmp.get();
		m_first = std::move(tmp);
	}
	++m_size;
	
}

template <class T>
T LinkedList<T>::pop_back()
{
	if (m_size > 0)
	{
		T td = m_last->data;
		if (nullptr != m_last->prev)
		{
			m_last = m_last->prev;
			m_last->next = nullptr;
		}
		else
		{
			m_last = nullptr;
			m_first = nullptr;
		}
		--m_size;
		return td;
	}
	else
	{
		std::length_error err("pop_back() called on empty list");
		throw err;
	}
}

//prevents stack overflow due to recursive unique_ptr deletions
template <class T>
void LinkedList<T>::empty_list()
{
	auto pEnd = m_last;
	while (nullptr != pEnd)
	{
		pEnd->next = nullptr;
		pEnd = pEnd->prev;
	}
	m_size = 0;
	m_last = nullptr;
	m_first = nullptr;
}


