// PieJam - An audio mixer for Raspberry Pi.
//
// Copyright (C) 2020  Dimitrij Kotrev
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <piejam/audio/pcm_io_config.h>
#include <piejam/audio/process_function.h>
#include <piejam/io/fwd.h>

#include <atomic>
#include <memory>
#include <vector>

namespace piejam::audio::alsa
{

class pcm_reader;
class pcm_writer;

class process_step
{
public:
    process_step(
            io::ioctl_device& input_fd,
            io::ioctl_device& output_fd,
            pcm_io_config const&,
            std::atomic<float>& cpu_load,
            std::atomic_size_t& xruns,
            process_function);
    process_step(process_step&&);
    ~process_step();

    void operator()();

private:
    io::ioctl_device& m_input_fd;
    io::ioctl_device& m_output_fd;
    pcm_io_config m_io_config;
    std::atomic<float>& m_cpu_load;
    std::atomic_size_t& m_xruns;
    process_function m_process_function;

    bool m_starting{true};
    std::unique_ptr<pcm_reader> m_reader;
    std::unique_ptr<pcm_writer> m_writer;
};

} // namespace piejam::audio::alsa
