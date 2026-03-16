#pragma once
#include <stdbool.h>
#include "ast/ast.h"

bool apply_redirs(const RedirVec *redirs, int stored_fds[2], bool save_fds);
void restore_redirs(int stored_fds[2]);