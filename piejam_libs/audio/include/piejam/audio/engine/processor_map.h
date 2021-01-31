// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/audio/engine/processor.h>

#include <boost/assert.hpp>

#include <algorithm>
#include <any>
#include <memory>
#include <vector>

namespace piejam::audio::engine
{

class processor_map
{
    using mapping_pair = std::pair<std::any, std::unique_ptr<processor>>;
    using map_t = std::vector<mapping_pair>;

public:
    template <class Id>
    auto find(Id const& id) -> processor*
    {
        auto it = find_it(id);
        return it != m_map.end() ? it->second.get() : nullptr;
    }

    template <class Id>
    auto insert(Id const& id, std::unique_ptr<processor> proc)
    {
        BOOST_ASSERT(m_map.end() == find_it(id));
        m_map.emplace_back(id, std::move(proc));
    }

    template <class Id>
    auto remove(Id const& id) -> std::unique_ptr<processor>
    {
        if (auto it = find_it(id); it != m_map.end())
        {
            auto proc = std::move(it->second);
            m_map.erase(it);
            return proc;
        }

        return nullptr;
    }

private:
    template <class Id>
    auto find_it(Id const& id)
    {
        return std::ranges::find_if(
                m_map,
                [&id](std::any const& any_id) {
                    Id const* const typed_id = std::any_cast<std::decay_t<Id>>(
                            std::addressof(any_id));
                    return typed_id && *typed_id == id;
                },
                &mapping_pair::first);
    }

    map_t m_map;
};

} // namespace piejam::audio::engine
