set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "DB Browser for SQLite"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/src/app.plist
)
