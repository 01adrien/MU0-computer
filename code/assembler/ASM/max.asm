
; Load max a, b in ACC 

data a 76
data b 50
data max 0

lda a
sub b
jgz maxA
lda b
str max
jmp end

addi 5
.maxA
    lda a
    str max

.end 
    lda max
    hlt 0