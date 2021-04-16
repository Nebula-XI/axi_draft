#pragma once

#include "units_info_base.h"

namespace InSys {

struct unit_info_axi_gpio final : unit_info_axi_base {
  using list_type = units_info_list<unit_info_axi_gpio>;
  unit_info_axi_gpio(const std::string_view &_name,
                     const std::string_view &_label, unit_info_uid _axi_offset)
      : unit_info_axi_base{_name, _label, _axi_offset, "axi-gpio"} {}
};

class unit_info_axi_gpio_parser
    : public units_info_base_parser<units_info_list, unit_info_axi_gpio> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    _info_list.emplace_back("GPIO", "PORT0", 0x00050000);
    _info_list.emplace_back("GPIO", "PORT1", 0x00060000);
    _info_list.emplace_back("GPIO", "PORT2", 0x00070000);
  }

 public:
  unit_info_axi_gpio_parser() = default;
  unit_info_axi_gpio_parser(const std::string_view &config) { parser(config); }
};

class unit_info_gpio_parser final : public unit_info_axi_gpio_parser {
 public:
  using axi_parser = unit_info_axi_gpio_parser;

  unit_info_gpio_parser(const std::string_view &config) { parser(config); }
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
    axi_parser::_info_list.emplace_back("GPIO", "PORT0", 0x00050000);
    axi_parser::_info_list.emplace_back("GPIO", "PORT1", 0x00060000);
    axi_parser::_info_list.emplace_back("GPIO", "PORT2", 0x00070000);
  }
};

}  // namespace InSys
