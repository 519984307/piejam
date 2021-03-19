// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/thread/configuration.h>
#include <piejam/thread/semaphore.h>

#include <atomic>
#include <concepts>
#include <functional>
#include <thread>

namespace piejam::thread
{

//! A thread which gets a task assigned. The worker needs to be
//! woken up to execute the task. When the work on the task is done,
//! the thread yields the cpu and waits for the next wakeup call.
class worker
{
public:
    using task_t = std::function<void()>;

    worker(thread::configuration conf = {})
        : m_thread([this, conf = std::move(conf)](std::stop_token stoken) {
            conf.apply();

            m_sem_finished.release();

            while (!stoken.stop_requested())
            {
                m_sem_work.acquire();

                if (stoken.stop_requested())
                    break;

                m_task();

                m_sem_finished.release();
            }
        })
    {
    }

    worker(worker const&) = delete;
    worker(worker&&) = delete;

    ~worker()
    {
        m_sem_finished.acquire();
        m_thread.request_stop();
        m_sem_finished.release();
        m_sem_work.release();
    }

    auto operator=(worker const&) -> worker& = delete;
    auto operator=(worker&&) -> worker& = delete;

    template <std::invocable<> F>
    bool wakeup(F&& task) noexcept
    {
        if (m_sem_finished.try_acquire())
        {
            m_task = std::forward<F>(task);
            m_sem_work.release();
            return true;
        }

        return false;
    }

private:
    semaphore m_sem_work;
    semaphore m_sem_finished;

    task_t m_task{[]() {}};
    std::jthread m_thread;
};

} // namespace piejam::thread
