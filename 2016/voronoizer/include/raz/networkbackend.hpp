/*
Copyright (C) 2016 - Gábor "Razzie" Görzsöny

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#pragma once

#pragma warning (disable : 4250)
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <cstring>
#include <exception>
#include <string>
#include <vector>

namespace raz
{
	class NetworkConnectionError : public std::exception
	{
	public:
		virtual const char* what() const
		{
			return "Connection error";
		}
	};

	class NetworkSocketError : public std::exception
	{
	public:
		virtual const char* what() const
		{
			return "Socket error";
		}
	};

	class NetworkInitializer
	{
	public:
		NetworkInitializer()
		{
			WSADATA wsaData;
			WSAStartup(MAKEWORD(2, 2), &wsaData);
		}

		~NetworkInitializer()
		{
			WSACleanup();
		}

		NetworkInitializer(const NetworkInitializer&) = delete;
	};


	/*
	 * TCP CLIENT AND SERVER BACKENDS
	 */

	class NetworkClientBackendTCP
	{
	public:
		NetworkClientBackendTCP() : m_socket(INVALID_SOCKET)
		{
		}

		NetworkClientBackendTCP(const char* host, uint16_t port, bool ipv6 = false) : m_socket(INVALID_SOCKET)
		{
			if (!open(host, port, ipv6))
				throw NetworkConnectionError();
		}

		~NetworkClientBackendTCP()
		{
			close();
		}

		bool open(const char* host, uint16_t port, bool ipv6 = false)
		{
			if (m_socket != INVALID_SOCKET)
			{
				close();
			}

			std::string port_str = std::to_string(port);
			struct addrinfo hints, *result = NULL, *ptr = NULL;

			std::memset(&m_sockaddr, 0, sizeof(SOCKADDR_STORAGE));

			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = ipv6 ? AF_INET6 : AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			// resolve the local address and port to be used by the server
			int rc = getaddrinfo(host, port_str.c_str(), &hints, &result);
			if (rc != 0)
			{
				return false;
			}

			m_socket = INVALID_SOCKET;

			// attempt to connect to the first possible address in the list returned by getaddrinfo
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET)
				{
					continue;
				}

				if (connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
				{
					closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					continue;
				}

				// everything is OK if we get here
				break;
			}

			freeaddrinfo(result);

			if (m_socket == INVALID_SOCKET)
			{
				return false;
			}

			return true;
		}

		size_t wait(uint32_t timeous_ms)
		{
			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);

			struct timeval timeout;
			timeout.tv_sec = timeous_ms / 1000;
			timeout.tv_usec = (timeous_ms % 1000) * 1000;

			int rc = select(m_socket + 1, &set, NULL, NULL, &timeout);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else if (rc > 0)
			{
				u_long bytes_available = 0;
				ioctlsocket(m_socket, FIONREAD, &bytes_available);
				return static_cast<size_t>(bytes_available);
			}
			else
			{
				return 0;
			}
		}

		size_t peek(char* ptr, size_t len)
		{
			int rc = recv(m_socket, ptr, len, MSG_PEEK);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		size_t read(char* ptr, size_t len)
		{
			int rc = recv(m_socket, ptr, len, 0);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		size_t write(const char* ptr, size_t len)
		{
			int rc = send(m_socket, ptr, len, 0);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		void close()
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

	private:
		SOCKET m_socket;
		SOCKADDR_STORAGE m_sockaddr;
	};

	class NetworkServerBackendTCP
	{
	public:
		struct Client
		{
			SOCKET socket;
			SOCKADDR_STORAGE sockaddr;
		};

		enum ClientState
		{
			UNSET,
			CLIENT_CONNECTED,
			PACKET_RECEIVED
		};

		NetworkServerBackendTCP() : m_socket(INVALID_SOCKET)
		{
		}

		NetworkServerBackendTCP(uint16_t port, bool ipv6 = false) : m_socket(INVALID_SOCKET)
		{
			if (!open(port, ipv6))
				throw NetworkConnectionError();
		}

		~NetworkServerBackendTCP()
		{
			close();
		}

		bool open(uint16_t port, bool ipv6 = false)
		{
			if (m_socket != INVALID_SOCKET)
			{
				close();
			}

			std::string port_str = std::to_string(port);
			struct addrinfo hints, *result = NULL, *ptr = NULL;

			std::memset(&m_sockaddr, 0, sizeof(SOCKADDR_STORAGE));

			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = ipv6 ? AF_INET6 : AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			// resolve the local address and port to be used by the server
			int rc = getaddrinfo(NULL, port_str.c_str(), &hints, &result);
			if (rc != 0)
			{
				return false;
			}

			m_socket = INVALID_SOCKET;

			// attempt to connect to the first possible address in the list returned by getaddrinfo
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET)
				{
					continue;
				}

				int no = 0;
				setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no));
				setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)ptr->ai_addr, (int)ptr->ai_addrlen);

				if (bind(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
				{
					closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					continue;
				}

				if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
				{
					closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					continue;
				}

				// everything is OK if we get here
				break;
			}

			freeaddrinfo(result);

			if (m_socket == INVALID_SOCKET)
			{
				return false;
			}

			return true;
		}

		size_t wait(Client& client, ClientState& state, uint32_t timeous_ms)
		{
			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);
			for (Client& c : m_clients)
				FD_SET(c.socket, &set);

			struct timeval timeout;
			timeout.tv_sec = timeous_ms / 1000;
			timeout.tv_usec = (timeous_ms % 1000) * 1000;

			int rc = select(getLargestSocket() + 1, &set, NULL, NULL, &timeout);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else if (rc > 0)
			{
				if (FD_ISSET(m_socket, &set))
				{
					int addrlen = sizeof(client.sockaddr);
					client.socket = accept(m_socket, reinterpret_cast<struct sockaddr*>(&client.sockaddr), &addrlen);
					if (client.socket == INVALID_SOCKET)
					{
						throw NetworkSocketError();
					}

					m_clients.push_back(client);

					state = ClientState::CLIENT_CONNECTED;
					return 0;
				}

				for (size_t i = 0; i < m_clients.size(); ++i)
				{
					size_t index = (i + m_client_to_check) % m_clients.size();
					SOCKET sock = m_clients[index].socket;
					if (FD_ISSET(sock, &set))
					{
						client = m_clients[index];
						state = ClientState::PACKET_RECEIVED;

						m_client_to_check = index + 1; // check an other client next time

						u_long bytes_available = 0;
						ioctlsocket(sock, FIONREAD, &bytes_available);
						return static_cast<size_t>(bytes_available);
					}
				}
			}

			state = ClientState::UNSET;
			return 0;
		}

		size_t peek(const Client& client, char* ptr, size_t len)
		{
			int rc = recv(client.socket, ptr, len, MSG_PEEK);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		size_t read(const Client& client, char* ptr, size_t len)
		{
			int rc = recv(client.socket, ptr, len, 0);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		size_t write(const Client& client, const char* ptr, size_t len)
		{
			int rc = send(client.socket, ptr, len, 0);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		void close()
		{
			for (Client& client : m_clients)
				closesocket(client.socket);
			m_clients.clear();

			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

		void close(const Client& client)
		{
			closesocket(client.socket);
			for (auto it = m_clients.begin(), end = m_clients.end(); it != end; ++it)
			{
				if (it->socket == client.socket)
				{
					m_clients.erase(it);
					return;
				}
			}
		}

	private:
		SOCKET m_socket;
		SOCKADDR_STORAGE m_sockaddr;
		std::vector<Client> m_clients;
		size_t m_client_to_check = 0;

		SOCKET getLargestSocket()
		{
			SOCKET s = m_socket;

			for (Client& client : m_clients)
			{
				if (client.socket > s)
					s = client.socket;
			}

			return s;
		}
	};


	/*
	 * UDP CLIENT AND SERVER BACKENDS
	 */

	template<size_t BUF_SIZE = 2048>
	class NetworkClientBackendUDP
	{
	public:
		NetworkClientBackendUDP() :
			m_socket(INVALID_SOCKET),
			m_data_len(0),
			m_data_pos(0)
		{
		}

		NetworkClientBackendUDP(const char* host, uint16_t port, bool ipv6 = false) :
			m_socket(INVALID_SOCKET),
			m_data_len(0),
			m_data_pos(0)
		{
			if (!open(host, port, ipv6))
				throw NetworkConnectionError();
		}

		~NetworkClientBackendUDP()
		{
			close();
		}

		bool open(const char* host, uint16_t port, bool ipv6 = false)
		{
			if (m_socket != INVALID_SOCKET)
			{
				close();
			}

			std::string port_str = std::to_string(port);
			struct addrinfo hints, *result = NULL, *ptr = NULL;

			std::memset(&m_sockaddr, 0, sizeof(SOCKADDR_STORAGE));

			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = ipv6 ? AF_INET6 : AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_protocol = IPPROTO_UDP;
			hints.ai_flags = AI_PASSIVE;

			// resolve the local address and port to be used by the server
			int rc = getaddrinfo(host, port_str.c_str(), &hints, &result);
			if (rc != 0)
			{
				return false;
			}

			m_socket = INVALID_SOCKET;

			// attempt to connect to the first possible address in the list returned by getaddrinfo
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET)
				{
					continue;
				}

				std::memcpy(&m_sockaddr, ptr->ai_addr, ptr->ai_addrlen);

				// everything is OK if we get here
				break;
			}

			freeaddrinfo(result);

			if (m_socket == INVALID_SOCKET)
			{
				return false;
			}

			return true;
		}

		size_t wait(uint32_t timeous_ms)
		{
			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);

			struct timeval timeout;
			timeout.tv_sec = timeous_ms / 1000;
			timeout.tv_usec = (timeous_ms % 1000) * 1000;

			int rc = select(m_socket + 1, &set, NULL, NULL, &timeout);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else if (rc > 0)
			{
				m_data_len = recv(m_socket, m_data, BUF_SIZE, MSG_PEEK);
				m_data_pos = 0;
				return m_data_len;
			}
			else
			{
				return 0;
			}
		}

		size_t peek(char* ptr, size_t len)
		{
			if (m_data_len - m_data_pos < len)
			{
				len = m_data_len - m_data_pos;
			}

			std::memcpy(ptr, &m_data[m_data_pos], len);

			return len;
		}

		size_t read(char* ptr, size_t len)
		{
			if (m_data_len - m_data_pos < len)
			{
				len = m_data_len - m_data_pos;
			}

			std::memcpy(ptr, &m_data[m_data_pos], len);
			m_data_pos += len;

			return len;
		}

		size_t write(const char* ptr, size_t len)
		{
			int rc = sendto(m_socket, ptr, len, 0, reinterpret_cast<const struct sockaddr*>(&m_sockaddr), sizeof(SOCKADDR_STORAGE));
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		void close()
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

	private:
		SOCKET m_socket;
		SOCKADDR_STORAGE m_sockaddr;
		size_t m_data_len;
		size_t m_data_pos;
		char m_data[BUF_SIZE];
	};

	template<size_t BUF_SIZE = 2048>
	class NetworkServerBackendUDP
	{
	public:
		struct Client
		{
			SOCKADDR_STORAGE sockaddr;

			bool operator!=(const Client& other) const
			{
				return (std::memcmp(&sockaddr, &other.sockaddr, sizeof(SOCKADDR_STORAGE)) != 0);
			}
		};

		enum ClientState
		{
			UNSET,
			PACKET_RECEIVED
		};

		NetworkServerBackendUDP() :
			m_socket(INVALID_SOCKET),
			m_data_len(0),
			m_data_pos(0)
		{
		}

		NetworkServerBackendUDP(uint16_t port, bool ipv6 = false) :
			m_socket(INVALID_SOCKET),
			m_data_len(0),
			m_data_pos(0)
		{
			if (!open(port, ipv6))
				throw NetworkConnectionError();
		}

		~NetworkServerBackendUDP()
		{
			close();
		}

		bool open(uint16_t port, bool ipv6 = false)
		{
			if (m_socket != INVALID_SOCKET)
			{
				close();
			}

			std::string port_str = std::to_string(port);
			struct addrinfo hints, *result = NULL, *ptr = NULL;

			std::memset(&m_sockaddr, 0, sizeof(SOCKADDR_STORAGE));

			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = ipv6 ? AF_INET6 : AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_protocol = IPPROTO_UDP;
			hints.ai_flags = AI_PASSIVE;

			// resolve the local address and port to be used by the server
			int rc = getaddrinfo(NULL, port_str.c_str(), &hints, &result);
			if (rc != 0)
			{
				return false;
			}

			m_socket = INVALID_SOCKET;

			// attempt to connect to the first possible address in the list returned by getaddrinfo
			for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
			{
				m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET)
				{
					continue;
				}

				int no = 0;
				setsockopt(m_socket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no));
				setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)ptr->ai_addr, (int)ptr->ai_addrlen);

				if (bind(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
				{
					closesocket(m_socket);
					m_socket = INVALID_SOCKET;
					continue;
				}

				// everything is OK if we get here
				break;
			}

			freeaddrinfo(result);

			if (m_socket == INVALID_SOCKET)
			{
				return false;
			}

			return true;
		}

		size_t wait(Client& client, ClientState& state, uint32_t timeous_ms)
		{
			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);

			struct timeval timeout;
			timeout.tv_sec = timeous_ms / 1000;
			timeout.tv_usec = (timeous_ms % 1000) * 1000;

			int rc = select(m_socket + 1, &set, NULL, NULL, &timeout);
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else if (rc > 0)
			{
				int addrlen = sizeof(client.sockaddr);
				m_data_len = recvfrom(m_socket, m_data, BUF_SIZE, 0, reinterpret_cast<struct sockaddr*>(&client.sockaddr), &addrlen);
				m_data_pos = 0;
				m_last_client = client;

				state = ClientState::PACKET_RECEIVED;
				return m_data_len;
			}

			state = ClientState::UNSET;
			return 0;
		}

		size_t peek(const Client& client, char* ptr, size_t len)
		{
			if (client != m_last_client)
			{
				throw NetworkSocketError();
			}

			if (m_data_len - m_data_pos < len)
			{
				len = m_data_len - m_data_pos;
			}

			std::memcpy(ptr, &m_data[m_data_pos], len);

			return len;
		}

		size_t read(const Client& client, char* ptr, size_t len)
		{
			if (client != m_last_client)
			{
				throw NetworkSocketError();
			}

			if (m_data_len - m_data_pos < len)
			{
				len = m_data_len - m_data_pos;
			}

			std::memcpy(ptr, &m_data[m_data_pos], len);
			m_data_pos += len;

			return len;
		}

		size_t write(const Client& client, const char* ptr, size_t len)
		{
			int rc = sendto(m_socket, ptr, len, 0, reinterpret_cast<const struct sockaddr*>(&client.sockaddr), sizeof(client.sockaddr));
			if (rc == SOCKET_ERROR)
			{
				throw NetworkSocketError();
			}
			else
			{
				return rc;
			}
		}

		void close()
		{
			closesocket(m_socket);
			m_socket = INVALID_SOCKET;
		}

	private:
		SOCKET m_socket;
		SOCKADDR_STORAGE m_sockaddr;
		Client m_last_client;
		size_t m_data_len;
		size_t m_data_pos;
		char m_data[BUF_SIZE];
	};
}
