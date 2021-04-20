#pragma once

#include "info_base.h"

namespace InSys {

class info_axi_sdram final : public info_axi_base {
  inline static constexpr auto k_unit{"axi-sdram"};

 public:
  using list_type = info_list<info_axi_sdram>;
  info_axi_sdram() : info_axi_base{k_unit} {}
  info_axi_sdram(const std::string_view &name, const std::string_view &label,
                 uint64_t axi_offset)
      : info_axi_base{name, label, axi_offset, k_unit} {}
};

class info_sdram_dev_base : public info_base {
  info_sdram_dev_base(const std::string_view &unit) : info_base{unit} {}
  info_sdram_dev_base(const std::string_view &name,
                      const std::string_view &label, info_uid parent_uid,
                      const std::string_view &unit)
      : info_base{name, label, parent_uid, unit} {}
  // TODO: add new parameters
};

}  // namespace InSys
