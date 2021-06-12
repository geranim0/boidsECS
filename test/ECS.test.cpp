#include <gtest/gtest.h>
#include <boidsECS/ECS.hpp>

namespace test
{
  struct UpdatePositionSystem
{
  void pre_tick(){

  }
  void post_tick()
  {

  }

	void operator()(position3f pos, DeltaTime const& dt) const
	{
		pos.x += 1 * double(dt);
	}
};

struct UpdateSpeedSystem
{
    void pre_tick(){

  }
  void post_tick()
  {
    
  }
	void operator()(speed3f &speed) const
	{
		speed.y += 1;
	}
};

struct UpdateCounterSystem
{
    void pre_tick(){

  }
  void post_tick()
  {
    
  }
	void operator()(Counter &counter) const
	{
		counter += 1;
	}
};

  struct CapturingSystem
  {
    void pre_tick()
    {

    }
    void post_tick()
    {
      
    }
    CapturingSystem(int& resource_to_capture_by_ref_) 
    :resource_to_capture_by_ref{resource_to_capture_by_ref_}
    {}

    void operator()(Counter& counter)
    {
      counter += 1;
      resource_to_capture_by_ref += 1;
    }
    int& resource_to_capture_by_ref;
  };
}

TEST(ArchetypeChunk, Size)
{

  auto ac = ArchetypeChunk<32, int, double, char>{};

  EXPECT_EQ(ac.Size(), 0);

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac.Size(), 3);

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  ac2.AddEntity(0, 69, 3.1416, 'x');
  ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac2.Size(), 1);

  ac2.DeleteEntityById(0);

  EXPECT_EQ(ac2.Size(), 0);
}

TEST(ArchetypeChunk, AddEntity)
{

  auto ac = ArchetypeChunk<32, int, double, char>{};

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_EQ(ac.Size(), 3);
  EXPECT_TRUE(ac.ContainsEntity(0));
  EXPECT_TRUE(ac.ContainsEntity(1));
  EXPECT_TRUE(ac.ContainsEntity(2));

  auto ac2 = ArchetypeChunk<1, int, double, char>{};

  bool successAdd = ac2.AddEntity(0, 69, 3.1416, 'x');
  EXPECT_TRUE(successAdd);

  successAdd = ac2.AddEntity(1, 420, 1.1416, 'y');
  ac2.AddEntity(2, 0, 0.1416, 'z');

  EXPECT_FALSE(successAdd);

  EXPECT_EQ(ac2.Size(), 1);
  EXPECT_TRUE(ac2.ContainsEntity(0));
  EXPECT_FALSE(ac2.ContainsEntity(1));
  EXPECT_FALSE(ac2.ContainsEntity(2));
}

TEST(ArchetypeChunk, ContainsEntity)
{

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

TEST(ArchetypeChunk, DeleteEntityById)
{

  auto ac = ArchetypeChunk<32, int, double, char>{};

  ac.AddEntity(0, 69, 3.1416, 'x');
  ac.AddEntity(1, 420, 1.1416, 'y');
  ac.AddEntity(2, 0, 0.1416, 'z');

  ac.DeleteEntityById(0);

  EXPECT_FALSE(ac.ContainsEntity(0));
  EXPECT_TRUE(ac.ContainsEntity(1));
  EXPECT_TRUE(ac.ContainsEntity(2));
  EXPECT_EQ(ac.Size(), 2);

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

  EXPECT_EQ(ac2.Size(), 0);
}

TEST(ArchetypeChunk, foreach)
{

  auto ac = ArchetypeChunk<3, int, double, char>{};
  ac.AddEntity(0, 1, 3.1416, 'x');
  ac.AddEntity(1, 1, 1.1416, 'y');
  ac.AddEntity(2, 1, 0.1416, 'z');

  ac.foreach ([](int &a) {
    a += 1;
  });

  bool correct = true;
  ac.foreach ([&](int const &a) {
    if (a != 2)
    {
      correct = false;
    }
  });
  EXPECT_TRUE(correct);

  bool correct2 = true;
  ac.foreach ([](int const &a, char &c, double d) {
    c = 's';
  });
  ac.foreach ([&](double const d, int a, char const &c) {
    if (c != 's')
    {
      correct2 = false;
    }
  });

  EXPECT_TRUE(correct2);
}

TEST(Archetype, Constructor)
{

  auto arc = Archetype<32, int, double, char>{};
  EXPECT_EQ(arc.Size(), 0);
}

TEST(Archetype, AddEntity)
{

  auto arc = Archetype<2, int, double, char>{};
  arc.AddEntity(0, 1, 1.1, 'a');
  arc.AddEntity(1, 3, 1.2, 'b');
  arc.AddEntity(2, 5, 1.3, 'c');

  EXPECT_EQ(arc.Size(), 3);
}

TEST(Archetype, Size)
{

  auto arc = Archetype<2, int, double, char>{};
  EXPECT_EQ(arc.Size(), 0);
  arc.AddEntity(0, 1, 1.1, 'a');
  arc.AddEntity(1, 3, 1.2, 'b');
  arc.AddEntity(2, 5, 1.3, 'c');
  EXPECT_EQ(arc.Size(), 3);
  arc.AddEntity(3, 1, 1.1, 'a');
  arc.AddEntity(4, 3, 1.2, 'b');
  arc.AddEntity(5, 5, 1.3, 'c');
  EXPECT_EQ(arc.Size(), 6);
}

TEST(Archetype, DeleteEntityById)
{
  auto arc = Archetype<2, int, double, char>{};
  arc.AddEntity(0, 1, 1.1, 'a');
  arc.AddEntity(1, 3, 1.2, 'b');
  arc.AddEntity(2, 5, 1.3, 'c');

  arc.DeleteEntityById(2);

  EXPECT_EQ(arc.Size(), 2);
  EXPECT_TRUE(arc.Contains(0));
  EXPECT_TRUE(arc.Contains(1));
  EXPECT_FALSE(arc.Contains(2));

  arc.AddEntity(3, 1, 1.1, 'a');
  arc.AddEntity(4, 3, 1.2, 'b');
  arc.AddEntity(5, 5, 1.3, 'c');

  arc.DeleteEntityById(0);
  arc.DeleteEntityById(5);

  EXPECT_EQ(arc.Size(), 3);
  EXPECT_TRUE(arc.Contains(1));
  EXPECT_TRUE(arc.Contains(3));
  EXPECT_TRUE(arc.Contains(4));
  EXPECT_FALSE(arc.Contains(0));
  EXPECT_FALSE(arc.Contains(5));
}

TEST(Archetype, foreach)
{
  auto arc = Archetype<2, int, double, char>{};
  arc.AddEntity(0, 1, 1.1, 'a');
  arc.AddEntity(1, 1, 1.2, 'b');
  arc.AddEntity(2, 1, 1.3, 'c');

  arc.foreach([](Id id, int i, double const& d, char& c)
  {
    EXPECT_EQ(i, 1);
    if (id == 0)
    {
      EXPECT_EQ(d, 1.1);
      EXPECT_EQ(c, 'a');
      c = 'x';
    }
    else if (id == 1)
    {
      EXPECT_EQ(d, 1.2);
      EXPECT_EQ(c, 'b');
      c = 'x';
    }
    if (id == 2)
    {
      EXPECT_EQ(d, 1.3);
      EXPECT_EQ(c, 'c');
      c = 'x';
    }
  });

  arc.foreach([](int& x)
  {
    x += 1;
  });

  arc.foreach([](char const c, int const& x)
  {
    EXPECT_EQ(x, 2);
    EXPECT_EQ(c, 'x');
  });
}

TEST(Archetype, Compress_Dummy)
{
  auto arc = Archetype<2, int, double, char>{};
  arc.AddEntity(0, 1, 1.1, 'a');
  arc.AddEntity(1, 2, 1.2, 'b');
  arc.AddEntity(2, 3, 1.3, 'c');

  arc.DeleteEntityById(1);

  arc.AddEntity(3, 4, 1.4, 'd');
  arc.AddEntity(4, 5, 1.5, 'e');
  arc.AddEntity(5, 6, 1.6, 'f');

  arc.DeleteEntityById(0);
  arc.DeleteEntityById(3);

  EXPECT_EQ(arc.CountUnfilledChunks(), 2);

  auto moved = arc.Compress_Dummy();

  EXPECT_EQ(moved, 2);
  EXPECT_EQ(arc.Size(), 3);
  EXPECT_EQ(arc.CountUnfilledChunks(), 1);

  //test doing nothing
  auto arc2 = Archetype<32, int, double, char>{};
  arc2.AddEntity(0, 1, 1.1, 'a');
  arc2.AddEntity(1, 2, 1.2, 'b');
  arc2.AddEntity(2, 3, 1.3, 'c');
  auto moved2 = arc2.Compress_Dummy<0>();
  EXPECT_EQ(arc2.CountUnfilledChunks(), 1);
}

TEST(ECS, Constructor)
{
  ECS<gmeta::types_t<
        Archetype<32, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>,
    gmeta::types_t<
        test::UpdatePositionSystem,
        test::UpdateSpeedSystem,
        test::UpdateCounterSystem>>
    ecs;

    EXPECT_EQ(ecs.Size(), 0);
}

TEST(ECS, Capturing_Constructor_functor)
{
  struct UpdateCounterSystem_capture
  {
    UpdateCounterSystem_capture(int& to_capture) : captured{to_capture}
    {
      
    }
    void operator()(Counter& counter) const
    {
      counter += 1;
      captured += 1;
    }

    void pre_tick(){}
    void post_tick(){}
    int& captured;
  };

  int resource_to_capture_by_ref = 0;

  ECS<gmeta::types_t<
      Archetype<32, position3f, speed3f, Mass, Counter>,
      Archetype<2, int, double, Counter>>, 
    gmeta::types_t<
      UpdateCounterSystem_capture>>
  ecs(std::move(UpdateCounterSystem_capture{resource_to_capture_by_ref}));

  ecs.AddEntity<position3f, speed3f, Mass>(position3f{4, 2, 3}, speed3f{4, 5, 4}, Mass{5.4}, Counter{0});
  ecs.AddEntity<position3f, speed3f, Mass>(position3f{5, 2, 3}, speed3f{4, 5, 5}, Mass{5.5}, Counter{0});

  ecs.AddEntity(1, 1.1, Counter{0});
  ecs.AddEntity(2, 2.2, Counter{0});

  ecs.tick();

  EXPECT_EQ(resource_to_capture_by_ref, 4);
  ecs.foreach([](Counter const& counter)
  {
    EXPECT_EQ(counter, 1);
  }, DeltaTime{0});
}

TEST(ECS, AddEntity)
{
  ECS<gmeta::types_t<
        Archetype<32, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>,
    gmeta::types_t<
        test::UpdatePositionSystem,
        test::UpdateSpeedSystem,
        test::UpdateCounterSystem>>
    ecs;

    ecs.AddEntity(position3f{1, 2, 3}, speed3f{4, 5, 1}, Mass{5.1}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{2, 2, 3}, speed3f{4, 5, 2}, Mass{5.2}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{3, 2, 3}, speed3f{4, 5, 3}, Mass{5.3}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{4, 2, 3}, speed3f{4, 5, 4}, Mass{5.4}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{5, 2, 3}, speed3f{4, 5, 5}, Mass{5.5}, Counter{0});

    ecs.AddEntity(1, 1.1, Counter{0});
    ecs.AddEntity(2, 2.2, Counter{0});

    EXPECT_EQ(ecs.Size(), 7);
}

TEST(ECS, DeleteEntity)
{
  ECS<gmeta::types_t<
        Archetype<32, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>,
    gmeta::types_t<
        test::UpdatePositionSystem,
        test::UpdateSpeedSystem,
        test::UpdateCounterSystem>>
    ecs;

    ecs.AddEntity(position3f{1, 2, 3}, speed3f{4, 5, 1}, Mass{5.1}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{2, 2, 3}, speed3f{4, 5, 2}, Mass{5.2}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{3, 2, 3}, speed3f{4, 5, 3}, Mass{5.3}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{4, 2, 3}, speed3f{4, 5, 4}, Mass{5.4}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{5, 2, 3}, speed3f{4, 5, 5}, Mass{5.5}, Counter{0});

    ecs.AddEntity(1, 1.1, Counter{0});
    ecs.AddEntity(2, 2.2, Counter{0});

    ecs.DeleteEntity(0);
    ecs.DeleteEntity(2);
    ecs.DeleteEntity(4);

    EXPECT_EQ(ecs.Size(), 4);
}

TEST(ECS, foreach)
{
  ECS<gmeta::types_t<
        Archetype<32, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>,
    gmeta::types_t<
        test::UpdatePositionSystem,
        test::UpdateSpeedSystem,
        test::UpdateCounterSystem>>
    ecs;

    ecs.AddEntity(position3f{1, 2, 3}, speed3f{41, 0, 1}, Mass{5.1}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{2, 2, 3}, speed3f{42, 1, 2}, Mass{5.2}, Counter{1});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{3, 2, 3}, speed3f{43, 2, 3}, Mass{5.3}, Counter{2});

    ecs.AddEntity(1, 1.1, Counter{3});
    ecs.AddEntity(2, 2.2, Counter{4});

    int foreach_counter = 0;
    ecs.foreach([&foreach_counter](Id id, position3f const pos, speed3f const& speed)
    {
      foreach_counter++;
      if (id == 0)
      {
        EXPECT_EQ(pos.x, 1);
        EXPECT_EQ(speed.x, 41);
      }
      else if (id == 1)
      {
        EXPECT_EQ(pos.x, 2);
        EXPECT_EQ(speed.x, 42);
      }
      if (id == 2)
      {
        EXPECT_EQ(pos.x, 3);
        EXPECT_EQ(speed.x, 43);
      }
    }, DeltaTime{0});

    EXPECT_EQ(foreach_counter, 3);

    ecs.foreach([](speed3f& speed)
    {
      speed.y += 1;
    }, DeltaTime{0});

    ecs.foreach([](speed3f const speed, Id const& id)
    {
      if (id == 0)
      {
        EXPECT_EQ(speed.y, 1);
      }
      else if (id == 1)
      {
        EXPECT_EQ(speed.y, 2);
      }
      if (id == 2)
      {
        EXPECT_EQ(speed.y, 3);
      }
    }, DeltaTime{0});

    ecs.foreach([](Id id, Counter counter)
    {
      EXPECT_EQ(counter, id);
    }, DeltaTime{0});

    ecs.DeleteEntity(3);
    ecs.DeleteEntity(4);

    int entities_with_int_counter = 0;
    ecs.foreach([&entities_with_int_counter](int x)
    {
      entities_with_int_counter += 1;
    }, DeltaTime{0});
    EXPECT_EQ(entities_with_int_counter, 0);

}

TEST(ECS, Compress)
{
    ECS<gmeta::types_t<
        Archetype<1, position3f, speed3f, Mass, Counter>,
        Archetype<2, int, double, Counter>>,
    gmeta::types_t<
        test::UpdatePositionSystem,
        test::UpdateSpeedSystem,
        test::UpdateCounterSystem>>
    ecs;

    ecs.AddEntity(position3f{1, 2, 3}, speed3f{4, 5, 1}, Mass{5.1}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{2, 2, 3}, speed3f{4, 5, 2}, Mass{5.2}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{3, 2, 3}, speed3f{4, 5, 3}, Mass{5.3}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{4, 2, 3}, speed3f{4, 5, 4}, Mass{5.4}, Counter{0});
    ecs.AddEntity<position3f, speed3f, Mass>(position3f{5, 2, 3}, speed3f{4, 5, 5}, Mass{5.5}, Counter{0});

    ecs.AddEntity(1, 1.1, Counter{0});
    ecs.AddEntity(2, 2.2, Counter{1});
    ecs.AddEntity(3, 3.3, Counter{2});

    ecs.DeleteEntity(0);
    ecs.DeleteEntity(2);
    ecs.DeleteEntity(4);
    ecs.DeleteEntity(5);

    std::size_t compressed = ecs.Compress();

    EXPECT_EQ(compressed, 2);
}