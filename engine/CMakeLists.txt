find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(Assimp REQUIRED)
find_package(GLEW REQUIRED)

#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_INFO")

add_library(Cobalt camera.cpp gl_exception.cpp lights.cpp material.h material.cpp mesh.cpp node.cpp
            scene.cpp shader.cpp uniforms.h uniforms.cpp simple_render_pass.cpp texture.cpp util.cpp
            cobalt.h render_pass.h framebuffer.cpp framebuffer.h framebuffer_attachment.cpp
			framebuffer_attachment.h texture_framebuffer_attachment.cpp texture_framebuffer_attachment.h
			renderbuffer_framebuffer_attachment.cpp renderbuffer_framebuffer_attachment.h
			texture_data_source.cpp texture_data_source.h texture_cache.cpp texture_cache.h
			texture2d.cpp texture2d.h cubemap.cpp cubemap.h window.cpp window.h bounding_box.cpp
			bounding_box.h name_manager.cpp library/standard.h library/std_material.h
			library/std_material.cpp library/std_render_pass.h library/std_render_pass.cpp
		library/std_scene.h library/std_scene.cpp indent.cpp)
			# CLion ignores header files that are not named like the corresponding .cpp file, so enumerate them manually
target_include_directories(Cobalt PUBLIC ${SOIL_INCLUDE_DIRS})
target_include_directories(Cobalt PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(Cobalt ${GLFW_LIBRARIES})
target_include_directories(Cobalt PRIVATE ${GLUT_INCLUDE_DIRS})
target_link_libraries(Cobalt ${GLUT_LIBRARIES})
target_include_directories(Cobalt PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(Cobalt ${OPENGL_LIBRARIES})
target_include_directories(Cobalt PRIVATE ${GLM_INCLUDE_DIR})
target_link_libraries(Cobalt "-lSOIL")
target_include_directories(Cobalt PRIVATE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(Cobalt ${ASSIMP_LIBRARIES})
target_include_directories(Cobalt PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(Cobalt ${GLEW_LIBRARIES})
target_link_libraries(Cobalt X11)
target_link_libraries(Cobalt pthread)
target_link_libraries(Cobalt Xrandr)
target_link_libraries(Cobalt Xxf86vm)
target_link_libraries(Cobalt Xcursor)
target_link_libraries(Cobalt Xinerama)
