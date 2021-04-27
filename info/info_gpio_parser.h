#pragma once

#include "info_gpio.h"

namespace InSys {

class info_axi_gpio_parser : public info_base_parser<info_list, info_axi_gpio> {
  void parser(const std::string_view &config) override {
    // TODO: add configuration parser
    /*
    m_info_list.emplace_back("GPIO", "PORT0", 0x00050000);
    m_info_list.emplace_back("GPIO", "PORT1", 0x00060000);
    m_info_list.emplace_back("GPIO", "PORT2", 0x00070000);
    */
  }

 public:
  info_axi_gpio_parser() = default;
  info_axi_gpio_parser(const std::string_view &config) { parser(config); }
};

class info_gpio_parser final : public info_axi_gpio_parser {
 public:
  using axi_parser = info_axi_gpio_parser;
  info_gpio_parser(const std::string_view &config) { parser(config); }
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
  std::optional<typename parser::value_type> get_by_uid(info_uid uid) const {
    return parser::get_by_uid(uid);
  }

 private:
  void parser(const std::string_view &config) final {
    // TODO: add configuration parser
    /*
    axi_parser::m_info_list.emplace_back("GPIO", "PORT0", 0x00050000);
    axi_parser::m_info_list.emplace_back("GPIO", "PORT1", 0x00060000);
    axi_parser::m_info_list.emplace_back("GPIO", "PORT2", 0x00070000);
    */
  }
};

}  // namespace InSys
