/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <iostream>
#include "game/Zone.hpp"
#include "game/Map.hpp"


game::Zone::Zone(ZoneID id, Map* map) :
	m_id(id),
	m_paths(NodeType::PATH),
	m_hubs(NodeType::HUB),
	m_portals(NodeType::PORTAL),
	m_map(map)
{
}

game::Zone::~Zone()
{
}

game::ZoneID game::Zone::getID() const
{
	return m_id;
}

const game::Zone::BoundingBox& game::Zone::getBoundingBox() const
{
	return m_bbox;
}

game::Node* game::Zone::getNode(NodeID id)
{
	switch (id.getType())
	{
	case NodeType::PATH:
		return m_paths.get(id);

	case NodeType::HUB:
		return m_hubs.get(id);

	case NodeType::PORTAL:
		return m_hubs.get(id);
	}

	return nullptr;
}

const game::Node* game::Zone::getNode(NodeID id) const
{
	switch (id.getType())
	{
	case NodeType::PATH:
		return m_paths.get(id);

	case NodeType::HUB:
		return m_hubs.get(id);

	case NodeType::PORTAL:
		return m_hubs.get(id);
	}

	return nullptr;
}

void game::Zone::removeNode(NodeID id)
{
	switch (id.getType())
	{
	case NodeType::PATH:
		m_paths.remove(id);

	case NodeType::HUB:
		m_hubs.remove(id);

	case NodeType::PORTAL:
		m_portals.remove(id);
	}
}

bool game::Zone::connectNodes(NodeID node1_id, NodeID node2_id)
{
	Node* node1 = getNode(node1_id);
	Node* node2 = getNode(node2_id);

	if (!node1 || !node2)
		return false;

	node1->connections.set(node2_id);
	node2->connections.set(node1_id);
	return true;
}

bool game::Zone::disconnectNodes(NodeID node1_id, NodeID node2_id)
{
	Node* node1 = getNode(node1_id);
	Node* node2 = getNode(node2_id);

	if (!node1 || !node2)
		return false;

	node1->connections.unset(node2_id);
	node2->connections.unset(node1_id);
	return true;
}

const game::NodeContainer<game::PathNode, game::Zone::MAX_NODES>& game::Zone::getPaths() const
{
	return m_paths;
}

const game::NodeContainer<game::HubNode, game::Zone::MAX_NODES>& game::Zone::getHubs() const
{
	return m_hubs;
}

const game::NodeContainer<game::PortalNode, game::Zone::MAX_NODES>& game::Zone::getPortals() const
{
	return m_portals;
}

float game::Zone::LeastCostEstimate(void* stateStart, void* stateEnd)
{
	Position p1 = reinterpret_cast<Node*>(stateStart)->position;
	Position p2 = reinterpret_cast<Node*>(stateEnd)->position;
	return p1.getDistanceFrom(p2);
}

void game::Zone::AdjacentCost(void* state, std::vector<micropather::StateCost>* adjacent)
{
	Node& node = *reinterpret_cast<Node*>(state);

	for (auto id : node.connections)
	{
		micropather::StateCost cost;
		cost.state = const_cast<Node*>(getNode(id));
		cost.cost = LeastCostEstimate(&node, cost.state);
		adjacent->push_back(cost);
	}

	if (node.getType() == NodeType::PORTAL && m_map)
	{
		const PortalNode& portal_node = static_cast<const PortalNode&>(node);

		const Zone* other_zone = m_map->getZone(portal_node.other_zone);
		const HubNode* other_hub = static_cast<const HubNode*>(other_zone->getNode(portal_node.other_portal));

		micropather::StateCost cost;
		cost.state = const_cast<HubNode*>(other_hub);
		cost.cost = 0.f;
		adjacent->push_back(cost);
	}
}

void game::Zone::PrintStateInfo(void* state)
{
#ifdef _DEBUG
	Node& node = *reinterpret_cast<Node*>(state);
	std::cerr << "(Node - type: " << node.getType() << ", id: " << node.id << ")" << std::flush;
#endif
}

const game::PathNode* game::Zone::addPath(NodeID hub1, NodeID hub2)
{
	// check hub1 and hub2
	auto hub1_node = static_cast<HubNode*>(getNode(hub1));
	auto hub2_node = static_cast<HubNode*>(getNode(hub2));

	if (!hub1_node || hub1_node->getType() != NodeType::HUB
		|| !hub2_node || hub2_node->getType() != NodeType::HUB)
	{
#ifdef _DEBUG
		std::cerr << "cannot connect path to non-hub" << std::endl;
#endif
		return nullptr;
	}

	// add path
	PathNode* path = m_paths.add();
	if (!path)
		return nullptr;

	// connect to hub1 and hub2
	path->hub1 = hub1;
	path->hub2 = hub2;
	path->connections.set(hub1);
	path->connections.set(hub2);

	hub1_node->connections.set(path->id);
	hub1_node->radius += 1.f;

	hub2_node->connections.set(path->id);
	hub2_node->radius += 1.f;

	// calculate midpoint
	path->position = {
		(hub1_node->position.x + hub2_node->position.x) / 2.f,
		(hub1_node->position.y + hub2_node->position.y) / 2.f
	};

	return path;
}

const game::HubNode* game::Zone::addHub(Position position)
{
	// add hub
	HubNode* hub = m_hubs.add();
	if (!hub)
		return nullptr;

	hub->position = position;
	hub->radius = 1.f;

	return hub;
}

const game::PortalNode* game::Zone::addPortal(NodeID hub, ZoneID other_zone, NodeID other_portal)
{
	// check hub
	auto hub_node = static_cast<HubNode*>(getNode(hub));

	if (hub_node->getType() != NodeType::HUB)
	{
#ifdef _DEBUG
		std::cerr << "cannot connect portal to non-hub" << std::endl;
#endif
		return nullptr;
	}

	// add portal
	PortalNode* portal = m_portals.add();
	if (!portal)
		return nullptr;

	// connect to hub in other zone
	hub_node->connections.set(portal->id);
	portal->connections.set(hub);
	portal->position = hub_node->position;
	portal->other_zone = other_zone;
	portal->other_portal = other_portal;

	return portal;
}
