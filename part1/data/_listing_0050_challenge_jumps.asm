mov ax, 10
mov bx, 10
mov cx, 10
cmp bx, cx
je $+7+0
add ax, 1
jp $+7+0
sub bx, 5
jb $+5+0
sub cx, 2
loopne $-17+0
