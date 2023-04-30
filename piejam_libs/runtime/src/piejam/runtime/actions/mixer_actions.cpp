// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/runtime/actions/mixer_actions.h>

#include <piejam/runtime/actions/delete_fx_module.h>
#include <piejam/runtime/state.h>
#include <piejam/runtime/ui/batch_action.h>
#include <piejam/runtime/ui/thunk_action.h>

#include <boost/hof/unpack.hpp>

#include <iterator>

namespace piejam::runtime::actions
{

auto
add_mixer_channel::reduce(state const& st) const -> state
{
    auto new_st = st;

    auto added_mixer_channel_id =
            runtime::add_mixer_channel(new_st, name, bus_type);

    if (auto_assign_input)
    {
        for (auto bus_id : *new_st.device_io_state.inputs)
        {
            auto it = std::ranges::find_if(
                    new_st.mixer_state.channels,
                    boost::hof::unpack([bus_id](
                                               mixer::channel_id,
                                               mixer::channel const&
                                                       mixer_channel) {
                        return std::holds_alternative<device_io::bus_id>(
                                       mixer_channel.in) &&
                               std::get<device_io::bus_id>(mixer_channel.in) ==
                                       bus_id;
                    }));

            if (it == new_st.mixer_state.channels.end() &&
                new_st.device_io_state.buses[bus_id].bus_type == bus_type)
            {
                new_st.mixer_state.channels.update(
                        added_mixer_channel_id,
                        [bus_id](mixer::channel& mixer_channel) {
                            mixer_channel.in = bus_id;
                        });
                break;
            }
        }
    }

    return new_st;
}

auto
delete_mixer_channel::reduce(state const& st) const -> state
{
    auto new_st = st;

    runtime::remove_mixer_channel(new_st, mixer_channel_id);

    return new_st;
}

auto
initiate_mixer_channel_deletion(mixer::channel_id mixer_channel_id)
        -> thunk_action
{
    return [=](auto&& get_state, auto&& dispatch) {
        state const& st = get_state();
        mixer::channel const& mixer_channel =
                st.mixer_state.channels[mixer_channel_id];
        if (mixer_channel.fx_chain->empty())
        {
            delete_mixer_channel action;
            action.mixer_channel_id = mixer_channel_id;
            dispatch(action);
        }
        else
        {
            batch_action batch;
            for (fx::module_id const fx_mod_id : *mixer_channel.fx_chain)
            {
                auto delete_fx_mod = std::make_unique<delete_fx_module>();
                delete_fx_mod->fx_mod_id = fx_mod_id;
                batch.push_back(std::move(delete_fx_mod));
            }

            auto action = std::make_unique<delete_mixer_channel>();
            action->mixer_channel_id = mixer_channel_id;
            batch.push_back(std::move(action));

            dispatch(batch);
        }
    };
}

auto
set_mixer_channel_name::reduce(state const& st) const -> state
{
    auto new_st = st;

    new_st.mixer_state.channels.update(
            channel_id,
            [this](mixer::channel& mixer_channel) {
                mixer_channel.name = name;
            });

    return new_st;
}

template <io_direction D>
auto
set_mixer_channel_route<D>::reduce(state const& st) const -> state
{
    auto new_st = st;

    new_st.mixer_state.channels.update(
            channel_id,
            [this](mixer::channel& mixer_channel) {
                (D == io_direction::input ? mixer_channel.in
                                          : mixer_channel.out) = route;
            });

    return new_st;
}

template auto
set_mixer_channel_route<io_direction::input>::reduce(state const&) const
        -> state;
template auto
set_mixer_channel_route<io_direction::output>::reduce(state const&) const
        -> state;

auto
move_mixer_channel_left::reduce(state const& st) const -> state
{
    auto new_st = st;

    new_st.mixer_state.inputs.update([this](mixer::channel_ids_t& channel_ids) {
        auto it = std::ranges::find(channel_ids, channel_id);
        BOOST_ASSERT(it != channel_ids.end());
        BOOST_ASSERT(it != channel_ids.begin());
        std::iter_swap(it, std::prev(it));
    });

    return new_st;
}

auto
move_mixer_channel_right::reduce(state const& st) const -> state
{
    auto new_st = st;

    new_st.mixer_state.inputs.update([this](mixer::channel_ids_t& channel_ids) {
        auto it = std::ranges::find(channel_ids, channel_id);
        BOOST_ASSERT(it != channel_ids.end());
        BOOST_ASSERT(std::next(it) != channel_ids.begin());
        std::iter_swap(it, std::next(it));
    });

    return new_st;
}

} // namespace piejam::runtime::actions
