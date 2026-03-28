mov di, 8
mov bp, 1000
mov byte [bp], 9
mov byte [bp + 1], 17
mov byte [bp + 2], 23
mov byte [bp + 3], 4
mov byte [bp + 4], 27
mov byte [bp + 5], 41
mov byte [bp + 6], 39
mov byte [bp + 7], 31
test di, di
je $+21+0
mov cx, di
xor si, si
xor ax, ax
add al, [bp + si]
add al, [bp + si + 1]
add si, 2
cmp si, cx
jb $-10+0
ret
xor ax, ax
ret
