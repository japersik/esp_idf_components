cmake_minimum_required(VERSION 3.16)

set(srcs i2c_device.c)
set(include_dir include)

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS ${include_dir}
        PRIV_REQUIRES driver
)
