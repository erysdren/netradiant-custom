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
#include "script/scripttokeniser.h"

#include "scenelib.h"
#include "string/string.h"
#include "stringio.h"
#include "generic/constant.h"

#include "modulesystem/singletonmodule.h"

#include "imap.h"

#include <kvpp/kvpp.h>

#include <format>

inline MapImporter* Node_getMapImporter( scene::Node& node ){
	return NodeTypeCast<MapImporter>::cast( node );
}

inline MapExporter* Node_getMapExporter( scene::Node& node ){
	return NodeTypeCast<MapExporter>::cast( node );
}

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
		GlobalFiletypesModule::getTable().addType( Type::Name, Name, filetype_t( "vmf region", "*.reg" ) );
	}
	MapFormat* getTable(){
		return this;
	}
	class MapVMFTextInputStream : public TextInputStream
	{
		std::string m_string;
		std::size_t m_pos;
	public:
		MapVMFTextInputStream( std::string& string ) : m_string( string ), m_pos( 0 ) {
		}
		virtual std::size_t read( char* buffer, std::size_t length ) {
			for ( std::size_t i = 0; i < length; i++ ) {
				if ( m_pos >= m_string.length() ) {
					return i;
				}
				buffer[i] = m_string[m_pos++];
			}
			return length;
		}
	};
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
			if (string_equal_nocase(key.data(), "versioninfo")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.data(), "cameras")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.data(), "cordon")) {
				// FIXME: do we care about any of this?
			} else if (string_equal_nocase(key.data(), "world") || string_equal_nocase(key.data(), "entity")) {
				bool hasSolids = false;
				EntityClass* entityClass;
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().data(), "solid")) {
						hasSolids = true;
						break;
					}
				}
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().data(), "classname")) {
						entityClass = GlobalEntityClassManager().findOrInsert( e.getValue().data(), hasSolids );
					}
				}
				scene::Node& entity( entityTable.createEntity( entityClass ) );
				for ( auto e : elem ) {
					if (string_equal_nocase(e.getKey().data(), "id")) {
						// FIXME: do we need to keep track of this?
					} else if (string_equal_nocase(e.getKey().data(), "solid")) {
						scene::Node& solid( GlobalBrushCreator().createBrush() );
						for ( auto solidelem : e ) {
							if (string_equal_nocase(solidelem.getKey().data(), "side")) {

								float uvaxis[2][4];
								float scale[2];
								auto uaxis = solidelem["uaxis"];
								auto vaxis = solidelem["vaxis"];
								sscanf(uaxis.getValue().data(), "[%f %f %f %f] %f", &uvaxis[0][0], &uvaxis[0][1], &uvaxis[0][2], &uvaxis[0][3], &scale[0]);
								sscanf(vaxis.getValue().data(), "[%f %f %f %f] %f", &uvaxis[1][0], &uvaxis[1][1], &uvaxis[1][2], &uvaxis[1][3], &scale[1]);

								std::string faceData = std::format("{} {} [{} {} {} {}] [{} {} {} {}] {} {} {}\n}}\n",
									solidelem["plane"].getValue(), solidelem["material"].getValue(),
									uvaxis[0][0], uvaxis[0][1], uvaxis[0][2], uvaxis[0][3],
									uvaxis[1][0], uvaxis[1][1], uvaxis[1][2], uvaxis[1][3],
									solidelem["rotation"].getValue(),
									scale[0], scale[1]
								);

								MapImporter* importer = Node_getMapImporter( solid );
								MapVMFTextInputStream istream( faceData );
								importer->importTokens( NewScriptTokeniser( istream ) );
							}
						}
						NodeSmartReference solidnode( solid );

						if ( !Node_getTraversable( entity ) ) {
							globalWarningStream() << "FIXME: bad solid data in " << Node_getEntity( entity )->getClassName() << '\n';
						} else {
							Node_getTraversable( entity )->insert( solidnode );
						}
					} else if (string_equal_nocase(e.getKey().data(), "editor")) {
						// FIXME: do we care about any of this?
					} else {
						if (hasSolids && string_equal_nocase(e.getKey().data(), "origin")) {
							// do nothing
						} else {
							Node_getEntity( entity )->setKeyValue( e.getKey().data(), e.getValue().data() );
						}
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
		class MapVMFTokenWriter : public TokenWriter
		{
			std::vector<std::string> m_lines;
		public:
			MapVMFTokenWriter() {
				m_lines.push_back("");
			}
			virtual void release() {
			}
			virtual void nextLine() {
				m_lines.push_back("");
			}
			virtual void writeToken( const char* token ) {
				m_lines.back().append( std::format( "{} ", token ) );
			}
			virtual void writeString( const char* string ) {
				m_lines.back().append( std::format( "\"{}\" ", string ) );
			}
			virtual void writeInteger( int i ) {
				m_lines.back().append( std::format( "{} ", i ) );
			}
			virtual void writeUnsigned( std::size_t i ) {
				m_lines.back().append( std::format( "{} ", i ) );
			}
			virtual void writeFloat( double f ) {
				m_lines.back().append( std::format( "{} ", f ) );
			}
			void forEachLine( int64_t& childID, kvpp::KV1ElementWritable<std::string>& solid, std::function<void(int64_t& childID, std::string& line, kvpp::KV1ElementWritable<std::string>& side)> func ) {
				for ( auto line : m_lines ) {
					if ( line[0] == '(' ) {
						func( childID, line, solid.addChild("side") );
					}
				}
			}
		};
		void writeSolid( scene::Node& node, kvpp::KV1ElementWritable<std::string>& solid ) const {
			solid["id"] = m_childID++;

			MapExporter* exporter = Node_getMapExporter( node );

			// since VMFs are a different beast from MAP files, we need to use Radiant's normal exporter and then do our own thing with it
			// FIXME: this sucks and could surely be done better.
			MapVMFTokenWriter writer;
			exporter->exportTokens( writer );
			writer.forEachLine( m_childID, solid, [](int64_t& childID, std::string& line, kvpp::KV1ElementWritable<std::string>& side){
				double points[3][3];
				char material[128];
				double uaxis[4];
				double vaxis[4];
				double rotation;
				double scale[2];
				sscanf(line.c_str(), "( %lf %lf %lf ) ( %lf %lf %lf ) ( %lf %lf %lf ) %128s [ %lf %lf %lf %lf ] [ %lf %lf %lf %lf ] %lf %lf %lf",
					&points[0][0], &points[0][1], &points[0][2], &points[1][0], &points[1][1], &points[1][2], &points[2][0], &points[2][1], &points[2][2],
					material, &uaxis[0], &uaxis[1], &uaxis[2], &uaxis[3], &vaxis[0], &vaxis[1], &vaxis[2], &vaxis[3], &rotation, &scale[0], &scale[1]
				);
				side["id"] = childID++;
				side["plane"] = std::format( "({} {} {}) ({} {} {}) ({} {} {})", points[0][0], points[0][1], points[0][2], points[1][0], points[1][1], points[1][2], points[2][0], points[2][1], points[2][2] );
				side["material"] = material;
				side["uaxis"] = std::format( "[{} {} {} {}] {}", uaxis[0], uaxis[1], uaxis[2], uaxis[3], scale[0] );
				side["vaxis"] = std::format( "[{} {} {} {}] {}", vaxis[0], vaxis[1], vaxis[2], vaxis[3], scale[1] );
				side["rotation"] = std::format( "{}", rotation );
				side["lightmapscale"] = 16; // FIXME: make configurable
				side["smoothing_groups"] = 0; // FIXME: make configurable
			});
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

		GlobalBrushCreator().toggleFormat(eBrushTypeValve220);

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
