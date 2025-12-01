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

class VpkArchive final : public Archive
{
	std::unique_ptr<vpkpp::VPK> m_vpk;

public:

	VpkArchive( const char* name ) {
		m_vpk = vpkpp::VPK::open(name);
	}

	~VpkArchive() {
		delete m_vpk;
	}

	void release() override {
		delete this;
	}

	ArchiveFile* openFile( const char* name ) override {
		if ( m_vpk ) {
			auto entry = m_vpk->findEntry( name );
			if ( entry ) {
				return StoredArchiveFile::create( name, m_name.c_str(), entry.offset, entry.length, entry.length );
			}
		}
		return 0;
	}
	virtual ArchiveTextFile* openTextFile( const char* name ) override {
		if ( m_vpk ) {
			auto entry = m_vpk->findEntry( name );
			if ( entry ) {
				return StoredArchiveTextFile::create( name, m_name.c_str(), entry.offset, entry.length );
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
		m_filesystem.traverse( visitor, root );
	}
};


Archive* OpenArchive( const char* name ){
	return new VpkArchive( name );
}
