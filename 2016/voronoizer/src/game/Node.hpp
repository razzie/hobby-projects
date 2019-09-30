/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <array>
#include <cstdint>
#include <iterator>
#include <raz/bitset.hpp>
#include "util/Math2D.hpp"

namespace game
{
	enum NodeType : uint16_t
	{
		UNKNOWN = 0,
		PATH,
		HUB,
		PORTAL
	};

	struct NodeID
	{
		uint16_t type : 3;
		uint16_t number : 13;

		NodeType getType() const;
		operator uint16_t() const;
	};

	struct Node
	{
		typedef util::Point2D<float> Position;

		class ConnectionContainer
		{
		public:
			static constexpr size_t MAX_CONNECTIONS = 32;

			class Iterator : public std::iterator<std::input_iterator_tag, NodeID, uint16_t, NodeID, NodeID>
			{
			public:
				Iterator(const Iterator& other);
				Iterator& operator=(const Iterator& other);
				Iterator& operator++();
				Iterator operator++(int);
				bool operator==(const Iterator& other) const;
				bool operator!=(const Iterator& other) const;
				reference operator*() const;

			private:
				const std::array<NodeID, MAX_CONNECTIONS>* m_data;
				raz::Bitset<MAX_CONNECTIONS>::TrueBitIterator m_internal_iterator;

				friend class ConnectionContainer;

				Iterator(const std::array<NodeID, MAX_CONNECTIONS>* data, raz::Bitset<MAX_CONNECTIONS>::TrueBitIterator internal_iterator);
			};

			ConnectionContainer();
			bool set(NodeID id);
			void unset(NodeID id);
			void reset();
			size_t count() const;
			Iterator begin() const;
			Iterator end() const;

		private:
			std::array<NodeID, MAX_CONNECTIONS> m_connections;
			raz::Bitset<MAX_CONNECTIONS> m_connections_bits;
		};

		NodeID id;
		Position position;
		ConnectionContainer connections;

		NodeType getType() const;
	};

	class Zone;
	struct PathNode : public Node
	{
		NodeID hub1;
		NodeID hub2;
	};

	struct HubNode : public Node
	{
		float radius;
	};

	struct PortalNode : public Node
	{
		NodeID other_portal;
		uint16_t other_zone;
	};

};
