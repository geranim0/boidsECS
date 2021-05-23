#include <gtest/gtest.h>
#include <boidsECS/gmeta.hpp>

// Demonstrate some basic assertions.
TEST(UnitTests, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(GMetaTests, first_of_pack_t) {
  using namespace gmeta;

  //using 3 types
  using result1 = first_of_pack_t<int, double, float>;
  using expected1 = int;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_TRUE(equal1);

  //using 1 type
  using result2 = first_of_pack_t<char>;
  using expected2 = char;
  bool equal2 = gis_same_v<result1, expected1>;
  EXPECT_TRUE(equal2);
}

TEST(GMetaTests, remove_first_T_from_pack_t) {
  using namespace gmeta;
  
  //test using 2 types
  using t1 = types_t<int, double>;
  using expected1 = types_t<double>;
  using result1 = remove_first_T_from_pack_t<t1>;
  bool equal1 = gis_same_v<expected1, result1>;
  EXPECT_TRUE(equal1);

  //test using 1 type
  using t2 = types_t<int>;
  using expected2 = types_t<>;
  using result2 = remove_first_T_from_pack_t<t2>;
  bool equal2 = gis_same_v<expected2, result2>;
  EXPECT_TRUE(equal2);
}

TEST(GMetaTests, gcond_t) {
  using namespace gmeta;

  using result1 = gcond_t<true_t, char, int>;
  using expected1 = char;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = gcond_t<false_t, char, int>;
  using expected2 = int;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);
}

TEST(GMetaTests, pack_contains_t) {
  using namespace gmeta;

  using result1 = pack_contains_t<true_t, false_t, int, char>;
  using expected1 = false_t;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  bool result2 = pack_contains_v<short, char, short, double>;
  EXPECT_EQ(result2, true);
}


TEST(GMetaTests, first_of_types_t_pack_t) {
  using namespace gmeta;

  using result1 = first_of_types_t_pack_t<types_t<char, false_t, int, float>>;
  using expected1 = char;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = first_of_types_t_pack_t<types_t<float>>;
  using expected2 = float;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);
}

TEST(GMetaTests, all_different_t) {
  using namespace gmeta;

  using result0 = all_different_t<types_t<>>;
  using expected0 = true_t;
  bool equal0 = gis_same_v<result0, expected0>;
  EXPECT_EQ(equal0, true);

  using result1 = all_different_t<types_t<char, false_t, int, float>>;
  using expected1 = true_t;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = all_different_t<types_t<float, float>>;
  using expected2 = false_t;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);

  using result3 = all_different_t<types_t<float, float, float>>;
  using expected3 = false_t;
  bool equal3 = gis_same_v<result3, expected3>;
  EXPECT_EQ(equal3, true);
}

TEST(GMetaTests, fusion_2_typest_t) {
  using namespace gmeta;

  using result1 = fusion_2_typest_t<types_t<>, types_t<>>;
  using expected1 = types_t<>;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = fusion_2_typest_t<types_t<float>, types_t<>>;
  using expected2 = types_t<float>;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);

  using result3 = fusion_2_typest_t<types_t<>, types_t<float>>;
  using expected3 = types_t<float>;
  bool equal3 = gis_same_v<result3, expected3>;
  EXPECT_EQ(equal3, true);

  using result4 = fusion_2_typest_t<types_t<float>, types_t<double, char>>;
  using expected4 = types_t<float, double, char>;
  bool equal4 = gis_same_v<result4, expected4>;
  EXPECT_EQ(equal4, true);
}

TEST(GMetaTests, remove_first_encountered_T_from_pack_t) {
  using namespace gmeta;

  using result1 = remove_first_encountered_T_from_pack_t<true_t, types_t<false_t, int, char>>;
  using expected1 = types_t<false_t, int, char>;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = remove_first_encountered_T_from_pack_t<false_t, types_t<false_t, int, char>>;
  using expected2 = types_t<int, char>;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);

  using result3 = remove_first_encountered_T_from_pack_t<int, types_t<false_t, int, char>>;
  using expected3 = types_t<false_t, char>;
  bool equal3 = gis_same_v<result3, expected3>;
  EXPECT_EQ(equal3, true);

  using result4 = remove_first_encountered_T_from_pack_t<char, types_t<false_t, int, char>>;
  using expected4 = types_t<false_t, int>;
  bool equal4 = gis_same_v<result4, expected4>;
  EXPECT_EQ(equal4, true);

  using result5 = remove_first_encountered_T_from_pack_t<char, types_t<char>>;
  using expected5 = types_t<>;
  bool equal5 = gis_same_v<result5, expected5>;
  EXPECT_EQ(equal5, true);
}

TEST(GMetaTests, subset_of_t) {
  using namespace gmeta;

  using result1 = subset_of_t<types_t<>, types_t<int>>;
  using expected1 = true_t;
  bool equal1 = gis_same_v<result1, expected1>;
  EXPECT_EQ(equal1, true);

  using result2 = subset_of_t<types_t<int>, types_t<int>>;
  using expected2 = true_t;
  bool equal2 = gis_same_v<result2, expected2>;
  EXPECT_EQ(equal2, true);

  using result3 = subset_of_t<types_t<float>, types_t<int, float>>;
  using expected3 = true_t;
  bool equal3 = gis_same_v<result3, expected3>;
  EXPECT_EQ(equal3, true);

  using result4 = subset_of_t<types_t<float>, types_t<>>;
  using expected4 = false_t;
  bool equal4 = gis_same_v<result4, expected4>;
  EXPECT_EQ(equal4, true);

  using result5 = subset_of_t<types_t<int, double>, types_t<int, char>>;
  using expected5 = false_t;
  bool equal5 = gis_same_v<result5, expected5>;
  EXPECT_EQ(equal5, true);
}