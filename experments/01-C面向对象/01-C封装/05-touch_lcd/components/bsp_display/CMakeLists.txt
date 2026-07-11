idf_component_register(SRCS "ili9488.c" "bsp_display.c"
                    INCLUDE_DIRS "include"
                    REQUIRES bsp_gpio bsp_spi bsp_ledc board_pin)
