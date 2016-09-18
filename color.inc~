normal_color	equ	80000000h
bold_color	equ	40000000h
underline	equ	20000000h
blink		equ	10000000h
reverse		equ	08000000h

black_char	equ	30h	;builds 3330h
red_char	equ	31h	;builds 3331h
green_char	equ	32h
yellow_char	equ	33h
blue_char	equ	34h
magenta_char	equ	35h
cyan_char	equ	36h
white_char	equ	37h

on_black	equ	3000h	;builds '40'
on_red		equ	3100h
on_green	equ	3200h
on_yellow	equ	3300h	;builds '43'
on_blue		equ	3400h	;
on_magenta	equ	3500h
on_cyan		equ	3600h
on_white	equ	3700h

;--------------------------------------------------------
  [section .data]
;-- display section ------------
;------------------------------------------------------------------------
; colors = aaxxffbb  (aa-attribute ff-foreground  bb-background)
;   30-black 31-red 32-green 33-brown 34-blue 35-purple 36-cyan 37-grey
;   attributes 30-normal 31-bold 34-underscore 37-inverse

;norm_text_color	dd 30003734h ;used for inactive window
norm_text_color	dd normal_color+white_char+on_blue

;bold_edit_color	dd 31003734h ;used for active window in edit mode
bold_edit_color	dd bold_color+underline+white_char+on_blue

;bold_cmd_color	dd 31003334h ;used for active window in command mode
bold_cmd_color	dd bold_color+yellow_char+on_blue

;high_text_color	dd 31003634h ;used for highlighting block
high_text_color	dd bold_color+cyan_char+on_blue

;asm_text_color	dd 31003234h ;used to highlight comments ";"
asm_text_color	dd bold_color+green_char+on_blue

;status_color	dd 30003037h ;used for status line
status_color	dd normal_color+black_char+on_white

;status_color1	dd 30003137h ;used for special data on status line
status_color1	dd normal_color+red_char+on_white

;status_color2	dd 31003331h ;used for error messags or macro record
status_color2	dd bold_color+yellow_char+on_red

;exit_screen_color dd 31003334h ;used for error messags on status line
exit_screen_color dd bold_color+yellow_char+on_blue

status_line_colors:
		dd normal_color+white_char+on_blue	;spacer for menus
		dd normal_color+black_char+on_white	;color 1
		dd normal_color+blink+white_char+on_red	;color 2 (selected)
		dd normal_color+white_char+on_blue	;color 3 (messages)

;cursor_color	dd 30003137h
cursor_color	dd bold_color+blink+white_char+on_red
;----------------------------------------------------------  
  [section .text]
;-------------------------------
; input: eax = color (aa??ffbb) attribute,foreground,background
;        bl = column
;        bh = row
;        cl = ascii char
;
display_char_at:
;  push	ecx
  push	ecx
  push	ebx
  call	set_color
  pop	eax
  call	move_cursor
  pop	ecx

  cmp	cl,20h
  jae	dca_2			;jmp if possible alpha
  mov	cl,'?'
dca_2:
  cmp	cl,7eh
  jbe	dca_4			;jmp if legal alpha
  mov	cl,'?'
dca_4:
  mov	byte [char_out],cl
  mov	ecx,char_out		;display data
  mov eax, 0x4			; system call 0x4 (write)
  mov ebx, stdout		; file desc. is stdout
  mov	edx,1			;write one char
  int 0x80
;  pop	ecx
  ret
  
;------------------------

; input: edi = storage pointer
color0:
  mov	eax,[status_color]
  call	move_color
  ret

; input: edi = storage ptr
color1:
  mov	eax,[status_color1]
  call	move_color
  ret
;----------------------

;sets text color for blocks and windows, this includes
; highlighting for .asm comments also.
;  inputs: [showblock] - 0 if no blocks active
;          [ebp] - current display posn
;          [edi] - storage ptr
;          [blockbegin] - block start
;          [blockend] - block end
;          edx 0100,0000=use bold_text_color <- set by caller
;              0001,0000=current attr = text_color <- set by program
;              0002,0000=current attr = block_color <- set by program
;              0004,0000=asm highlight
;
check_color:
  cmp byte [showblock],0
  je isb_no_blk			;jmp if no block
  mov	esi,[editbuf_cursor_ptr]
  cmp	esi,[blockbegin]	;check if block grows up or down
  jb  growing_up
; block is normal, gowing down, display from blockbegin to cursor
  cmp	ebp,[blockbegin]
  jb	isb_no_blk		;jmp if display infront of block
  cmp	ebp,esi
  ja	isb_no_blk		;jmp if beyond block
  jmp	isb_in_blk
growing_up:			;growing up, display cursor to blockbegin
  cmp	ebp,esi
  jb	isb_no_blk		;jmp if display infront of block
  cmp	ebp,[blockbegin]
  ja	isb_no_blk		;jmp if beyond block
isb_in_blk:
  test	edx,00020000h		;check if block attr active
  jnz	isb_exita		;exit if in-block and attr set already
  and	edx,0ff00ffffh		;clear other attributes flags
  or	edx,00020000h
;
; move block attribute to buffer
;
  mov	eax,[high_text_color]
  call	move_color
isb_exita:
  jmp	isb_exit

isb_no_blk:
  test	edx,00020000h
  jz	isb_10			;jmp if not in blk and attr is not block
  and	edx,0ff00ffffh		;clear block active flag and text active flg
;
; block attribute not active and not inside block - asm check
;
isb_10:
  cmp	byte [show_asm],0
  je	isb_20			;jmp if asm comments not active
  mov	al,[ebp]		;get current char
  test	edx,00040000h		;check if comment active
  jz	isb_asm_no
;
; asm comments highlighted, wait for 0ah to disable
;
  cmp	al,0ah
  jne	isb_exit
  and	edx,0ff00ffffh		;clear block active flag and text active flg
  jmp	isb_20			;go set normal attr
;
; block not active, asm comments not active, check for ':'
;
isb_asm_no:
  cmp	al,[asm_comment_char]	;  ';'
  jne	isb_20			;go check for normal color state
  or	edx,00040000h		;set asm highlight active
  mov	eax,[asm_text_color]
  call	move_color
  jmp	isb_exit
isb_20:
  test	edx,00010000h		;check if text color attribute active
  jnz	isb_exit		;exit if text attribute set
  or	edx,00010000h		;set text attribute
;
; move normal attribute to buffer
;
  mov	eax,[bold_edit_color]
  cmp	byte [key_mode],1
  je	isb_24			;jmp if in command mode
  mov	eax,[bold_cmd_color]	;use edit color for non-command modes
isb_24:
  test	edx,01000000h
  jnz	isb_30			;jmp if bold text needed
  mov	eax,[norm_text_color]
isb_30:
  call	move_color
isb_exit:
  ret
;----------------------------
;
color_cursor:
  mov	eax,[cursor_color]	;get color (change to status_color)
  mov	bx,[crt_cursor]  
  mov	ecx,[editbuf_cursor_ptr]	;get ptr to data
  mov	cl,[ecx]		;get char under cursor
  cmp	cl,0ah
  je	cc_x05			;jmp   if 0ah
  cmp	cl,09h			;check if tab
  jne	cc_x10
cc_x05:
  mov	cl,' '
cc_x10:
  call	display_char_at	;display message
  call	hide_cursor
  ret  
;----------------------------
;
remove_color_cursor:
  mov	eax,[norm_text_color]	;get color (change to status_color)
  mov	bx,[crt_cursor]  
  mov	ecx,[editbuf_cursor_ptr]	;get ptr to data
  mov	cl,[ecx]		;get char under cursor
  cmp	cl,0ah
  je	rcc_x05			;jmp   if 0ah
  cmp	cl,09h			;check if tab
  jne	rcc_x10
rcc_x05:
  mov	cl,' '
rcc_x10:
  call	display_char_at	;display message
  call	hide_cursor
  ret  
;---------------------------------------
;input: eax=colors
;       edi=destination
;output: edi points to zero at end of color string
;
move_color:
  push	ecx
  push	esi
  push	edx
  push	edi
  mov	edx,eax
  call	build_color
  mov	esi,color_buf
  mov	ecx,edi		;get length
  sub	ecx,esi		;compute len
  dec	ecx
  pop	edi
  rep	movsb
  pop	edx
  pop	esi
  pop	ecx
  ret
;---------------------------------------
;input: edx=color
build_color:
  cld
  mov	edi,color_buf
  mov	al,1bh
  stosb
  mov	al,'['
  stosb
  push	edx
  or	edx,edx
  jns	sc_05
  mov	al,'0'
  stosb
  mov	al,';'
  stosb
sc_05:
  shl	edx,1
  jns	sc_10
  mov	al,'1'	;bold
  stosb
  mov	al,';'
  stosb
sc_10:
  shl	edx,1
  jns	sc_15
  mov	al,'4'	;underline
  stosb
  mov	al,';'
  stosb
sc_15:
  shl	edx,1
  jns	sc_20
  mov	al,'5'	;blink
  stosb
  mov	al,';'
  stosb
sc_20:
  shl	edx,1
  jns	sc_25
  mov	al,'7'	;reverse video
  stosb
  mov	al,';'
  stosb
sc_25:
  pop	edx
  or	dl,dl
  jz	sc_30
  mov	al,'3'	;char color
  stosb
  mov	al,dl
  stosb
  mov	al,';'
  stosb
sc_30:
  or	dh,dh
  jz	sc_35
  mov	al,'4'
  stosb
  mov	al,dh
  stosb
sc_35:
  cmp	byte [edi-1],';'
  jne	sc_40		;jmp if no ';' at end
  dec	edi
sc_40:
  mov	al,'m'
  stosb
  mov	al,0
  stosb
  ret
;-----------------------------------------------------
;------------------------------------------------
  [section .data]

color_buf	times 20 db 0

  [section .text]
;---------------------------------------
; input: color codes in eax
;
set_color:
  push	edx
  push	edi
  mov	edx,eax
  call	build_color
  push	esi
  mov	ecx,color_buf
  call	display_asciiz
  pop	esi
  pop	edi
  pop	edx
  ret
;-------------------------------
; input: eax = color (aa??ffbb) attribute,foreground,background
;        bl = column
;        bh = row
;       ecx = message ptr (asciiz)
;
display_color_at:
  push	ecx
  push	ebx
  call	set_color
  pop	eax
  call	move_cursor
  pop	ecx
  call	display_asciiz
  ret
;-----------------------------------
hide_cursor:
  mov	ah,[term_rows]
  mov	al,[term_columns]
  call	move_cursor
  ret  
