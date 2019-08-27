set(ofs_cpp_SRCS
	engine/engine.cpp
	engine/object.cpp
	engine/player.cpp
	main/coreapp.cpp
	main/date.cpp
	render/gl/buffer.cpp
	render/gl/context.cpp
	render/gl/shader.cpp
	render/gl/texture.cpp
	render/context.cpp
	render/mesh.cpp
	render/render.cpp
	render/object.cpp
	render/planet.cpp
	render/terrain.cpp
	universe/astro.cpp
	universe/body.cpp
	universe/star.cpp
	universe/universe.cpp
	util/tree.cpp
)

#set(ofs_qt_cpp_SECS
#	main/qt/qtmain.cpp
#	main/qt/ofswinapp.cpp
#	main/qt/ofscoreapp.cpp
#	main/qt/ofswidget.cpp
#)

set(ofs_sdl2_cpp_SRCS
	main/sdl2/coreapp.cpp
	main/sdl2/main.cpp
)

set(ofs_h_SRCS
	engine/engine.h
	engine/object.h
	engine/player.h
	main/core.h
	main/coreapp.h
	main/date.h
	main/math.h
	render/gl/buffer.h
	render/gl/context.h
	render/gl/shader.h
	render/gl/texture.h
	render/context.h
	render/mesh.h
	render/render.h
	render/object.h
	render/planet.h
	render/terrain.h
	universe/astro.h
	universe/body.h
	universe/star.h
	universe/universe.h
	util/color.h
	util/stb_image.h
	util/tree.h
)

#set(ofs_qt_h_SRCS
#	main/qt/ofswinapp.h
#	main/qt/ofscoreapp.h
#	main/qt/ofswidget.h
#)

set(ofs_sdl2_h_SRCS
	main/sdl2/coreapp.h
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
#	${fmt_INCLUDE_DIRS}
)

#add_executable(ofs ${ofs_cpp_SRCS} ${ofs_qt_cpp_SRCS} ${ofs_h_SRCS} ${ofs_qt_h_SRCS})
#qt5_use_modules(ofs Widgets OpenGL)
#target_link_libraries(ofs ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

add_executable(ofs ${ofs_cpp_SRCS} ${ofs_sdl2_cpp_SRCS} ${ofs_h_SRCS} ${ofs_sdl2_h_SRCS})
target_link_libraries(ofs
	${SDL2_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${ZLIB_LIBRARIES}
#	${fmt_LIBRARIES}
	-lfmt
)
