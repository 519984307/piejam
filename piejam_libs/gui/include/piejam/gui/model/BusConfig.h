// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/gui/model/SubscribableModel.h>

namespace piejam::gui::model
{

class BusConfig : public SubscribableModel
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(bool mono READ mono NOTIFY monoChanged FINAL)
    Q_PROPERTY(int monoChannel READ monoChannel NOTIFY monoChannelChanged FINAL)
    Q_PROPERTY(int stereoLeftChannel READ stereoLeftChannel NOTIFY
                       stereoLeftChannelChanged FINAL)
    Q_PROPERTY(int stereoRightChannel READ stereoRightChannel NOTIFY
                       stereoRightChannelChanged FINAL)

public:
    using SubscribableModel::SubscribableModel;

    auto name() const noexcept -> QString const& { return m_name; }
    void setName(QString const& x)
    {
        if (m_name != x)
        {
            m_name = x;
            emit nameChanged();
        }
    }

    Q_INVOKABLE virtual void changeName(QString const&) = 0;

    auto mono() const noexcept -> bool { return m_mono; }
    void setMono(bool const x)
    {
        if (m_mono != x)
        {
            m_mono = x;
            emit monoChanged();
        }
    }

    auto monoChannel() const noexcept -> unsigned { return m_monoChannel; }
    void setMonoChannel(unsigned const x)
    {
        if (m_monoChannel != x)
        {
            m_monoChannel = x;
            emit monoChannelChanged();
        }
    }

    Q_INVOKABLE virtual void changeMonoChannel(unsigned) = 0;

    auto stereoLeftChannel() const noexcept -> unsigned
    {
        return m_stereoLeftChannel;
    }

    void setStereoLeftChannel(unsigned const x)
    {
        if (m_stereoLeftChannel != x)
        {
            m_stereoLeftChannel = x;
            emit stereoLeftChannelChanged();
        }
    }

    Q_INVOKABLE virtual void changeStereoLeftChannel(unsigned) = 0;

    auto stereoRightChannel() const noexcept -> unsigned
    {
        return m_stereoRightChannel;
    }

    void setStereoRightChannel(unsigned const x)
    {
        if (m_stereoRightChannel != x)
        {
            m_stereoRightChannel = x;
            emit stereoRightChannelChanged();
        }
    }

    Q_INVOKABLE virtual void changeStereoRightChannel(unsigned) = 0;

    Q_INVOKABLE virtual void deleteBus() = 0;

signals:

    void nameChanged();
    void monoChanged();
    void monoChannelChanged();
    void stereoLeftChannelChanged();
    void stereoRightChannelChanged();

private:
    QString m_name;
    bool m_mono{true};
    unsigned m_monoChannel{};
    unsigned m_stereoLeftChannel{};
    unsigned m_stereoRightChannel{};
};

} // namespace piejam::gui::model
