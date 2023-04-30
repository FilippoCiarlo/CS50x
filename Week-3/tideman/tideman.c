#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // cycle through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // if found the candidate
        if (strcasecmp(candidates[i], name) == 0)
        {
            // update the rank
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count ; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // update the number of voters who
            // prefer candidate i over candidate j
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // if the candidate i has more
            // voters than candidate j
            if (preferences[i][j] > preferences[j][i])
            {
                // add the pair (i=winner;j=loser)
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        bool swapped = false;

        // compute the stregth of the vitory for the first pair
        int w1 = pairs[i].winner;
        int l1 = pairs[i].loser;
        int strength1 = preferences[w1][l1] - preferences[l1][w1];


        for (int j = 1; j < pair_count; j++)
        {
            // compute the stregth of the vitory for the second pair
            int w2 = pairs[j].winner;
            int l2 = pairs[j].loser;
            int strength2 = preferences[w2][l2] - preferences[l2][w2];

            if (strength2 > strength1)
            {
                // swap
                pair tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // cycle through all candidates
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        while (winner != -1 && winner != loser)
        {
            bool found = false;

            // cycle through all candidates
            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[j][winner])
                {
                    found = true;
                    winner = j;
                }
            }
            if (found == false)
            {
                winner = -1;
            }
        }

        // if find an edge lock it in
        if (winner != loser)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // cycle through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        bool winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            // it's not a winner
            if (locked[j][i] == true)
            {
                winner = false;
                break;
            }
        }
        // if is a winner print it
        if (winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}