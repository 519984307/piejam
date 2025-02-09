// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2023  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/gui/model/Subscribable.h>
#include <piejam/gui/model/SubscribableModel.h>
#include <piejam/gui/model/fwd.h>

#include <piejam/runtime/root_view_mode.h>

#include <memory>

namespace piejam::gui::model
{

class RootView final : public Subscribable<SubscribableModel>
{
    Q_OBJECT

    Q_PROPERTY(int mode READ mode NOTIFY modeChanged FINAL)
    Q_PROPERTY(bool canShowFxModule READ canShowFxModule NOTIFY
                       canShowFxModuleChanged FINAL)

public:
    RootView(runtime::store_dispatch, runtime::subscriber&);
    ~RootView() override;

    auto mode() const -> int
    {
        return m_mode;
    }

    auto canShowFxModule() const -> bool
    {
        return m_canShowFxModule;
    }

    Q_INVOKABLE void showMixer();
    Q_INVOKABLE void showFxModule();
    Q_INVOKABLE void showInfo();
    Q_INVOKABLE void showSettings();
    Q_INVOKABLE void showPower();

signals:
    void modeChanged();
    void canShowFxModuleChanged();

private:
    void onSubscribe() override;

    void setMode(int x)
    {
        if (m_mode != x)
        {
            m_mode = x;
            emit modeChanged();
        }
    }

    void setCanShowFxModule(bool const x)
    {
        if (m_canShowFxModule != x)
        {
            m_canShowFxModule = x;
            emit canShowFxModuleChanged();
        }
    }

    void switchRootViewMode(runtime::root_view_mode);

    int m_mode{};
    bool m_canShowFxModule{};
};

} // namespace piejam::gui::model
