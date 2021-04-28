#pragma once

#include "info_i2c.h"

namespace InSys {

// FIXME: черновой вариант, нужен рефакторинг

class info_axi_i2c_parser : public info_base_parser<info_list, info_axi_i2c> {
 protected:
  void parser(const units_tree_type &units_tree) override {
    for (auto &units : units_tree) {
      auto name = units.second.get<std::string>("name");
      auto label = units.second.get<std::string>("label");
      auto offset = std::strtol(units.second.get<std::string>("offset").c_str(),
                                nullptr, 16);
      if (name == "i2c") {
        m_info_list.emplace_back(name, label, offset);
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(get_units_tree(config).get_child("units"));
  }

 public:
  info_axi_i2c_parser() = default;
  info_axi_i2c_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_mux_parser : public info_base_parser<info_list, info_i2c_mux> {
 protected:
  void parser(const units_tree_type &units_tree) override {
    for (auto &units : units_tree) {
      auto parent_name = units.second.get<std::string>("name");
      auto parent_label = units.second.get<std::string>("label");
      auto paren_offset = std::strtol(
          units.second.get<std::string>("offset").c_str(), nullptr, 16);
      if (parent_name == "i2c") {
        auto i2c_tree = units.second.get_child("units");
        for (auto &i2c : i2c_tree) {
          auto segments = i2c.second.get_optional<uint32_t>("segs");
          if (!segments) {
            continue;
          }
          auto name = i2c.second.get<std::string>("name");
          auto label = i2c.second.get<std::string>("label");
          auto addr = uint32_t(std::strtol(
              i2c.second.get<std::string>("addr").c_str(), nullptr, 16));
          auto freq = i2c.second.get<double>("freq");
          m_info_list.emplace_back(name, label, addr, freq, *segments,
                                   make_info_uid{}(parent_name, parent_label));
        }
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(get_units_tree(config).get_child("units"));
  }

 public:
  info_i2c_mux_parser() = default;
  info_i2c_mux_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_dev_parser : public info_base_parser<info_list, info_i2c_dev> {
 protected:
  void parser(const units_tree_type &units_tree) override {
    for (auto &units : units_tree) {
      auto parent_name = units.second.get<std::string>("name");
      auto parent_label = units.second.get<std::string>("label");
      auto paren_offset = std::strtol(
          units.second.get<std::string>("offset").c_str(), nullptr, 16);
      if (parent_name == "i2c") {
        auto i2c_tree = units.second.get_child("units");
        for (auto &i2c : i2c_tree) {
          auto name = i2c.second.get<std::string>("name");
          auto label = i2c.second.get<std::string>("label");
          auto addr = std::strtol(i2c.second.get<std::string>("addr").c_str(),
                                  nullptr, 16);
          auto freq = i2c.second.get<double>("freq");
          auto segments = i2c.second.get_optional<uint32_t>("segs");
          if (segments.has_value()) {
            auto i2c_subtree = i2c.second.get_child("units");
            for (auto &i2c_sub : i2c_subtree) {
              // FIXME: нужна рекурсивность
              auto child_name = i2c_sub.second.get<std::string>("name");
              auto child_label = i2c_sub.second.get<std::string>("label");
              auto child_addr = std::strtol(
                  i2c_sub.second.get<std::string>("addr").c_str(), nullptr, 16);
              auto child_freq = i2c_sub.second.get<double>("freq");
              auto segment = i2c_sub.second.get<uint32_t>("seg");
              m_info_list.emplace_back(
                  child_name, child_label, child_addr, child_freq,
                  make_info_uid{}(name, label, std::to_string(segment)));
            }
          } else {
            m_info_list.emplace_back(
                name, label, addr, freq,
                make_info_uid{}(parent_name, parent_label));
          }
        }
      }
    }
  }
  void parser(const std::string_view &config) override {
    parser(get_units_tree(config).get_child("units"));
  }

 public:
  info_i2c_dev_parser() = default;
  info_i2c_dev_parser(const std::string_view &config) { parser(config); }
};

class info_i2c_parser final : public info_axi_i2c_parser,
                              public info_i2c_dev_parser,
                              public info_i2c_mux_parser {
 public:
  using axi_parser = info_axi_i2c_parser;
  using dev_parser = info_i2c_dev_parser;
  using mux_parser = info_i2c_mux_parser;

  info_i2c_parser(const std::string_view &config) { parser(config); }
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
    auto units_tree = get_units_tree(config).get_child("units");
    axi_parser::parser(units_tree);
    mux_parser::parser(units_tree);
    dev_parser::parser(units_tree);
  }
};

}  // namespace InSys
