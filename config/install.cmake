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

    string(REGEX MATCH "^([0-9]+)\.([0-9]+)" SSL_OUT "${OPENSSL_VERSION}")
    set(DLL_CRYPTO_NAMES
        "libcrypto-${CMAKE_MATCH_1}_${CMAKE_MATCH_2}-x64.dll"
        "libcrypto-${CMAKE_MATCH_1}-x64.dll"
        "libcrypto-${CMAKE_MATCH_1}_${CMAKE_MATCH_2}.dll"
        "libcrypto-${CMAKE_MATCH_1}.dll"
    )

    set(DLL_SSL_NAMES
        "libssl-${CMAKE_MATCH_1}_${CMAKE_MATCH_2}-x64.dll"
        "libssl-${CMAKE_MATCH_1}-x64.dll"
        "libssl-${CMAKE_MATCH_1}_${CMAKE_MATCH_2}.dll"
        "libssl-${CMAKE_MATCH_1}.dll"
    )

    find_file(DLL_CRYPTO NAMES ${DLL_CRYPTO_NAMES} PATH_SUFFIXES bin ../bin ../../bin)
    find_file(DLL_SSL NAMES ${DLL_SSL_NAMES} PATH_SUFFIXES bin ../bin ../../bin)

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

    if(QT_MAJOR STREQUAL "Qt5")
        set(OPT_ANGLE "--no-angle")
    endif()

    find_file(QT_DEPLOY windeployqt.exe HINTS ${${QT_MAJOR}_DIR}/../../../bin)
    if(NOT ${QT_DEPLOY} STREQUAL "QT_DEPLOY-NOTFOUND")
        install (CODE
            "execute_process(COMMAND_ECHO STDOUT COMMAND ${QT_DEPLOY}
                --no-system-d3d-compiler
                ${OPT_ANGLE}
                --no-opengl-sw
                \"\${CMAKE_INSTALL_PREFIX}/$<TARGET_FILE_NAME:${PROJECT_NAME}>\"
            )"
        )
    endif()
endif()
