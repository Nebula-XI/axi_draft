#pragma once

#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <cstdint>
#include <cstdlib>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "utility.h"

namespace InSys {

using info_uid = std::size_t;
class make_info_uid final {
  std::string m_str{};
  info_uid operator()() { return std::hash<std::string>{}(m_str); }

 public:
  template <typename arg_type = std::string, typename... args_type>
  info_uid operator()(arg_type &&str, args_type... other_str) {
    m_str += std::forward<arg_type>(str);
    return operator()(other_str...);
  }
};

class info_base {
  info_uid m_uid{};
  std::string m_name{};
  std::string m_label{};
  info_uid m_parent_uid{};
  std::string m_unit{};

 public:
  info_base() = delete;
  info_base(const std::string_view &unit)
      : m_uid{}, m_name{}, m_label{}, m_parent_uid{}, m_unit{unit} {}
  info_base(const std::string_view &name, const std::string_view &label,
            info_uid parent_uid, const std::string_view &unit)
      : m_uid{make_info_uid{}(name, label)},
        m_name{name},
        m_label{label},
        m_parent_uid{parent_uid},
        m_unit{unit} {}
  info_base(const info_base &other) = default;
  info_base(info_base &&) noexcept = default;
  info_base &operator=(const info_base &) = default;
  info_base &operator=(info_base &&) noexcept = default;
  virtual ~info_base() noexcept = default;
  auto uid() const noexcept { return m_uid; }
  auto &name() const noexcept { return m_name; }
  auto &label() const noexcept { return m_label; }
  auto parent_uid() const noexcept { return m_parent_uid; }
  auto &unit() const noexcept { return m_unit; }
};

class info_axi_base : public info_base {
  uint64_t m_axi_offset{};

 public:
  info_axi_base(const std::string_view &unit)
      : info_base{unit}, m_axi_offset{} {}
  info_axi_base(const std::string_view &name, const std::string_view &label,
                uint64_t axi_offset, const std::string_view &unit)
      : info_base{name, label, {}, unit}, m_axi_offset{axi_offset} {}
  auto axi_offset() const noexcept { return m_axi_offset; }
};

template <typename info_type>
using info_list = std::vector<info_type>;

template <template <typename> typename info_list, typename info_type>
class info_base_parser {
 public:
  using list_type = info_list<info_type>;
  using value_type = info_type;
  info_base_parser() = default;
  virtual ~info_base_parser() noexcept = default;
  list_type get_info() const { return m_info_list; }
  list_type find_by_name(const std::string_view &name) const {
    list_type ret_info_list{};
    for (const auto &info : m_info_list) {
      if (info.name() == name) {
        ret_info_list.push_back(info);
      }
    }
    return ret_info_list;
  }
  std::optional<value_type> get_by_label(const std::string_view &label) const {
    std::optional<value_type> result{};
    for (const auto &info : m_info_list) {
      if (info.label() == label) {
        result.emplace(info);
      }
    }
    return result;
  }
  std::optional<value_type> get_by_uid(info_uid uid) const {
    std::optional<value_type> result{};
    for (const auto &info : m_info_list) {
      if (info.uid() == uid) {
        result.emplace(info);
      }
    }
    return result;
  }

 protected:
  list_type m_info_list{};

  virtual void parser(const std::string_view &config) = 0;
  auto get_units_tree(const std::string_view &config) {
    namespace pt = boost::property_tree;
    pt::ptree units_tree{};
    std::stringstream config_sstream{};
    config_sstream << config;
    pt::read_json(config_sstream, units_tree);
    return units_tree.get_child("units");
  }
};

}  // namespace InSys
