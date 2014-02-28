# Add defines for code that care about configuration types.

if (CMAKE_VERSION VERSION_LESS 2.8.12)
  foreach (config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER "${config}" upper_config)

    set(config_defines
      "SPROKIT_CONFIGURATION=\"${config}\""
      "SPROKIT_CONFIGURATION_L=L\"${config}\"")

    set_directory_properties(
      PROPERTIES
        "COMPILE_DEFINITIONS_${upper_config}" "${config_defines}")
  endforeach ()
else ()
  add_compile_options(
    "-DSPROKIT_CONFIGURATION=\"$<CONFIGURATION>\""
    "-DSPROKIT_CONFIGURATION_L=L\"$<CONFIGURATION>\"")
endif ()
