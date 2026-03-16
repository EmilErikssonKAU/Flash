#include "exec/exec.h"
#include "builtin/builtin.h"
#include "exec/path.h"
#include "exec/redir.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

static ExecResult make_result(int status, bool shouldExit)
{
    ExecResult result;
    result.status = status;
    result.shouldExit = shouldExit;
    return result;
}

static int wait_for_children(pid_t *pids, size_t n)
{
    int last_status = 0;

    for (size_t i = 0; i < n; i++)
    {
        int wait_status = 0;
        waitpid(pids[i], &wait_status, 0);
        if (i == n - 1)
        {
            if (WIFEXITED(wait_status))
            {
                last_status = WEXITSTATUS(wait_status);
            }
            else
            {
                last_status = 1;
            }
        }
    }

    return last_status;
}

ExecResult exec_pipeline(AstPipeline *pipeline)
{
    // Command could not be parsed
    if (pipeline == NULL)
    {
        return make_result(0, false);
    }

    if (pipeline->commands == NULL || pipeline->n == 0)
    {
        return make_result(0, false);
    }

    // No pipeline
    if (pipeline->n == 1)
    {
        return exec_simple_command(&pipeline->commands[0]);
    }

    pid_t *pids = calloc(pipeline->n, sizeof(pid_t));
    if (pids == NULL)
    {
        return make_result(1, false);
    }

    int prev_read_fd = -1;
    size_t started = 0;

    for (size_t i = 0; i < pipeline->n; i++)
    {
        AstCommand *cmd = &pipeline->commands[i];
        bool has_next = (i + 1 < pipeline->n);

        // fd[0]: read, fd[1]: write
        int pipe_fd[2] = {-1, -1};
        if (has_next && pipe(pipe_fd) < 0)
        {
            if (prev_read_fd != -1)
            {
                close(prev_read_fd);
            }
            int status = wait_for_children(pids, started);
            free(pids);
            return make_result(status, false);
        }

        pid_t pid = fork();

        // Failure safe-guard
        if (pid < 0)
        {
            if (prev_read_fd != -1)
            {
                close(prev_read_fd);
            }
            if (has_next)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            int status = wait_for_children(pids, started);
            free(pids);
            return make_result(status, false);
        }

        // In child process
        else if (pid == 0)
        {
            // if prev pipe, replace child stdin with prev read-end
            if (prev_read_fd != -1)
            {
                dup2(prev_read_fd, STDIN_FILENO);
                close(prev_read_fd);
            }
            // if another command, replace stdout with write-end
            if (has_next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            apply_redirs(&cmd->redirs, NULL, false);

            char **argv = cmd->argv.v;
            size_t argc = cmd->argv.n;
            if (argv == NULL || argc == 0 || argv[0] == NULL)
            {
                _exit(0);
            }

            // if builtin-command
            if (isBuiltIn(argv[0]))
            {
                bool ok = executeBuiltIn((int)argc, argv, &cmd->redirs);
                _exit(ok ? 0 : 1);
            }

            // if command can't be found
            if (!checkPath(argv[0], false))
            {
                fprintf(stderr, "%s: command not found\n", argv[0]);
                _exit(127);
            }

            // if command in path
            execvp(argv[0], argv);
            _exit(1);
        }

        // In parent process
        pids[started++] = pid;

        if (prev_read_fd != -1)
        {
            close(prev_read_fd);
        }

        if (has_next)
        {
            close(pipe_fd[1]);
            prev_read_fd = pipe_fd[0];
        }
        else
        {
            prev_read_fd = -1;
        }
    }

    if (prev_read_fd != -1)
    {
        close(prev_read_fd);
    }

    int status = wait_for_children(pids, started);
    free(pids);

    return make_result(status, false);
}
