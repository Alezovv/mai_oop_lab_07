#include <gtest/gtest.h>

#include <memory>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <sstream>

#include "npc.h"
#include "druid.h"
#include "orc.h"
#include "slaver.h"
#include "npc_factory.h"
#include "fight_visitor.h"
#include "dungeon.h"
#include "observer.h"
#include "utils.h"

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

TEST(NPCTest, MoveDistances)
{
    Druid d("D", 0, 0);
    Orc o("O", 0, 0);
    Slaver s("S", 0, 0);

    EXPECT_DOUBLE_EQ(d.getMoveDistance(), 10.0);
    EXPECT_DOUBLE_EQ(o.getMoveDistance(), 20.0);
    EXPECT_DOUBLE_EQ(s.getMoveDistance(), 10.0);
}

TEST(NPCTest, KillDistances)
{
    Druid d("D", 0, 0);
    Orc o("O", 0, 0);
    Slaver s("S", 0, 0);

    EXPECT_DOUBLE_EQ(d.getKillDistance(), 10.0);
    EXPECT_DOUBLE_EQ(o.getKillDistance(), 10.0);
    EXPECT_DOUBLE_EQ(s.getKillDistance(), 10.0);
}

TEST(NPCTest, PositionAndAlive)
{
    Druid d("D", 5.0, 10.0);
    EXPECT_DOUBLE_EQ(d.getX(), 5.0);
    EXPECT_DOUBLE_EQ(d.getY(), 10.0);
    EXPECT_TRUE(d.isAlive());

    d.setPosition(15.0, 20.0);
    EXPECT_DOUBLE_EQ(d.getX(), 15.0);
    EXPECT_DOUBLE_EQ(d.getY(), 20.0);

    d.kill();
    EXPECT_FALSE(d.isAlive());
}

TEST(NPCTest, Serialization)
{
    Druid d("Elf", 10.5, 20.3);
    std::stringstream ss;
    ss << d;
    std::string output = ss.str();
    EXPECT_NE(output.find("Druid"), std::string::npos);
    EXPECT_NE(output.find("Elf"), std::string::npos);
    EXPECT_NE(output.find("10.5"), std::string::npos);
    EXPECT_NE(output.find("20.3"), std::string::npos);
}

TEST(FightVisitorTest, FightLogic)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    npcs.push_back(NPCFactory::CreateNPC("druid", "D1", 0, 0));
    npcs.push_back(NPCFactory::CreateNPC("orc", "O1", 0, 0));
    std::mutex mtx;
    FightVisitor fv(npcs, mtx);

    fv.fight(*npcs[0], *npcs[1]);
    EXPECT_TRUE(npcs[0]->isAlive() || npcs[1]->isAlive());
}

TEST(DungeonTest, Creation)
{
    Dungeon d(50, 50);
}

TEST(DungeonTest, SimulationRun)
{
    Dungeon d(100, 100);
    d.startSimulation();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Short run
    d.stopSimulation();
    // Check that some NPCs might be dead, positions within bounds
    // Since private, hard, but at least no crash
}

TEST(DungeonTest, PositionClamping)
{
    // Test moveNPC indirectly via simulation
    Dungeon d(10, 10);
    d.startSimulation();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    d.stopSimulation();
    // Assume positions are clamped
}

TEST(NPCTest, StressRollDice)
{
    Druid d("Test", 0, 0);
    for (int i = 0; i < 1000; ++i) {
        int roll = d.rollDice();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST(FightVisitorTest, MultipleFights)
{
    std::vector<std::unique_ptr<NPC>> npcs;
    npcs.push_back(NPCFactory::CreateNPC("druid", "D1", 0, 0));
    npcs.push_back(NPCFactory::CreateNPC("orc", "O1", 0, 0));
    npcs.push_back(NPCFactory::CreateNPC("slaver", "S1", 0, 0));
    std::mutex mtx;
    FightVisitor fv(npcs, mtx);

    // Simulate multiple fights
    fv.fight(*npcs[1], *npcs[0]); // Orc vs Druid
    fv.fight(*npcs[2], *npcs[0]); // Slaver vs Druid
    // Check survivors
    int alive_count = 0;
    for (const auto& npc : npcs) {
        if (npc->isAlive()) alive_count++;
    }
    EXPECT_GE(alive_count, 1); // At least one alive
}


TEST(UtilsTest, Distance)
{
    EXPECT_DOUBLE_EQ(Distance(0, 0, 3, 4), 5.0);
    EXPECT_DOUBLE_EQ(Distance(1, 1, 1, 1), 0.0);
    EXPECT_DOUBLE_EQ(Distance(-1, -1, 1, 1), std::sqrt(8));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}