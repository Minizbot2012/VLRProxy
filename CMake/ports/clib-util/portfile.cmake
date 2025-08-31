# header-only library
vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL https://github.com/powerof3/CLibUtil
    HEAD_REF master
    REF 2efbbac132d020b0401904e9b1c5f4d72ed44475
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")