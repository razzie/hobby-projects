/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <iostream>
#include "game/Node.hpp"
#include "game/Zone.hpp"


game::NodeType game::NodeID::getType() const
{
	return static_cast<NodeType>(type);
}

game::NodeID::operator uint16_t() const
{
	return *reinterpret_cast<const uint16_t*>(this);
}


game::NodeType game::Node::getType() const
{
	return static_cast<NodeType>(id.type);
}


game::Node::ConnectionContainer::Iterator::Iterator(const Iterator & other) :
	m_data(other.m_data),
	m_internal_iterator(other.m_internal_iterator)
{
}

game::Node::ConnectionContainer::Iterator::Iterator(const std::array<NodeID, MAX_CONNECTIONS>* data, raz::Bitset<MAX_CONNECTIONS>::TrueBitIterator internal_iterator) :
	m_data(data),
	m_internal_iterator(internal_iterator)
{
}

game::Node::ConnectionContainer::Iterator& game::Node::ConnectionContainer::Iterator::operator=(const Iterator& other)
{
	m_data = other.m_data;
	m_internal_iterator = other.m_internal_iterator;
	return *this;
}

game::Node::ConnectionContainer::Iterator& game::Node::ConnectionContainer::Iterator::operator++()
{
	++m_internal_iterator;
	return *this;
}

game::Node::ConnectionContainer::Iterator game::Node::ConnectionContainer::Iterator::operator++(int)
{
	Iterator retval = *this;
	++(*this);
	return retval;
}

bool game::Node::ConnectionContainer::Iterator::operator==(const Iterator& other) const
{
	return (m_data == other.m_data && m_internal_iterator == other.m_internal_iterator);
}

bool game::Node::ConnectionContainer::Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}

game::Node::ConnectionContainer::Iterator::reference game::Node::ConnectionContainer::Iterator::operator*() const
{
	return m_data->at(*m_internal_iterator);
}


game::Node::ConnectionContainer::ConnectionContainer()
{
}

bool game::Node::ConnectionContainer::set(NodeID id)
{
	auto it = m_connections_bits.falsebits().begin();

	if (it == m_connections_bits.falsebits().end())
	{
#ifdef _DEBUG
		std::cerr << "ConnectionContainer: node limit exceeded" << std::endl;
#endif
		return false;
	}

	size_t pos = *it;
	m_connections_bits.set(pos);

	m_connections[pos] = id;
	return true;
}

void game::Node::ConnectionContainer::unset(NodeID id)
{
	for (auto it : m_connections_bits.truebits())
	{
		if (m_connections[it] == id)
		{
			m_connections_bits.unset(it);
			return;
		}
	}
}

void game::Node::ConnectionContainer::reset()
{
	m_connections_bits.reset();
}

size_t game::Node::ConnectionContainer::count() const
{
	return m_connections_bits.truebits().count();
}

game::Node::ConnectionContainer::Iterator game::Node::ConnectionContainer::begin() const
{
	return Iterator(&m_connections, m_connections_bits.truebits().begin());
}

game::Node::ConnectionContainer::Iterator game::Node::ConnectionContainer::end() const
{
	return Iterator(&m_connections, m_connections_bits.truebits().end());
}
