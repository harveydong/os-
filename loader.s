section loader vstart=0x900

	jmp loader_start

	gdt_base:	dd	0x00000000
			dd 	0x00000000	

	code_base:	dd	0x0000ffff
			dd	0x00cf9800	
	
	data_stack_base: dd	0x0000ffff
			 dd 	0x00cf9200

	video_base:	dd	0x80000007
			dd	0x00c0920b

	gdt_size	equ	$ - gdt_base

	gdt_limit	equ	gdt_size - 1

	times 60 dq 0

	sel_code	equ	0x0008
	sel_data	equ	0x0010
	sel_video	equ	0x0018

	total_mem_size	dd	0

	gdt_ptr		dw	gdt_limit	
			dd	gdt_base


	ards_buf	times 244 db 0
	ards_nr		dw	0

	page_dir_table	equ	0x100000

loader_start:

	mov byte [gs:0x30],'S'
	mov byte [gs:0x31],0xa4
	xor ebx,ebx
	mov edx,0x534d4150
	mov di,ards_buf
.e820_mem_get:
	mov eax,0x0000e820
	mov ecx,20
	int 0x15
	jc .e820_failed_try_e801

	add di,cx
	inc word [ ards_nr]	
	cmp ebx,0
	jnz .e820_mem_get

	mov cx,[ards_nr]
	mov ebx,ards_buf
	xor edx,edx
.find_max_mem_area:
	mov eax,[ebx]
	add eax,[ebx+8]
	add ebx,20

	cmp edx,eax
	jge .next_ards
	mov edx,eax

.next_ards:
	loop .find_max_mem_area
	jmp .mem_get_ok

.e820_failed_try_e801:
	mov ax,0xe801
	int 0x15
	jc .e801_failed_try_88
	
	mov cx,0x400
	mul cx
	shl edx,16
	and eax,0x0000FFFF

	or edx,eax
	add edx,0x100000
	mov esi,edx
	
	xor eax,eax
	mov ax,bx
	mov ecx,0x10000
	mul ecx
	add esi,eax
	mov edx,esi
	jmp .mem_get_ok

.e801_failed_try_88:
	mov ah,0x88
	int 0x15
	jc .error_hlt
	and eax,0x0000FFFF
	mov cx,0x400
	mul cx
	shl edx,16
	or edx,eax
	add edx,0x100000
	
.mem_get_ok:
	mov [total_mem_size],edx	
	mov byte [gs:0x20],'Y'
	mov byte [gs:0x21],0xa4

	in al,0x92
	or al,0x02
	out 0x92,al

	lgdt [gdt_ptr]

	mov eax,cr0
	or eax,0x00000001
	mov cr0,eax

	jmp dword sel_code:p_mode

.error_hlt:
	hlt

[bits 32]

p_mode:
	mov ax,sel_data
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov esp,0x900
	mov ax,sel_video
	mov gs,ax

	mov byte [gs:160],'t'
	mov byte [gs:161],0xa4
	mov byte [gs:162],'w'
	mov byte [gs:163],0xa4
	mov byte [gs:164],'o'
	mov byte [gs:165],0xa4


	mov eax,0x09
	mov ebx,0x70000
	mov ecx,200
	
	call rd_disk_m_32
	call setup_page
	
	sgdt [gdt_ptr]
	mov ebx,[gdt_ptr + 2]
	or dword [ebx + 0x18 + 4],0xc0000000
	
	add dword [gdt_ptr + 2],0xc0000000
	add esp,0xc0000000
	
	mov eax,page_dir_table
	mov cr3,eax

	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	lgdt [gdt_ptr]

	jmp sel_code:enter_kernel

enter_kernel:

	call kernel_init
	
	mov esp,0xc0009f00

	mov byte [gs:0x168],'K'
	mov byte [gs:0x169],0xa4

	jmp 0xc0001500
	jmp $


kernel_init:
	xor eax,eax
	xor ebx,ebx
	xor ecx,ecx
	xor edx,edx
	
	mov dx,[0x70000 +42]
	mov ebx,[0x70000 + 28]
	add ebx,0x70000
	mov cx,[0x70000 + 44]

.each_segment:
	cmp byte [ebx+0],0
	je .PTNULL

	push dword [ebx+16]
	mov eax,[ebx+4]
	add eax,0x70000
	push eax
	push dword [ebx+8]
	call mem_cpy
	add esp,12

.PTNULL:
	add ebx,edx
	loop .each_segment
	ret

mem_cpy:
	cld
	push ebp
	mov ebp ,esp
	push ecx
	mov edi,[ebp+8]
	mov esi,[ebp+12]
	mov ecx,[ebp+16]
	rep movsb
	
	pop ecx
	pop ebp
	ret

setup_page:
	mov ecx,4096
	mov esi,0
.clear_page_dir:
	mov byte [ page_dir_table + esi],0
	inc esi
	loop .clear_page_dir


.create_pde:
	mov eax,page_dir_table
	add eax,0x1000
	mov ebx,eax


	or eax,0x7
	mov [page_dir_table + 0x0],eax
	mov [page_dir_table + 0xc00],eax
	
	sub eax,0x1000
	mov [page_dir_table + 4092],eax

	mov ecx,256
	mov esi,0
	mov edx,0x7

.create_pte:
	mov [ebx + esi*4],edx
	add edx,4096
	inc esi
	loop .create_pte

	
	mov eax,page_dir_table
	add eax,0x2000
	or eax,0x7
	mov ebx,page_dir_table
	mov ecx,254
	mov esi,769

.create_kernel_pde:
	mov [ebx + esi*4],eax
	inc esi
	add eax,0x1000
	loop .create_kernel_pde
	ret

rd_disk_m_32:
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
	mov [ebx],ax
	add ebx,2
	loop .go_on_read
	ret
	
