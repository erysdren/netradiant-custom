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

#include "plugin.h"

#include "iscenegraph.h"
#include "irender.h"
#include "iselection.h"
#include "iimage.h"
#include "imodel.h"
#include "igl.h"
#include "ifilesystem.h"
#include "iundo.h"
#include "ifiletypes.h"
#include "iscriplib.h"

#include "modulesystem/singletonmodule.h"
#include "typesystem.h"

#include "mdl.h"

class SourceModelLoader : public ModelLoader
{
public:
	scene::Node& loadModel( ArchiveFile& file ) override {
		return loadSourceModel( file );
	}
};

class ModelDependencies :
	public GlobalFileSystemModuleRef,
	public GlobalOpenGLModuleRef,
	public GlobalUndoModuleRef,
	public GlobalSceneGraphModuleRef,
	public GlobalShaderCacheModuleRef,
	public GlobalSelectionModuleRef,
	public GlobalFiletypesModuleRef
{
};

class ModelSourceAPI : public TypeSystemRef
{
	SourceModelLoader m_modelsource;
public:
	typedef ModelLoader Type;
	STRING_CONSTANT( Name, "mdl" );

	ModelSourceAPI(){
		GlobalFiletypesModule::getTable().addType( Type::Name, Name, filetype_t( "source models", "*.mdl" ) );
	}
	ModelLoader* getTable(){
		return &m_modelsource;
	}
};

typedef SingletonModule<ModelSourceAPI, ModelDependencies> ModelSourceModule;

ModelSourceModule g_ModelSourceModule;

extern "C" void RADIANT_DLLEXPORT Radiant_RegisterModules( ModuleServer& server ){
	initialiseModule( server );

	g_ModelSourceModule.selfRegister();
}
