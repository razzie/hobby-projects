/*
 * Copyright (C) Gábor Görzsöny <gabor@gorzsony.com> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#pragma once

#include <map>
#include "game/Zone.hpp"
#include "game/ZoneBuilder.hpp"

namespace game
{
	class Map
	{
	public:
		Map();
		~Map();
		ZoneID addZone(const std::vector<ZoneBuilder::Point>& polygon);
		ZoneID addZone(int radius, int edges, float angle, ZoneBuilder::Point position);
		const Zone* getZone(ZoneID zone);

	private:
		std::map<ZoneID, Zone> m_zones;
		ZoneID m_next_zone = 0;
	};
};
