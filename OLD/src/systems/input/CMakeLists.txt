add_library(input STATIC
        keyboard_system.cpp
        mouse_input_system.cpp


        )
target_link_libraries(input rltk utils components raws bfmain)
cotire(input)
