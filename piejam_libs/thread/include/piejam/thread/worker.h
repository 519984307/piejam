// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <piejam/thread/affinity.h>
#include <piejam/thread/configuration.h>
#include <piejam/thread/priority.h>
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
            if (conf.affinity)
                this_thread::set_affinity(*conf.affinity);
            if (conf.priority)
                this_thread::set_priority(*conf.priority);

            while (!stoken.stop_requested())
            {
                m_sem.acquire();

                if (stoken.stop_requested())
                    break;

                m_task();
            }
        })
    {
    }

    worker(worker const&) = delete;
    worker(worker&&) = delete;

    ~worker()
    {
        m_thread.request_stop();
        wakeup([]() {});
    }

    auto operator=(worker const&) -> worker& = delete;
    auto operator=(worker&&) -> worker& = delete;

    template <std::invocable<> F>
    void wakeup(F&& task)
    {
        m_task = std::forward<F>(task);
        m_sem.release();
    }

private:
    semaphore m_sem;

    task_t m_task{[]() {}};
    std::jthread m_thread;
};

} // namespace piejam::thread
