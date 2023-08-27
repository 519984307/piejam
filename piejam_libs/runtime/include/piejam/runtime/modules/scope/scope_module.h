// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/runtime/audio_stream.h>
#include <piejam/runtime/fwd.h>
#include <piejam/runtime/fx/fwd.h>

#include <piejam/audio/types.h>

namespace piejam::runtime::modules::scope
{

enum class mode : int
{
    free,
    trigger_a,
    trigger_b,
};

enum class trigger_slope : int
{
    rising_edge,
    falling_edge
};

enum class window_size : int
{
    very_small,
    small,
    middle,
    large,
    very_large
};

enum class stereo_channel : int
{
    left,
    right,
    middle,
    side,

    _min = left,
    _max = side,
};

enum class parameter_key : fx::parameter_key
{
    mode,
    trigger_slope,
    trigger_level,
    hold_time,
    waveform_window_size,
    scope_window_size,
    stream_a_active,
    stream_b_active,
    channel_a,
    channel_b
};

enum class stream_key : fx::stream_key
{
    input
};

auto
make_module(audio::bus_type, fx_parameter_factory const&, audio_streams_cache&)
        -> fx::module;

} // namespace piejam::runtime::modules::scope
