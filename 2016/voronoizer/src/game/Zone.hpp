/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <thirdparty/micropather.h>
#include "game/Node.hpp"
#include "game/NodeContainer.hpp"
#include "util/Math2D.hpp"

namespace game
{
	class Map;

	typedef uint16_t ZoneID;

	class Zone : public micropather::Graph
	{
	public:
		static constexpr size_t MAX_NODES = 256;

		typedef util::Point2D<float> Position;
		typedef util::BoundingBox2D<float> BoundingBox;

		Zone(ZoneID id, Map* map);
		~Zone();
		ZoneID getID() const;
		const BoundingBox& getBoundingBox() const;
		const Node* getNode(NodeID id) const;
		const NodeContainer<PathNode, MAX_NODES>& getPaths() const;
		const NodeContainer<HubNode, MAX_NODES>& getHubs() const;
		const NodeContainer<PortalNode, MAX_NODES>& getPortals() const;

		// inherited from Graph
		virtual float LeastCostEstimate(void* stateStart, void* stateEnd);
		virtual void AdjacentCost(void* state, std::vector<micropather::StateCost>* adjacent);
		virtual void  PrintStateInfo(void* state);

	private:
		friend class ZoneBuilder;

		ZoneID m_id;
		NodeContainer<PathNode, MAX_NODES> m_paths;
		NodeContainer<HubNode, MAX_NODES> m_hubs;
		NodeContainer<PortalNode, MAX_NODES> m_portals;
		Map* m_map;
		BoundingBox m_bbox;

		Node* getNode(NodeID id);
		void removeNode(NodeID id);
		bool connectNodes(NodeID node1, NodeID node2);
		bool disconnectNodes(NodeID node1, NodeID node2);
		const PathNode* addPath(NodeID hub1, NodeID hub2);
		const HubNode* addHub(Position position);
		const PortalNode* addPortal(NodeID hub, ZoneID other_zone, NodeID other_node);
	};
};
