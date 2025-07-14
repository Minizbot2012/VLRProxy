vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO powerof3/CommonLibSSE
  HEAD_REF dev
  REF f8d0bca159e85448953a2bbe0c07a3f002207d3b
  SHA512 5313cfe804faa3ac73fac0078ed43903527e50c4e20bd5c8f39ee14f21e5f82ba811a53d23a2bf6cf983d65018eea17a9f14a7ac2e7c04d853591ad111c4d3b7
)

set(VCPKG_POLICY_EMPTY_PACKAGE enabled)

vcpkg_configure_cmake(
        SOURCE_PATH "${SOURCE_PATH}"
        PREFER_NINJA
        OPTIONS -DSKSE_SUPPORT_XBYAK=on -DSKYRIM_SUPPORT_AE=on
)

vcpkg_install_cmake()
vcpkg_cmake_config_fixup(PACKAGE_NAME CommonLibSSE CONFIG_PATH lib/cmake)
vcpkg_copy_pdbs()

file(GLOB CMAKE_CONFIGS "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE/*.cmake")
file(INSTALL ${CMAKE_CONFIGS} DESTINATION "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE")

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/share/CommonLibSSE/CommonLibSSE")

file(
        INSTALL "${SOURCE_PATH}/LICENSE"
        DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
        RENAME copyright)
