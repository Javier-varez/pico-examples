include(FetchContent)

FetchContent_Declare(
  Postform
    GIT_REPOSITORY https://github.com/Javier-varez/Postform
    GIT_TAG main)

# This one is needed because we need some atomic polyfill for Postform
# (cortex-m0plus does not have ldrex and strex instructions)
FetchContent_Declare(
    Cortex-m-atomics
    GIT_REPOSITORY https://github.com/Javier-varez/cortex-m_atomics
    GIT_TAG main)

FetchContent_MakeAvailable(Postform Cortex-m-atomics)

add_executable(pico_postform
        pico_postform.cpp
        postform_config.cpp
        )

target_link_libraries(pico_postform
        pico_stdlib              # for core functionality
        pico_cyw43_arch_none     # we need Wifi to access the GPIO, but we don't need anything else

        # Make sure to link all postform symbols, because we need to bring object symbols
        # even if they are not directly used (like postform_version, which is needed for the postform decoder)
        -Wl,--whole-archive
        postform
        -Wl,--no-whole-archive

        cortex-m_atomics
        )

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_postform)
