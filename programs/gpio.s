mov r1, #0x2000
add r1, r1, #0x20
lsl r1, #0x10
mov r4, #0x4
mov r0, #0x0
mov r5, r2
lsl r5, #0x10

L1:
mov r4, #0x28
str r0, [r1, r4]
mov r4, #0x1C
str r5, [r1, r4]

mov r3,
lsl r3, #0x19

L2:
sub r3, r3, #0x1
cmp r3, #0x0
bne L2

mov r4, #0x1C
str r0, [r1, r4]
mov r4, #0x28
str r5, [r1, r4]

mov r3, r2
lsl r3, #0x19

L3:
sub r3, r3, #0x1
cmp r3, #0x0
bne L3

b L1