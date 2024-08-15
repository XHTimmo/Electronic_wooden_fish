idf_component_register(SRCS "ssd1306.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver
                    REQUIRES I2C
                    REQUIRES unity
                    )
