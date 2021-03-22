# file management
file(GLOB_RECURSE SOURCES "src/common/*.cpp" "src/common/*.h")
source_group("src" FILES ${SOURCES}) 

# set up project
add_executable(sandbox src/pixmap_art.cpp ${SOURCES})
target_link_libraries(sandbox ${CORE})
target_compile_features(sandbox PRIVATE cxx_std_17)

add_executable(pixmap_test src/pixmap_test.cpp ${SOURCES})
target_link_libraries(pixmap_test ${CORE})
target_compile_features(pixmap_test PRIVATE cxx_std_17)
