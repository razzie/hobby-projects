/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include <iosfwd>
#include "gg/serializable.hpp"

namespace gg
{
	class IArchive;

	class Version : public ISerializable
	{
	public:
		Version(uint16_t major, uint16_t minor, uint16_t revision = 0) :
			m_major(major),
			m_minor(minor),
			m_revision(revision)
		{
		}

		Version(const Version& ver) :
			m_major(ver.m_major),
			m_minor(ver.m_minor),
			m_revision(ver.m_revision)
		{
		}

		~Version() = default;

		Version& operator=(const Version& ver)
		{
			m_major = ver.m_major;
			m_minor = ver.m_minor;
			m_revision = ver.m_revision;
		}

		uint16_t getMajor() const
		{
			return m_major;
		}

		uint16_t getMinor() const
		{
			return m_minor;
		}

		uint16_t getRevision() const
		{
			return m_revision;
		}

		bool operator==(const Version& ver) const
		{
			return (m_major == ver.m_major
				&& m_minor == ver.m_minor
				&& m_revision == ver.m_revision);
		}

		bool operator>(const Version& ver) const
		{
			if (m_major != ver.m_major)
				return (m_major > ver.m_major);
			else if (m_minor != ver.m_minor)
				return (m_minor > ver.m_minor);
			else
				return m_revision > ver.m_revision;
		}

		bool operator<(const Version& ver) const
		{
			if (m_major != ver.m_major)
				return (m_major < ver.m_major);
			else if (m_minor != ver.m_minor)
				return (m_minor < ver.m_minor);
			else
				return m_revision < ver.m_revision;
		}

		virtual void serialize(IArchive& ar, Version& ver)
		{
			ar & ver.m_major & ver.m_minor & ver.m_revision;
		}

		std::ostream& operator<<(std::ostream& o)
		{
			o << m_major << '.' << m_minor << '.' << m_revision;
		}

	private:
		uint16_t m_major;
		uint16_t m_minor;
		uint16_t m_revision;
	};
};
