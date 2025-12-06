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

#include "iscriplib.h"
#include "ibrush.h"
#include "ipatch.h"
#include "ifiletypes.h"
#include "ieclass.h"
#include "qerplugin.h"
#include "eclasslib.h"
#include "ientity.h"
#include "layers.h"

#include "scenelib.h"
#include "string/string.h"
#include "stringio.h"
#include "generic/constant.h"

#include "modulesystem/singletonmodule.h"

#include "imap.h"

#include <kvpp/kvpp.h>

#include <format>

NodeSmartReference g_nullNode( NewNullNode() );

class MapDependencies :
	public GlobalRadiantModuleRef,
	public GlobalBrushModuleRef,
	public GlobalPatchModuleRef,
	public GlobalFiletypesModuleRef,
	public GlobalScripLibModuleRef,
	public GlobalEntityClassManagerModuleRef,
	public GlobalSceneGraphModuleRef
{
public:
	MapDependencies() :
		GlobalBrushModuleRef( GlobalRadiant().getRequiredGameDescriptionKeyValue( "brushtypes" ) ),
		GlobalPatchModuleRef( GlobalRadiant().getRequiredGameDescriptionKeyValue( "patchtypes" ) ),
		GlobalEntityClassManagerModuleRef( GlobalRadiant().getRequiredGameDescriptionKeyValue( "entityclass" ) ){
	}
};

class MapVMFAPI final : public TypeSystemRef, public MapFormat
{
public:
	typedef MapFormat Type;
	STRING_CONSTANT( Name, "mapvmf" );

	MapVMFAPI(){
		GlobalFiletypesModule::getTable().addType( Type::Name, Name, filetype_t( "vmf maps", "*.vmf" ) );
	}
	MapFormat* getTable(){
		return this;
	}

	void readGraph( scene::Node& root, TextInputStream& inputStream, EntityCreator& entityTable ) const override {
		char buffer[2048];
		size_t len = 0;
		std::string kv1Data = "";

		GlobalBrushCreator().toggleFormat(eBrushTypeValve220);

		while ( ( len = inputStream.read(buffer, sizeof(buffer)) ) > 0 ) {
			kv1Data.append(buffer, len);
		}

		kvpp::KV1 kv1(kv1Data);

		for ( auto elem : kv1 ) {
			auto key = elem.getKey();
			if (string_equal_nocase(key.begin(), "versioninfo")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.begin(), "cameras")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.begin(), "cordon")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.begin(), "world") || string_equal_nocase(key.begin(), "entity")) {
				bool hasSolids = false;
				EntityClass* entityClass;
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().begin(), "solid")) {
						hasSolids = true;
						break;
					}
				}
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().begin(), "classname")) {
						entityClass = GlobalEntityClassManager().findOrInsert( e.getValue().begin(), hasSolids );
					}
				}
				scene::Node& entity( entityTable.createEntity( entityClass ) );
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().begin(), "id")) {
						// FIXME: do we need to keep track of this?
					} else if (string_equal_nocase(e.getKey().begin(), "solid")) {
						scene::Node& solid( GlobalBrushCreator().createBrush() );
						for ( auto solidelem : e ) {
							if (string_equal_nocase(solidelem.getKey().begin(), "side")) {
								auto material = solidelem["material"];
								auto plane = solidelem["plane"];
								auto uaxis = solidelem["uaxis"];
								auto vaxis = solidelem["vaxis"];
								auto rotation = solidelem["rotation"];

								std::string shader = std::format("materials/{}", material.getValue());

								_QERFaceData faceData;
								faceData.m_shader = shader.c_str();
								faceData.m_texdef.rotate = std::stof(rotation.getValue().begin());

								float dummy;
								sscanf(plane.getValue().begin(), "(%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf)", &faceData.m_p0[0], &faceData.m_p0[1], &faceData.m_p0[2], &faceData.m_p1[0], &faceData.m_p1[1], &faceData.m_p1[2], &faceData.m_p2[0], &faceData.m_p2[1], &faceData.m_p2[2]);
								sscanf(uaxis.getValue().begin(), "[%f %f %f %f] %f", &dummy, &dummy, &dummy, &dummy, &faceData.m_texdef.scale[0]);
								sscanf(vaxis.getValue().begin(), "[%f %f %f %f] %f", &dummy, &dummy, &dummy, &dummy, &faceData.m_texdef.scale[1]);

								GlobalBrushCreator().Brush_addFace(solid, faceData);
							}
						}
						NodeSmartReference solidnode( solid );
						Node_getTraversable( entity )->insert( solidnode );
					} else if (string_equal_nocase(e.getKey().begin(), "editor")) {
						// FIXME: do we care about any of this?
					} else {
						Node_getEntity( entity )->setKeyValue( e.getKey().begin(), e.getValue().begin() );
					}
				}
				NodeSmartReference node( entity );
				Node_getTraversable( root )->insert( node );
			}
		}
	}
	class MapVMFWriteKeyValue : public Entity::Visitor
	{
		int64_t& m_childID;
		kvpp::KV1ElementWritable<std::string>& m_element;
	public:
		MapVMFWriteKeyValue( kvpp::KV1ElementWritable<std::string>& element, int64_t& childID ) : m_childID( childID ), m_element( element ) {
			m_element["id"] = m_childID++;
		}
		void visit( const char* key, const char* value ) override {
			m_element[key] = value;
		}
	};
	class MapVMFWalker : public scene::Traversable::Walker
	{
		mutable int64_t m_childID; // global element index, seems to be shared between entities and solids
		mutable int64_t m_childIndex;
		int64_t m_worldIndex;
		kvpp::KV1Writer<std::string>& m_writer;
	public:
		MapVMFWalker( kvpp::KV1Writer<std::string>& writer, int64_t worldIndex ) : m_childID( 1 ), m_childIndex( worldIndex ), m_worldIndex( worldIndex ), m_writer( writer ) {
		}
		struct MapVMFWriteFaceEnv {
			int64_t& childID;
			kvpp::KV1ElementWritable<std::string>& solid;
			MapVMFWriteFaceEnv(int64_t& childID, kvpp::KV1ElementWritable<std::string>& solid) : childID( childID ), solid( solid ) {
			}
		};
		static void writeSide( int64_t& childID, kvpp::KV1ElementWritable<std::string>& side, const _QERFaceData& faceData ) {
			side["id"] = childID++;
			side["plane"] = std::format("({} {} {}) ({} {} {}) ({} {} {})", faceData.m_p0[0], faceData.m_p0[1], faceData.m_p0[2], faceData.m_p1[0], faceData.m_p1[1], faceData.m_p1[2], faceData.m_p2[0], faceData.m_p2[1], faceData.m_p2[2]);
			std::string material(faceData.m_shader);
			if ( material.rfind( "materials/", 0 ) == 0 ) {
				side["material"] = faceData.m_shader + string_length("materials/");
			} else {
				side["material"] = faceData.m_shader;
			}
			side["uaxis"] = std::format("[{} {} {} {}] {}", 0, 0, 0, 0, faceData.m_texdef.scale[0]); // FIXME: convert from texdef_t
			side["vaxis"] = std::format("[{} {} {} {}] {}", 0, 0, 0, 0, faceData.m_texdef.scale[1]); // FIXME: convert from texdef_t
			side["rotation"] = faceData.m_texdef.rotate;
			side["lightmapscale"] = 16; // FIXME: make configurable
			side["smoothing_groups"] = 0; // FIXME: make configurable
		}
		static void writeFace( void *environment, const _QERFaceData& faceData ) {
			MapVMFWriteFaceEnv *env = (MapVMFWriteFaceEnv *)environment;
			writeSide(env->childID, env->solid.addChild("side"), faceData);
		}
		void writeSolid( scene::Node& node, kvpp::KV1ElementWritable<std::string>& solid ) const {
			solid["id"] = m_childID++;
			MapVMFWriteFaceEnv env(m_childID, solid);
			GlobalBrushCreator().Brush_forEachFace( node, BrushFaceDataCallback(&env, writeFace) );
		}
		virtual bool pre( scene::Node& node ) const {
			Entity* entity = Node_getEntity( node );
			if ( entity ) {
				if ( string_equal( entity->getClassName(), "worldspawn" ) ) {
					MapVMFWriteKeyValue visitor( m_writer["world"], m_childID );
					entity->forEachKeyValue( visitor );
					m_childIndex = m_worldIndex;
				} else {
					MapVMFWriteKeyValue visitor( m_writer.addChild("entity"), m_childID );
					entity->forEachKeyValue( visitor );
					m_childIndex = m_writer.getChildCount() - 1;
				}
			} else if ( Node_isBrush( node ) ) {
				writeSolid( node, m_writer[m_childIndex].addChild("solid") );
			}

			return true;
		}
	};
	void writeGraph( scene::Node& root, GraphTraversalFunc traverse, TextOutputStream& outputStream ) const override {
		kvpp::KV1Writer writer;

		// make up some shit
		writer["versioninfo"]["editorextension"] = "sourceradiant"; // strata checks this
		writer["versioninfo"]["editorversion"] = RADIANT_VERSION; // probably should be a single number
		writer["versioninfo"]["editorbuild"] = 0; // not tracked in radiant
		writer["versioninfo"]["mapversion"] = 0; // not tracked in radiant
		writer["versioninfo"]["formatversion"] = 100; // not sure what this corresponds to
		writer["versioninfo"]["prefab"] = 0; // FIXME: make configurable

		// traverse tree
		traverse(root, MapVMFWalker( writer, writer.getChildCount() ));

		// extra garbage
		writer["cameras"]["activecamera"] = -1; // shrug
		writer["cordons"]["active"] = 0;

		// bake and write
		auto baked = writer.bake();
		outputStream.write(baked.c_str(), baked.length());
	}
};

typedef SingletonModule<MapVMFAPI, MapDependencies> MapVMFModule;

MapVMFModule g_MapVMFModule;


extern "C" void RADIANT_DLLEXPORT Radiant_RegisterModules( ModuleServer& server ){
	initialiseModule( server );

	g_MapVMFModule.selfRegister();
}
