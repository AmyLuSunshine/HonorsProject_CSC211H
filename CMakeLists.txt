cmake_minimum_required(VERSION 3.16)
project(campus_Hire_app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Make Qt prefix path more flexible
if(NOT DEFINED CMAKE_PREFIX_PATH)
    if(WIN32)
        # Use Community installation path
        set(CMAKE_PREFIX_PATH "C:/Qt_Community/6.9.0/mingw_64")
    endif()
endif()

# Find Qt packages
find_package(Qt6 COMPONENTS 
    Core
    Gui
    Widgets
    Sql
    REQUIRED
)

# Source files organization
set(PROJECT_SOURCES
    src/main.cpp
    src/auth/loginwindow.cpp
    src/auth/registerwindow.cpp
    src/database/database.cpp
    src/models/user.cpp
    src/models/job.cpp
    src/models/application.cpp
    src/ui/jobdetailwidget.cpp
    src/ui/joblistwidget.cpp
    src/ui/mainwindow.cpp
    src/ui/profilewidget.cpp
    src/ui/searchfilterwidget.cpp
)

set(PROJECT_HEADERS
    include/auth/loginwindow.h
    include/auth/registerwindow.h
    include/database/database.h
    include/models/user.h
    include/models/job.h
    include/models/application.h
    include/ui/jobdetailwidget.h
    include/ui/joblistwidget.h
    include/ui/mainwindow.h
    include/ui/profilewidget.h
    include/ui/searchfilterwidget.h
)

# Add resource files if any
# set(PROJECT_RESOURCES
#     resources/resources.qrc
# )

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    # ${PROJECT_RESOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

# Deployment (Windows specific)
# if(WIN32)
#     # Check if in Debug mode
#     set(DEBUG_SUFFIX)
#     if(CMAKE_BUILD_TYPE MATCHES "Debug")
#         set(DEBUG_SUFFIX "")
#     endif()
    
#     # Copy Qt DLLs to output directory
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms"
#         COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers"
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             $<TARGET_FILE:Qt6::Core>
#             $<TARGET_FILE:Qt6::Gui>
#             $<TARGET_FILE:Qt6::Widgets>
#             $<TARGET_FILE:Qt6::Sql>
#             "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#     )
    
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             "${CMAKE_PREFIX_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
#             "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/"
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             "${CMAKE_PREFIX_PATH}/plugins/sqldrivers/qsqlite${DEBUG_SUFFIX}.dll"
#             "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/sqldrivers/"
#     )
# endif()
