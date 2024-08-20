if((NOT WIN32 AND NOT APPLE) OR MINGW)
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


if(WIN32 AND MSVC)
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "/"
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    set(QT5_BIN_PATH ${QT5_PATH}/bin)

    # The Qt5 Debug configuration library files have a 'd' postfix
    install(FILES
        ${QT5_BIN_PATH}/Qt5Cored.dll
        ${QT5_BIN_PATH}/Qt5Guid.dll
        ${QT5_BIN_PATH}/Qt5Networkd.dll
        ${QT5_BIN_PATH}/Qt5PrintSupportd.dll
        ${QT5_BIN_PATH}/Qt5Widgetsd.dll
        ${QT5_BIN_PATH}/Qt5Concurrentd.dll
        ${QT5_BIN_PATH}/Qt5Svgd.dll
        DESTINATION "/"
        CONFIGURATIONS Debug
    )

    # The Qt5 Release configuration files don't have a postfix
    install(FILES
        ${QT5_BIN_PATH}/Qt5Core.dll
        ${QT5_BIN_PATH}/Qt5Gui.dll
        ${QT5_BIN_PATH}/Qt5Network.dll
        ${QT5_BIN_PATH}/Qt5PrintSupport.dll
        ${QT5_BIN_PATH}/Qt5Widgets.dll
        ${QT5_BIN_PATH}/Qt5Concurrent.dll
        ${QT5_BIN_PATH}/Qt5Svg.dll
        DESTINATION "/"
        CONFIGURATIONS Release
    )

    # The files below are common to all configurations
    install(FILES
        ${SQLITE3_DLL}
        ${OPENSSL_PATH}/libeay32.dll
        ${OPENSSL_PATH}/ssleay32.dll
        DESTINATION "/"
    )

    install(FILES
        ${QT5_PATH}/plugins/platforms/qwindows.dll
        DESTINATION platforms
    )

    # The XML dll
    install(FILES
        "${QT5_PATH}/bin/Qt5Xmld.dll"
        DESTINATION "/"
        CONFIGURATIONS Debug
    )

    install(FILES
        "${QT5_PATH}/bin/Qt5Xml.dll"
        DESTINATION "/"
        CONFIGURATIONS Release
    )

    # The image format plugins
    install(FILES
        ${WIN_IMG_PLUGINS_DEBUG}
        DESTINATION imageformats
        CONFIGURATIONS Debug
    )

    install(FILES
        ${WIN_IMG_PLUGINS}
        DESTINATION imageformats
        CONFIGURATIONS Release
    )

    # The license files
    install(FILES
        LICENSE
        LICENSE-PLUGINS
        DESTINATION licenses
    )

    # The batch file launcher
    install(FILES
        distri/winlaunch.bat
        DESTINATION "/"
    )
endif()
