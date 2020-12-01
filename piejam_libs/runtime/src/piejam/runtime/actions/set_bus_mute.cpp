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

#include <piejam/runtime/actions/set_bus_volume.h>

#include <piejam/runtime/actions/set_bool_parameter.h>
#include <piejam/runtime/audio_state.h>
#include <piejam/runtime/ui/thunk_action.h>

namespace piejam::runtime::actions
{

auto
set_bus_mute(mixer::bus_id bus_id, bool mute) -> thunk_action
{
    return [=](auto const& get_state, auto const& dispatch) {
        state const& st = get_state();
        if (mixer::bus const* const bus = st.mixer_state.buses.get()[bus_id])
        {
            if (st.bool_params.contains(bus->mute))
                dispatch(set_bool_parameter{bus->mute, mute});
        }
    };
}

} // namespace piejam::runtime::actions
