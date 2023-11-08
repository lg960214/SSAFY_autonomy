set(requires "")
if(${IDF_TARGET} STREQUAL "linux")
    list(APPEND requires esp_stubs esp-tls esp_http_client protocol_examples_common nvs_flash)
endif()

idf_component_register(SRCS "app_main.cpp" "test.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${requires})

spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)
target_compile_options(${COMPONENT_LIB} PRIVATE "-DCHIP_HAVE_CONFIG_H")
