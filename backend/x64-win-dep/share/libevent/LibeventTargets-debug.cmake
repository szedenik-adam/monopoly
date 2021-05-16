#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "event_core_shared" for configuration "Debug"
set_property(TARGET event_core_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(event_core_shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/event_core.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/event_core.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS event_core_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_event_core_shared "${_IMPORT_PREFIX}/debug/lib/event_core.lib" "${_IMPORT_PREFIX}/debug/bin/event_core.dll" )

# Import target "event_extra_shared" for configuration "Debug"
set_property(TARGET event_extra_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(event_extra_shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/event_extra.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/event_extra.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS event_extra_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_event_extra_shared "${_IMPORT_PREFIX}/debug/lib/event_extra.lib" "${_IMPORT_PREFIX}/debug/bin/event_extra.dll" )

# Import target "event_shared" for configuration "Debug"
set_property(TARGET event_shared APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(event_shared PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/debug/lib/event.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/bin/event.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS event_shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_event_shared "${_IMPORT_PREFIX}/debug/lib/event.lib" "${_IMPORT_PREFIX}/debug/bin/event.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
