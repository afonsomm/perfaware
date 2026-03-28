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
je $+25+0
mov cx, di
xor dx, dx
xor ax, ax
xor di, di
add al, [bp + di]
add bl, [bp + di + 1]
add di, 2
cmp di, cx
jb $-10+0
add al, bl
ret
xor ax, ax
ret
