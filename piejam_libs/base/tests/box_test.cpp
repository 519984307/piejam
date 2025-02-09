// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

#include <piejam/box.h>

#include <gtest/gtest.h>

namespace piejam::test
{

TEST(box, default_ctor_and_get)
{
    box<int> x;
    EXPECT_EQ(int{}, x.get());
}

TEST(box, ctor_with_args_and_get)
{
    box<int> x(5);
    EXPECT_EQ(int{5}, x.get());
}

TEST(box, copy_ctor)
{
    box<int> x(5);
    box<int> y(x);
    EXPECT_EQ(5, y.get());
}

TEST(box, assign_operator)
{
    box<int> x(5);
    box<int> y(6);
    ASSERT_EQ(5, x.get());

    x = y;

    EXPECT_EQ(6, x.get());
}

TEST(box, cast_operator)
{
    box<int> x(5);
    int const& y = static_cast<int const&>(x);
    EXPECT_EQ(5, y);
}

TEST(box, deref_operator)
{
    box<int> x(5);
    EXPECT_EQ(5, *x);
}

TEST(box, arrow_operator)
{
    struct struct_with_method
    {
        [[nodiscard]] auto meth() const noexcept -> int
        {
            return 5;
        }
    };

    box<struct_with_method> x;
    EXPECT_EQ(5, x->meth());
}

TEST(box, eq_operator)
{
    box<int> x(5);
    box<int> y(5);
    box<int> z(7);

    EXPECT_TRUE(x == x);
    EXPECT_TRUE(x == y);
    EXPECT_FALSE(x == z);
}

TEST(box, neq_operator)
{
    box<int> x(5);
    box<int> y(5);
    box<int> z(7);

    EXPECT_FALSE(x != x);
    EXPECT_FALSE(x != y);
    EXPECT_TRUE(x != z);
}

TEST(box, assign_lvalue)
{
    box<int> x(5);
    box<int> y(x);

    int const v = 6;

    x = v;

    EXPECT_EQ(6, x.get());
    EXPECT_EQ(5, y.get());
}

TEST(box, assign_rvalue)
{
    box<int> x(5);
    box<int> y(x);

    auto f = []() -> int { return 6; };

    x = f();

    EXPECT_EQ(6, x.get());
    EXPECT_EQ(5, y.get());
}

TEST(box, update_without_return_value)
{
    box<int> sut(5);

    sut.update([](int& x) { x = 7; });

    EXPECT_EQ(7, sut.get());
}

TEST(box, update_with_return_value)
{
    box<int> sut(5);

    auto res = sut.update([](int& x) {
        auto y = x;
        x = 7;
        return y;
    });

    EXPECT_EQ(7, sut.get());
    EXPECT_EQ(5, res);
}

TEST(unique_box, copy_and_eq)
{
    unique_box<std::vector<float>> v(std::vector<float>{1, 2, 3});
    auto w = v;
    EXPECT_EQ(v, w);
}

TEST(unique_box, assign_and_eq)
{
    unique_box<std::vector<float>> v(std::vector<float>{1, 2, 3});
    unique_box<std::vector<float>> w;
    w = v;
    EXPECT_EQ(v, w);
}

} // namespace piejam::test
