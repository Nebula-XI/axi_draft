#pragma once

#include "units_info_base.h"

namespace InSys {

class unit_info_axi_sdram final : public unit_info_axi_base {
  inline static constexpr auto k_unit{"axi-sdram"};

 public:
  using list_type = units_info_list<unit_info_axi_sdram>;
  unit_info_axi_sdram() : unit_info_axi_base{k_unit} {}
  unit_info_axi_sdram(const std::string_view &name,
                      const std::string_view &label, uint64_t axi_offset)
      : unit_info_axi_base{name, label, axi_offset, k_unit} {}
};

class unit_info_sdram_dev_base : public unit_info_base {
  unit_info_sdram_dev_base(const std::string_view &unit)
      : unit_info_base{unit} {}
  unit_info_sdram_dev_base(const std::string_view &name,
                           const std::string_view &label,
                           unit_info_uid parent_uid,
                           const std::string_view &unit)
      : unit_info_base{name, label, parent_uid, unit} {}
  // TODO: add new parameters
};

class unit_info_axi_sdram_parser
    : public units_info_base_parser<units_info_list, unit_info_axi_sdram> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    m_info_list.emplace_back("SDRAM", "DDR0", 0x00080000);
    m_info_list.emplace_back("SDRAM", "DDR1", 0x00090000);
  }

 public:
  unit_info_axi_sdram_parser() = default;
  unit_info_axi_sdram_parser(const std::string_view &config) { parser(config); }
};

class unit_info_sdram_parser final : public unit_info_axi_sdram_parser {
 public:
  using axi_parser = unit_info_axi_sdram_parser;

  unit_info_sdram_parser(const std::string_view &config) { parser(config); }
  template <typename parser>
  typename parser::list_type get_info() const {
    return parser::get_info();
  }
  template <typename parser>
  typename parser::list_type find_by_name(const std::string_view &name) const {
    return parser::find_by_name(name);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_label(
      const std::string_view &label) const {
    return parser::get_by_label(label);
  }
  template <typename parser>
  std::optional<typename parser::value_type> get_by_uid(
      unit_info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    // TODO: add configuration parser
    axi_parser::m_info_list.emplace_back("SDRAM", "DDR0", 0x00080000);
    axi_parser::m_info_list.emplace_back("SDRAM", "DDR1", 0x00090000);
  }
};

}  // namespace InSys
