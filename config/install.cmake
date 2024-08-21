if(NOT WIN32 AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(UNIX)
    install(FILES src/icons/${PROJECT_NAME}.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps/
    )

    install(FILES images/logo.svg
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps/
        RENAME ${PROJECT_NAME}.svg
    )

    install(FILES distri/${PROJECT_NAME}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
    )

    install(FILES distri/${PROJECT_NAME}.desktop.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/
    )
endif()


if(WIN32)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "."
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(FILES
        "$<TARGET_FILE_DIR:${LIBSQLITE_NAME}>/../bin/sqlite3.dll"
        "$<TARGET_FILE_DIR:OpenSSL::SSL>/../bin/libcrypto-1_1-x64.dll"
        "$<TARGET_FILE_DIR:OpenSSL::SSL>/../bin/libssl-1_1-x64.dll"
        DESTINATION "."
    )

    # The license files
    install(FILES
        LICENSE
        LICENSE-GPL-3.0
        LICENSE-MIT
        LICENSE-MPL-2.0
        LICENSE-PLUGINS
        DESTINATION licenses
    )

    find_file(QT_DEPLOY windeployqt.exe HINTS ${${QT_MAJOR}_DIR}/../../../bin)
    if(NOT ${QT_DEPLOY} STREQUAL "QT_DEPLOY-NOTFOUND")
        install (CODE
            "execute_process(COMMAND_ECHO STDOUT COMMAND ${QT_DEPLOY}
                --no-system-d3d-compiler
                --no-angle
                --no-opengl-sw
                \"${CMAKE_INSTALL_PREFIX}\"
            )"
        )
    endif()
endif()
