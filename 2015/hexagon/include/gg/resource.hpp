/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

/**
 * Resources are archives that act like a directory.
 *
 *
 * HOW TO USE:
 * -----------
 *
 * In this example we have a 'textures.res' named resource in 'media' folder.
 * It contains 3 files: 'ground.png', 'water.png' and 'sky.png'.
 *
 * 1st step:
 * Create a resource pool 'auto pool = gg::res.createResourcePool()' or use the default
 * 'gg::res.getDefaultResourcePool()'.
 *
 * 1st step:
 * Include the resource by calling 'pool.addResource("media/textures.res")'.
 *
 * 2nd step:
 * Let's assume the program needs to open 'sky.png', which is a part of 'textures.res'
 * resource. It can be done by calling: 'auto file = pool.openFile("textures.res/sky.png")'
 *
 * Important:
 * - Do NOT use backslash '\' characters in a file path. Always use slash '/' instead.
 * - Memory used by the file content gets released if no instances of the same file exist
 *   or they all released the file content (by calling 'file->unload()').
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "gg/serializable.hpp"

#if defined GGRESOURCE_BUILD
#	define GG_API __declspec(dllexport)
#else
#	define GG_API __declspec(dllimport)
#endif

namespace gg
{
	class IResourcePool;
	class IResourceCreator;
	class IFileSerializer;
	class IDirectory;
	class IFile;

	typedef std::shared_ptr<IResourcePool> ResourcePoolPtr;
	typedef std::shared_ptr<IResourceCreator> ResourceCreatorPtr;
	typedef std::shared_ptr<IFileSerializer> FileSerializerPtr;
	typedef std::shared_ptr<IDirectory> DirectoryPtr;
	typedef std::shared_ptr<IFile> FilePtr;

	class IResourceManager
	{
	public:
		virtual ~IResourceManager() = default;
		virtual ResourcePoolPtr createResourcePool() const = 0;
		virtual ResourcePoolPtr getDefaultResourcePool() = 0;
		virtual FileSerializerPtr getFileSerializer() = 0;
		virtual ResourceCreatorPtr createResource(const std::string& res_path, bool append_mode = false) const = 0;
	};

	extern GG_API IResourceManager& res;

	class IResourcePool
	{
	public:
		virtual ~IResourcePool() = default;
		virtual bool includeResource(const std::string& res_path) = 0;
		virtual void releaseResources() = 0;
		virtual DirectoryPtr openDirectory(const std::string& dir_name) const = 0;
		virtual FilePtr openFile(const std::string& file_name) const = 0;
	};

	class IResourceCreator
	{
	public:
		virtual ~IResourceCreator() = default;
		virtual bool addFile(const std::string& file_path, const std::string& res_file_name) = 0;
		virtual bool addFile(const std::wstring& file_path, const std::string& res_file_name) = 0;
		virtual FilePtr addFile(const std::string& res_file_name) = 0; // writeable, saved when file is destructed
		virtual bool addDirectory(const std::string& dir_path) = 0;
		virtual bool addDirectory(const std::wstring& dir_path) = 0;
	};

	class IFileSerializer
	{
	public:
		enum OpenMode
		{
			READ,
			APPEND,
			REWRITE
		};

		virtual ~IFileSerializer() = default;
		virtual FilePtr openFile(const std::string& file_name, OpenMode) const = 0;
		virtual FilePtr openFile(const std::wstring& file_name, OpenMode) const = 0;
	};

	class IDirectory
	{
	public:
		struct FileOrDirectory
		{
			enum Type
			{
				FILE,
				DIRECTORY
			};

			std::string name;
			uint32_t size;
			Type type;
		};

		typedef std::vector<FileOrDirectory>::const_iterator Iterator;

		virtual ~IDirectory() = default;
		virtual const std::string& getName() const = 0;
		virtual Iterator begin() const = 0;
		virtual Iterator end() const = 0;
	};

	class IFile : public virtual IArchive
	{
	public:
		virtual ~IFile() = default;
		virtual const std::string& getName() const = 0;
		virtual const char* getData() const = 0;
		virtual size_t getSize() const = 0;
		virtual void unload() = 0; // releases memory used by the file content/data
	};

	template<class T>
	IFile& operator& (FilePtr f, T& t)
	{
		IFile& file = *f;
		file & t;
		return file;
	}

	class FileStream : public std::iostream, public std::streambuf
	{
	public:
		// reading is always supported, writing depends on file->getMode()
		FileStream(FilePtr file) :
			std::iostream(this), m_file(file), m_pos(0)
		{
		}

		// read only
		FileStream(const std::string& file_name, ResourcePoolPtr res_pool = res.getDefaultResourcePool()) :
			FileStream(res_pool->openFile(file_name))
		{
		}

		FilePtr getFile() const
		{
			return m_file;
		}

		operator bool() const
		{
			return static_cast<bool>(m_file);
		}

		void reset()
		{
			m_pos = 0;
		}

	protected:
		// the following are inherited from std::streambuf

		virtual int underflow() // get character without advancing position
		{
			if (m_pos >= m_file->getSize())
				return std::char_traits<char>::eof();
			else
				return static_cast<int>(m_file->getData()[m_pos]);
		}

		virtual int uflow() // get character and advance position
		{
			int c = underflow();
			++m_pos;
			return c;
		}

		virtual int overflow(int c = std::char_traits<char>::eof()) // append character
		{
			char _c = c;
			m_file->write(&_c, 1);
			return c;
		}

	private:
		FilePtr m_file;
		size_t m_pos;
	};
};
