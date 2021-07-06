#pragma once

#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
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
    std::string m_str {};
    info_uid operator()() { return std::hash<std::string> {}(m_str); }

public:
    template <typename arg_type = std::string, typename... args_type>
    info_uid operator()(arg_type&& str, args_type... other_str)
    {
        m_str += std::forward<arg_type>(str);
        return operator()(other_str...);
    }
};

class info_base {
    info_uid m_uid {};
    std::string m_name {};
    std::string m_label {};
    info_uid m_parent_uid {};
    std::string m_unit {};

public:
    info_base() = delete;
    info_base(const std::string_view& unit)
        : m_uid {}
        , m_name {}
        , m_label {}
        , m_parent_uid {}
        , m_unit { unit }
    {
    }
    info_base(const std::string_view& name, const std::string_view& label,
        info_uid parent_uid, const std::string_view& unit)
        : m_uid { make_info_uid {}(name, label) }
        , m_name { name }
        , m_label { label }
        , m_parent_uid { parent_uid }
        , m_unit { unit }
    {
    }
    info_base(const info_base& other) = default;
    info_base(info_base&&) noexcept = default;
    info_base& operator=(const info_base&) = default;
    info_base& operator=(info_base&&) noexcept = default;
    virtual ~info_base() noexcept = default;
    auto uid() const noexcept { return m_uid; }
    auto& name() const noexcept { return m_name; }
    auto& label() const noexcept { return m_label; }
    auto parent_uid() const noexcept { return m_parent_uid; }
    auto& unit() const noexcept { return m_unit; }
};

class info_axi_base : public info_base {
    uint64_t m_axi_offset {};

public:
    info_axi_base(const std::string_view& unit)
        : info_base { unit }
        , m_axi_offset {}
    {
    }
    info_axi_base(const std::string_view& name, const std::string_view& label,
        uint64_t axi_offset, const std::string_view& unit)
        : info_base { name, label, {}, unit }
        , m_axi_offset { axi_offset }
    {
    }
    auto axi_offset() const noexcept { return m_axi_offset; }
};

template <typename info_type>
using info_list = std::vector<info_type>;

class units_tree {
    static constexpr auto units { "units" };
    static constexpr auto name { "name" };
    static constexpr auto label { "label" };
    static constexpr auto offset { "offset" };
    using tree_type = boost::property_tree::ptree;

public:
    units_tree(const std::string_view& config)
    {
        std::stringstream units_config {};
        units_config << config;
        boost::property_tree::read_json(units_config, m_units_tree);
    }
    template <typename arg_type = tree_type>
    units_tree(arg_type&& units_tree)
        : m_units_tree { std::forward<arg_type>(units_tree) }
    {
    }
    auto get_units_optional() const
    {
        return m_units_tree.get_child_optional(units);
    }
    auto get_units() const { return m_units_tree.get_child(units); }
    auto get_name() const { return m_units_tree.get<std::string>(name); }
    auto get_label() const { return m_units_tree.get<std::string>(label); }
    auto get_offset() const
    {
        return std::strtol(m_units_tree.get<std::string>(offset).c_str(), nullptr,
            16);
    }
    auto begin() { return m_units_tree.begin(); }
    auto end() { return m_units_tree.end(); }
    auto begin() const { return m_units_tree.begin(); }
    auto end() const { return m_units_tree.end(); }
    auto rbegin() { return m_units_tree.rbegin(); }
    auto rend() { return m_units_tree.rend(); }
    auto rbegin() const { return m_units_tree.rbegin(); }
    auto rend() const { return m_units_tree.rend(); }

protected:
    tree_type m_units_tree {};
};

template <template <typename> typename info_list, typename info_type>
class info_base_parser {
public:
    using list_type = info_list<info_type>;
    using value_type = info_type;
    info_base_parser() = default;
    virtual ~info_base_parser() noexcept = default;
    list_type get_info() const { return m_info_list; }
    list_type find_by_name(const std::string_view& name) const
    {
        list_type ret_info_list {};
        for (const auto& info : m_info_list) {
            if (info.name() == name) {
                ret_info_list.push_back(info);
            }
        }
        return ret_info_list;
    }
    std::optional<value_type> get_by_label(const std::string_view& label) const
    {
        std::optional<value_type> result {};
        for (const auto& info : m_info_list) {
            if (info.label() == label) {
                result.emplace(info);
            }
        }
        return result;
    }
    std::optional<value_type> get_by_uid(info_uid uid) const
    {
        std::optional<value_type> result {};
        for (const auto& info : m_info_list) {
            if (info.uid() == uid) {
                result.emplace(info);
            }
        }
        return result;
    }

protected:
    virtual void parser(const std::string_view&) = 0;
    virtual void parser(const units_tree&) = 0;

    list_type m_info_list {};
};

} // namespace InSys
