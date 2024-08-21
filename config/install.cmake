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

    if(sqlcipher)
        find_file(DLL_NAME sqlcipher.dll PATH_SUFFIXES bin ../bin ../../bin)
    else()
        find_file(DLL_NAME sqlite3.dll PATH_SUFFIXES bin ../bin ../../bin)
    endif()

    find_file(DLL_CRYPTO
        NAMES libcrypto-1_1-x64.dll libcrypto-1_1.dll
        PATH_SUFFIXES bin ../bin ../../bin
    )
    find_file(DLL_SSL
        NAMES libssl-1_1-x64.dll libssl-1_1.dll
        PATH_SUFFIXES bin ../bin ../../bin
    )

    install(FILES
        ${DLL_NAME}
        ${DLL_CRYPTO}
        ${DLL_SSL}
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
                \"${CMAKE_INSTALL_PREFIX}/$<TARGET_FILE_NAME:${PROJECT_NAME}>\"
            )"
        )
    endif()
endif()
