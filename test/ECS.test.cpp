#include <gtest/gtest.h>
#include <boidsECS/ECS.hpp>



TEST(ArchetypeChunk, ContainsEntity) {
  
  auto ac = ArchetypeChunk<32, int, double, char>{};

  EXPECT_FALSE(ac.ContainsEntity(0));

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_TRUE(ac.ContainsEntity(0));
  EXPECT_TRUE(ac.ContainsEntity(1));
  EXPECT_TRUE(ac.ContainsEntity(2));

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  ac2.AddEntity(0, 69, 3.1416, 'x');
  ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_TRUE(ac2.ContainsEntity(0));
  EXPECT_FALSE(ac2.ContainsEntity(1));
  EXPECT_FALSE(ac2.ContainsEntity(2));
}

TEST(ArchetypeChunk, Size) {
  
  auto ac = ArchetypeChunk<32, int, double, char>{};

  EXPECT_EQ(ac.size(), 0);

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac.size(), 3);

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  ac2.AddEntity(0, 69, 3.1416, 'x');
  ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac2.size(), 1);

  ac2.DeleteEntityById(0);

  EXPECT_EQ(ac2.size(), 0);
}

TEST(ArchetypeChunk, AddEntity) {
  
  auto ac = ArchetypeChunk<32, int, double, char>{};

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac.size(), 3);
  EXPECT_TRUE(ac.ContainsEntity(0));
  EXPECT_TRUE(ac.ContainsEntity(1));
  EXPECT_TRUE(ac.ContainsEntity(2));

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  bool successAdd = ac2.AddEntity(0, 69, 3.1416, 'x');
  EXPECT_TRUE(successAdd);

  successAdd = ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_FALSE(successAdd);

  EXPECT_EQ(ac2.size(), 1);
  EXPECT_TRUE(ac2.ContainsEntity(0));
  EXPECT_FALSE(ac2.ContainsEntity(1));
  EXPECT_FALSE(ac2.ContainsEntity(2));
}

TEST(ArchetypeChunk, DeleteEntityById) {
  
  auto ac = ArchetypeChunk<32, int, double, char>{};

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  ac.DeleteEntityById(0);

  EXPECT_FALSE(ac.ContainsEntity(0));
  EXPECT_TRUE(ac.ContainsEntity(1));
  EXPECT_TRUE(ac.ContainsEntity(2));
  EXPECT_EQ(ac.size(), 2);

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  ac2.AddEntity(0, 69, 3.1416, 'x');
  ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  ac2.DeleteEntityById(2);

  EXPECT_TRUE(ac2.ContainsEntity(0));
  EXPECT_FALSE(ac2.ContainsEntity(1));
  EXPECT_FALSE(ac2.ContainsEntity(2));

  ac2.DeleteEntityById(0);
  EXPECT_FALSE(ac2.ContainsEntity(0));

  EXPECT_EQ(ac2.size(), 0);

}

TEST(ArchetypeChunk, foreach) {
  
  auto ac = ArchetypeChunk<3, int, double, char>{};
  ac.AddEntity(0, 1, 3.1416, 'x');
  ac.AddEntity(1, 1, 1.1416, 'y');
  ac.AddEntity(2, 1, 0.1416, 'z');
  
  ac.foreach([](int& a)
  {
    a += 1;
  });

  bool correct = true;
  ac.foreach([&](int const& a)
  {
    if (a != 2)
    {
      correct = false;
    }
  });
  EXPECT_TRUE(correct);

  bool correct2 = true;
  ac.foreach([](int const& a, char& c, double d)
  {
    c = 's';
  });
  ac.foreach([&](double const d, int a, char const& c)
  {
    if (c != 's')
    {
      correct2 = false;
    }
  });

  EXPECT_TRUE(correct2);

}