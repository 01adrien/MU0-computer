
; push constant 9
ldi 9
sta SP
lda SP
add ONE
str SP

; push constant 1
ldi 1
sta SP
lda SP
add ONE
str SP

; push constant 8
ldi 8
sta SP
lda SP
add ONE
str SP

; add
lda SP
sub ONE
str SP
lea SP
str R0
lda SP
sub ONE
str SP
lea SP
add R0
sta SP
lda SP
add ONE
str SP

; eq
lda SP
sub ONE
str SP
lea SP
str R0
lda SP
sub ONE
str SP
lea SP
str R1
lda R0
sub R1
jez true#0
ldi 0
jmp end#0
.true#0
ldi 1
.end#0
sta SP
lda SP
add ONE
str SP

hlt 0
