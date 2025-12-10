cmake_minimum_required(VERSION 3.21)
project(radiant LANGUAGES C CXX)

# external dependencies

cmake_policy(SET CMP0077 NEW)

include(FetchContent)

### THIS IS TEMPORARY!
FetchContent_Declare(
	CMP_Compressonator
	GIT_REPOSITORY https://github.com/craftablescience/compressonator
	GIT_TAG 7b3532c36e974a42850af123dcd65dacdacc9f47
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(CMP_Compressonator)
set_target_properties(CMP_Compressonator PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(CMP_Core PROPERTIES POSITION_INDEPENDENT_CODE ON)
###

FetchContent_Declare(
	sourcepp
	GIT_REPOSITORY https://github.com/craftablescience/sourcepp.git
	GIT_TAG "v2025.12.4"
	EXCLUDE_FROM_ALL
)

set(SOURCEPP_LIBS_START_ENABLED OFF CACHE BOOL "" FORCE)
set(SOURCEPP_USE_KVPP ON CACHE BOOL "" FORCE) # for VMTs
set(SOURCEPP_USE_MDLPP ON CACHE BOOL "" FORCE) # for MDLs
set(SOURCEPP_USE_TOOLPP ON CACHE BOOL "" FORCE) # for FGDs
set(SOURCEPP_USE_VPKPP ON CACHE BOOL "" FORCE) # for VPKs
set(SOURCEPP_USE_VTFPP ON CACHE BOOL "" FORCE) # for VTFs
set(SOURCEPP_VTFPP_SUPPORT_EXR OFF CACHE BOOL "" FORCE)
set(SOURCEPP_VTFPP_SUPPORT_QOI OFF CACHE BOOL "" FORCE)
set(SOURCEPP_VTFPP_SUPPORT_WEBP OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sourcepp)

find_package(LibXml2)
if(NOT LibXml2_FOUND)
	FetchContent_Declare(
		libxml2
		GIT_REPOSITORY https://gitlab.gnome.org/GNOME/libxml2.git
		GIT_TAG "v2.15.1"
		EXCLUDE_FROM_ALL
	)
	set(LIBXML2_WITH_ICONV OFF CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(libxml2)
endif()

find_package(PNG)
if(NOT PNG_FOUND)
	FetchContent_Declare(
		libpng
		GIT_REPOSITORY https://github.com/pnggroup/libpng.git
		GIT_TAG "v1.6.53"
		EXCLUDE_FROM_ALL
	)
	set(PNG_SHARED OFF CACHE BOOL "" FORCE)
	set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
	set(PNG_TESTS OFF CACHE BOOL "" FORCE)
	set(PNG_STATIC ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(libpng)
	add_library(PNG::PNG ALIAS png_static)
endif()

# packages

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Svg)

# options

set(RADIANT_VERSION "1.6.0n" CACHE STRING "")
set(RADIANT_MAJOR_VERSION "6" CACHE STRING "")
set(RADIANT_MINOR_VERSION "0" CACHE STRING "")
set(RADIANT_ABOUTMSG "trans rights are human rights" CACHE STRING "")

# commandlib

add_library(commandlib STATIC
	${PROJECT_SOURCE_DIR}/libs/commandlib.cpp
)

target_link_libraries(commandlib PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
target_include_directories(commandlib PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# gtkutil

add_library(gtkutil STATIC
	${PROJECT_SOURCE_DIR}/libs/gtkutil/accelerator.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/clipboard.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/dialog.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/entry.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/filechooser.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/glfont.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/glwidget.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/guisettings.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/idledraw.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/image.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/menu.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/messagebox.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/nonmodal.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/toolbar.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/widget.cpp
	${PROJECT_SOURCE_DIR}/libs/gtkutil/xorrectangle.cpp
)

target_link_libraries(gtkutil PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
target_include_directories(gtkutil PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# l_net

add_library(l_net STATIC
	${PROJECT_SOURCE_DIR}/libs/l_net/l_net.c
	${PROJECT_SOURCE_DIR}/libs/l_net/l_net_wins.c
)

target_include_directories(l_net PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# xmllib

add_library(xmllib STATIC
	${PROJECT_SOURCE_DIR}/libs/xml/ixml.cpp
	${PROJECT_SOURCE_DIR}/libs/xml/xmlelement.cpp
	${PROJECT_SOURCE_DIR}/libs/xml/xmlparser.cpp
	${PROJECT_SOURCE_DIR}/libs/xml/xmltextags.cpp
	${PROJECT_SOURCE_DIR}/libs/xml/xmlwriter.cpp
)

target_include_directories(xmllib PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

target_link_libraries(xmllib PRIVATE LibXml2::LibXml2)

# quickhull

add_library(quickhull STATIC
	${PROJECT_SOURCE_DIR}/libs/quickhull/QuickHull.cpp
)

# filematch

add_library(filematch STATIC
	${PROJECT_SOURCE_DIR}/libs/filematch.c
)

target_include_directories(filematch PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# mathlib

add_library(mathlib STATIC
	${PROJECT_SOURCE_DIR}/libs/mathlib/bbox.c
	${PROJECT_SOURCE_DIR}/libs/mathlib/line.c
	${PROJECT_SOURCE_DIR}/libs/mathlib/m4x4.c
	${PROJECT_SOURCE_DIR}/libs/mathlib/mathlib.c
	${PROJECT_SOURCE_DIR}/libs/mathlib/ray.c
)

target_include_directories(mathlib PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# radiant

add_executable(radiant WIN32
	${PROJECT_SOURCE_DIR}/radiant/autosave.cpp
	${PROJECT_SOURCE_DIR}/radiant/brushmanip.cpp
	${PROJECT_SOURCE_DIR}/radiant/brushmodule.cpp
	${PROJECT_SOURCE_DIR}/radiant/brushnode.cpp
	${PROJECT_SOURCE_DIR}/radiant/brush.cpp
	${PROJECT_SOURCE_DIR}/radiant/brush_primit.cpp
	${PROJECT_SOURCE_DIR}/radiant/brushtokens.cpp
	${PROJECT_SOURCE_DIR}/radiant/brushxml.cpp
	${PROJECT_SOURCE_DIR}/radiant/build.cpp
	${PROJECT_SOURCE_DIR}/radiant/camwindow.cpp
	${PROJECT_SOURCE_DIR}/radiant/clippertool.cpp
	${PROJECT_SOURCE_DIR}/radiant/colors.cpp
	${PROJECT_SOURCE_DIR}/radiant/commands.cpp
	${PROJECT_SOURCE_DIR}/radiant/console.cpp
	${PROJECT_SOURCE_DIR}/radiant/csg.cpp
	${PROJECT_SOURCE_DIR}/radiant/dialog.cpp
	${PROJECT_SOURCE_DIR}/radiant/eclass_def.cpp
	${PROJECT_SOURCE_DIR}/radiant/eclass_doom3.cpp
	${PROJECT_SOURCE_DIR}/radiant/eclass_fgd.cpp
	${PROJECT_SOURCE_DIR}/radiant/eclass.cpp
	${PROJECT_SOURCE_DIR}/radiant/eclass_xml.cpp
	${PROJECT_SOURCE_DIR}/radiant/entityinspector.cpp
	${PROJECT_SOURCE_DIR}/radiant/entitylist.cpp
	${PROJECT_SOURCE_DIR}/radiant/entity.cpp
	${PROJECT_SOURCE_DIR}/radiant/environment.cpp
	${PROJECT_SOURCE_DIR}/radiant/error.cpp
	${PROJECT_SOURCE_DIR}/radiant/feedback.cpp
	${PROJECT_SOURCE_DIR}/radiant/filetypes.cpp
	${PROJECT_SOURCE_DIR}/radiant/filterbar.cpp
	${PROJECT_SOURCE_DIR}/radiant/filters.cpp
	${PROJECT_SOURCE_DIR}/radiant/findtexturedialog.cpp
	${PROJECT_SOURCE_DIR}/radiant/glwidget.cpp
	${PROJECT_SOURCE_DIR}/radiant/grid.cpp
	${PROJECT_SOURCE_DIR}/radiant/groupdialog.cpp
	${PROJECT_SOURCE_DIR}/radiant/gtkdlgs.cpp
	${PROJECT_SOURCE_DIR}/radiant/gtkmisc.cpp
	${PROJECT_SOURCE_DIR}/radiant/help.cpp
	${PROJECT_SOURCE_DIR}/radiant/image.cpp
	${PROJECT_SOURCE_DIR}/radiant/layerswindow.cpp
	${PROJECT_SOURCE_DIR}/radiant/mainframe.cpp
	${PROJECT_SOURCE_DIR}/radiant/main.cpp
	${PROJECT_SOURCE_DIR}/radiant/map.cpp
	${PROJECT_SOURCE_DIR}/radiant/modelwindow.cpp
	${PROJECT_SOURCE_DIR}/radiant/mru.cpp
	${PROJECT_SOURCE_DIR}/radiant/nullmodel.cpp
	${PROJECT_SOURCE_DIR}/radiant/parse.cpp
	${PROJECT_SOURCE_DIR}/radiant/patchdialog.cpp
	${PROJECT_SOURCE_DIR}/radiant/patchmanip.cpp
	${PROJECT_SOURCE_DIR}/radiant/patchmodule.cpp
	${PROJECT_SOURCE_DIR}/radiant/patch.cpp
	${PROJECT_SOURCE_DIR}/radiant/pluginapi.cpp
	${PROJECT_SOURCE_DIR}/radiant/pluginmanager.cpp
	${PROJECT_SOURCE_DIR}/radiant/pluginmenu.cpp
	${PROJECT_SOURCE_DIR}/radiant/plugin.cpp
	${PROJECT_SOURCE_DIR}/radiant/plugintoolbar.cpp
	${PROJECT_SOURCE_DIR}/radiant/points.cpp
	${PROJECT_SOURCE_DIR}/radiant/preferencedictionary.cpp
	${PROJECT_SOURCE_DIR}/radiant/preferences.cpp
	${PROJECT_SOURCE_DIR}/radiant/qe3.cpp
	${PROJECT_SOURCE_DIR}/radiant/qgl.cpp
	${PROJECT_SOURCE_DIR}/radiant/referencecache.cpp
	${PROJECT_SOURCE_DIR}/radiant/renderer.cpp
	${PROJECT_SOURCE_DIR}/radiant/renderstate.cpp
	${PROJECT_SOURCE_DIR}/radiant/scenegraph.cpp
	${PROJECT_SOURCE_DIR}/radiant/selection.cpp
	${PROJECT_SOURCE_DIR}/radiant/select.cpp
	${PROJECT_SOURCE_DIR}/radiant/server.cpp
	${PROJECT_SOURCE_DIR}/radiant/sockets.cpp
	${PROJECT_SOURCE_DIR}/radiant/stacktrace.cpp
	${PROJECT_SOURCE_DIR}/radiant/surfacedialog.cpp
	${PROJECT_SOURCE_DIR}/radiant/texmanip.cpp
	${PROJECT_SOURCE_DIR}/radiant/textures.cpp
	${PROJECT_SOURCE_DIR}/radiant/texwindow.cpp
	${PROJECT_SOURCE_DIR}/radiant/theme.cpp
	${PROJECT_SOURCE_DIR}/radiant/tools.cpp
	${PROJECT_SOURCE_DIR}/radiant/treemodel.cpp
	${PROJECT_SOURCE_DIR}/radiant/undo.cpp
	${PROJECT_SOURCE_DIR}/radiant/url.cpp
	${PROJECT_SOURCE_DIR}/radiant/view.cpp
	${PROJECT_SOURCE_DIR}/radiant/watchbsp.cpp
	${PROJECT_SOURCE_DIR}/radiant/winding.cpp
	${PROJECT_SOURCE_DIR}/radiant/windowobservers.cpp
	${PROJECT_SOURCE_DIR}/radiant/xmlstuff.cpp
	${PROJECT_SOURCE_DIR}/radiant/xywindow.cpp
)

target_compile_definitions(radiant PRIVATE QT_NO_KEYWORDS)
target_compile_definitions(radiant PRIVATE
	RADIANT_VERSION=\"${RADIANT_VERSION}\"
	RADIANT_MAJOR_VERSION=\"${RADIANT_MAJOR_VERSION}\"
	RADIANT_MINOR_VERSION=\"${RADIANT_MINOR_VERSION}\"
	RADIANT_ABOUTMSG=\"${RADIANT_ABOUTMSG}\"
)

target_include_directories(radiant PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

target_link_libraries(radiant PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
target_link_libraries(radiant PRIVATE LibXml2::LibXml2)
target_link_libraries(radiant PRIVATE commandlib gtkutil l_net xmllib quickhull)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(radiant PRIVATE ws2_32)
endif()

set_target_properties(radiant commandlib gtkutil xmllib PROPERTIES CXX_STANDARD_REQUIRED ON CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(radiant PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
	target_compile_options(radiant PRIVATE -Wreorder -fno-exceptions -fno-rtti -fpermissive)
endif()

set_target_properties(radiant
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install
)

file(WRITE ${PROJECT_SOURCE_DIR}/install/RADIANT_MINOR ${RADIANT_MINOR_VERSION})
file(WRITE ${PROJECT_SOURCE_DIR}/install/RADIANT_MAJOR ${RADIANT_MAJOR_VERSION})

file(GLOB RADIANT_DATA_FILES "${PROJECT_SOURCE_DIR}/setup/data/tools/*")
file(COPY ${RADIANT_DATA_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/install/)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.2")
	string(CONCAT RADIANT_STATIC_RUNTIME $<
		$<CXX_COMPILER_LINKER_FRONTEND_VARIANT:GCC>:
		LINKER:SHELL:-static-libstdc++ -static-libgcc
	>)
else()
	set(RADIANT_STATIC_RUNTIME -static-libstdc++ -static-libgcc)
endif()
target_link_options(radiant PRIVATE ${RADIANT_STATIC_RUNTIME})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(radiant PRIVATE WIN32)
	target_compile_definitions(commandlib PRIVATE WIN32)
	target_compile_definitions(gtkutil PRIVATE WIN32)
	target_compile_definitions(l_net PRIVATE WIN32)
	target_compile_definitions(xmllib PRIVATE WIN32)
	target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"exe\")
	install(CODE [[
		file(GET_RUNTIME_DEPENDENCIES
			RESOLVED_DEPENDENCIES_VAR RADIANT_RESOLVED_DEPENDENCIES
			UNRESOLVED_DEPENDENCIES_VAR RADIANT_UNRESOLVED_DEPENDENCIES
			EXECUTABLES
				$<TARGET_FILE:radiant>
			LIBRARIES
				$<TARGET_RUNTIME_DLLS:radiant>
			PRE_EXCLUDE_REGEXES
				"^api-ms-win-crt-.*"
			DIRECTORIES
				$<TARGET_RUNTIME_DLL_DIRS:radiant>
		)
		message(WARNING "unresolved dependencies: ${RADIANT_UNRESOLVED_DEPENDENCIES}")
		file(COPY ${RADIANT_RESOLVED_DEPENDENCIES} DESTINATION $<TARGET_FILE_DIR:radiant>)
		file(COPY $<TARGET_RUNTIME_DLLS:radiant> DESTINATION $<TARGET_FILE_DIR:radiant>)
	]])
else()
	target_compile_definitions(radiant PRIVATE POSIX XWINDOWS)
	target_compile_definitions(commandlib PRIVATE POSIX XWINDOWS)
	target_compile_definitions(gtkutil PRIVATE POSIX XWINDOWS)
	target_compile_definitions(l_net PRIVATE POSIX XWINDOWS)
	target_compile_definitions(xmllib PRIVATE POSIX XWINDOWS)
	if(DEFINED CMAKE_SYSTEM_PROCESSOR)
		string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_PROCESSOR)
		target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"${SYSTEM_PROCESSOR}\")
		set_target_properties(radiant PROPERTIES SUFFIX ".${SYSTEM_PROCESSOR}")
	else()
		target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"unknown\")
	endif()
endif()

# gamepacks

function(add_gamepack name)
	cmake_parse_arguments(PARSE_ARGV 1 ARG "HAS_BASEGAME" "PREFIX;TITLE;TITLE_BASEGAME;GAMEDIR;GAMEDIR_BASEGAME;PATH_WIN32;PATH_LINUX;PATH_MACOS;EXECUTABLE_WIN32;EXECUTABLE_LINUX;EXECUTABLE_MACOS" "")
	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/install/gamepacks/games/")
	file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/install/gamepacks/${name}.game/")
	file(COPY "${PROJECT_SOURCE_DIR}/cmake/default_build_menu.xml" DESTINATION "${PROJECT_SOURCE_DIR}/install/gamepacks/${name}.game/")
	if(ARG_HAS_BASEGAME)
		configure_file("${PROJECT_SOURCE_DIR}/cmake/gamepack-multi.xml.in" "${PROJECT_SOURCE_DIR}/install/gamepacks/games/${name}.game" @ONLY)
	else()
		configure_file("${PROJECT_SOURCE_DIR}/cmake/gamepack-single.xml.in" "${PROJECT_SOURCE_DIR}/install/gamepacks/games/${name}.game" @ONLY)
	endif()
endfunction()

add_gamepack(cstrike
	HAS_BASEGAME
	TITLE_BASEGAME "Half-Life 2"
	GAMEDIR_BASEGAME "hl2"
	TITLE "Counter-Strike: Source"
	GAMEDIR "cstrike"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Counter-Strike Source/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Counter-Strike Source/"
	EXECUTABLE_WIN32 "cstrike.exe"
	EXECUTABLE_LINUX "cstrike.sh"
	EXECUTABLE_MACOS "cstrike.sh"
	PREFIX ".cstrike"
)

add_gamepack(ep2
	HAS_BASEGAME
	TITLE_BASEGAME "Half-Life 2"
	GAMEDIR_BASEGAME "hl2"
	TITLE "Half-Life 2: Episode Two"
	GAMEDIR "ep2"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Half-Life 2/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Half-Life 2/"
	EXECUTABLE_WIN32 "hl2.exe"
	EXECUTABLE_LINUX "hl2.sh"
	EXECUTABLE_MACOS "hl2.sh"
	PREFIX ".ep2"
)

add_gamepack(episodic
	HAS_BASEGAME
	TITLE_BASEGAME "Half-Life 2"
	GAMEDIR_BASEGAME "hl2"
	TITLE "Half-Life 2: Episode One"
	GAMEDIR "episodic"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Half-Life 2/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Half-Life 2/"
	EXECUTABLE_WIN32 "hl2.exe"
	EXECUTABLE_LINUX "hl2.sh"
	EXECUTABLE_MACOS "hl2.sh"
	PREFIX ".episodic"
)

add_gamepack(garrysmod
	HAS_BASEGAME
	TITLE_BASEGAME "sourceengine"
	GAMEDIR_BASEGAME "Source Engine Base Content"
	TITLE "Garry's Mod"
	GAMEDIR "garrysmod"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/"
	PATH_LINUX "~/.steam/steam/steamapps/common/GarrysMod/"
	EXECUTABLE_WIN32 "hl2.exe"
	EXECUTABLE_LINUX "hl2.sh"
	EXECUTABLE_MACOS "hl2.sh"
	PREFIX ".garrysmod"
)

add_gamepack(hl2
	TITLE "Half-Life 2"
	GAMEDIR "hl2"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Half-Life 2/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Half-Life 2/"
	EXECUTABLE_WIN32 "hl2.exe"
	EXECUTABLE_LINUX "hl2.sh"
	EXECUTABLE_MACOS "hl2.sh"
	PREFIX ".hl2"
)

add_gamepack(portal
	HAS_BASEGAME
	TITLE_BASEGAME "Half-Life 2"
	GAMEDIR_BASEGAME "hl2"
	TITLE "Portal"
	GAMEDIR "portal"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Portal/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Portal/"
	EXECUTABLE_WIN32 "hl2.exe"
	EXECUTABLE_LINUX "hl2.sh"
	EXECUTABLE_MACOS "hl2.sh"
	PREFIX ".portal"
)

add_gamepack(tf2
	HAS_BASEGAME
	TITLE_BASEGAME "Half-Life 2"
	GAMEDIR_BASEGAME "hl2"
	TITLE "Team Fortress 2"
	GAMEDIR "tf"
	PATH_WIN32 "C:/Program Files (x86)/Steam/steamapps/common/Team Fortress 2/"
	PATH_LINUX "~/.steam/steam/steamapps/common/Team Fortress 2/"
	EXECUTABLE_WIN32 "tf.exe"
	EXECUTABLE_LINUX "tf.sh"
	EXECUTABLE_MACOS "tf.sh"
	PREFIX ".tf"
)

# modules

function(add_module name)
	cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES")
	add_library(${name} SHARED ${ARG_SOURCES})
	set_target_properties(${name}
		PROPERTIES
			CXX_STANDARD_REQUIRED ON
			CXX_STANDARD 20
			LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/modules
			RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/modules
			PREFIX ""
	)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_compile_definitions(${name} PRIVATE WIN32)
	else()
		target_compile_definitions(${name} PRIVATE POSIX XWINDOWS)
	endif()
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(${name} PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
		target_compile_options(${name} PRIVATE -Wreorder -fno-rtti)
	endif()
	target_include_directories(${name} PRIVATE
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/libs
	)
	target_compile_definitions(${name} PRIVATE QT_NO_KEYWORDS)
	target_compile_definitions(${name} PRIVATE
		RADIANT_VERSION=\"${RADIANT_VERSION}\"
		RADIANT_MAJOR_VERSION=\"${RADIANT_MAJOR_VERSION}\"
		RADIANT_MINOR_VERSION=\"${RADIANT_MINOR_VERSION}\"
		RADIANT_ABOUTMSG=\"${RADIANT_ABOUTMSG}\"
	)
endfunction()

add_module(archivevpk
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/archivevpk/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivevpk/plugin.cpp
)
target_link_libraries(archivevpk PRIVATE sourcepp::vpkpp)
set_target_properties(sourcepp_vpkpp miniz minizip-ng zlib-ng bzip2 liblzma sourcepp_crypto sourcepp_parser sourcepp PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_module(entity
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/entity/angle.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/angles.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/colour.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/doom3group.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/eclassmodel.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/entity.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/filters.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/generic.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/group.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/light.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/miscmodel.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/model.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/modelskinkey.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/namedentity.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/origin.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/rotation.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/scale.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/skincache.cpp
		${PROJECT_SOURCE_DIR}/plugins/entity/targetable.cpp
)
target_link_libraries(entity PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

add_module(imagepng
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/imagepng/plugin.cpp
)
target_link_libraries(imagepng PRIVATE PNG::PNG)

add_module(imagevtf
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/imagevtf/imagevtf.cpp
		${PROJECT_SOURCE_DIR}/plugins/imagevtf/vtf.cpp
)
target_link_libraries(imagevtf PRIVATE sourcepp::vtfpp)
set_target_properties(sourcepp_vtfpp sourcepp_compression PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_module(sourcemodel
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/sourcemodel/mdl.cpp
		${PROJECT_SOURCE_DIR}/plugins/sourcemodel/model.cpp
		${PROJECT_SOURCE_DIR}/plugins/sourcemodel/plugin.cpp
)
target_link_libraries(sourcemodel PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
target_link_libraries(sourcemodel PRIVATE sourcepp::mdlpp)
set_target_properties(sourcepp_mdlpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_module(mapvmf
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/mapvmf/plugin.cpp
)
target_link_libraries(mapvmf PRIVATE sourcepp::kvpp)

add_module(shaders
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/shaders/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/shaders/shaders.cpp
)
target_link_libraries(shaders PRIVATE commandlib)
target_link_libraries(shaders PRIVATE LibXml2::LibXml2)
target_link_libraries(shaders PRIVATE sourcepp::kvpp)
set_target_properties(sourcepp_kvpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_module(vfspk3
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/vfs.cpp
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/vfspk3.cpp
)
target_link_libraries(vfspk3 PRIVATE LibXml2::LibXml2)
target_link_libraries(vfspk3 PRIVATE filematch)

# plugins

function(add_plugin name)
	cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "SOURCES")
	add_library(${name} SHARED ${ARG_SOURCES})
	set_target_properties(${name}
		PROPERTIES
			CXX_STANDARD_REQUIRED ON
			CXX_STANDARD 20
			LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/plugins
			RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/plugins
			PREFIX ""
	)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_compile_definitions(${name} PRIVATE WIN32)
	else()
		target_compile_definitions(${name} PRIVATE POSIX XWINDOWS)
	endif()
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(${name} PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
		target_compile_options(${name} PRIVATE -Wreorder -fno-exceptions -fno-rtti)
	endif()
	target_include_directories(${name} PRIVATE
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/libs
	)
	target_compile_definitions(${name} PRIVATE QT_NO_KEYWORDS)
	target_compile_definitions(${name} PRIVATE
		RADIANT_VERSION=\"${RADIANT_VERSION}\"
		RADIANT_MAJOR_VERSION=\"${RADIANT_MAJOR_VERSION}\"
		RADIANT_MINOR_VERSION=\"${RADIANT_MINOR_VERSION}\"
		RADIANT_ABOUTMSG=\"${RADIANT_ABOUTMSG}\"
	)
endfunction()

if(0)
add_plugin(bobtoolz
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/bobToolz-GTK.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/bsploader.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/cportals.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DBobView.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DBrush.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DEntity.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DEPair.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/dialogs/dialogs-gtk.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DMap.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DPatch.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DPlane.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DPoint.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DShape.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DTrainDrawer.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DTreePlanter.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DVisDrawer.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/DWinding.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/funchandlers-GTK.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/lists.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/misc.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/ScriptParser.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/shapes.cpp
		${PROJECT_SOURCE_DIR}/contrib/bobtoolz/visfind.cpp
)
target_link_libraries(bobtoolz PRIVATE commandlib mathlib)
target_link_libraries(bobtoolz PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
endif()

add_plugin(brushexport
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/brushexport/callbacks.cpp
		${PROJECT_SOURCE_DIR}/contrib/brushexport/export.cpp
		${PROJECT_SOURCE_DIR}/contrib/brushexport/interface.cpp
		${PROJECT_SOURCE_DIR}/contrib/brushexport/plugin.cpp
)
target_link_libraries(brushexport PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

add_plugin(prtview
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/prtview/AboutDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/prtview/ConfigDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/prtview/LoadPortalFileDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/prtview/portals.cpp
		${PROJECT_SOURCE_DIR}/contrib/prtview/prtview.cpp
)
target_link_libraries(prtview PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

add_plugin(sunplug
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/sunplug/sunplug.cpp
)
target_link_libraries(sunplug PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

add_plugin(ufoaiplug
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/ufoaiplug/ufoai_filters.cpp
		${PROJECT_SOURCE_DIR}/contrib/ufoaiplug/ufoai_gtk.cpp
		${PROJECT_SOURCE_DIR}/contrib/ufoaiplug/ufoai_level.cpp
		${PROJECT_SOURCE_DIR}/contrib/ufoaiplug/ufoai.cpp
)
target_link_libraries(ufoaiplug PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)

add_plugin(meshtex
	SOURCES
		${PROJECT_SOURCE_DIR}/contrib/meshtex/GeneralFunctionDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/GenericDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/GenericMainMenu.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/GenericPluginUI.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/GetInfoDialog.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/MainMenu.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/MeshEntity.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/MeshVisitor.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/PluginModule.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/PluginRegistration.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/PluginUI.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/RefCounted.cpp
		${PROJECT_SOURCE_DIR}/contrib/meshtex/SetScaleDialog.cpp
)
target_link_libraries(meshtex PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Svg)
