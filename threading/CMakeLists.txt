project(threading)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
  INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Config
        DESTINATION share/${PROJECT_NAME}/cmake
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION include
)

install(
  EXPORT ${PROJECT_NAME}Config
  FILE ${PROJECT_NAME}Config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME}
)
export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

# add_executable(
#   fstar_test
#   tests/fstar_test.cc
# )

# target_link_libraries(
#   fstar_test
#   LINK_PUBLIC ${Boost_LIBRARIES}
#   ${PROJECT_NAME}
# )
