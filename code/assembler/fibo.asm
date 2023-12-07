
; fibonnaci nth term

data 0 20   ; num1
data 1 21   ; num2
data 0 22   ; sum
data 11 23  ; term
data 1 24   ; one

lda 20
add 21
str 22
lda 21
str 20
lda 22
str 21
lda 23
sub 24
str 23 
jgz 0
lda 21 
hlt 0
