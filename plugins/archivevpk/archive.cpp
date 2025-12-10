/*
   Copyright (C) 2001-2006, William Joseph.
   All Rights Reserved.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "archive.h"

#include "idatastream.h"
#include "commandlib.h"
#include "bytestreamutils.h"
#include <algorithm>
#include "stream/filestream.h"

#include "iarchive.h"

#include "archivelib.h"

#include "plugin.h"

#include <map>
#include "string/string.h"
#include "fs_filesystem.h"

#include <vpkpp/vpkpp.h>

class VpkFileInputStream : public InputStream
{
	std::vector<std::byte> m_data;
	InputStream::size_type m_pos;
public:
	typedef InputStream::size_type size_type;
	typedef InputStream::byte_type byte_type;

	VpkFileInputStream( std::vector<std::byte>&& data ) : m_data( data ), m_pos( 0 ) { }

	virtual size_type read( byte_type* buffer, size_type length ) {
		for (size_type i = 0; i < length; i++) {
			if (m_pos >= m_data.size()) {
				return i;
			}
			buffer[i] = (byte_type)m_data[m_pos++];
		}
		return length;
	}
};

class VpkArchiveFile final : public ArchiveFile
{
	CopiedString m_name;
	FileInputStream::size_type m_size;
	VpkFileInputStream m_istream;
public:
	typedef FileInputStream::size_type size_type;
	typedef FileInputStream::position_type position_type;

	VpkArchiveFile( const char* name, vpkpp::PackFile* vpk, vpkpp::Entry& entry ) : m_name( name ), m_size( entry.length ), m_istream( std::move(vpk->readEntry(name).value()) ) {

	}

	void release() override {
		delete this;
	}
	size_type size() const override {
		return m_size;
	}
	const char* getName() const override {
		return m_name.c_str();
	}
	InputStream& getInputStream() override {
		return m_istream;
	}
};

class VpkTextInputStream : public TextInputStream
{
	std::vector<std::byte> m_data;
	std::size_t m_pos;
public:
	VpkTextInputStream( std::vector<std::byte>&& data ) : m_data( data ), m_pos( 0 ) { }

	virtual std::size_t read( char* buffer, std::size_t length ) {
		for (std::size_t i = 0; i < length; i++) {
			if (m_pos >= m_data.size()) {
				return i;
			}
			buffer[i] = (char)m_data[m_pos++];
		}
		return length;
	}
};

class VpkArchiveTextFile final : public ArchiveTextFile
{
	CopiedString m_name;
	FileInputStream::size_type m_size;
	VpkTextInputStream m_istream;
public:
	typedef FileInputStream::size_type size_type;
	typedef FileInputStream::position_type position_type;

	VpkArchiveTextFile( const char* name, vpkpp::PackFile* vpk, vpkpp::Entry& entry ) : m_name( name ), m_size( entry.length ), m_istream( std::move(vpk->readEntry(name).value()) ) {

	}

	void release() override {
		delete this;
	}
	TextInputStream& getInputStream() override {
		return m_istream;
	}
};

class VpkArchive final : public Archive
{
	std::unique_ptr<vpkpp::PackFile> m_vpk;
	CopiedString m_name;

public:

	VpkArchive( const char* name ) : m_name( name ) {
		if ( string_equal_suffix_nocase( name, "_dir.vpk" ) ) {
			m_vpk = vpkpp::VPK::open(name);
		} else if ( string_equal_suffix_nocase( name, ".vpk" ) ) {
			const char *filename = name + string_length(name) - 11; // string_length("pack000.vpk");
			if ( string_equal_nocase_n( filename, "pack", 4 /* string_length("pack") */ )
				&& std::isdigit( filename[4] )
				&& std::isdigit( filename[5] )
				&& std::isdigit( filename[6] )
			) {
				m_vpk = vpkpp::VPK_VTMB::open(name);
			}
		} else if ( string_equal_suffix_nocase( name, ".gma" ) ) {
			m_vpk = vpkpp::GMA::open(name);
		} else if ( string_equal_suffix_nocase( name, ".gcf" ) ) {
			m_vpk = vpkpp::GCF::open(name);
		}
	}

	~VpkArchive() { }

	void release() override {
		delete this;
	}

	ArchiveFile* openFile( const char* name ) override {
		if ( m_vpk ) {
			auto entry = m_vpk->findEntry( name );
			if ( entry ) {
				return new VpkArchiveFile( name, m_vpk.get(), entry.value() );
			}
		}
		return 0;
	}
	virtual ArchiveTextFile* openTextFile( const char* name ) override {
		if ( m_vpk ) {
			auto entry = m_vpk->findEntry( name );
			if ( entry ) {
				return new VpkArchiveTextFile( name, m_vpk.get(), entry.value() );
			}
		}
		return 0;
	}
	bool containsFile( const char* name ) override {
		if ( m_vpk ) {
			auto entry = m_vpk->findEntry( name );
			if ( entry ) {
				return true;
			}
		}
		return false;
	}
	void forEachFile( VisitorFunc visitor, const char* root ) override {
		if ( m_vpk ) {
			m_vpk->runForAllEntries(root, [&visitor](const std::string& path, const vpkpp::Entry& entry) {
				visitor.file( path.c_str() );
			});
		}
	}
};


Archive* OpenArchive( const char* name ){
	return new VpkArchive( name );
}
