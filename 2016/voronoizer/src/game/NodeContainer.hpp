/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <array>
#include <iostream>
#include <iterator>
#include <raz/bitset.hpp>
#include "game/Node.hpp"

namespace game
{
	template<class T, size_t N>
	class NodeContainer
	{
	public:
		class Iterator : public std::iterator<std::input_iterator_tag, const T>
		{
		public:
			Iterator(const Iterator& other) :
				m_data(other.m_data), m_internal_iterator(other.m_internal_iterator)
			{
			}

			Iterator& operator=(const Iterator& other)
			{
				m_data = other.m_data;
				m_internal_iterator = other.m_internal_iterator;
				return *this;
			}

			Iterator& operator++()
			{
				++m_internal_iterator;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator retval = *this;
				++(*this);
				return retval;
			}

			bool operator==(const Iterator& other) const
			{
				return (m_data == other.m_data && m_internal_iterator == other.m_internal_iterator);
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			reference operator*() const
			{
				return m_data->at(*m_internal_iterator);
			}

		private:
			const std::array<T, N>* m_data;
			typename raz::Bitset<N>::TrueBitIterator m_internal_iterator;

			friend class NodeContainer;

			Iterator(const std::array<T, N>* data, typename raz::Bitset<N>::TrueBitIterator internal_iterator) :
				m_data(data), m_internal_iterator(internal_iterator)
			{
			}
		};

		NodeContainer(NodeType type) :
			m_type(type)
		{
		}

		T* add()
		{
			auto it = m_nodes_bits.falsebits().begin();

			if (it == m_nodes_bits.falsebits().end())
			{
#ifdef _DEBUG
				std::cerr << "NodeContainer: node limit exceeded" << std::endl;
#endif
				return nullptr;
			}

			size_t pos = *it;
			m_nodes_bits.set(pos);

			T* node = &m_nodes[pos];
			node->id.type = m_type;
			node->id.number = pos;
			return node;
		}

		T* add(NodeID id)
		{
#ifdef _DEBUG
			if (id.type != m_type)
			{
				std::cerr << "NodeContainer: node type mismatch" << std::endl;
				return nullptr;
			}
#endif

			size_t pos = id.number;

			if (m_nodes_bits.isset(pos))
			{
#ifdef _DEBUG
				std::cerr << "NodeContainer: node ID already exists" << std::endl;
#endif
				return nullptr;
			}

			m_nodes_bits.set(pos);

			T* node = &m_nodes[pos];
			node->id.type = m_type;
			node->id.number = pos;
			return node;
		}

		T* get(NodeID id)
		{
#ifdef _DEBUG
			if (id.type != m_type)
			{
				std::cerr << "NodeContainer: node type mismatch" << std::endl;
				return nullptr;
			}
#endif

			size_t pos = id.number;

			if (pos < N && m_nodes_bits.isset(pos))
				return &m_nodes[pos];
			else
				return nullptr;
		}

		const T* get(NodeID id) const
		{
#ifdef _DEBUG
			if (id.type != m_type)
			{
				std::cerr << "NodeContainer: node type mismatch" << std::endl;
				return nullptr;
			}
#endif

			size_t pos = id.number;

			if (pos < N && m_nodes_bits.isset(pos))
				return &m_nodes[pos];
			else
				return nullptr;
		}

		void remove(NodeID id)
		{
#ifdef _DEBUG
			if (id.type != m_type)
			{
				std::cerr << "NodeContainer: node type mismatch" << std::endl;
				//return;
			}
#endif

			size_t pos = id.number;

			if (m_nodes_bits.isset(pos))
			{
				m_nodes[pos].connections.reset();
				m_nodes_bits.unset(pos);
			}
		}

		size_t count() const
		{
			return m_nodes_bits.truebits().count();
		}

		Iterator begin() const
		{
			return Iterator(&m_nodes, m_nodes_bits.truebits().begin());
		}

		Iterator end() const
		{
			return Iterator(&m_nodes, m_nodes_bits.truebits().end());
		}

	private:
		NodeType m_type;
		std::array<T, N> m_nodes;
		raz::Bitset<N> m_nodes_bits;
	};
};
