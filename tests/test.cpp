// Copyright byteihq 2021 <kotov038@gmail.com>

#include <gtest/gtest.h>
#include <Vector.h>
#include <string>
#include <type_traits>

TEST(Vector, ConstructAssign) {
    {
        Vector<size_t> vec;
        EXPECT_TRUE(vec.empty());
        EXPECT_EQ(vec.size(), 0);
        EXPECT_EQ(vec.capacity(), 0);
    }
    {
        const int data[] = {1, 2, 4, -6};
        Vector<int> vec(4, data);
        EXPECT_EQ(vec[0], 1);
        EXPECT_EQ(vec[1], 2);
        EXPECT_EQ(vec[2], 4);
        EXPECT_EQ(vec[3], -6);
        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(vec.size(), 4);
        EXPECT_EQ(vec.capacity(), 4);
    }
    {
        Vector<std::string> vec(4);
        EXPECT_FALSE(vec.empty());
        EXPECT_EQ(vec.size(), 4);
        EXPECT_EQ(vec.capacity(), 4);
        EXPECT_TRUE(vec[0].empty());
        EXPECT_TRUE(vec[1].empty());
        EXPECT_TRUE(vec[2].empty());
        EXPECT_TRUE(vec[3].empty());
    }
}

TEST(Vector, Copy) {
    EXPECT_TRUE(std::is_copy_assignable<Vector<int>>::value);
    {
        Vector<int> v1;
        EXPECT_TRUE(v1.empty());
        EXPECT_EQ(v1.size(), 0);
        EXPECT_EQ(v1.capacity(), 0);
        Vector<int> v2(v1);
        EXPECT_TRUE(v2.empty());
        EXPECT_EQ(v2.size(), 0);
        EXPECT_EQ(v2.capacity(), 0);
    }
    {
        Vector<char> v1(10, 'c');
        EXPECT_FALSE(v1.empty());
        EXPECT_EQ(v1.size(), 10);
        EXPECT_EQ(v1.capacity(), 10);
        EXPECT_EQ(v1[3], 'c');
        Vector<char> v2(v1);
        EXPECT_FALSE(v2.empty());
        EXPECT_EQ(v2.size(), 10);
        EXPECT_EQ(v2.capacity(), 10);
        EXPECT_EQ(v2[3], 'c');
        v2[3] = 'd';
        EXPECT_EQ(v2[3], 'd');
        EXPECT_EQ(v1[3], 'c');
        EXPECT_EQ(v1.size(), 10);
        EXPECT_EQ(v2.size(), 10);
    }
}

TEST(Vector, Move) {
    EXPECT_TRUE(std::is_move_constructible<Vector<int>>::value);
    {
        Vector<int> v1;
        EXPECT_TRUE(v1.empty());
        EXPECT_EQ(v1.size(), 0);
        EXPECT_EQ(v1.capacity(), 0);
        Vector<int> v2(std::move(v1));
        EXPECT_TRUE(v2.empty());
        EXPECT_EQ(v2.size(), 0);
        EXPECT_EQ(v2.capacity(), 0);
    }
    {
        Vector<char> v1(10, 'c');
        EXPECT_FALSE(v1.empty());
        EXPECT_EQ(v1.size(), 10);
        EXPECT_EQ(v1.capacity(), 10);
        EXPECT_EQ(v1[3], 'c');
        Vector<char> v2(std::move(v1));
        EXPECT_FALSE(v2.empty());
        EXPECT_EQ(v2.size(), 10);
        EXPECT_EQ(v2.capacity(), 10);
        EXPECT_EQ(v2[3], 'c');
        v2[3] = 'd';
        EXPECT_EQ(v2[3], 'd');
        EXPECT_EQ(v1.size(), 0);
        EXPECT_EQ(v2.size(), 10);
    }
}

TEST(Vector, GetByIndex) {
    Vector<double> d_vec(4, 10.5);
    EXPECT_FALSE(d_vec.empty());
    EXPECT_EQ(d_vec.size(), 4);
    EXPECT_EQ(d_vec[3], 10.5);
    EXPECT_NO_THROW(d_vec.at(2));
    d_vec.at(2) = 9;
    EXPECT_EQ(d_vec.at(2), 9);
    EXPECT_THROW(d_vec.at(10), std::out_of_range);
    EXPECT_EQ(d_vec.front(), d_vec[0]);
    EXPECT_EQ(d_vec.front(), 10.5);
    EXPECT_EQ(d_vec.back(), d_vec[3]);
    d_vec[3] = -3;
    EXPECT_EQ(d_vec.back(), -3);
}

TEST(Vector, Clear) {
    {
        Vector<size_t> vec;
        EXPECT_TRUE(vec.empty());
        EXPECT_EQ(vec.size(), 0);
        EXPECT_EQ(vec.capacity(), 0);
        vec.clear();
        EXPECT_TRUE(vec.empty());
        EXPECT_EQ(vec.size(), 0);
        EXPECT_EQ(vec.capacity(), 0);
    }
    {
        Vector<char> c_vec(4, 12);
        EXPECT_FALSE(c_vec.empty());
        EXPECT_EQ(c_vec.size(), 4);
        c_vec.clear();
        EXPECT_TRUE(c_vec.empty());
        EXPECT_THROW(c_vec.at(1), std::out_of_range);
        c_vec.clear();
        EXPECT_TRUE(c_vec.empty());
        EXPECT_THROW(c_vec.at(1), std::out_of_range);
    }
}

TEST(Vector, Reserve) {
    {
        Vector<int> i_vec;
        EXPECT_TRUE(i_vec.empty());
        i_vec.reserve(10);
        EXPECT_TRUE(i_vec.empty());
        EXPECT_EQ(i_vec.capacity(), 10);
        EXPECT_THROW(i_vec.at(0), std::out_of_range);
    }
    {
        const float data[] = {1.3, 2.5, -0.6};
        Vector<float> f_vec(3, data);
        f_vec.reserve(2);
        EXPECT_EQ(f_vec.capacity(), 3);
        EXPECT_EQ(f_vec.size(), 3);
        EXPECT_NO_THROW(f_vec.at(2));
    }
    {
        const float data[] = {1.3, 2.5, -0.6};
        Vector<float> f_vec(3, data);
        f_vec.reserve(10);
        EXPECT_EQ(f_vec.capacity(), 10);
        EXPECT_EQ(f_vec.size(), 3);
        EXPECT_NO_THROW(f_vec.at(2));
        EXPECT_THROW(f_vec.at(3), std::out_of_range);
    }
}

TEST(Vector, Iterators) {
    const int data[] = {1, 3, -7, 10};
    Vector<int> i_vec(4, data);
    auto it = i_vec.begin();
    auto const_it = i_vec.cbegin();
    auto reverse_iterator = i_vec.rend();
    EXPECT_EQ(it, Vector<int>::iterator(i_vec.data()));
    EXPECT_EQ(*it, i_vec.front());
    EXPECT_EQ(*it, *const_it);
    EXPECT_EQ(*it, *reverse_iterator);
    ++it;
    ++const_it;
    --reverse_iterator;
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(*it, *const_it);
    EXPECT_EQ(*it, *reverse_iterator);
    it += 2;
    EXPECT_EQ(*it, 10);
    it = i_vec.end();
    --it;
    EXPECT_EQ(*it, 10);
    it -= 2;
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(*(it - 1), 1);
    it = i_vec.begin();
    auto it_end = i_vec.end();
    EXPECT_TRUE(it < it_end);
    EXPECT_EQ(it_end-it, i_vec.size());
}

TEST(Vector, Insert) {
    Vector<int> i_vec(4, 100);  // 100 100 100 100
    auto it = i_vec.begin();
    it = i_vec.insert(it, 200);  // 200 100 100 100 100
    EXPECT_EQ(i_vec.front(), 200);
    EXPECT_EQ(i_vec.at(1), 100);
    EXPECT_EQ(i_vec.size(), 5);
    it += 2;
    it = i_vec.insert(it, 300);  // 200 100 300 100 100 100
    EXPECT_EQ(*it, 300);
    EXPECT_EQ(i_vec.size(), 6);
    it = i_vec.end();
    EXPECT_THROW(i_vec.insert(it, 0), std::out_of_range);
}

TEST(Vector, PushBack) {
    Vector<size_t> s_vec(2, 10);
    s_vec.push_back(12);
    EXPECT_EQ(s_vec.front(), 10);
    EXPECT_EQ(s_vec.back(), 12);
    EXPECT_EQ(s_vec.size(), 3);
    EXPECT_GT(s_vec.capacity(), s_vec.size());
    s_vec.push_back(123);
    EXPECT_EQ(s_vec.back(), 123);
}

class Student {
private:
    std::string name_;
    size_t age_;
public:
    Student() = default;

    explicit Student(const std::string& name, size_t age) : name_(name), age_(age) {}

    [[nodiscard]] auto get_name() const -> std::string { return name_; }

    [[nodiscard]] auto get_age() const -> size_t { return age_; }
};

TEST(Vector, EmplaceBack) {
    {
        Vector<std::string> vec;
        std::string hello_world = "hello world";
        vec.emplace_back(hello_world.begin(), hello_world.begin() + 5);
        EXPECT_EQ(vec.back(), vec.front());
        EXPECT_EQ(vec.back(), "hello");
        EXPECT_EQ(vec.size(), 1);
        vec.emplace_back("world");
        EXPECT_EQ(vec.front(), "hello");
        EXPECT_EQ(vec.back(), "world");
        EXPECT_EQ(vec.size(), 2);
    }
    {
        Vector<Student> vec;
        vec.emplace_back("Petr", 20u);
        EXPECT_EQ(vec.size(), 1);
        EXPECT_EQ(vec[0].get_name(), "Petr");
        EXPECT_EQ(vec[0].get_age(), 20u);
        vec.emplace_back("Sasha", 19u);
        EXPECT_EQ(vec.size(), 2);
        EXPECT_EQ(vec[1].get_name(), "Sasha");
        EXPECT_EQ(vec[1].get_age(), 19u);
    }
}

TEST(Vector, PopBack) {
    Vector<int> i_vec;
    i_vec.push_back(-1);
    EXPECT_EQ(i_vec.size(), 1);
    i_vec.pop_back();
    EXPECT_TRUE(i_vec.empty());
    for (size_t i = 0; i < 5; ++i) {
        i_vec.push_back(i);
        if (i % 2 == 0) {
            i_vec.pop_back();
        }
    }
    EXPECT_EQ(i_vec.back(), 3);
    EXPECT_EQ(i_vec.size(), 2);
}

TEST(Vector, Swap) {
    Vector<int> i_vec1(2, 0);
    EXPECT_EQ(i_vec1.size(), 2);
    Vector<int> i_vec2;
    EXPECT_TRUE(i_vec2.empty());
    i_vec2.swap(i_vec1);
    EXPECT_TRUE(i_vec1.empty());
    EXPECT_EQ(i_vec2.size(), 2);
}
