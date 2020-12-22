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

#pragma once

#include <piejam/runtime/fwd.h>
#include <piejam/runtime/locations.h>

namespace piejam::runtime
{

class persistence_middleware
{
public:
    persistence_middleware(get_state_f, dispatch_f, next_f);

    void operator()(action const&);

private:
    template <class Action>
    void process_persistence_action(Action const&);

    get_state_f m_get_state;
    dispatch_f m_dispatch;
    next_f m_next;
};

} // namespace piejam::runtime
