if(${SG_BUILD_PRESET} STREQUAL "release-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/proj3/cmake-build-release/proj3.${SG_DL_EXT}")
elseif(${SG_BUILD_PRESET} STREQUAL "debug-host")
    list(APPEND SungearEngine_LIBS "$ENV{SUNGEAR_SOURCES_ROOT}/Plugins/proj3/cmake-build-debug/proj3.${SG_DL_EXT}")
endif()