
MAIN: mov S1.1 ,LENGTH
 add r2, STR
LENGTH: .data 6,-9,15
K: .data 22
LOOP: jmp END
macro m1
 LABEL: inc K
 mov S1 ,#4
.entry MAIN
endmacro
 prn #-5
 sub r1, r4
m1
 bne LOOP
END: stop
STR: .string "abcdef"

S1: .string "8, ab" 