//
// Created by 吴凡 on 2018/7/19.
//
#include "groupedlinkedmap.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace Catch;
using GLM = glm::GroupedLinkedMap<int, std::string>;

TEST_CASE("Empty", "[GroupedLinkedMap]") {
  GLM m;

  SECTION("IsEmpty") {
    REQUIRE(m.IsEmpty());
  }

  SECTION("Contains") {
    REQUIRE_FALSE(m.Contains(0));
  }

  SECTION("Get") {
    REQUIRE_THROWS_AS(m.Get(0), std::out_of_range);
  }

  SECTION("Put then Get") {
    m.Put(1, "a");
    REQUIRE_THAT(m.Get(1), Equals("a"));
    REQUIRE(m.IsEmpty());
  }

  SECTION("Remove last") {
    REQUIRE_THROWS_AS(m.RemoveLast(), std::out_of_range);
  }
}

TEST_CASE("Has one value", "[GroupedLinkedMap]") {
  GLM m;
  m.Put(1, "a");

  SECTION("IsEmpty") {
    REQUIRE_FALSE(m.IsEmpty());
  }

  SECTION("Contains") {
    REQUIRE_FALSE(m.Contains(2));
    REQUIRE(m.Contains(1));
  }

  SECTION("Get") {
    SECTION("Key not matches") {
      REQUIRE_THROWS_AS(m.Get(2), std::out_of_range);
    }

    SECTION("Key matches") {
      REQUIRE_THAT(m.Get(1), Equals("a"));
      REQUIRE(m.IsEmpty());
    }

    SECTION("Get then put") {
      m.Get(1);
      REQUIRE(m.IsEmpty());

      m.Put(1, "a");
      REQUIRE_FALSE(m.IsEmpty());
    }

    SECTION("Put same key") {
      m.Put(1, "b");
      REQUIRE_THAT(m.Get(1), Equals("b"));
      REQUIRE_FALSE(m.IsEmpty());
    }
  }

  SECTION("Remove last") {
    REQUIRE_THAT(m.RemoveLast(), Equals("a"));
    REQUIRE(m.IsEmpty());
    REQUIRE_FALSE(m.Contains(1));
  }
}

TEST_CASE("Has more values", "[GroupedLinkedMap]") {
  GLM m;
  m.Put(1, "a");
  m.Put(1, "b");
  m.Put(1, "c");

  SECTION("IsEmpty") {
    REQUIRE_FALSE(m.IsEmpty());
  }

  SECTION("Get") {
    REQUIRE_THAT(m.Get(1), Equals("c"));
    REQUIRE_THAT(m.Get(1), Equals("b"));
    REQUIRE_THAT(m.Get(1), Equals("a"));
    REQUIRE(m.IsEmpty());
  }

  SECTION("Put") {
    SECTION("Same key") {
      m.Put(1, "d");
      REQUIRE_THAT(m.Get(1), Equals("d"));
    }

    SECTION("Different key") {
      m.Put(2, "h");
      REQUIRE_THAT(m.Get(2), Equals("h"));
      REQUIRE_FALSE(m.Contains(2));
    }
  }

  SECTION("Remove last") {
    REQUIRE_THAT(m.RemoveLast(), Equals("c"));
    REQUIRE_THAT(m.RemoveLast(), Equals("b"));
    REQUIRE_THAT(m.RemoveLast(), Equals("a"));
    REQUIRE(m.IsEmpty());
  }
}

TEST_CASE("LRU", "[GroupedLinkedMap]") {
  GLM m;
  m.Put(1, "a");
  m.Put(1, "b");
  m.Put(2, "c");
  m.Put(3, "d");
  m.Put(3, "f");

  SECTION("Last") {
    REQUIRE_THAT(m.RemoveLast(), Equals("f"));
  }

  SECTION("After Get") {
    m.Get(3); // key 3 left 1 value "d", and key 3 is head

    REQUIRE_THAT(m.RemoveLast(), Equals("c"));
    REQUIRE_FALSE(m.Contains(2));

    // Remove key 1
    m.RemoveLast();
    m.RemoveLast();
    REQUIRE_FALSE(m.Contains(2));

    // Left key 3
    REQUIRE_THAT(m.RemoveLast(), Equals("d"));

    // Empty
    REQUIRE(m.IsEmpty());
  }
}