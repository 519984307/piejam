// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2021  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/gui/model/BusType.h>
#include <piejam/gui/model/FxModuleContent.h>
#include <piejam/gui/model/SpectrumData.h>
#include <piejam/gui/model/StereoChannel.h>
#include <piejam/gui/model/Subscribable.h>
#include <piejam/gui/model/fwd.h>
#include <piejam/runtime/fx/fwd.h>

#include <memory>

namespace piejam::gui::model
{

class FxSpectrum final : public Subscribable<FxModuleContent>
{
    Q_OBJECT

    Q_PROPERTY(piejam::gui::model::BusType busType READ busType CONSTANT FINAL)
    Q_PROPERTY(
            piejam::gui::model::SpectrumData* dataA READ dataA CONSTANT FINAL)
    Q_PROPERTY(
            piejam::gui::model::SpectrumData* dataB READ dataB CONSTANT FINAL)
    Q_PROPERTY(bool activeA READ activeA NOTIFY activeAChanged FINAL)
    Q_PROPERTY(bool activeB READ activeB NOTIFY activeBChanged FINAL)
    Q_PROPERTY(piejam::gui::model::StereoChannel channelA READ channelA NOTIFY
                       channelAChanged FINAL)
    Q_PROPERTY(piejam::gui::model::StereoChannel channelB READ channelB NOTIFY
                       channelBChanged FINAL)

public:
    FxSpectrum(
            runtime::store_dispatch,
            runtime::subscriber&,
            runtime::fx::module_id);
    ~FxSpectrum() override;

    auto type() const noexcept -> Type override
    {
        return Type::Spectrum;
    }

    auto busType() const noexcept -> BusType;

    auto activeA() const noexcept -> bool
    {
        return m_activeA;
    }

    Q_INVOKABLE void changeActiveA(bool const active);

    auto channelA() const noexcept -> StereoChannel
    {
        return m_channelA;
    }

    Q_INVOKABLE void changeChannelA(piejam::gui::model::StereoChannel const x);

    auto dataA() noexcept -> SpectrumData*
    {
        return &m_spectrumDataA;
    }

    auto activeB() const noexcept -> bool
    {
        return m_activeB;
    }

    Q_INVOKABLE void changeActiveB(bool const active);

    auto channelB() const noexcept -> StereoChannel
    {
        return m_channelB;
    }

    Q_INVOKABLE void changeChannelB(piejam::gui::model::StereoChannel const x);

    auto dataB() noexcept -> SpectrumData*
    {
        return &m_spectrumDataB;
    }

    Q_INVOKABLE void clear()
    {
        m_spectrumDataA.clear();
        m_spectrumDataB.clear();
    }

signals:
    void activeAChanged();
    void activeBChanged();
    void channelAChanged();
    void channelBChanged();

private:
    void onSubscribe() override;

    struct Impl;
    std::unique_ptr<Impl> m_impl;

    bool m_activeA{true};
    StereoChannel m_channelA{StereoChannel::Left};
    SpectrumData m_spectrumDataA;
    bool m_activeB{false};
    StereoChannel m_channelB{StereoChannel::Right};
    SpectrumData m_spectrumDataB;
};

} // namespace piejam::gui::model
