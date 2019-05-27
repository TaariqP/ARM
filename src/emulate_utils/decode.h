//
// Created by taariq on 5/23/19.
//

#ifndef ARM11_11_DECODE_H
#define ARM11_11_DECODE_H

#include "defs.h"

void decode_dpi(current_state *);

void decode_sdt(current_state *);

void decode_mul(current_state *);

void decode_branch(current_state *);

#endif //ARM11_11_DECODE_H
