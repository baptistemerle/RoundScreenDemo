idf_component_register(
  SRC_DIRS "."
           "Connectivity"
           "Core"
           "Decoders"
           "Sensors"
           "Storage"
           "UI"
  INCLUDE_DIRS "."
  REQUIRES SharedProtocols ESP32Utilities esp-nimble-cpp cjson
)

target_compile_options(${COMPONENT_LIB} PRIVATE
  -Wall
  -Wextra
  -Wshadow
  -Wdouble-promotion
  -Werror=return-type
)

target_compile_options(${COMPONENT_LIB} PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-Werror=reorder>
)

# Remove error on NimBLEConnInfo.h
set_source_files_properties(Connectivity/ble_controller.cpp PROPERTIES
  COMPILE_FLAGS "-Wno-double-promotion"
)