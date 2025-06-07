cmake_minimum_required(VERSION 3.21)
project(radiant LANGUAGES C CXX)

# external dependencies

include(FetchContent)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
	GIT_TAG "v6.0.1"
	EXCLUDE_FROM_ALL
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

# packages

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(LibXml2 REQUIRED)

# options

set(RADIANT_VERSION "1.6.0n" CACHE STRING "")
set(RADIANT_MAJOR_VERSION "6" CACHE STRING "")
set(RADIANT_MINOR_VERSION "0" CACHE STRING "")
set(RADIANT_ABOUTMSG "Custom build" CACHE STRING "")

# commandlib

add_library(commandlib STATIC
	${PROJECT_SOURCE_DIR}/libs/commandlib.cpp
)

target_link_libraries(commandlib PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
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

target_link_libraries(gtkutil PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
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

# ddslib

add_library(ddslib STATIC
	${PROJECT_SOURCE_DIR}/libs/ddslib/ddslib.c
)

target_include_directories(ddslib PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# etclib

add_library(etclib STATIC
	${PROJECT_SOURCE_DIR}/libs/etclib.c
)

target_include_directories(etclib PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/libs
)

# radiant

add_executable(radiant
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

target_link_libraries(radiant PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(radiant PRIVATE LibXml2::LibXml2)
target_link_libraries(radiant PRIVATE ${GLIB2_LIBRARIES})
target_include_directories(radiant PRIVATE ${GLIB2_INCLUDE_DIRS})

target_link_libraries(radiant PRIVATE commandlib gtkutil l_net xmllib quickhull)

set_target_properties(radiant commandlib gtkutil xmllib PROPERTIES CXX_STANDARD_REQUIRED ON CXX_STANDARD 20)

target_compile_options(radiant PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
target_compile_options(radiant PRIVATE -Wreorder -fno-exceptions -fno-rtti)

set_target_properties(radiant
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install
)

file(WRITE ${PROJECT_SOURCE_DIR}/install/RADIANT_MINOR ${RADIANT_MINOR_VERSION})
file(WRITE ${PROJECT_SOURCE_DIR}/install/RADIANT_MAJOR ${RADIANT_MAJOR_VERSION})

file(GLOB RADIANT_DATA_FILES "${PROJECT_SOURCE_DIR}/setup/data/tools/*")
file(COPY ${RADIANT_DATA_FILES} DESTINATION ${PROJECT_SOURCE_DIR}/install/)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(radiant PRIVATE WIN32)
	target_compile_definitions(commandlib PRIVATE WIN32)
	target_compile_definitions(gtkutil PRIVATE WIN32)
	target_compile_definitions(l_net PRIVATE WIN32)
	target_compile_definitions(xmllib PRIVATE WIN32)
	target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"exe\")
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
	target_compile_options(${name} PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
	target_compile_options(${name} PRIVATE -Wreorder -fno-exceptions -fno-rtti)
	target_include_directories(${name} PRIVATE
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/libs
	)
endfunction()

add_module(archivepak
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/archivepak/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivepak/pak.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivepak/plugin.cpp
)

add_module(archivewad
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/archivewad/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivewad/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivewad/wad.cpp
)

add_module(archivezip
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/archivezip/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivezip/pkzip.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivezip/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/archivezip/zlibstream.cpp
)

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
target_link_libraries(entity PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

add_module(image
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/image/bmp.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/dds.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/image.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/jpeg.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/ktx.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/pcx.cpp
		${PROJECT_SOURCE_DIR}/plugins/image/tga.cpp
)
target_link_libraries(image PRIVATE ddslib etclib)

add_module(imagehl
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/imagehl/hlw.cpp
		${PROJECT_SOURCE_DIR}/plugins/imagehl/imagehl.cpp
		${PROJECT_SOURCE_DIR}/plugins/imagehl/mip.cpp
		${PROJECT_SOURCE_DIR}/plugins/imagehl/sprite.cpp
)

add_module(imagepng
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/imagepng/plugin.cpp
)

add_module(imageq2
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/imageq2/imageq2.cpp
		${PROJECT_SOURCE_DIR}/plugins/imageq2/wal32.cpp
		${PROJECT_SOURCE_DIR}/plugins/imageq2/wal.cpp
)

add_module(mapq3
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/mapq3/parse.cpp
		${PROJECT_SOURCE_DIR}/plugins/mapq3/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/mapq3/write.cpp
)

add_module(mapxml
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/mapxml/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/mapxml/xmlparse.cpp
		${PROJECT_SOURCE_DIR}/plugins/mapxml/xmlwrite.cpp
)
target_link_libraries(mapxml PRIVATE LibXml2::LibXml2)

add_module(assmodel
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/assmodel/mdlimage.cpp
		${PROJECT_SOURCE_DIR}/plugins/assmodel/model.cpp
		${PROJECT_SOURCE_DIR}/plugins/assmodel/plugin.cpp
)
target_link_libraries(assmodel PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(assmodel PRIVATE assimp)
set_target_properties(assimp
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/modules
		RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/install/modules
)

add_module(shaders
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/shaders/plugin.cpp
		${PROJECT_SOURCE_DIR}/plugins/shaders/shaders.cpp
)
target_link_libraries(shaders PRIVATE commandlib)
target_link_libraries(shaders PRIVATE LibXml2::LibXml2)
target_link_libraries(shaders PRIVATE ${GLIB2_LIBRARIES})
target_include_directories(shaders PRIVATE ${GLIB2_INCLUDE_DIRS})

add_module(vfspk3
	SOURCES
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/archive.cpp
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/vfs.cpp
		${PROJECT_SOURCE_DIR}/plugins/vfspk3/vfspk3.cpp
)
target_link_libraries(vfspk3 PRIVATE LibXml2::LibXml2)
target_link_libraries(vfspk3 PRIVATE ${GLIB2_LIBRARIES})
target_link_libraries(vfspk3 PRIVATE filematch)
target_include_directories(vfspk3 PRIVATE ${GLIB2_INCLUDE_DIRS})
