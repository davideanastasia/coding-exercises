/*
 * KiloManX - TopCoder
 *
 * Full description here:
 * http://community.topcoder.com/stat?c=problem_statement&pm=2288&rd=4725
 *
 * Interesting problem for the usage of priority_queue and Dijkstra algorithm
 *
 * Solution described at:
 * http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=graphsDataStrucs3
 *
 */

#include <vector>
#include <iostream>
#include <queue>
#include <iterator>

#include <gtest/gtest.h>

using namespace std;

struct Node
{
    Node(int weapons, int shots)
        : weapons_(weapons)
        , shots_(shots)
    {}

    int weapons_;
    int shots_;
};

// reverse condition to put on top of the priority queue the minimum
bool operator<(const Node& n1, const Node& n2)
{
    if (n1.shots_ < n2.shots_) return false;
    return true;
}

int leastShots(const vector<string>& damageChart, const vector<int>& bossHealth)
{
    const int numWeapons = bossHealth.size();
    vector<bool> visited(32768, false);

    priority_queue<Node> pq;
    pq.push( Node(0, 0) );

    while ( !pq.empty() )
    {
        Node top = pq.top();
        pq.pop();

        // Make sure we don't visit the same configuration twice
        if ( visited[top.weapons_] ) continue;
        visited[top.weapons_] = true;

        // A quick trick to check if we have all the weapons, meaning we
        // defeated all the bosses.
        // We use the fact that (2^numWeapons - 1) will have all the numWeapons
        // bits set to 1.
        if ( top.weapons_ == (1 << numWeapons) - 1) {
            return top.shots_;
        }

        for (int i = 0; i < damageChart.size(); ++i)
        {
            if ( (top.weapons_ >> i) & 1 ) continue;

            // Now figure out what the best amount of time that we can destroy
            // this boss is, given the weapons we have.
            // We initialize this value to the boss's health, as that is our
            // default (with our KiloBuster).
            int best = bossHealth[i];
            for (int j = 0; j < damageChart.size(); ++j)
            {
                if (i == j) continue;
                if ( ((top.weapons_ >> j) & 1) &&       // If I've got this weapon
                     (damageChart[j][i] != '0') )       // and this weapon has power
                {
                    // We have this weapon, so try using it to defeat this boss
                    int shotsNeeded = bossHealth[i] / (damageChart[j][i] - '0');
                    if (bossHealth[i] % (damageChart[j][i] - '0') != 0) {
                        shotsNeeded++;
                    }
                    best = min(best, shotsNeeded);
                }
            }

            // Add the new node to be searched, showing that we defeated boss i,
            // and we used 'best' shots to defeat him.
            pq.push( Node(top.weapons_ | (1 << i), top.shots_ + best));
        }
    }
    return -1;
}

TEST(KiloManX, Test0)
{
    vector<string> damageChart(3);
    damageChart[0] = "070";
    damageChart[1] = "500";
    damageChart[2] = "140";

    vector<int> bossHealth(3);
    bossHealth[0] = 150;
    bossHealth[1] = 150;
    bossHealth[2] = 150;

    ASSERT_EQ( leastShots(damageChart, bossHealth), 218);
}

TEST(KiloManX, Test1)
{
    vector<string> damageChart(4);
    damageChart[0] = "1542";
    damageChart[1] = "7935";
    damageChart[2] = "1139";
    damageChart[3] = "8882";

    vector<int> bossHealth(4);
    bossHealth[0] = 150;
    bossHealth[1] = 150;
    bossHealth[2] = 150;
    bossHealth[3] = 150;

    ASSERT_EQ( leastShots(damageChart, bossHealth), 205);
}

TEST(KiloManX, Test2)
{
    vector<string> damageChart(2);
    damageChart[0] = "07";
    damageChart[1] = "40";

    vector<int> bossHealth(2);
    bossHealth[0] = 150;
    bossHealth[1] = 10;

    ASSERT_EQ( leastShots(damageChart, bossHealth), 48);
}
