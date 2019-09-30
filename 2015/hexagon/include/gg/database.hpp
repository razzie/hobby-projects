/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#if defined GGDATABASE_BUILD
#	define GG_API __declspec(dllexport)
#else
#	define GG_API __declspec(dllimport)
#endif

#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <vector>
#include "gg/serializable.hpp"

namespace gg
{
	class IDatabase : public ISerializable
	{
	public:
		typedef uint16_t Key;

		enum AccessType
		{
			NO_ACCESS,
			READ,
			READ_WRITE
		};

		class IAccessError : public std::exception
		{
		public:
			virtual ~IAccessError() = default;
			virtual const char* what() = 0;
			virtual AccessType getRequestedAccess() const = 0;
			virtual AccessType getActualAccess() const = 0;
		};

		class ICell : public ISerializable
		{
		public:
			enum Type : uint16_t
			{
				NONE,
				INT32,
				INT64,
				FLOAT,
				DOUBLE,
				STRING
			};

			virtual ~ICell() = default;
			virtual Type getType() const = 0;
			virtual int32_t getInt32() const = 0;
			virtual int64_t getInt64() const = 0;
			virtual float getFloat() const = 0;
			virtual double getDouble() const = 0;
			virtual std::string getString() const = 0;
			virtual void set(int32_t) = 0;
			virtual void set(int64_t) = 0;
			virtual void set(float) = 0;
			virtual void set(double) = 0;
			virtual void set(const std::string&) = 0;
		};

		class IRow : public ISerializable
		{
		public:
			virtual ~IRow() = default;
			virtual AccessType getAccessType() const = 0;
			virtual Key getKey() const = 0;
			virtual ICell* cell(unsigned column) = 0;
			virtual ICell* cell(const std::string& column) = 0;
			virtual const ICell* cell(unsigned column) const = 0;
			virtual const ICell* cell(const std::string& column) const = 0;
			virtual void remove() = 0; // removes row after it's not referenced anywhere
		};

		typedef std::shared_ptr<IRow> RowPtr;

		class ITable : public ISerializable
		{
		public:
			virtual ~ITable() = default;
			virtual AccessType getAccessType() const = 0;
			virtual const std::string& getName() const = 0;
			virtual RowPtr createAndGetRow(bool write_access = true) = 0;
			virtual RowPtr getRow(Key, bool write_access = true) = 0;
			virtual RowPtr getNextRow(Key, bool write_access = true) = 0;
			virtual void remove() = 0; // removes table after it's not referenced anywhere
		};

		typedef std::shared_ptr<ITable> TablePtr;

		virtual ~IDatabase() = default;
		virtual const std::string& getFilename() const = 0;
		virtual TablePtr createAndGetTable(const std::string& table, const std::vector<std::string>& columns, bool write_access = true) = 0;
		virtual TablePtr createAndGetTable(const std::string& table, unsigned columns, bool write_access = true) = 0;
		virtual TablePtr getTable(const std::string& table, bool write = true) = 0;
		virtual void getTableNames(std::vector<std::string>& tables) const = 0;
		virtual bool save() = 0;
	};

	typedef std::shared_ptr<IDatabase> DatabasePtr;

	class IDatabaseManager
	{
	public:
		virtual ~IDatabaseManager() = default;
		virtual DatabasePtr open(const std::string& filename) const = 0;
	};

	extern GG_API IDatabaseManager& db;
};
