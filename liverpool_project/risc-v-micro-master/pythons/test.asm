/*  # a

    This is a test

*/ /*program count from 0 to 10 and then to 0 and repeat

    addi x1 x0 10   # x1 = 10
    sd x1 x0 0      # mem(0) = 10
    add x1 x0 x0    # x1 = 0
    ld x1 x0 0      # x1 = mem(0)
    addi x2 x0 1    # x2 = 1

add:
    addi x2 x2 1    # x2++
    sd x2 x0 1      # mem(1) = x2
    bne x2 x1 add   # goto add if x2 != x1
sub:
    addi x2 x2 -1   # x2--
    
    bne x2 x0 sub   # goto sub if x2 != 0
    beq x2 x0 add   # goto add if x2 == 0
*/

/* This program test pipelined cpu */
/* data hazard 1a 
addi x1 x0 5
add x3 x1 x2
*/

/* data hazard 1a with immediate 
addi x1 x0 5
addi x3 x1 5
*/

/* data hazard 1b 
addi x1 x0 5
add x3 x2 x1
*/

/* data hazard all 
addi x1 x0 5    #   x1 = 5
add x2 x1 x0    #   x2 = 5
addi x3 x2 10   #   x3 = 15
addi x3 x3 10   #   x3 = 25
add x3 x3 x1    #   x3 = 30
*/

/* data hazard with store 
addi x1 x0 5
sd x1 x0 0
*/

/* memory copy 

addi x1 x0 10
sd x1 x0 0
ld x2 x0 0
sd x2 x0 2
sd x2 x0 3
*/

/* stalling 
addi x1 x0 10
sd x1 x0 0
ld x2 x0 0
addi x3 x2 10
sd x3 x0 1
ld x1 x0 1
sd x1 x0 2
*/


/* test control hazard 
addi x1 x0 0
L1:
addi x1 x1 10
sd x1 x0 0
ld x2 x0 0
beq x1 x2 L1
*/


/* test case 5 
addi x1 x0 10
sd x1 x0 0
*/

/* test case 6 
addi x1 x0 20
addi x1 x1 -5
sd x1 x0 0
*/

/* test case 7 
addi x1 x0 20   # x1 = 20
sd x1 x0 0      # mem(0) = 20
ld x2 x0 0      # x2 = 20
add x3 x2 x1    # x3 = 40
add x2 x2 x1    # x2 = 40
*/

/* test case 8 
addi x1 x0 10
sd x1 x1 0
*/

/* test case 9 
addi x1 x0 2
sd x1 x1 0
ld x2 x1 0
*/

/* test case 10 
addi x1 x1 2
sd x1 x0 0
ld x2 x0 0
sd x2 x0 1
*/

/* test case 11
L1: 
addi x1 x1 5
add x2 x1 x1
beq x0 x0 L1
*/

/* test case 12 
addi x1 x0 2
L1:
addi x2 x2 1
bne x1 x2 L1
addi x1 x0 10
*/

/* test case 13
L1:addi x1 x0 2
sd x1 x0 0
ld x2 x1 -2
beq x1 x2 L1
*/


/* test case 14
addi x1 x0 20
sd x1 x0 0
ld x2 x1 -20
L1: addi x2 x2 -1
bne x2 x0 L1
*/

/* test case 15 fibo
    addi x1 x0 10  # number
    addi x2 x0 0
    addi x3 x0 1
    addi x4 x0 1
    addi x5 x0 0
    addi x6 x0 2
    bne x1 x3 L1
L2: 
    addi x2 x0 1
    addi x5 x0 1
    beq x0 x0 L5
L1:
    bne x1 x6 L3
    addi x2 x0 1
    addi x5 x0 2
    beq x0 x0 L5
L3:
    addi x1 x1 -2
    addi x5 x5 2
L4:
    add x2 x3 x4
    add x5 x5 x2
    add x3 x0 x4
    add x4 x0 x2
    beq x1 x0 L5
    addi x1 x1 -1
    bne x1 x0 L4
L5:
    sd x5 x0 0

*/

/* combine test 1
addi x1 x0 28
addi x2 x0 3
sd x1 x0 1
sd x2 x0 2
ld x3 x0 3
*/

/* combine test uart
addi x1 x0 10
addi x2 x0 1
sd x1 x0 11
sd x2 x0 4
add x2 x0 x0
sd x2 x0 4
*/

/* testing gpio 
addi x1 x0 0    #0
addi x2 x0 6    #4
sd x1 x0 1      #8
addi x3 x0 0    #12
L1:
addi x3 x3 1    #16
sd x3 x0 2      #20
bne x3 x2 L1    #24
addi x3 x0 0    #28
sd x3 x0 2      #32
beq x0 x0 L1    #36
*/

/* testing gpio 
addi x1 x0 0    #0      set general io as output
addi x4 x0 6    #4      set threshold limit
sd x1 x0 1      #8
addi x3 x0 0    #12     set counter
L1:
addi x3 x3 1    #16
sd x3 x0 2      #20     write to general io
bne x4 x3 L1    #24     loop back if not threshold
addi x3 x0 0    #28     reset and loop
sd x3 x0 2      #32
beq x0 x0 L1    #36
*/

/* testing input and output 
addi x1 x0 16   # 0
sd x1 x0 1      # 4
L1:
ld x3 x0 3      # 8
and x4 x3 x1    # 12
beq x4 x1 L2    # 16
addi x5 x0 0    # 20
sd x5 x0 2      # 24
beq x0 x0 L1    # 28
L2:
addi x5 x0 1    # 32
sd x5 x0 2      # 36
beq x0 x0 L1    # 40
*/

/* testing uart */
addi x1 x0 0
sd x1 x0 4
addi x1 x0 682
sd x1 x0 11
addi x1 x0 1
sd x1 x0 4
addi x1 x0 0
sd x1 x0 4
L1:
beq x0 x0 L1


/* simulation testing 
addi x1 x0 3
addi x2 x0 1
addi x3 x0 1
addi x1 x1 -2
L1: add x4 x2 x3
add x2 x0 x3
add x3 x0 x4
addi x1 x1 -1
bne x1 x0 L1
*/