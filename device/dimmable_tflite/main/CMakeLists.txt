idf_component_register(#SRC_DIRS          "."
					  SRCS app_main.cpp app_driver.cpp model.cc 
                      PRIV_INCLUDE_DIRS  ".")

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
