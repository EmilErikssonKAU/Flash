#include "redir.h"
#include "ast/ast.h"
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

static int kind_to_fd(RedirKind kind)
{
    switch (kind)
    {
    case REDIR_STDOUT:
        return STDOUT_FILENO;
    case REDIR_FD1:
        return STDOUT_FILENO;
    case REDIR_FD2:
        return STDERR_FILENO;
    default:
        return -1;
    }
}

// saved_fds[0] = saved stdout (use -1 if not saved)
// saved_fds[1] = saved stderr (use -1 if not saved)
bool apply_redirs(const RedirVec *redirs, int stored_fds[2], bool save_fds)
{
    for (size_t i = 0; i < redirs->n; i++)
    {
        Redir redir = redirs->v[i];
        int target_fd = kind_to_fd(redir.kind);

        if (save_fds)
        {
            if (target_fd == STDOUT_FILENO)
            {
                stored_fds[0] = dup(target_fd);
            }
            else if (target_fd == STDERR_FILENO)
            {
                stored_fds[1] = dup(target_fd);
            }
        }

        int fd = open(redir.target, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd, target_fd);
        close(fd);
    }

    return true;
}

void restore_redirs(int stored_fds[2])
{
    if (stored_fds[0] != -1)
    {
        dup2(stored_fds[0], STDOUT_FILENO);
        close(stored_fds[0]);
    }
    if (stored_fds[1] != -1)
    {
        dup2(stored_fds[1], STDOUT_FILENO);
        close(stored_fds[1]);
    }
}