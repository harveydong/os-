section mbr vstart=0x7c00
	mov ax,cs
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov ss,ax
	mov sp,0x7c00
	mov ax,0xb800
	mov gs,ax

	mov ax,0x0600
	mov bx,0x0700
	mov cx,0
	mov dx,0x184f

	int 0x10

	mov byte [gs:0x00],'l'
	mov byte [gs:0x01],0xa4
	
	mov byte [gs:0x02],'o'
	mov byte [gs:0x03],0xa4
	mov byte [gs:0x04],'a'
	mov byte [gs:0x05],0xa4
	
	mov byte [gs:0x06],'d'
	mov byte [gs:0x07],0xa4
	
	mov byte [gs:0x08],'i'
	mov byte [gs:0x09],0xa4
	mov byte [gs:0x0a],'n'
	mov byte [gs:0x0b],0xa4
	mov byte [gs:0x0c],'g'
	mov byte [gs:0x0d],0xa4

	mov byte [gs:0x0e],' '
	mov byte [gs:0x10],'l'
	mov byte [gs:0x11],0xa4
	mov byte [gs:0x12],'o'
	mov byte [gs:0x13],0xa4
	mov byte [gs:0x14],'a'
	mov byte [gs:0x15],0xa4
	mov byte [gs:0x16],'d'
	mov byte [gs:0x17],0xa4
	mov byte [gs:0x18],'e'
	mov byte [gs:0x19],0xa4
	mov byte [gs:0x1a],'r'
	mov byte [gs:0x1b],0xa4


	mov eax,0x2
	mov bx,0x900
	mov cx,4
	call rd_disk_m_16

	mov byte [gs:0x1e],'O'

	jmp 0x900

rd_disk_m_16:
	
	mov esi,eax
	mov di,cx
	mov dx,0x1f2
	mov al,cl
	out dx,al

	mov eax,esi
	mov dx,0x1f3
	out dx,al
	
	mov cl,8
	shr eax,cl
	mov dx,0x1f4
	out dx,al

	
	shr eax,cl
	mov dx,0x1f5
	out dx,al

	shr eax,cl
	and al,0x0f
	or al,0xe0
	mov dx,0x1f6
	out dx,al

	mov dx,0x1f7
	mov al,0x20
	out dx,al

.not_ready:
	nop
	in al,dx
	and al,0x88
	cmp al,0x08
	jnz .not_ready

	mov ax,di
	mov dx,256
	mul dx
	mov cx,ax
	mov dx,0x1f0

.go_on_read:
	in ax,dx
	mov [bx],ax
	add bx,2
	loop .go_on_read
	ret

	times 510 - ($ -$$) db 0
	db 0x55,0xaa
