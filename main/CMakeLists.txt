set(APP_SOURCES "main.c"
    "screen/st7789.c"
    "screen/fontx.c"
    "screen/pngle.c"
    "camera/app_camera.c")

idf_component_register(SRCS ${APP_SOURCES} INCLUDE_DIRS "." "./screen" "./camera")
spiffs_create_partition_image(storage ../font FLASH_IN_PROJECT)
