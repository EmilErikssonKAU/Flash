#include "levenshtein/levenshtein_cluster.h"
#include "levenshtein/levenshtein.h"
#include <string.h>

int find_most_levenshtein(char *words[], int n)
{
    if (n < 2) return -1;

    const char *query = words[0];
    int query_len = strlen(query);
    int threshold = query_len / 3;
    if (threshold < 2) threshold = 2;

    int best_index = -1;
    int best_dist = threshold + 1;

    for (int i = 1; i < n; i++)
    {
        int dist = levenshtein(query, words[i]);
        if (dist < best_dist)
        {
            best_dist = dist;
            best_index = i;
        }
    }

    return best_index;
}