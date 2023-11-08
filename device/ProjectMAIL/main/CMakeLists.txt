idf_component_register(SRCS "app_main.cpp" "ondevice_ai.cpp" "model.cc" "matter_start.cpp" "app_driver.cpp" "sensor.cpp" "light_control.cpp"
                    INCLUDE_DIRS "." "includes"
                    REQUIRES ${requires})
# "matter_start.cpp" "app_driver.cpp" "sensor.cpp" "light_control.cpp"
spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")

