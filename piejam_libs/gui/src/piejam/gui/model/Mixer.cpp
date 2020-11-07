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

#include <piejam/gui/model/Mixer.h>

#include <algorithm>
#include <cassert>

namespace piejam::gui::model
{

void
Mixer::setInputSoloActive(bool const x)
{
    if (m_inputSoloActive != x)
    {
        m_inputSoloActive = x;
        emit inputSoloActiveChanged();
    }
}

} // namespace piejam::gui::model
