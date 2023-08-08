MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP
sub @r1, @r4
bne END
L1: inc K
bne LOOP
LENGTH: .data 6,-9,15
K: .data 22
LOOP: jmp L1
.entry LENGTH
.extern W
MAIN: mov @r3 ,LENGTH
mcro m2
    sub @r7, LENGTH
    bne STR
endmcro
