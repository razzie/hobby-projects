/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

/**
 * HOW TO USE:
 * -----------
 *
 */

#pragma once

#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <typeinfo>
#include "gg/serializable.hpp"
#include "gg/event.hpp"
#include "gg/storage.hpp"

#if defined GGNETWORK_BUILD
#	define GG_API __declspec(dllexport)
#else
#	define GG_API __declspec(dllimport)
#endif

namespace gg
{
	class IPacket;
	class IConnection;
	class IConnectionBackend;
	class IServer;
	class IServerBackend;

	typedef std::shared_ptr<IPacket> PacketPtr;
	typedef std::shared_ptr<IConnection> ConnectionPtr;
	typedef std::unique_ptr<IConnectionBackend> ConnectionBackendPtr;
	typedef std::shared_ptr<IServer> ServerPtr;
	typedef std::unique_ptr< IServerBackend > ServerBackendPtr;

	class IPacket : public virtual IArchive
	{
	public:
		typedef IEvent::Type Type;

		virtual ~IPacket() = default;
		virtual Type getType() const = 0;
		virtual const char* getData() const = 0;
		virtual size_t getSize() const = 0;

		/* inherits all IArchive functions */
	};

	template<class T>
	IPacket& operator& (PacketPtr p, T& t)
	{
		IPacket& packet = *p;
		packet & t;
		return packet;
	}

	class IConnectionBackend // adaption to external APIs like Steam
	{
	public:
		virtual ~IConnectionBackend() = default;
		virtual bool connect(void* user_data = nullptr) = 0;
		virtual void disconnect() = 0;
		virtual bool isAlive() const = 0;
		virtual const std::string& getAddress() const = 0;
		virtual size_t availableData() = 0;
		virtual size_t waitForData(size_t len, uint32_t timeoutMs = 0) = 0; // 0: non-blocking
		virtual size_t peek(char* ptr, size_t len) = 0;
		virtual size_t read(char* ptr, size_t len) = 0;
		virtual size_t write(const char* ptr, size_t len) = 0;
	};

	class IConnection
	{
	public:
		virtual ~IConnection() = default;
		virtual bool connect(void* user_data = nullptr) = 0;
		virtual void disconnect() = 0;
		virtual bool isAlive() const = 0;
		virtual const std::string& getAddress() const = 0;
		virtual PacketPtr getNextPacket(uint32_t timeoutMs = 0) = 0; // 0: non-blocking
		virtual PacketPtr createPacket(IPacket::Type) const = 0;
		virtual PacketPtr createPacket(EventPtr) const = 0;
		virtual bool send(PacketPtr) = 0;
	};

	class IServerBackend // adaption to external APIs like Steam
	{
	public:
		virtual ~IServerBackend() = default;
		virtual bool start(void* user_data = nullptr) = 0;
		virtual void stop() = 0;
		virtual bool isAlive() const = 0;
		virtual ConnectionBackendPtr getNextConnection(uint32_t timeoutMs = 0) = 0; // 0: non-blocking
	};

	class IServer
	{
	public:
		virtual ~IServer() = default;
		virtual bool start(void* user_data = nullptr) = 0;
		virtual void stop() = 0;
		virtual bool isAlive() const = 0;
		virtual ConnectionPtr getNextConnection(uint32_t timeoutMs = 0) = 0; // 0: non-blocking
		virtual void closeConnections() = 0;
	};

	class INetworkException : public std::exception
	{
	public:
		virtual ~INetworkException() = default;
		virtual const char* what() const = 0;
	};

	class INetworkManager
	{
	public:
		virtual ~INetworkManager() = default;
		virtual ConnectionPtr createConnection(const std::string& host, uint16_t port) const = 0;
		virtual ConnectionPtr createConnection(ConnectionBackendPtr&&) const = 0;
		virtual ServerPtr createServer(uint16_t port) const = 0;
		virtual ServerPtr createServer(ServerBackendPtr&&) const = 0;
		virtual PacketPtr createPacket(IPacket::Type) const = 0;
		virtual PacketPtr createPacket(EventPtr) const = 0;
	};

	extern GG_API INetworkManager& net;
};
