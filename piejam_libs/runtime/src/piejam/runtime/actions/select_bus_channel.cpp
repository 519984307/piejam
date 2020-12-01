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

#include <piejam/runtime/actions/select_bus_channel.h>

#include <piejam/runtime/audio_state.h>

namespace piejam::runtime::actions
{

auto
select_bus_channel::reduce(state const& st) const -> state
{
    auto new_st = st;

    new_st.mixer_state.buses = [this](mixer::buses_t buses) {
        mixer::bus& bus = buses[bus_id];

        switch (channel_selector)
        {
            case audio::bus_channel::mono:
                bus.device_channels = channel_index_pair{channel_index};
                break;

            case audio::bus_channel::left:
                bus.device_channels.left = channel_index;
                break;

            case audio::bus_channel::right:
                bus.device_channels.right = channel_index;
                break;
        }

        return buses;
    }(new_st.mixer_state.buses);

    return new_st;
}

} // namespace piejam::runtime::actions
