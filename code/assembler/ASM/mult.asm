data sum 0 
data a 5 
data b 6 

lda a
jez end
lda b
jez end
.loop
    lda sum
    add a
    str sum
    lda b
    subi 1
    str b
    jgz loop
    ;;
.end
    lda sum
    hlt 0