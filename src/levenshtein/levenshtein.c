#include "levenshtein/levenshtein.h"
#include <stdlib.h>
#include <string.h>

static int min(int a, int b, int c)
{
    int m = a < b ? a : b;
    return m < c ? m : c;
}

int levenshtein(const char *s1, const char *s2)
{
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    int **dp = malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; i++)
    {
        dp[i] = malloc((len2 + 1) * sizeof(int));
    }

    for (int i = 0; i <= len1; i++)
        dp[i][0] = i;

    for (int j = 0; j <= len2; j++)
        dp[0][j] = j;

    for (int i = 1; i <= len1; i++)
    {
        for (int j = 1; j <= len2; j++)
        {

            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            dp[i][j] = min(
                dp[i - 1][j] + 1,       // deletion
                dp[i][j - 1] + 1,       // insertion
                dp[i - 1][j - 1] + cost // substitution
            );
        }
    }

    int distance = dp[len1][len2];

    for (int i = 0; i <= len1; i++)
        free(dp[i]);
    free(dp);

    return distance;
}
