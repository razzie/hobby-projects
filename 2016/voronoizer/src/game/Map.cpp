/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "game/Map.hpp"


game::Map::Map()
{
}

game::Map::~Map()
{
}

game::ZoneID game::Map::addZone(const std::vector<ZoneBuilder::Point>& polygon)
{
	return ZoneID();
}

game::ZoneID game::Map::addZone(int radius, int edges, float angle, ZoneBuilder::Point position)
{
	return ZoneID();
}

const game::Zone* game::Map::getZone(ZoneID zone)
{
	return nullptr;
}
