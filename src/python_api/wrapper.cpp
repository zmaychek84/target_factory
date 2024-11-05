/*
 * Copyright (C) 2022 Xilinx, Inc. All rights reserved.
 * Copyright (C) 2023 - 2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// python 3.7 deprecate PyCreateThread, but pybind11 2.2.3 still uses
// this function.
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "vitis/ai/target_factory.hpp"

namespace py = pybind11;

namespace vitis {
namespace ai {

PYBIND11_MODULE(target_factory, m) {
  
  // Target interface
  py::class_<Target>(m, "Target")
      .def("get_pdi_size", &Target::pdi_size)
      .def("get_target_name", &Target::name)
      .def("get_ipu_overlay_type", &Target::ipu_overlay_type)
      .def("get_target_type", &Target::type)
      .def("get_feature_code", &Target::feature_code)
      .def("get_isa_version", &Target::isa_version)
      .def("get_pdi_by_idx", py::overload_cast<int>(&Target::pdi, py::const_));

  py::class_<Target_PDI_partition>(m, "Target_PDI_partition")
      .def("get_pdi_name", &Target_PDI_partition::name)
      .def("get_pdi_id", &Target_PDI_partition::id)
      .def("get_kernels_size", &Target_PDI_partition::kernels_size)
      .def("get_stack_size", &Target_PDI_partition::stack_size)
      .def("get_rounding_mode", &Target_PDI_partition::rounding_mode)
      .def("get_kernel_by_idx", py::overload_cast<int>(&Target_PDI_partition::kernels, py::const_));

  // PDI kernel interface
  py::class_<Target_Kernel>(m, "Target_Kernel")
      .def("get_aw_type", &Target_Kernel::aw_type)
      .def("has_conv_kernel", &Target_Kernel::has_conv_kernel)
      .def("get_conv_kernel", py::overload_cast<>(&Target_Kernel::conv_kernel, py::const_))
      .def("has_eltwise_kernel", &Target_Kernel::has_eltwise_kernel)
      .def("get_qeltwise_kernel", py::overload_cast<>(&Target_Kernel::qeltwise_kernel, py::const_))
      .def("has_qeltwise_kernel", &Target_Kernel::has_qeltwise_kernel)
      .def("get_eltwise_kernel", py::overload_cast<>(&Target_Kernel::eltwise_kernel, py::const_))
      .def("has_nonlinear_kernel", &Target_Kernel::has_nonlinear_kernel)
      .def("get_nonlinear_kernel", py::overload_cast<>(&Target_Kernel::nonlinear_kernel, py::const_))
      .def("has_pool_kernel", &Target_Kernel::has_pool_kernel)
      .def("get_pool_kernel", py::overload_cast<>(&Target_Kernel::pool_kernel, py::const_))
      .def("has_norm_kernel", &Target_Kernel::has_norm_kernel)
      .def("get_norm_kernel", py::overload_cast<>(&Target_Kernel::norm_kernel, py::const_))
      .def("has_dwconv_kernel", &Target_Kernel::has_dwconv_kernel)
      .def("get_dwconv_kernel", py::overload_cast<>(&Target_Kernel::dwconv_kernel, py::const_))
      .def("has_l2_normalize_kernel", &Target_Kernel::has_l2_normalize_kernel)
      .def("get_l2_normalize_kernel", py::overload_cast<>(&Target_Kernel::l2_normalize_kernel, py::const_))
      .def("has_qlinear_sigmoid_kernel", &Target_Kernel::has_qlinear_sigmoid_kernel)
      .def("get_qlinear_sigmoid_kernel", py::overload_cast<>(&Target_Kernel::qlinear_sigmoid_kernel, py::const_))
      .def("has_qlinear_swish_kernel", &Target_Kernel::has_qlinear_swish_kernel)
      .def("get_qlinear_swish_kernel", py::overload_cast<>(&Target_Kernel::qlinear_swish_kernel, py::const_))
      .def("has_arg_kernel", &Target_Kernel::has_arg_kernel)
      .def("get_arg_kernel", py::overload_cast<>(&Target_Kernel::arg_kernel, py::const_))
      .def("has_reduce_kernel", &Target_Kernel::has_reduce_kernel)
      .def("get_reduce_kernel", py::overload_cast<>(&Target_Kernel::reduce_kernel, py::const_))
      .def("has_sigmoid_kernel", &Target_Kernel::has_sigmoid_kernel)
      .def("get_sigmoid_kernel", py::overload_cast<>(&Target_Kernel::sigmoid_kernel, py::const_))
      .def("has_qpow_kernel", &Target_Kernel::has_qpow_kernel)
      .def("get_qpow_kernel", py::overload_cast<>(&Target_Kernel::qpow_kernel, py::const_))
      .def("get_pm_size", &Target_Kernel::pm_size)
      .def("get_ops_size", &Target_Kernel::ops_size)
      .def("get_op_by_idx", py::overload_cast<int>(&Target_Kernel::ops, py::const_))
      .def("has_optimized_3x3_dwc", &Target_Kernel::has_optimized_3x3_dwc)
      .def("has_optimized_conv_zero_result", &Target_Kernel::has_optimized_conv_zero_result)
      .def("has_globalpool_kernel", &Target_Kernel::has_globalpool_kernel)
      .def("has_exp_kernel", &Target_Kernel::has_exp_kernel)
      .def("has_mac_kernel", &Target_Kernel::has_mac_kernel)
      .def("has_tanh_kernel", &Target_Kernel::has_tanh_kernel)
      .def("has_qlinear_sigmoid_kernel", &Target_Kernel::has_qlinear_sigmoid_kernel)
      .def("get_bf", &Target_Kernel::bf)
      .def("has_bf16_eltwise_kernel", &Target_Kernel::has_bf16_eltwise_kernel)
      .def("get_bf16_eltwise_kernel", py::overload_cast<>(&Target_Kernel::bf16_eltwise_kernel, py::const_))
      .def("has_bf16_reduce_kernel", &Target_Kernel::has_bf16_reduce_kernel)
      .def("get_bf16_reduce_kernel", py::overload_cast<>(&Target_Kernel::bf16_reduce_kernel, py::const_));

  // Conv kernel interface
  py::class_<Target_Conv>(m, "Target_Conv")
      .def("has_kernel_nonlinear", &Target_Conv::has_nonlinear)
      .def("get_kernel_nonlinear", py::overload_cast<>(&Target_Conv::nonlinear, py::const_))
      .def("get_small_channel", py::overload_cast<>(&Target_Conv::small_channel, py::const_))
      .def("get_ntconv", py::overload_cast<>(&Target_Conv::ntconv, py::const_))
      .def("is_three_term", py::overload_cast<>(&Target_Conv::is_three_term, py::const_))
      .def("has_conv_limit", &Target_Conv::has_conv_limit)
      .def("get_conv_limit", py::overload_cast<>(&Target_Conv::conv_limit, py::const_));

  // // Dwconv kernel interface
  // py::class_<Target_Dwconv>(m, "Target_Dwconv")
  //     .def("has_kernel_nonlinear", &Target_Dwconv::has_nonlinear)
  //     .def("get_kernel_nonlinear", py::overload_cast<>(&Target_Dwconv::nonlinear, py::const_));

  // Dwconv kernel interface
  py::class_<Target_Dwconv>(m, "Target_Dwconv")
      .def("has_dwconv_limit", &Target_Dwconv::has_dwconv_limit)
      .def("get_dwconv_limit", py::overload_cast<>(&Target_Dwconv::dwconv_limit, py::const_))
      .def("has_kernel_nonlinear", &Target_Dwconv::has_nonlinear)
      .def("get_kernel_nonlinear", py::overload_cast<>(&Target_Dwconv::nonlinear, py::const_));
      
  // Norm kernel interface
  py::class_<Target_Norm>(m, "Target_Norm")
      .def("has_kernel_nonlinear", &Target_Norm::has_nonlinear)
      .def("get_kernel_nonlinear", py::overload_cast<>(&Target_Norm::nonlinear, py::const_));

  // Conv limit interface
  py::class_<Target_Conv_ConvLimit>(m, "Target_Conv_ConvLimit")
      .def("get_kernel_size", py::overload_cast<>(&Target_Conv_ConvLimit::kernel_size, py::const_))
      .def("get_stride", py::overload_cast<>(&Target_Conv_ConvLimit::stride, py::const_));

  // Dwconv limit interface
  py::class_<Target_Dwconv_DwconvLimit>(m, "Target_Dwconv_DwconvLimit")
      .def("get_dilation", py::overload_cast<>(&Target_Dwconv_DwconvLimit::dilation, py::const_));

  // nonlinear interface
  py::class_<Target_Nonlinear>(m, "Target_Nonlinear")
      .def("get_nonlinear_type_size", &Target_Nonlinear::nonlinear_type_size)
      .def("get_nonlinear_enum_by_idx", py::overload_cast<int>(&Target_Nonlinear::nonlinear_type, py::const_));
  
  // nonlinear_enum to str
  py::enum_<Target_Nonlinear_NonlinearType>(m, "Nonlinear")
      .value("relu", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_relu)
      .value("prelu", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_prelu)
      .value("leaky_relu", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_leaky_relu)
      .value("relu_six", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_relu_six)
      .value("hsigmoid", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_hsigmoid)
      .value("qsilu", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_qsilu)
      .value("hswish", Target_Nonlinear_NonlinearType::Target_Nonlinear_NonlinearType_hswish);
  
  // QEltwise kernel interface
  py::class_<Target_QEltwise>(m, "Target_QEltwise")
      .def("get_qeltwise_type_size", &Target_QEltwise::qelew_type_size)
      .def("debug_string", &Target_QEltwise::DebugString)
      .def("is_broadcast", py::overload_cast<>(&Target_QEltwise::is_broadcast, py::const_))
      .def("qeltwise_enum_is_valid", py::overload_cast<int>(&Target_QEltwise::QElewType_IsValid))
      .def("get_qeltwise_enum_by_idx", py::overload_cast<int>(&Target_QEltwise::qelew_type, py::const_));

  // Eltwise kernel interface
  py::class_<Target_Eltwise>(m, "Target_Eltwise")
      .def("get_eltwise_type_size", &Target_Eltwise::elew_type_size)
      .def("debug_string", &Target_Eltwise::DebugString)
      .def("is_broadcast", py::overload_cast<>(&Target_Eltwise::is_broadcast, py::const_))
      .def("eltwise_enum_is_valid", py::overload_cast<int>(&Target_Eltwise::ElewType_IsValid))
      .def("eltwise_add_act_disable", py::overload_cast<>(&Target_Eltwise::elew_add_act_disable, py::const_))
      .def("get_eltwise_enum_by_idx", py::overload_cast<int>(&Target_Eltwise::elew_type, py::const_));

  // Bf16_eltwise kernel interface
  py::class_<Target_Bf16_Eltwise>(m, "Target_Bf16_Eltwise")
      .def("get_eltwise_type_size", &Target_Bf16_Eltwise::elew_type_size)
      .def("debug_string", &Target_Bf16_Eltwise::DebugString)
      .def("is_broadcast", py::overload_cast<>(&Target_Bf16_Eltwise::is_broadcast, py::const_))
      .def("eltwise_enum_is_valid", py::overload_cast<int>(&Target_Bf16_Eltwise::ElewType_IsValid))
      .def("eltwise_add_act_disable", py::overload_cast<>(&Target_Bf16_Eltwise::elew_add_act_disable, py::const_))
      .def("get_eltwise_enum_by_idx", py::overload_cast<int>(&Target_Bf16_Eltwise::elew_type, py::const_));

  // qelew to str
  py::enum_<Target_QEltwise_QElewType>(m, "QEltwise")
      .value("add", Target_QEltwise_QElewType::Target_QEltwise_QElewType_add)
      .value("requantize", Target_QEltwise_QElewType::Target_QEltwise_QElewType_requantize)
      .value("relu", Target_QEltwise_QElewType::Target_QEltwise_QElewType_relu)
      .value("tanh", Target_QEltwise_QElewType::Target_QEltwise_QElewType_tanh)
      .value("sub", Target_QEltwise_QElewType::Target_QEltwise_QElewType_sub)
      .value("div", Target_QEltwise_QElewType::Target_QEltwise_QElewType_div)
      .value("clamp", Target_QEltwise_QElewType::Target_QEltwise_QElewType_clamp)
      .value("leaky_relu", Target_QEltwise_QElewType::Target_QEltwise_QElewType_leaky_relu)
      .value("mult", Target_QEltwise_QElewType::Target_QEltwise_QElewType_mult);

  // elew to str
  py::enum_<Target_Eltwise_ElewType>(m, "Eltwise")
      .value("add", Target_Eltwise_ElewType::Target_Eltwise_ElewType_add)
      .value("mult", Target_Eltwise_ElewType::Target_Eltwise_ElewType_mult)
      .value("hsigmoid", Target_Eltwise_ElewType::Target_Eltwise_ElewType_hsigmoid)
      .value("hswish", Target_Eltwise_ElewType::Target_Eltwise_ElewType_hswish)
      .value("sigmoid", Target_Eltwise_ElewType::Target_Eltwise_ElewType_sigmoid)
      .value("qlinear_sigmoid", Target_Eltwise_ElewType::Target_Eltwise_ElewType_qlinear_sigmoid)
      .value("pow", Target_Eltwise_ElewType::Target_Eltwise_ElewType_pow)
      .value("l2norm", Target_Eltwise_ElewType::Target_Eltwise_ElewType_l2norm)
      .value("qlinear_swish", Target_Eltwise_ElewType::Target_Eltwise_ElewType_qlinear_swish)
      .value("gelu", Target_Eltwise_ElewType::Target_Eltwise_ElewType_gelu)
      .value("sub", Target_Eltwise_ElewType::Target_Eltwise_ElewType_sub)
      .value("max", Target_Eltwise_ElewType::Target_Eltwise_ElewType_max)
      .value("min", Target_Eltwise_ElewType::Target_Eltwise_ElewType_min)
      .value("div", Target_Eltwise_ElewType::Target_Eltwise_ElewType_div)
      .value("equal", Target_Eltwise_ElewType::Target_Eltwise_ElewType_equal)
      .value("greater", Target_Eltwise_ElewType::Target_Eltwise_ElewType_greater)
      .value("greater_equal", Target_Eltwise_ElewType::Target_Eltwise_ElewType_greater_equal)
      .value("less", Target_Eltwise_ElewType::Target_Eltwise_ElewType_less)
      .value("less_equal", Target_Eltwise_ElewType::Target_Eltwise_ElewType_less_equal)
      .value("not_op", Target_Eltwise_ElewType::Target_Eltwise_ElewType_not_op)
      .value("neg", Target_Eltwise_ElewType::Target_Eltwise_ElewType_neg)
      .value("abs", Target_Eltwise_ElewType::Target_Eltwise_ElewType_abs)
      .value("exp", Target_Eltwise_ElewType::Target_Eltwise_ElewType_exp)
      .value("elu", Target_Eltwise_ElewType::Target_Eltwise_ElewType_elu)
      .value("sqrt", Target_Eltwise_ElewType::Target_Eltwise_ElewType_sqrt)
      .value("and_op", Target_Eltwise_ElewType::Target_Eltwise_ElewType_and_op)
      .value("or_op", Target_Eltwise_ElewType::Target_Eltwise_ElewType_or_op)
      .value("clamp", Target_Eltwise_ElewType::Target_Eltwise_ElewType_clamp)
      .value("relu", Target_Eltwise_ElewType::Target_Eltwise_ElewType_relu)
      .value("prelu", Target_Eltwise_ElewType::Target_Eltwise_ElewType_prelu)
      .value("leaky_relu", Target_Eltwise_ElewType::Target_Eltwise_ElewType_leaky_relu)
      .value("relu_six", Target_Eltwise_ElewType::Target_Eltwise_ElewType_relu_six);

  // Bf16_elew to str
  py::enum_<Target_Bf16_Eltwise_ElewType>(m, "Bf16_Eltwise")
      .value("add", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_add)
      .value("mult", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_mult)
      .value("hsigmoid", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_hsigmoid)
      .value("hswish", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_hswish)
      .value("sigmoid", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_sigmoid)
      .value("qlinear_sigmoid", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_qlinear_sigmoid)
      .value("pow", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_pow)
      .value("l2norm", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_l2norm)
      .value("qlinear_swish", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_qlinear_swish)
      .value("gelu", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_gelu)
      .value("sub", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_sub)
      .value("max", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_max)
      .value("min", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_min)
      .value("div", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_div)
      .value("equal", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_equal)
      .value("greater", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_greater)
      .value("greater_equal", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_greater_equal)
      .value("less", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_less)
      .value("less_equal", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_less_equal)
      .value("not_op", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_not_op)
      .value("neg", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_neg)
      .value("abs", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_abs)
      .value("exp", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_exp)
      .value("elu", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_elu)
      .value("sqrt", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_sqrt)
      .value("and_op", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_and_op)
      .value("or_op", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_or_op)
      .value("clamp", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_clamp)
      .value("relu", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_relu)
      .value("prelu", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_prelu)
      .value("leaky_relu", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_leaky_relu)
      .value("relu_six", Target_Bf16_Eltwise_ElewType::Target_Bf16_Eltwise_ElewType_relu_six);

  // Pool kernel interface
  py::class_<Target_Pool>(m, "Target_Pool")
      .def("get_pool_type_size", &Target_Pool::pool_type_size)
      .def("get_pool_enum_by_idx", py::overload_cast<int>(&Target_Pool::pool_type, py::const_));

  // Pool enum to str
  py::enum_<Target_Pool_PoolType>(m, "Pool")
      .value("max", Target_Pool_PoolType_max)
      .value("avg", Target_Pool_PoolType_avg);

  // Arg kernel interface
  py::class_<Target_Arg>(m, "Target_Arg")
      .def("get_arg_type_size", &Target_Arg::arg_type_size)
      .def("get_arg_enum_by_idx", py::overload_cast<int>(&Target_Arg::arg_type, py::const_));

  // Arg enum to str
  py::enum_<Target_Arg_argType>(m, "Arg")
      .value("max", Target_Arg_argType_max)
      .value("min", Target_Arg_argType_min);

  // Reduce kernel interface
  py::class_<Target_Reduce>(m, "Target_Reduce")
      .def("get_reduce_type_size", &Target_Reduce::reduce_type_size)
      .def("get_reduce_enum_by_idx", py::overload_cast<int>(&Target_Reduce::reduce_type, py::const_));

  // Bf16_reduce kernel interface
  py::class_<Target_Bf16_Reduce>(m, "Target_Bf16_Reduce")
      .def("get_bf16_reduce_kernel_type_size", &Target_Bf16_Reduce::reduce_type_size)
      .def("get_bf16_reduce_kernel_enum_by_idx", py::overload_cast<int>(&Target_Bf16_Reduce::reduce_type, py::const_));

  // Reduce enum to str
  py::enum_<Target_Reduce_reduceType>(m, "Reduce")
      .value("mean", Target_Reduce_reduceType_mean)
      .value("sum", Target_Reduce_reduceType_sum)
      .value("max", Target_Reduce_reduceType_max)
      .value("min", Target_Reduce_reduceType_min);

  // Bf16_reduce enum to str
  py::enum_<Target_Bf16_Reduce_reduceType>(m, "Bf16_Reduce")
      .value("mean", Target_Bf16_Reduce_reduceType_mean)
      .value("sum", Target_Bf16_Reduce_reduceType_sum)
      .value("max", Target_Bf16_Reduce_reduceType_max)
      .value("min", Target_Bf16_Reduce_reduceType_min);

  m.def("get_target_by_name",
        [](const std::string& name) { return target_factory()->create(name); });
  m.def("get_target_by_fingerprint", [](const std::uint64_t fingerprint) {
    return target_factory()->create(fingerprint);
  });
  m.def("dump_proto_txt", [](const Target& target, const std::string& file) {
    return target_factory()->dump(target, file);
  });
  m.def("is_registered_target", [](const std::string& name) {
    return target_factory()->is_registered_target(name);
  });
  m.def("get_target_str_by_name",
         [](const std::string& name) { 
         return target_factory()->create(name).DebugString();});
  m.def("list_internal_target_names",
         []() { 
         return target_factory()->get_registered_target_names();});
  m.def("get_fingerprint",
         [](const std::string name) { 
         return target_factory()->get_fingerprint(name);});
}


}  // namespace ai
}  // namespace vitis
