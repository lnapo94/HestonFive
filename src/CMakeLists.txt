
#----- Check for the required RTLib library
find_package(BbqRTLib REQUIRED)

#----- Add compilation dependencies
include_directories(${BBQUE_RTLIB_INCLUDE_DIR})

#----- Add "hestonfive" target application
set(HESTONFIVE_SRC version HestonFive_exc HestonFive_main HestonWorker EuropeanCall EuropeanPut Option)
add_executable(hestonfive ${HESTONFIVE_SRC})

#----- Linking dependencies
target_link_libraries(
	hestonfive
	${Boost_LIBRARIES}
	${BBQUE_RTLIB_LIBRARY}
)

# Use link path ad RPATH
set_property(TARGET hestonfive PROPERTY
	INSTALL_RPATH_USE_LINK_PATH TRUE)

#----- Install the HestonFive files
install (TARGETS hestonfive RUNTIME
	DESTINATION ${HESTONFIVE_PATH_BINS})

#----- Generate and Install HestonFive configuration file
configure_file (
	"${PROJECT_SOURCE_DIR}/HestonFive.conf.in"
	"${PROJECT_BINARY_DIR}/HestonFive.conf"
)
install (FILES "${PROJECT_BINARY_DIR}/HestonFive.conf"
	DESTINATION ${HESTONFIVE_PATH_CONFIG})
