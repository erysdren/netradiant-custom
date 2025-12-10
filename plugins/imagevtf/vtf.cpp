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

#include "vtf.h"

#include "ifilesystem.h"

#include "os/path.h"
#include "stream/stringstream.h"
#include "bytestreamutils.h"
#include "imagelib.h"

#include <vtfpp/vtfpp.h>

Image* ProcessVtf( vtfpp::VTF& vtf ) {
	if (!vtf.hasImageData())
		return NULL;

	auto image = new RGBAImageFlags( vtf.getWidth(), vtf.getHeight(), 0, 0, 0 );

	auto vtfData = vtf.getImageDataAsRGBA8888();

	size_t index = 0;
	byte *dest = image->getRGBAPixels();
	for (int y = 0; y < vtf.getHeight(); y++)
	{
		for (int x = 0; x < vtf.getWidth(); x++)
		{
			dest[index + 0] = (byte)vtfData[index + 0];
			dest[index + 1] = (byte)vtfData[index + 1];
			dest[index + 2] = (byte)vtfData[index + 2];
			dest[index + 3] = (byte)vtfData[index + 3];

			index += 4;
		}
	}

	return image;
}

Image* LoadVtf( ArchiveFile& file ){
	ScopedArchiveBuffer buffer( file );
	auto vtf = vtfpp::VTF({reinterpret_cast<const std::byte *>(buffer.buffer), buffer.length});
	return ProcessVtf(vtf);
}

#if !defined(MAX_PATH) && defined(PATH_MAX)
#define MAX_PATH PATH_MAX
#else
#define MAX_PATH 260
#endif

Image* LoadTth( ArchiveFile& file ){
	const char *tthFilename = file.getName();
	const char *tthFilenameExtPtr = std::strrchr( tthFilename, '.' );
	size_t tthFilenameExtPos = tthFilenameExtPtr - tthFilename;

	char ttzFilename[MAX_PATH];
	std::strncpy( ttzFilename, tthFilename, sizeof(ttzFilename) );
	std::strncpy( ttzFilename + tthFilenameExtPos, ".ttz", sizeof(ttzFilename) - tthFilenameExtPos );

	ArchiveFile *ttzFile = GlobalFileSystem().openFile( ttzFilename );
	if ( ttzFile ) {
		ScopedArchiveBuffer tthBuffer( file );
		ScopedArchiveBuffer ttzBuffer( *ttzFile );
		auto ttx = vtfpp::TTX({reinterpret_cast<const std::byte *>(tthBuffer.buffer), tthBuffer.length}, {reinterpret_cast<const std::byte *>(ttzBuffer.buffer), ttzBuffer.length});
		ttzFile->release();
		return ProcessVtf(ttx.getVTF());
	} else {
		ScopedArchiveBuffer tthBuffer( file );
		auto ttx = vtfpp::TTX({reinterpret_cast<const std::byte *>(tthBuffer.buffer), tthBuffer.length});
		return ProcessVtf(ttx.getVTF());
	}
}
