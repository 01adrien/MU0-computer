
;; y < x ? 1 : 0

; push constant 15 => x
ldi 10
sta SP
lda SP
add ONE
str SP

; push constant 45 => y
ldi 5
sta SP
lda SP
add ONE
str SP

; pop and store in R0 => y
lda SP
sub ONE
str SP
lea SP
str R0

; pop and store in R1 => x
lda SP
sub ONE
str SP
lea SP
str R1

; compare R0 - R1 => y - x
lda R0
sub R1

jlz lower
ldi 0
jmp end 

.lower
ldi 1

.end
sta SP
lda SP
add ONE
str SP

; 
hlt 0
