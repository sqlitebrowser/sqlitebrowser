if(NOT FORCE_INTERNAL_QSCINTILLA)
    find_package(QScintilla 2.8.10)
endif()

if(NOT FORCE_INTERNAL_QCUSTOMPLOT)
    find_package(QCustomPlot)
endif()

if(NOT FORCE_INTERNAL_QHEXEDIT)
    find_package(QHexEdit)
endif()

if(NOT QSCINTILLA_FOUND)
    add_subdirectory(libs/qscintilla_2.14.1/Qt5Qt6)
endif()

if(NOT QHexEdit_FOUND)
    add_subdirectory(libs/qhexedit)
endif()

if(NOT QCustomPlot_FOUND)
    add_subdirectory(libs/qcustomplot-source)
endif()
