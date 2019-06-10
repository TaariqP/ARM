//
// Created by taariq on 5/23/19.
//

#ifndef ARM11_11_EXECUTE_H
#define ARM11_11_EXECUTE_H

#include "defs.h"

void execute_dpi(current_state *);

void execute_sdt(current_state *);

void execute_mul(current_state *);

void execute_branch(current_state *);

#endif //ARM11_11_EXECUTE_H
