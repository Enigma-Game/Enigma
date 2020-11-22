add_executable(tolua tolua.c toluabind.c toluabind.h)
target_link_libraries(tolua lua)

