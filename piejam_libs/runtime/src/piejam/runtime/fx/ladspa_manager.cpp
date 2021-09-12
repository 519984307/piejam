// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/runtime/fx/ladspa_manager.h>

#include <piejam/audio/engine/processor.h>
#include <piejam/audio/sample_rate.h>
#include <piejam/entity_id_hash.h>
#include <piejam/ladspa/plugin.h>
#include <piejam/ladspa/plugin_descriptor.h>

#include <spdlog/spdlog.h>

namespace piejam::runtime::fx
{

ladspa_manager::~ladspa_manager() = default;

auto
ladspa_manager::load(ladspa::plugin_descriptor const& pd) -> ladspa::instance_id
{
    try
    {
        auto plugin = ladspa::load(pd);
        auto id = entity_id<ladspa::instance_id_tag>::generate();
        m_instances.emplace(id, std::move(plugin));
        return id;
    }
    catch (std::exception const& err)
    {
        spdlog::error("Loading LADSPA fx plugin failed: {}", err.what());
        return {};
    }
}

void
ladspa_manager::unload(ladspa::instance_id const& id)
{
    m_instances.erase(id);
}

auto
ladspa_manager::control_inputs(ladspa::instance_id const& id) const
        -> std::span<ladspa::port_descriptor const>
{
    if (auto it = m_instances.find(id); it != m_instances.end())
    {
        return it->second->control_inputs();
    }

    return {};
}

auto
ladspa_manager::make_processor(
        ladspa::instance_id const& id,
        audio::sample_rate const& sample_rate) const
        -> std::unique_ptr<audio::engine::processor>
{
    if (auto it = m_instances.find(id); it != m_instances.end())
    {
        return it->second->make_processor(sample_rate);
    }

    return {};
}

} // namespace piejam::runtime::fx
