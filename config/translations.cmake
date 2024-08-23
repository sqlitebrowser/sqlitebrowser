# Translation files
set(SQLB_TSS
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ar_SA.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_cs.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_zh_TW.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_de.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_es_ES.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_fr.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ru.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_pl.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_pt_BR.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_en_GB.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ko_KR.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_tr.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_uk_UA.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_it.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ja.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_nl.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_sv.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_id.ts"
    "${CMAKE_SOURCE_DIR}/src/translations/sqlb_ro.ts"
)

if(SQLB_TSS)
    # add translations
    foreach(SQLB_TS ${SQLB_TSS})
        set_source_files_properties("${SQLB_TS}" PROPERTIES OUTPUT_LOCATION "${CMAKE_SOURCE_DIR}/src/translations")
    endforeach()
    if(COMMAND qt_add_translation)
        qt_add_translation(SQLB_QMS ${SQLB_TSS})
    else()
        qt5_add_translation(SQLB_QMS ${SQLB_TSS})
    endif()
endif()
