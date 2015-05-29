.sample program for SIC/XE architecture
.
.This program should assemble correctly
.
prog    start   $1000    .start of sample program
        lds     #3
        ldt     #300
        ldx     #0
		lda     alpha,x .comment    
        add     beta,x
        sta     gamma,x                         
        addr    s,x
        compr   x,t
        jlt     addlp
number	EQU	4230
        .
. storage allocation section        
alpha   resw    100    .test
beta    resw    100
gamma   resw    100   
        end     prog    .end of sample program
