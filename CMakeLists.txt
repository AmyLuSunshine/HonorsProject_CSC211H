cmake_minimum_required(VERSION 3.16)

project(campus_Hire_app VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt Meta-Object Compiler settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF) # Disabled since you're not using .ui files

# Find Qt packages
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Sql
    REQUIRED
)

# Source files
set(PROJECT_SOURCES
    src/main.cpp
    src/auth/loginwindow.cpp
    src/auth/registerwindow.cpp
    src/database/database.cpp
    src/models/user.cpp
    src/models/job.cpp
    src/models/application.cpp
    src/ui/mainwindow.cpp
    src/ui/myprofilepage.cpp
    src/ui/myapplicationpage.cpp
    src/ui/oncampusjobspage.cpp
    src/ui/profilewidget.cpp
    src/ui/searchfilterwidget.cpp
    src/ui/interviewwidget.cpp
    src/ui/landingpage.cpp
    src/ui/studentsurveydialog.cpp
)

# Header files
set(PROJECT_HEADERS
    include/auth/loginwindow.h
    include/auth/registerwindow.h
    include/database/database.h
    include/models/user.h
    include/models/job.h
    include/models/application.h
    include/ui/mainwindow.h
    include/ui/myprofilepage.h
    include/ui/myapplicationpage.h
    include/ui/oncampusjobspage.h
    include/ui/profilewidget.h
    include/ui/searchfilterwidget.h
    include/ui/interviewwidget.h
    include/ui/landingpage.h
    include/ui/studentsurveydialog.h
)

# Optional: Add resource files when needed
# set(PROJECT_RESOURCES
# resources/resources.qrc
# )

# Create executable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}

    # ${PROJECT_RESOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/auth
    ${CMAKE_CURRENT_SOURCE_DIR}/include/database
    ${CMAKE_CURRENT_SOURCE_DIR}/include/models
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
)

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

# Platform-specific settings
if(WIN32)
    # Set Windows subsystem to GUI (no console window)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )
endif()

# Installation rules (optional)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
