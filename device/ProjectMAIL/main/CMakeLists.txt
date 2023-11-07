idf_component_register(SRCS "app_main.cpp" "matter_start.cpp" "app_driver.cpp" "light_control.cpp" "sensor.cpp"
                      SRC_DIRS          "."
                      PRIV_INCLUDE_DIRS  "."
                      INCLUDE_DIRS ".")

# spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")

