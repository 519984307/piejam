// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/runtime/modules/spectrum/spectrum_module.h>

#include <piejam/audio/multichannel_buffer.h>
#include <piejam/entity_map.h>
#include <piejam/runtime/fx/internal.h>
#include <piejam/runtime/fx/module.h>
#include <piejam/runtime/parameter/float_.h>
#include <piejam/runtime/parameter/float_normalize.h>
#include <piejam/runtime/parameter/generic_value.h>
#include <piejam/runtime/parameter/int_.h>
#include <piejam/runtime/parameter_factory.h>
#include <piejam/runtime/parameter_value_to_string.h>
#include <piejam/runtime/ui_parameter_descriptors_map.h>
#include <piejam/to_underlying.h>

#include <fmt/format.h>

#include <boost/container/flat_map.hpp>

namespace piejam::runtime::modules::spectrum
{

namespace
{

auto
to_stereo_channel_string(int const n) -> std::string
{
    using namespace std::string_literals;

    switch (n)
    {
        case to_underlying(stereo_channel::left):
            return "L"s;

        case to_underlying(stereo_channel::right):
            return "R"s;

        case to_underlying(stereo_channel::middle):
            return "M"s;

        case to_underlying(stereo_channel::side):
            return "S"s;

        default:
            return "ERROR"s;
    }
}

struct dB_ival
{
    static constexpr auto min{-24.f};
    static constexpr auto max{24.f};

    static constexpr auto min_gain{std::pow(10.f, min / 20.f)};
    static constexpr auto max_gain{std::pow(10.f, max / 20.f)};

    static constexpr auto to_normalized = &parameter::to_normalized_dB<dB_ival>;
    static constexpr auto from_normalized =
            &parameter::from_normalized_dB<dB_ival>;
};

auto
to_dB_string(float x) -> std::string
{
    return fmt::format("{:.1f} dB", std::log10(x) * 20.f);
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
            .fx_instance_id = fx::internal::spectrum,
            .name = "Spectrum"s,
            .bus_type = bus_type,
            .parameters =
                    fx::module_parameters{
                            {to_underlying(parameter_key::stream_a_active),
                             ui_params_factory.make_parameter(
                                     bool_parameter{.default_value = true},
                                     {.name = "Stream A Active",
                                      .value_to_string =
                                              &bool_parameter_value_to_string})},
                            {to_underlying(parameter_key::stream_b_active),
                             ui_params_factory.make_parameter(
                                     bool_parameter{.default_value = false},
                                     {.name = "Stream B Active",
                                      .value_to_string =
                                              &bool_parameter_value_to_string})},
                            {to_underlying(parameter_key::channel_a),
                             ui_params_factory.make_parameter(
                                     int_parameter{
                                             .default_value = to_underlying(
                                                     stereo_channel::left),
                                             .min = to_underlying(
                                                     stereo_channel::_min),
                                             .max = to_underlying(
                                                     stereo_channel::_max)},
                                     {.name = "Channel A",
                                      .value_to_string =
                                              &to_stereo_channel_string})},
                            {to_underlying(parameter_key::channel_b),
                             ui_params_factory.make_parameter(
                                     int_parameter{
                                             .default_value = to_underlying(
                                                     stereo_channel::right),
                                             .min = to_underlying(
                                                     stereo_channel::_min),
                                             .max = to_underlying(
                                                     stereo_channel::_max)},
                                     {.name = "Channel B",
                                      .value_to_string =
                                              &to_stereo_channel_string})},
                            {to_underlying(parameter_key::gain_a),
                             ui_params_factory.make_parameter(
                                     float_parameter{
                                             .default_value = 1.f,
                                             .min = dB_ival::min_gain,
                                             .max = dB_ival::max_gain,
                                             .to_normalized =
                                                     dB_ival::to_normalized,
                                             .from_normalized =
                                                     dB_ival::from_normalized},
                                     {.name = "Gain A"s,
                                      .value_to_string = &to_dB_string})},
                            {to_underlying(parameter_key::gain_b),
                             ui_params_factory.make_parameter(
                                     float_parameter{
                                             .default_value = 1.f,
                                             .min = dB_ival::min_gain,
                                             .max = dB_ival::max_gain,
                                             .to_normalized =
                                                     dB_ival::to_normalized,
                                             .from_normalized =
                                                     dB_ival::from_normalized},
                                     {.name = "Gain B"s,
                                      .value_to_string = &to_dB_string})},
                            {to_underlying(parameter_key::freeze),
                             ui_params_factory.make_parameter(
                                     bool_parameter{.default_value = false},
                                     {.name = "Freeze",
                                      .value_to_string =
                                              &bool_parameter_value_to_string})},
                    },
            .streams = fx::module_streams{
                    {to_underlying(stream_key::input),
                     streams.add(audio_stream_buffer(
                             std::in_place,
                             audio::num_channels(bus_type)))}}};
}

} // namespace piejam::runtime::modules::spectrum
