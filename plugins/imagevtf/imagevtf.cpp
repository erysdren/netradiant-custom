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

#include "imagevtf.h"

#include "debugging/debugging.h"
#include "ifilesystem.h"
#include "iimage.h"

#include "vtf.h"

#include "modulesystem/singletonmodule.h"

class ImageDependencies : public GlobalFileSystemModuleRef
{
};

class ImageVtfAPI
{
	_QERPlugImageTable m_imagevtf;
public:
	typedef _QERPlugImageTable Type;
	STRING_CONSTANT( Name, "vtf" );

	ImageVtfAPI(){
		m_imagevtf.loadImage = LoadVtf;
	}
	_QERPlugImageTable* getTable(){
		return &m_imagevtf;
	}
};

typedef SingletonModule<ImageVtfAPI, ImageDependencies> ImageVtfModule;

ImageVtfModule g_ImageVtfModule;

class ImageTthAPI
{
	_QERPlugImageTable m_imagetth;
public:
	typedef _QERPlugImageTable Type;
	STRING_CONSTANT( Name, "tth" );

	ImageTthAPI(){
		m_imagetth.loadImage = LoadTth;
	}
	_QERPlugImageTable* getTable(){
		return &m_imagetth;
	}
};

typedef SingletonModule<ImageTthAPI, ImageDependencies> ImageTthModule;

ImageTthModule g_ImageTthModule;

////////////////////////////////////////////////////////

extern "C" void RADIANT_DLLEXPORT Radiant_RegisterModules( ModuleServer& server ){
	initialiseModule( server );

	g_ImageVtfModule.selfRegister();
	g_ImageTthModule.selfRegister();
}
