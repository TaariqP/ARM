//
// Created by taariq on 5/22/19.
//

#include <stdint.h>
#include "defs.h"
#include "utils.h"

void decode_dpi(current_state *state){
    uint32_t value = state->fetched_instruction.binary_value;
    uint8_t cond = mask_4_bit(value, 4);
}
