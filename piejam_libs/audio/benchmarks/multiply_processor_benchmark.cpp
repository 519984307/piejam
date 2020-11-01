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

#include <piejam/audio/engine/multiply_processor.h>

#include <piejam/audio/engine/event_input_buffers.h>
#include <piejam/audio/engine/event_output_buffers.h>
#include <piejam/audio/engine/process_context.h>
#include <piejam/audio/engine/processor.h>

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace piejam::audio::engine
{

static void
BM_multiply_processor_2_inputs(benchmark::State& state)
{
    std::srand(std::time(nullptr));

    auto sut = make_multiply_processor(2);

    std::size_t const buffer_size = state.range(0);

    std::vector<float> in_buf1(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf2(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<audio_slice> in_slices{in_buf1, in_buf2};
    std::vector<std::reference_wrapper<audio_slice const>> in(
            in_slices.begin(),
            in_slices.end());

    std::vector<float> out_buf(buffer_size);
    std::vector<std::span<float>> out{out_buf};

    std::vector<audio_slice> res{{}};

    process_context ctx{in, out, res, {}, {}, buffer_size};

    for (auto _ : state)
        sut->process(ctx);
}

static void
BM_multiply_processor_3_inputs(benchmark::State& state)
{
    std::srand(std::time(nullptr));

    auto sut = make_multiply_processor(3);

    std::size_t const buffer_size = state.range(0);

    std::vector<float> in_buf1(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf2(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf3(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<audio_slice> in_slices{in_buf1, in_buf2, in_buf3};
    std::vector<std::reference_wrapper<audio_slice const>> in(
            in_slices.begin(),
            in_slices.end());

    std::vector<float> out_buf(buffer_size);
    std::vector<std::span<float>> out{out_buf};

    std::vector<audio_slice> res{{}};

    process_context ctx{in, out, res, {}, {}, buffer_size};

    for (auto _ : state)
        sut->process(ctx);
}

static void
BM_multiply_processor_4_inputs(benchmark::State& state)
{
    std::srand(std::time(nullptr));

    auto sut = make_multiply_processor(4);

    std::size_t const buffer_size = state.range(0);

    std::vector<float> in_buf1(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf2(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf3(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<float> in_buf4(
            buffer_size,
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    std::vector<audio_slice> in_slices{in_buf1, in_buf2, in_buf3, in_buf4};
    std::vector<std::reference_wrapper<audio_slice const>> in(
            in_slices.begin(),
            in_slices.end());

    std::vector<float> out_buf(buffer_size);
    std::vector<std::span<float>> out{out_buf};

    std::vector<audio_slice> res{{}};

    process_context ctx{in, out, res, {}, {}, buffer_size};

    for (auto _ : state)
        sut->process(ctx);
}

BENCHMARK(BM_multiply_processor_2_inputs)->RangeMultiplier(2)->Range(1, 1024);
BENCHMARK(BM_multiply_processor_3_inputs)->RangeMultiplier(2)->Range(1, 1024);
BENCHMARK(BM_multiply_processor_4_inputs)->RangeMultiplier(2)->Range(1, 1024);

} // namespace piejam::audio::engine
