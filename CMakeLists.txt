cmake_minimum_required(VERSION 3.21)
project(radiant LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(LibXml2 REQUIRED)

set(RADIANT_VERSION "1.6.0n" CACHE STRING "")
set(RADIANT_MAJOR_VERSION "6" CACHE STRING "")
set(RADIANT_MINOR_VERSION "0" CACHE STRING "")
set(RADIANT_ABOUTMSG "Custom build" CACHE STRING "")

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

set_target_properties(radiant PROPERTIES CXX_STANDARD_REQUIRED ON CXX_STANDARD 20)

target_compile_options(radiant PRIVATE -MMD -W -Wall -Wcast-align -Wcast-qual -Wno-unused-parameter -Wno-unused-function -fno-strict-aliasing)
target_compile_options(radiant PRIVATE -Wreorder -fno-exceptions -fno-rtti)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_compile_definitions(radiant PRIVATE WIN32)
	target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"exe\")
else()
	target_compile_definitions(radiant PRIVATE POSIX)
	target_compile_definitions(radiant PRIVATE RADIANT_EXECUTABLE=\"${CMAKE_SYSTEM_PROCESSOR}\")
endif()
