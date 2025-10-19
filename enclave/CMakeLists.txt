add_subdirectory(inference)

# Generate EDL stubs
add_custom_command(
    OUTPUT inference_t.h inference_t.c inference_args.h
    DEPENDS inference.edl
    COMMAND openenclave::oeedger8r --trusted inference.edl --search-path \${OE_INCLUDEDIR}
)

add_enclave(
    TARGET furcate_enclave
    SOURCES
        inference.c
        attestation.c
        \${CMAKE_CURRENT_BINARY_DIR}/inference_t.c
    EDL inference.edl
)

target_include_directories(furcate_enclave PRIVATE \${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(furcate_enclave openenclave::oeenclave openenclave::oelibc)
