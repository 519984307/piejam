// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/runtime/modules/filter/filter_module.h>

#include <piejam/audio/multichannel_buffer.h>
#include <piejam/entity_map.h>
#include <piejam/runtime/fx/internal.h>
#include <piejam/runtime/fx/module.h>
#include <piejam/runtime/parameter/float_.h>
#include <piejam/runtime/parameter/float_normalize.h>
#include <piejam/runtime/parameter/int_.h>
#include <piejam/runtime/parameter_factory.h>
#include <piejam/runtime/ui_parameter_descriptors_map.h>
#include <piejam/to_underlying.h>

#include <fmt/format.h>

#include <boost/container/flat_map.hpp>

namespace piejam::runtime::modules::filter
{

namespace
{

auto
to_type_string(int const n) -> std::string
{
    switch (n)
    {
        case to_underlying(type::lp2):
            return "LP2";
        case to_underlying(type::lp4):
            return "LP4";
        case to_underlying(type::bp2):
            return "BP2";
        case to_underlying(type::bp4):
            return "BP4";
        case to_underlying(type::hp2):
            return "HP2";
        case to_underlying(type::hp4):
            return "HP4";
        case to_underlying(type::br):
            return "BR";

        default:
            return "Pass";
    }
}

auto
to_cutoff_string(float const f)
{
    return f < 1000.f ? fmt::format("{:.2f} Hz", f)
                      : fmt::format("{:.2f} kHz", f / 1000.f);
}

auto
to_resonance_string(float const r)
{
    auto p = r * 100;
    return p < 10.f ? fmt::format("{:.2f}%", p)
                    : (p < 100.f ? fmt::format("{:.1f}%", p)
                                 : fmt::format("{:.0f}%", p));
}

} // namespace

auto
make_module(
        audio::bus_type const bus_type,
        parameters_map& params,
        ui_parameter_descriptors_map& ui_params,
        audio_streams_cache& streams) -> fx::module
{
    using namespace std::string_literals;

    parameter_factory ui_params_factory{params, ui_params};

    return fx::module{
            .fx_instance_id = fx::internal::filter,
            .name = "Filter"s,
            .bus_type = bus_type,
            .parameters =
                    fx::module_parameters{
                            {to_underlying(parameter_key::type),
                             ui_params_factory.make_parameter(
                                     int_parameter{
                                             .default_value =
                                                     to_underlying(type::lp2),
                                             .min = 0,
                                             .max = 7},
                                     {.name = "Type",
                                      .value_to_string = &to_type_string})},
                            {to_underlying(parameter_key::cutoff),
                             ui_params_factory.make_parameter(
                                     float_parameter{
                                             .default_value = 440.f,
                                             .min = 10.f,
                                             .max = 20000.f,
                                             .to_normalized =
                                                     &runtime::parameter::
                                                             to_normalized_log,
                                             .from_normalized =
                                                     &runtime::parameter::
                                                             from_normalized_log},
                                     {.name = "Cutoff"s,
                                      .value_to_string = &to_cutoff_string})},
                            {to_underlying(parameter_key::resonance),
                             ui_params_factory.make_parameter(
                                     float_parameter{
                                             .default_value = 0.f,
                                             .min = 0.f,
                                             .max = 1.f,
                                             .to_normalized =
                                                     &runtime::parameter::
                                                             to_normalized_linear,
                                             .from_normalized =
                                                     &runtime::parameter::
                                                             from_normalized_linear},
                                     {.name = "Resonance"s,
                                      .value_to_string =
                                              &to_resonance_string})}},
            .streams = fx::module_streams{
                    {to_underlying(stream_key::in_out),
                     streams.add(audio_stream_buffer(
                             std::in_place,
                             num_channels(bus_type) * 2))}}};
}

} // namespace piejam::runtime::modules::filter
