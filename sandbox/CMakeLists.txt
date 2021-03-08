# file management
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
source_group("src" FILES ${SOURCES}) 

# add project
add_executable(sandbox ${SOURCES})

# set up project
target_link_libraries(sandbox ${CORE})
target_compile_features(sandbox PRIVATE cxx_std_17)