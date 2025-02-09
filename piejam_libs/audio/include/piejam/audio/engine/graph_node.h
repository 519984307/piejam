// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2023  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/audio/engine/component.h>
#include <piejam/audio/engine/graph_endpoint.h>
#include <piejam/audio/engine/processor.h>

#include <boost/assert.hpp>

#include <type_traits>

namespace piejam::audio::engine
{

template <class C>
concept graph_node = std::is_base_of_v<component, std::remove_cvref_t<C>> ||
                     std::is_base_of_v<processor, std::remove_cvref_t<C>>;

inline constexpr auto
src_endpoint(component const& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.num_outputs());
    return src.outputs()[port];
}

inline constexpr auto
src_endpoint(processor& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.num_outputs());
    return graph_endpoint{.proc = src, .port = port};
}

inline constexpr auto
dst_endpoint(component const& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.num_inputs());
    return src.inputs()[port];
}

inline constexpr auto
dst_endpoint(processor& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.num_inputs());
    return graph_endpoint{.proc = src, .port = port};
}

inline constexpr auto
src_event_endpoint(component const& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.event_outputs().size());
    return src.event_outputs()[port];
}

inline constexpr auto
src_event_endpoint(processor& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.event_outputs().size());
    return graph_endpoint{.proc = src, .port = port};
}

inline constexpr auto
dst_event_endpoint(component const& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.event_inputs().size());
    return src.event_inputs()[port];
}

inline constexpr auto
dst_event_endpoint(processor& src, std::size_t port) noexcept
{
    BOOST_ASSERT(port < src.event_inputs().size());
    return graph_endpoint{.proc = src, .port = port};
}

template <graph_node N>
inline constexpr auto
in_endpoit(N&& node, std::size_t port) noexcept
{
    return dst_endpoint(std::forward<N>(node), port);
}

template <graph_node N>
inline constexpr auto
out_endpoit(N&& node, std::size_t port) noexcept
{
    return src_endpoint(std::forward<N>(node), port);
}

template <graph_node N>
inline constexpr auto
in_event_endpoit(N&& node, std::size_t port) noexcept
{
    return dst_event_endpoint(std::forward<N>(node), port);
}

template <graph_node N>
inline constexpr auto
out_event_endpoit(N&& node, std::size_t port) noexcept
{
    return src_event_endpoint(std::forward<N>(node), port);
}

} // namespace piejam::audio::engine
