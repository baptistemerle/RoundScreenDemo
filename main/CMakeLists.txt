idf_component_register(
  SRCS "main.cpp"
  INCLUDE_DIRS "."
  REQUIRES SharedProtocols
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