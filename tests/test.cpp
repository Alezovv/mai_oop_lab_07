#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <fstream>

#include "npc.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"
#include "npc_factory.h"
#include "fight_visitor.h"
#include "dungeon.h"
#include "observer.h"

TEST(NPCFactoryTest, CreatesCorrectTypes)
{
    auto npc1 = NPCFactory::CreateNPC("druid", "Elf", 10, 10);
    EXPECT_EQ(npc1->getType(), "Druid");
    EXPECT_EQ(npc1->getName(), "Elf");

    auto npc2 = NPCFactory::CreateNPC("orc", "Zog", 20, 20);
    EXPECT_EQ(npc2->getType(), "Orc");

    auto npc3 = NPCFactory::CreateNPC("slaver", "Bob", 30, 30);
    EXPECT_EQ(npc3->getType(), "Slaver");
}

TEST(NPCFactoryTest, ThrowsOnUnknownType)
{
    EXPECT_THROW(NPCFactory::CreateNPC("alien", "X", 0, 0), std::runtime_error);
}

TEST(NPCTest, RollDice)
{
    Druid d("Test", 0, 0);
    int roll = d.rollDice();
    EXPECT_GE(roll, 1);
    EXPECT_LE(roll, 6);
}

TEST(NPCTest, CanKill)
{
    Druid d("D", 0, 0);
    Orc o("O", 0, 0);
    Slaver s("S", 0, 0);

    EXPECT_FALSE(d.canKill(o));
    EXPECT_TRUE(o.canKill(d));
    EXPECT_TRUE(s.canKill(d));
    EXPECT_FALSE(s.canKill(o));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}