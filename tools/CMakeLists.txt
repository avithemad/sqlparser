project(sql-parser)
add_executable(sqlparser sqlparser.cpp)

target_link_libraries(sqlparser
    PRIVATE 
    parser)