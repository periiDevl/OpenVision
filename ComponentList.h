#ifndef COMPONENT_LIST_HEADER
#define COMPONENT_LIST_HEADER

#include <cinttypes>
#include <vector>
#include "UUID.h"


template<typename T>
class Iterator
{
	using iterator_category = std::random_access_iterator_tag;
	using difference_typye  = std::ptrdiff_t;
	using value_type        = T;
	using pointer           = T*;
	using reference         = T&;

public:
	Iterator(pointer ptr) : m_ptr(ptr) {}

	reference operator *() { return *m_ptr; }
	pointer operator ->() { return m_ptr; }

	Iterator<T>& operator++()
	{
		m_ptr++;
		return *this;
	}

	Iterator<T> operator++(int)
	{
		Iterator<T> tmp = *this;

		++(*this);

		return tmp;
	}


	Iterator<T>& operator--()
	{
		m_ptr--;
		return *this;
	}

	Iterator<T> operator--(int)
	{
		Iterator<T> tmp = *this;

		--(*this);

		return tmp;
	}

	friend bool operator==(const Iterator<T>& a, const Iterator<T>& b)
	{
		return a.m_ptr == b.m_ptr;
	}

	friend bool operator!=(const Iterator<T>& a, const Iterator<T>& b)
	{
		return a.m_ptr != b.m_ptr;
	}

private:
	pointer m_ptr;
};


template <typename T>
class ComponentList
{
public:
	ComponentList()
	{
		m_componentID = UUIDManager::getUUID<T>();
	}

	// Copy Constructor
	ComponentList(const ComponentList& other) : m_componentID(other.m_componentID), m_list(other.m_list)
	{ }

	// Move Constructor
	ComponentList(ComponentList&& other) noexcept :
		m_componentID(std::move(other.m_componentID)), m_list(std::move(other.m_list))
	{
		other.m_componentID = 0;
	}

	// Move Assignment Operator
	ComponentList& operator=(ComponentList&& other) noexcept
	{
		if (this != &other)
		{
			m_componentID = std::move(other.m_componentID);
			other.m_componentID = 0;

			m_list = std::move(other.m_list);
		}
		return *this;
	}

	/// <summary>
	/// Add an rvalue to the end of the list
	/// The rvalue is the component, assumes the component list is used correctly
	/// </summary>
	/// <typeparam name="T">Component type</typeparam>
	/// <param name="value">Component as rvalue</param>
	/// <returns>The index it was saved at</returns>
	size_t add(T&& value)
	{
		m_list.emplace_back(std::move(value));
		return m_list.size() - 1;
	}

	/// <summary>
	/// Remove the value at the specified index
	/// Removing an item means, clearing it's memory and calling it's destructor
	/// It also copies the value from the end of the list, to this place for O(1) complexity
	/// It returns the index of the value that was moved, what now is the list size.
	/// </summary>
	/// <typeparam name="T">Component type</typeparam>
	/// <param name="index">The position of the component that needs to be removed</param>
	/// <returns>Index that was moved to the index</returns>
	size_t remove(size_t index)
	{
		T& last = m_list.back();
		m_list[index] = std::move(last);
		m_list.pop_back();
		return m_list.size();
	}

	T& operator[](size_t index)
	{
		return m_list[index];
	}

	Iterator<T> begin()
	{
		return Iterator<T>(m_list.data());
	}

	Iterator<T> end()
	{
		return Iterator<T>(m_list.data() + m_list.size());
	}
	~ComponentList()
	{ }

private:
	std::vector<T> m_list;
	UUID m_componentID;
};

#endif 
