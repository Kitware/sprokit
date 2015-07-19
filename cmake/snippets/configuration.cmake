# Add defines for code that care about configuration types.

#add_compile_options(
#  "SPROKIT_CONFIGURATION=\"$<CONFIGURATION>\""
#  "SPROKIT_CONFIGURATION_L=L\"$<CONFIGURATION>\"")

if (DEFINED CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE
    PROPERTY
      STRINGS "Release;Debug;RelWithDebInfo;MinSizeRel")
endif ()

# XXX(cmake): 2.8.12
foreach (config IN LISTS CMAKE_CONFIGURATION_TYPES)
  string(TOUPPER "${config}" upper_config)

  set(config_defines
    "SPROKIT_CONFIGURATION=\"${config}\""
    "SPROKIT_CONFIGURATION_L=L\"${config}\"")

  set_directory_properties(
    PROPERTIES
      COMPILE_DEFINITIONS_${upper_config} "${config_defines}")
endforeach ()
