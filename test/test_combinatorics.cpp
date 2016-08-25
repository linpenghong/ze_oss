// Copyright (C) ETH Zurich, Wyss Zurich, Zurich Eye - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

#include <iostream>
#include <ze/common/test_entrypoint.hpp>
#include <ze/common/types.hpp>
#include <ze/common/combinatorics.hpp>

TEST(CombinatoricsTests, testGetMatchingIndices)
{
  using namespace ze;

  // Create test data.
  using LandmarkIds = Eigen::Matrix<int32_t, Eigen::Dynamic, 1>;
  LandmarkIds ids_cur(10), ids_ref(7);
  ids_cur << -1, 2, 3, 7, 9, -1, 10, 12, 4, 5;
  ids_ref << -1, 1, 2, 3, 12, 9, -1;

  // Find matches.
  auto matches_cur_ref = getMatchIndices<int32_t>(
        ids_cur, ids_ref, [](int32_t id) { return id != -1; });
  EXPECT_EQ(matches_cur_ref.size(), 4u);
  for(auto it : matches_cur_ref)
  {
    EXPECT_EQ(ids_cur(it.first), ids_ref(it.second));
  }
}

TEST(CombinatoricsTests, tetstGetUnmatchedIndices)
{
  using namespace ze;

  // Create test data.
  using LandmarkIds = Eigen::Matrix<int32_t, Eigen::Dynamic, 1>;
  LandmarkIds ids_cur(10), ids_ref(7);
  ids_cur << -1, 2, 3, 7, 9, -1, 10, 12, 4, 5;
  ids_ref << -1, 1, 2, 3, 12, 9, -1;

  // Find unmached matches.
  auto unmatched_cur = getUnmatchedIndices<int32_t>(
        ids_cur, ids_ref, [](int32_t id) { return id != -1; });
  EXPECT_EQ(unmatched_cur[0], 3u);
  EXPECT_EQ(unmatched_cur[1], 6u);
  EXPECT_EQ(unmatched_cur[2], 8u);
  EXPECT_EQ(unmatched_cur[3], 9u);
}

TEST(CombinatoricsTests, testGetOutliersFromInliers1)
{
  std::vector<int> inliers { 0, 1, 5, 4 };
  std::vector<int> outliers = ze::getOutlierIndicesFromInlierIndices<int>(inliers, 7);
  ASSERT_EQ(outliers.size(), 3u);
  EXPECT_EQ(outliers[0], 2);
  EXPECT_EQ(outliers[1], 3);
  EXPECT_EQ(outliers[2], 6);
}

TEST(CombinatoricsTests, testGetOutliersFromInliers2)
{
  std::vector<size_t> inliers { 0, 1, 2 };
  auto outliers = ze::getOutlierIndicesFromInlierIndices<size_t>(inliers, 3);
  ASSERT_TRUE(outliers.empty());
}

ZE_UNITTEST_ENTRYPOINT
