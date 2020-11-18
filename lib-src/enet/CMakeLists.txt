include_directories(include)

add_compile_definitions(HAS_SOCKLEN_T=)

add_library(enet
    host.c list.c callbacks.c packet.c peer.c protocol.c unix.c win32.c)

