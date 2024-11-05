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
import subprocess
import os
import argparse
import google.protobuf.text_format as text_format

def check_pdi(proto_path, pb2_path, check_unref_kernel):
    if pb2_path != "":
        target_pb2_path = pb2_path
        pb_env_path = os.path.dirname(target_pb2_path)
    else:
        lsb_output = subprocess.check_output(['lsb_release', '-a']).decode('utf-8').splitlines()
        os_id = next((line.split(':', 1)[1].strip() for line in lsb_output if line.startswith('Distributor ID')), None)
        os_version = next((line.split(':', 1)[1].strip() for line in lsb_output if line.startswith('Release')), None)
        arch = subprocess.check_output(['uname', '-p']).decode('utf-8').strip()
        build_type = 'Debug'
        target_info = f"{os_id}.{os_version}.{arch}.{build_type}"
        vitis_ai_suffix = "include/vitis/ai"
        install_prefix_default = os.path.join(os.path.expanduser('~'), '.local', target_info, vitis_ai_suffix)
        target_pb2_path = os.path.join(install_prefix_default, "target_pb2.py")
        pb_env_path = install_prefix_default
    
    if not os.path.exists(target_pb2_path):
        print("target_pb2 not found", target_pb2_path)
    if not os.path.exists(proto_path):
        print("prototxt not found", proto_path)
    else:
        print("analysis the {}".format(proto_path))

    import sys
    sys.path.append(pb_env_path)
    import target_pb2

    def cmp_kernel(k1, k2, check=True):
        if check:
            assert isinstance(k1, target_pb2.Target.Kernel), f"k1 is not an instance of target_pb2.Kernel: {type(k1)}, {k1}"
            assert isinstance(k2, target_pb2.Target.Kernel), f"k1 is not an instance of target_pb2.Kernel: {type(k1)}, {k2}"
        all_match = True
        k1_attrs = k1.DESCRIPTOR.fields_by_name.keys()
        k2_attrs = k2.DESCRIPTOR.fields_by_name.keys()
        assert set(k1_attrs) == set(k2_attrs), "Attributes of k1 and k2 do not match"
        skip_attrs_list = ["has_optimized_3x3_dwc", "has_optimized_conv_zero_result", "elew_add_act_disable"]
        for attr_name in k1_attrs:
            if attr_name in skip_attrs_list:
                continue
            elif "kernel" in attr_name:
                all_match = all_match and cmp_kernel(getattr(k1, attr_name), getattr(k2, attr_name), False)
            else:
                if getattr(k1, attr_name) != getattr(k2, attr_name):
                    all_match = False
                else:
                    pass
        return all_match

    target_message = target_pb2.Target()

    with open(proto_path, 'r') as f:
        tgt = text_format.Parse(f.read(), target_message)
    kernels = tgt.kernels
    pdi_partitions = tgt.PDI
    all_found = True
    for pdi_i in pdi_partitions:
        print("analysis {}".format(pdi_i.name))
        for pdi_kernel in pdi_i.kernels:
            if any(cmp_kernel(kernel, pdi_kernel) for kernel in kernels):
                pass
            else:
                print("{} not found in kernels".format(pdi_kernel))
                all_found = False
    if all_found:
       print("all PDI partition kernels match successfully.") 

    if check_unref_kernel:
        print("*" * 50)
        print("CHECK UNREFERENCED KERNELS")
        print("*" * 50)
        kernel_found = {key: [] for key in range(len(kernels))}
        for i, kernel in enumerate(kernels):
            for pdi_i in pdi_partitions:
                if (any(cmp_kernel(kernel, pdi_kernel) for pdi_kernel in pdi_i.kernels)):
                    kernel_found[i].append(pdi_i.name)
        for i in kernel_found:
            if len(kernel_found[i]) == 0:
                print("kernel {} doesn't belongs to no PDI partition\n {}".format(i, kernels[i]))

if  __name__ ==  "__main__":
    parser = argparse.ArgumentParser(description='Check the PDI Partition tool')
    parser.add_argument("--proto_path", help="the path of prototxt", type=str, required=True)
    parser.add_argument('--pb2_path', help="target_pb2 path", default="", required=False)
    parser.add_argument('--check_unref_kernel', help="check whether there is unreferenced kernel in prototxt", default=False, required=False)
    args = parser.parse_args()
    check_pdi(proto_path=args.proto_path, pb2_path=args.pb2_path, check_unref_kernel=args.check_unref_kernel)