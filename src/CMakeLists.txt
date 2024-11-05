# Copyright (C) 2022 Xilinx, Inc. All rights reserved.
# Copyright (C) 2023 - 2024 Advanced Micro Devices, Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
#
#

# get the git repo branch and commit id and write them into config.hpp
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE PROJECT_GIT_BRANCH_NAME
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND git rev-parse HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE PROJECT_GIT_COMMIT_ID
  OUTPUT_STRIP_TRAILING_WHITESPACE)
configure_file(config.hpp.in config.hpp @ONLY)

# run protoc
if(COMMAND protobuf_generate_cpp)
  protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO TARGET_FACTORY_DLLESPEC target.proto)
  protobuf_generate_python(PROTO_PY target.proto)
else()
  set(PROTO_SRCS "")
  set(PROTO_HDRS "")
  foreach(PROTO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/target.proto)
    get_filename_component(PROTO_FILE_NAME ${PROTO_FILE} NAME_WE)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILE_NAME}.pb.cc
             ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILE_NAME}.pb.h
      COMMAND protobuf::protoc
      ARGS --cpp_out=dllexport_decl=TARGET_FACTORY_DLLESPEC:${CMAKE_CURRENT_BINARY_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}
           ${PROTO_FILE}
      DEPENDS ${PROTO_FILE})
    list(APPEND PROTO_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILE_NAME}.pb.cc)
    list(APPEND PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILE_NAME}.pb.h)
  endforeach(PROTO_FILE ${PROTO_FILES})
endif()
#set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_FLAGS -Wno-unused-variable)
if(BUILD_SHARED_LIBS)
    if(MSVC)
        set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_DEFINITIONS "TARGET_FACTORY_DLLESPEC=__declspec(dllexport)")
    else(MSVC)
        set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_DEFINITIONS "TARGET_FACTORY_DLLESPEC=__attribute__((visibility(\"default\")))")
    endif(MSVC)
else(BUILD_SHARED_LIBS)
    set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_DEFINITIONS "TARGET_FACTORY_DLLESPEC=")
endif(BUILD_SHARED_LIBS)

if(NOT MSVC)
    set_source_files_properties(${PROTO_SRCS} PROPERTIES COMPILE_FLAGS -Wno-array-bounds)
endif(NOT MSVC)
# process target files
file(GLOB PROTOTXT_FILE_LIST ${PROJECT_SOURCE_DIR}/targets/*.prototxt)
if(NOT ENABLE_IPU)
    file(GLOB AIE2_SRCS ${PROJECT_SOURCE_DIR}/targets/*AMD_AIE2*.prototxt)
    list(REMOVE_ITEM PROTOTXT_FILE_LIST ${AIE2_SRCS})
endif(NOT ENABLE_IPU)
add_custom_target(target_list.hpp
    DEPENDS
        ${PROTOTXT_FILE_LIST}
        target_list.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/target_list.hpp.cmake
    COMMAND ${CMAKE_COMMAND} -DIN=${CMAKE_CURRENT_SOURCE_DIR}/target_list.hpp.in -DOUT=target_list.hpp
        -DPROTOTXT_FILE_LIST="${PROTOTXT_FILE_LIST}" -P ${CMAKE_CURRENT_SOURCE_DIR}/target_list.hpp.cmake
)

# build library
aux_source_directory(. CPP_SRCS)
add_library(${PROJECT_NAME}
  ../include/vitis/ai/target_factory.hpp
  ${PROTO_SRCS} ${PROTO_PY} ${CPP_SRCS})
add_dependencies(${PROJECT_NAME} target_list.hpp)
# Enable Export of public APIs
target_compile_definitions(${PROJECT_NAME} PRIVATE -DTARGET_FACTORY_EXPORT
	# for an unknown reason, g++ 9.3 produces compilation errors related to PRI64X
	-D__STDC_FORMAT_MACROS=1)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DTARGET_FACTORY_USE_DLL=1)
else(BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DTARGET_FACTORY_USE_DLL=0)
endif(BUILD_SHARED_LIBS)

set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION "${PROJECT_VERSION}"
  SOVERSION "${PROJECT_VERSION_MAJOR}")
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME} PUBLIC protobuf::libprotobuf PRIVATE unilog::unilog)
if(NOT MSVC)
  if(Protobuf_USE_STATIC_LIBS)
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,--exclude-libs,ALL)
  endif(Protobuf_USE_STATIC_LIBS)
endif(NOT MSVC)

# add python api
if(BUILD_PYTHON)
    add_subdirectory(python_api)
endif()

if(CMAKE_SOURCE_DIR STREQUAL target-factory_SOURCE_DIR)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(
  FILES ${PROTO_HDRS} ${PROTO_PY} ${PROJECT_SOURCE_DIR}/include/vitis/ai/target_factory.hpp
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_Nx4_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_Nx4_Overlay_CFG0.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_Nx4_Overlay_CFG1.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_8x4x1_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_8x4x1_Overlay_CFG2.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay_CFG0.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay_CFG1.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_2x4x1_Overlay_CFG1.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay_CFG2.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay_CFG3.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_2x4x1_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2_Nx4_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2_4x4_Overlay.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/AMD_AIE2P_4x4_Overlay_CFG4.prototxt
  FILES ${PROJECT_SOURCE_DIR}/targets/DPUCV3DX8G_ISA0_C8SP1.prototxt
  DESTINATION include/vitis/ai)
install(
  EXPORT ${PROJECT_NAME}-targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION share/cmake/${PROJECT_NAME})
endif()
