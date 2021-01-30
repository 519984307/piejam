// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/runtime/processors/midi_learn_processor.h>

#include <piejam/audio/engine/event_input_buffers.h>
#include <piejam/audio/engine/event_output_buffers.h>
#include <piejam/audio/engine/event_port.h>
#include <piejam/audio/engine/named_processor.h>
#include <piejam/audio/engine/verify_process_context.h>
#include <piejam/midi/event.h>

#include <memory>

namespace piejam::runtime::processors
{

namespace
{

bool
is_control_event(midi::external_event const& ev)
{
    return std::holds_alternative<midi::channel_cc_event>(ev.event);
}

class midi_learn_processor final : public audio::engine::named_processor
{
public:
    auto type_name() const -> std::string_view override { return "midi_learn"; }

    auto num_inputs() const -> std::size_t override { return 0; }
    auto num_outputs() const -> std::size_t override { return 0; }

    auto event_inputs() const -> event_ports override
    {
        static std::array<audio::engine::event_port, 1> s_ports{
                audio::engine::event_port(
                        std::in_place_type<midi::external_event>,
                        "event_in")};

        return s_ports;
    }

    auto event_outputs() const -> event_ports override
    {
        static std::array<audio::engine::event_port, 2> s_ports{
                audio::engine::event_port(
                        std::in_place_type<midi::external_event>,
                        "learned"),
                audio::engine::event_port(
                        std::in_place_type<midi::external_event>,
                        "out")};

        return s_ports;
    }

    void process(audio::engine::process_context const& ctx) override
    {
        audio::engine::verify_process_context(*this, ctx);

        auto const& in = ctx.event_inputs.get<midi::external_event>(0);

        auto& out = ctx.event_outputs.get<midi::external_event>(1);

        if (m_learning)
        {
            auto& learned = ctx.event_outputs.get<midi::external_event>(0);

            for (auto const& ev : in)
            {
                if (m_learning && is_control_event(ev.value()))
                {
                    learned.insert(ev.offset(), ev.value());
                    m_learning = false;
                }

                out.insert(ev.offset(), ev.value());
            }
        }
        else
        {
            for (auto const& ev : in)
                out.insert(ev.offset(), ev.value());
        }
    }

private:
    bool m_learning{true};
};

} // namespace

auto
make_midi_learn_processor() -> std::unique_ptr<audio::engine::processor>
{
    return std::make_unique<midi_learn_processor>();
}

} // namespace piejam::runtime::processors
