// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/redux/flag_resetter.h>
#include <piejam/redux/functors.h>

namespace piejam::redux
{

template <class Action>
class batch_middleware
{
public:
    batch_middleware(bool& batching, next_f<Action> next)
        : m_batching(batching)
        , m_next(std::move(next))
    {
    }

    void operator()(Action const& a) const
    {
        if (auto* const batch = as_batch_action(a))
        {
            flag_resetter reset_batching{&m_batching};

            using std::begin;
            using std::end;

            for (auto it = begin(*batch), it_end = end(*batch); it != it_end;
                 ++it)
            {
                m_batching = std::next(it) != it_end;
                m_next(*it);
            }
        }
        else
        {
            m_next(a);
        }
    }

private:
    bool& m_batching;
    next_f<Action> m_next;
};

} // namespace piejam::redux
