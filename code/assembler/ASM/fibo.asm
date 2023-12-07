
; fibonnaci nth term

data a 0
data b 1
data sum 0
data nth 11


.loop
    lda a
    add b
    str sum  ; sum = a + b   
    lda b
    str a    ; a = b
    lda sum
    str b    ; b = sum
    lda nth
    subi 1
    str nth 
    jgz loop
    ;;
lda b 
hlt 0
