project(graphics_assign1)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(src SRC_LIST) 
add_executable(${PROJECT_NAME} ${SRC_LIST}) # Naming convention of exectuable

find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW3 REQUIRED glfw3) # libglfw3-dev libglfw3
include_directories(${GLFW3_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GLFW3_LIBRARIES})

pkg_check_modules(GLEW REQUIRED glew) # libglew-dev
include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GLEW_LIBRARIES})

pkg_check_modules(GL REQUIRED gl) 
include_directories(${GL_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${GL_LIBRARIES})

pkg_check_modules(AO REQUIRED ao) #libao-dev
include_directories(${AO_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${AO_LIBRARIES})

pkg_check_modules(LIBMPG123 REQUIRED libmpg123) # libmpg123-dev
include_directories(${LIBMPG123_INCLUDE_DIRS})
target_link_libraries (${PROJECT_NAME} ${LIBMPG123_LIBRARIES})

configure_file(src/Sample_GL.vert Sample_GL.vert COPYONLY)
configure_file(src/Sample_GL.frag Sample_GL.frag COPYONLY)
# configure_file(src/fontrender.vert fontrender.vert COPYONLY)
# configure_file(src/fontrender.frag fontrender.frag COPYONLY)
configure_file(assets/supermarine.mp3 supermarine.mp3 COPYONLY) # Music
# configure_file(assets/silent.mp3 silent.mp3 COPYONLY)
configure_file(assets/arial.ttf arial.ttf COPYONLY) # Font
