// PieJam - An audio mixer for Raspberry Pi.
//
// Copyright (C) 2020  Dimitrij Kotrev
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <piejam/runtime/open_alsa_device.h>

#include <piejam/audio/alsa/pcm_io.h>
#include <piejam/runtime/audio_state.h>

namespace piejam::runtime
{

static auto
period_count(audio_state const& state) -> unsigned
{
    constexpr unsigned default_period_count = 2u;
    unsigned min_period_count{};
    unsigned max_period_count{};

    if (state.input.index && state.output.index)
    {
        min_period_count = std::max(
                state.input.hw_params->period_count_min,
                state.output.hw_params->period_count_min);
        max_period_count = std::min(
                state.input.hw_params->period_count_max,
                state.output.hw_params->period_count_max);
    }
    else if (state.input.index)
    {
        min_period_count = state.input.hw_params->period_count_min;
        max_period_count = state.input.hw_params->period_count_max;
    }
    else
    {
        min_period_count = state.output.hw_params->period_count_min;
        max_period_count = state.output.hw_params->period_count_max;
    }

    return std::clamp(default_period_count, min_period_count, max_period_count);
}

auto
open_alsa_device(audio_state const& state) -> std::unique_ptr<audio::device>
{
    return std::make_unique<audio::alsa::pcm_io>(
            state.pcm_devices->inputs[state.input.index],
            state.pcm_devices->outputs[state.output.index],
            audio::pcm_io_config{
                    audio::pcm_device_config{
                            state.input.hw_params->interleaved,
                            state.input.hw_params->format,
                            state.input.hw_params->num_channels},
                    audio::pcm_device_config{
                            state.output.hw_params->interleaved,
                            state.output.hw_params->format,
                            state.output.hw_params->num_channels},
                    audio::pcm_process_config{
                            state.samplerate,
                            state.period_size,
                            period_count(state)}});
}

} // namespace piejam::runtime
