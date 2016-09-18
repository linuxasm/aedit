

  [section .text]
;---------------------
; decode_key - look up processing for this key
;  input - kbuf - has char zero terminated
;  output - ecx has process ptr or zero if unknown key
;            NormChar - if alpha key in edit mode
;            (process from table)
decode_key:
  mov	esi,key_table1
  call	key_decode1	;retruns processs in eax
;eax= key process, alpha char process, no match process
  mov	ecx,eax
  jecxz	dk_exit
  cmp	eax,alpha_key
  jne	dk_exit
alpha_key:
  cmp	[key_mode],byte 0	;cmd mode
  je	dk_cmd_mode		;jmp if in CMD mode
  mov	ecx,NormChar
  jmp	short dk_exit
dk_cmd_mode:
  xor	ecx,ecx
  mov	cl,[kbuf]
  sub	cl,' '
  shl	ecx,2
  add	ecx,key_table2	;lookup processing
  mov	ecx,[ecx]	;get process
  jmp	short dk_exit
no_key_match:
  xor	ecx,ecx		;set no action (unknown key)
dk_exit:
  ret
;---------------------
tab_handler:
  cmp	[key_mode],byte 0	;cmd mode?
  je	cmd_tab
  jmp	NormChar		;edit mode action
cmd_tab:
  jmp	other
;---------------------
  extern is_alpha
;****f* key_mouse/key_decode1 *
;
; NAME
;>1 menu
;  key_decode1 - decode non-aplha key strings and get process
; INPUTS
;    kbuf - global library buffer with key string
;    esi = table of key strings and processes
;          first table entry is alpha key process
;          middle entries are non alpha keys
;          final process is called if no match
;    example
;    dd	gs_normal_char ;alpha key process
;    db 1bh,5bh,48h,0  ; pad_home
;    dd gs_home        ; home process 
;    db 1bh,5bh,44h,0  ; pad_left
;    dd gs_left        ; left arrow process
;    db 7fh,0          ; backspace
;    dd gs_backspace   ; backspace process
;    db 0              ;end of table
;    dd no_match       ;no-match process
;     
; OUTPUT
;    eax = process pointer
; NOTES
;   source file: key_decode.asm
;   see also crt_open, mouse_enable
;<
; * ----------------------------------------------
;*******
  global key_decode1
key_decode1:
  mov	al,[kbuf]
  call	is_alpha
  je	ka_exit		;jmp if alpha key
;
; key is not alpha, scan key strings
;
not_alpha:
  add	esi,4		;move past alpha process at top of table
ka_lp:
  mov	edi,kbuf
  cmpsb
  je	first_char_match
ka_10:
  lodsb
  or	al,al		;scan to end of table key string
  jnz	ka_10
  add	esi,4		;move past process
  cmp	byte [esi],0	;check if end of table
  je	get_process
  jmp	ka_lp
first_char_match:
  cmp	byte [esi],0	;check if all match
  jne	check_next
  cmp	byte [edi],0
  je	get_process
  jmp	ka_10		;go restart search
check_next:
  cmpsb
  je	first_char_match
  jmp	ka_10
get_process:
  inc	esi		;move past zero
ka_exit:
  lodsd			;get process
  ret



  [section .data]


key_table1:
  dd alpha_key
;--
  db 1bh,0			;1 esc
    dd set_cmd_mode

  db 1bh,5bh,31h,31h,7eh,0	;2 f1
    dd KeyHelp

;#  db 1bh,5bh,31h,32h,7eh,0	;3 f2
;#    dd f2_todo

  db 1bh,5bh,31h,33h,7eh,0	;4 f3
    dd f_make

;  db 1bh,5bh,31h,34h,7eh,0	;5 f4
;    dd f_bug

  db 1bh,5bh,31h,35h,7eh,0	;6 f5
    dd key_f5

  db 1bh,5bh,31h,37h,7eh,0	;7 f6
    dd key_f6

  db 1bh,5bh,31h,38h,7eh,0	;8 f7
    dd key_f7

;  db 1bh,5bh,31h,39h,7eh,0	;9 f8
;    dd key_f8

;  db 1bh,5bh,32h,30h,7eh,0	;10 f9
;    dd key_f9

  db 1bh,5bh,32h,31h,7eh,0	;11 f10
    dd key_fa

;  db 1bh,5bh,32h,33h,7eh,0	;12 f11
;    dd key_fb

;  db 1bh,5bh,32h,34h,7eh,0	;13 f12
;    dd key_fc

  db 1bh,5bh,48h,0		;14 pad_home
    dd KeyHome

  db 1bh,5bh,41h,0		;15 pad_up
    dd key_up

  db 1bh,5bh,35h,7eh,0		;16 pad_pgup
    dd KeyPgUp

  db 1bh,5bh,44h,0		;17 pad_left
    dd key_left

  db 1bh,5bh,43h,0		;18 pad_right
    dd key_right

  db 1bh,5bh,46h,0		;19 pad_end
    dd KeyEnd

  db 1bh,5bh,42h,0		;20 pad_down
    dd key_down

  db 1bh,5bh,36h,7eh,0		;21 pad_pgdn
    dd KeyPgDn

  db 1bh,5bh,32h,7eh,0		;22 pad_ins
    dd KeyIns

  db 1bh,5bh,33h,7eh,0		;23 pad_del
    dd KeyDel

  db 7fh,0			;24 backspace
    dd KeyDell

  db 1ah,0			;25 ctrl_z
    dd delete_line

  db 01h,0			;26 ctrl_a
    dd delete_right

  db 09h,0			;29 tab
    dd tab_handler

  db 1bh,4fh,50h,0		;123 F1
    dd KeyHelp

;#  db 1bh,4fh,51h,0		;123 F2
;#    dd f2_todo

  db 1bh,4fh,52h,0		;123 F3
    dd f_make

;  db 1bh,4fh,53h,0		;123 F4
;    dd f_bug

  db 1bh,5bh,5bh,41h,0		;127 F1
    dd KeyHelp

;#  db 1bh,5bh,5bh,42h,0		;128 f2
;#    dd f2_todo

  db 1bh,5bh,5bh,43h,0		;129 f3
    dd f_make

;  db 1bh,5bh,5bh,44h,0		;130 f4
;    dd f_bug

  db 1bh,5bh,5bh,45h,0		;131 f5
    dd key_f5

  db 0ah,0			;132 enter
    dd NormChar

  db 0dh,0
    dd NormChar

;  db 03h,0			;133 ctrl-c
;    dd no_key_match

  db 18h,0			;134 ctrl-x
    dd delete_left

  db 15h,0                      ;135 ctrl-u
    dd restore_line

  db 12h,0			;136 ctrl-r
    dd hex_input

;  db 21h,0			;137 explamation point

  db 1bh,5bh,31h,7eh,0		;138 home (non-keypad)
    dd KeyHome

  db 1bh,5bh,34h,7eh,0		;139 end (non-keypad)
    dd KeyEnd

  db 08,0			;140 backspace
    dd KeyDell

  db 1bh,4fh,78h,0		;141 pad_up
    dd key_up

  db 1bh,4fh,79h,0		;142 pad_pgup
    dd KeyPgUp

  db 1bh,4fh,74h,0		;143 pad_left
    dd key_left

  db 1bh,4fh,76h,0		;144 pad_right
    dd key_right

  db 1bh,4fh,71h,0		;145 pad_end
    dd KeyEnd

  db 1bh,4fh,72h,0		;146 pad_down
    dd key_down

  db 1bh,4fh,73h,0		;147 pad_pgdn
    dd KeyPgDn

  db 1bh,4fh,70h,0		;148 pad_ins
    dd KeyIns

  db 1bh,4fh,6eh,0		;149 pad_del
    dd KeyDel

  db 1bh,4fh,77h,0		;150 pad_home
    dd KeyHome

  db 1bh,4fh,42h,0		;151 pad down
    dd key_down

  db 1bh,4fh,41h,0		;152 pad up
    dd key_up

  db 1bh,4fh,43h,0		;153 pad right
    dd key_right

  db 1bh,4fh,44h,0		;154 pad left
    dd key_left

  db 1bh,4fh,48h,0		;155 pad home
    dd KeyHome

  db 1bh,4fh,46h,0		;156 pad end
    dd KeyEnd

  db 0		;end of table
;--
  dd no_key_match

key_table2:
 dd m1mode	;20 SP    (Space)
 dd 00000000	;21  !    (exclamation mark)
 dd 00000000	;22  "    (double quote)
 dd 00000000	;23  #    (number sign)
 dd 00000000	;24  $    (dollar sign)
 dd 00000000	;25  %    (percent)
 dd 00000000	;26  &    (ampersand)
 dd 00000000	;27  '    (single quote)
 dd 00000000	;28  (    (left opening parenthesis)
 dd 00000000	;29  )    (right closing parenthesis)
 dd 00000000	;2A  *    (asterisk)
 dd m_find_fwd	;2B  +    (plus)
 dd 00000000	;2C  ,    (comma)
 dd find_back	;2D  -    (minus or dash)
 dd 00000000	;2E  .    (dot)
 dd 00000000	;2F  /    (forward slash)
 dd 00000000	;30  0
 dd 00000000	;31  1
 dd 00000000	;32  2
 dd 00000000	;33  3
 dd 00000000	;34  4
 dd 00000000	;35  5
 dd 00000000	;36  6
 dd 00000000	;37  7
 dd 00000000	;38  8
 dd 00000000	;39  9
 dd 00000000	;3A  :    (colon)
 dd 00000000	;3B  ;    (semi-colon)
 dd 00000000	;3C  <    (less than sign)
 dd 00000000	;3D  =    (equal sign)
 dd 00000000	;3E  >    (greater than sign)
 dd 00000000	;3F  ?    (question mark)
 dd 00000000	;40  @    (AT symbol)
 dd 00000000	;41  A
 dd 00000000	;42  B
 dd 00000000	;43  C
 dd 00000000	;44  D
 dd 00000000	;45  E
 dd 00000000	;46  F
 dd 00000000	;47  G
 dd 00000000	;48  H
 dd 00000000	;49  I
 dd 00000000	;4A  J
 dd 00000000	;4B  K
 dd 00000000	;4C  L
 dd 00000000	;4D  M
 dd 00000000	;4E  N
 dd 00000000	;4F  O
 dd 00000000	;50  P
 dd 00000000	;51  Q
 dd 00000000	;52  R
 dd 00000000	;53  S
 dd 00000000	;54  T
 dd 00000000	;55  U
 dd 00000000	;56  V
 dd 00000000	;57  W
 dd 00000000	;58  X
 dd 00000000	;59  Y
 dd 00000000	;5A  Z
 dd 00000000	;5B  [    (left opening bracket)
 dd 00000000	;5C  \    (back slash)
 dd 00000000	;5D  ]    (right closing bracket)
 dd 00000000	;5E  ^    (caret cirumflex)
 dd 00000000	;5F  _    (underscore)
 dd 00000000	;60  `
 dd again	;61  a
 dd mark_block	;62  b
 dd calc	;63  c
 dd dmark_block	;64  d
 dd macro_execute_menu 	;65  e
 dd find_forward	;66  f
 dd get_file	;67  g
 dd KeyHelp	;68  h
 dd insert_mode	;69  i
 dd jump_menu	;6A  j
 dd 00000000	;6B  k
 dd 00000000	;6C  l
 dd macro_record_toggle	;6D  m
 dd 00000000	;6E  n
 dd other	;6F  o
 dd paragraph	;70  p
 dd quit_menu	;71  q
 dd find_and_replace	;72  r
 dd 00000000	;73  s
 dd set_tag	;74  t
 dd 00000000	;75  u
 dd 00000000	;76  v
 dd window	;77  w
 dd xchange_mode ;78  x
 dd 00000000	;79  y
 dd 00000000	;7A  z
 dd 00000000	;7B  {    (left opening brace)
 dd 00000000	;7C  |    (vertical bar)
 dd 00000000	;7D  }    (right closing brace)
 dd 00000000	;7E  ~    (tilde)

  [section .text]
;-------------------------------------------------------