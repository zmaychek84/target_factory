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
import re

if __name__ == '__main__':
    CtrlIndex = {"ctrl_shim_col_num": ["uint32", 4],
                 "ctrl_shim_row_num": ["uint32", 1],
                 "ctrl_shim_col_start": ["uint32", 0],
                 "ctrl_shim_row_start": ["uint32", 0],
                 "ctrl_shim_bd_num": ["uint32", 16],
                 "ctrl_memtile_col_num": ["uint32", 5],
                 "ctrl_memtile_row_num": ["uint32", 1],
                 "ctrl_memtile_col_start": ["uint32", 0],
                 "ctrl_memtile_row_start": ["uint32", 1],
                 "ctrl_memtile_bd_num": ["uint32", 48],
                 "ctrl_aie_col_num": ["uint32", 5],
                 "ctrl_aie_row_num": ["uint32", 4],
                 "ctrl_aie_col_start": ["uint32", 0],
                 "ctrl_aie_row_start": ["uint32", 2],
                 "ctrl_aie_bd_num": ["uint32", 16],
                 "ctrl_memtile_lock_num": ["uint32", 64],
                 "ctrl_aietile_lock_num": ["uint32", 16],
                 "ctrl_casc_num": ["uint32", 4],
                 "ctrl_chain_num": ["uint32", 4],
                 "ctrl_layer_param_size": ["uint32", 64],
                 "ctrl_mt_base": ["uint32_hex", 0x80000],
                 "ctrl_ifm_mt_base": ["uint32_hex", 0x80000],
                 "ctrl_bias_mt_base": ["uint32_hex", 0x80000],
                 "ctrl_wgt_core_ping": ["uint32_hex", 0x0000],
                 "ctrl_wgt_core_pong": ["uint32_hex", 0x8000],
                 "ctrl_bias_core_ping": ["uint32_hex", 0x0000],
                 "ctrl_bias_core_pong": ["uint32_hex", 0x8000]}

    MTLockInfo = {"MT_LOCK_BASE_ADDR": ["uint32_hex", 0xc0000], "MT_LOCK_OFFSET": ["uint32_hex", 0x10]}
    ATLockInfo = {"AT_LOCK_BASE_ADDR": ["uint32_hex", 0x1f000], "AT_LOCK_OFFSET": ["uint32_hex", 0x10]}
    ATChannleInfo = {
        "ATChCtrlBase": ["uint32_hex", 0x1de00],
        "ATChQueueBase": ["uint32_hex", 0x1de04],
        "ATNumChannels": ["uint32_hex", 0x2],
        "ATChIdxOffset": ["uint32_hex", 0x8],
    }
    MTChannleInfo = {
        "MTChCtrlBase": ["uint32_hex", 0xa0600],
        "MTChQueueBase": ["uint32_hex", 0xa0604],
        "MTChConstantPadValueBase": ["uint32_hex", 0xa06e0],
        "MTNumChannels": ["uint32_hex", 0x6],
        "MTChIdxOffset": ["uint32_hex", 0x8],
        "MTChConstantPadValueOffset": ["uint32_hex", 0x4],
    }
    ShimChannleInfo = {
        "ShimChCtrlBase": ["uint32_hex", 0x1d200],
        "ShimChQueueBase": ["uint32_hex", 0x1d204],
        "ShimNumChannels": ["uint32_hex", 0x2],
        "ShimChIdxOffset": ["uint32_hex", 0x8],
    }
    ctrl_index_dicts = {
        "CtrlIndex": CtrlIndex,
        "MTLockInfo": MTLockInfo,
        "ATLockInfo": ATLockInfo,
        "ATChannleInfo": ATChannleInfo,
        "MTChannleInfo": MTChannleInfo,
        "ShimChannleInfo": ShimChannleInfo,
    }


    def title_to_snake(s: str):
        s1 = ''
        i = 0
        upper_flag = False
        for i in range(len(s)):
            if s[i].isupper() and i + 1 < len(s) and i != 0 and s[i + 1].islower():
                s1 += '_'
            s1 += s[i].lower()
        return s1


    # print message
    first_lower = title_to_snake
    print("message MatrixValue {")
    c_id = 1
    for k, v_dict in ctrl_index_dicts.items():
        print("message %s {" % k)
        k_id = 1
        for c, v in v_dict.items():
            vtype = ""
            if v[0] == "uint32" or v[0] == "uint32_hex":
                vtype = "uint32"
            else:
                print("Error! cannot get vtype %s" % vtype)
            print("  %s %s = %d;" % (vtype, first_lower(c), k_id))
            k_id += 1
        print("}")
    for k in ctrl_index_dicts.keys():
        print("%s %s = %d;" % (k, first_lower(k), c_id))
        c_id += 1
    print("}")
    print("MatrixValue matrix_value = 31;")

    # print prototxt
    print("MatrixValue {")
    for k, v_dict in ctrl_index_dicts.items():
        print("  %s {" % k)
        for c, v in v_dict.items():
            v_str = str(v[1])
            if v[0] == "uint32_hex":
                v_str = hex(v[1])
            print("    %s : %s " % (first_lower(c), v_str))
        print("  }")
    print("}")

    # print struct
    print("struct MatrixValue {")
    for k, v_dict in ctrl_index_dicts.items():
        for c, v in v_dict.items():
            vtype = ""
            if v[0] == "uint32" or v[0] == "uint32_hex":
                vtype = "uint32_t"
            else:
                print("Error! cannot get vtype %s" % vtype)
            print("%s %s;" % (vtype, first_lower(c)))
    print("explicit MatrixValue(const vai::Target &target);")
    print("};")

    # print struct constructor
    print("MatrixValue::MatrixValue(const vai::Target &target) {")
    for k, v_dict in ctrl_index_dicts.items():
        for c, v in v_dict.items():
            v_str = str(v[1])
            print("  %s = target.matrix_value().%s().%s();" % (
                first_lower(c), first_lower(k), first_lower(c)))
    print("}")
