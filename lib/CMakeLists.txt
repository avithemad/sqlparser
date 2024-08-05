project(sql-parser)

add_library(parser 
            lexer.cpp 
            parser.cpp 
            relalg.cpp
            )

target_include_directories(
    parser
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)