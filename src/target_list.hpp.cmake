#
# Copyright 2019 Xilinx, Inc.
# Copyright 2022 - 2024 Advanced Micro Devices, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set(TARGET_PROTOTXT_LST "")
set(TARGET_PROTOTXT_CODE "")
string(REPLACE " " ";" PROTOTXT_FILE_LIST ${PROTOTXT_FILE_LIST})
foreach(PROTOTXT_FILE ${PROTOTXT_FILE_LIST})
  file(READ ${PROTOTXT_FILE} _hex HEX)
  # Separate into individual bytes.
  string(REGEX MATCHALL "([A-Za-z0-9][A-Za-z0-9])" _separated_hex "${_hex}")
  list(APPEND _separated_hex "00")
  # Append the "0x" to each byte.
  list(JOIN _separated_hex ",0x" _formatted_hex)
  # JOIN misses the first byte's "0x", so add it here.
  string(PREPEND _formatted_hex "0x")
  string(MAKE_C_IDENTIFIER ${PROTOTXT_FILE} _c_name)
  string(APPEND TARGET_PROTOTXT_CODE "static constexpr char ${_c_name} [] = { ${_formatted_hex} };\n")
  list(APPEND TARGET_PROTOTXT_LST "(const char *)${_c_name}")
endforeach()
list(JOIN TARGET_PROTOTXT_LST "," TARGET_PROTOTXTS)
configure_file(${IN} ${OUT} ESCAPE_QUOTES @ONLY)
