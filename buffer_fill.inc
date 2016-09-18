
make:
 db '#! /bin/bash',0ah
 db 'make &> /tmp/aedit.tmp.2',0ah
make_end:

spell:
 db '#! /bin/bash',0ah
 db 'if test -e ispell ; then',0ah
 db 'ispell $1 ;',0ah
 db 'else',0ah
 db 'aspell -c $1',0ah
 db 'fi',0ah
spell_end:

compar:
 db '#! /bin/bash',0ah
 db 'fldiff=$(which fldiff)',0ah
 db 'if [ -x "$fldiff" ]; then',0ah
 db '# compare two files and optionally make changes',0ah
 db 'fldiff $1 $2',0ah
 db 'exit',0ah
 db 'fi',0ah
 db 'clear',0ah
 db 'echo "program fldiff needed for compare"',0ah
 db 'echo "press <enter key> to continue"',0ah
 db 'read key',0ah
compar_end:

print:
 db '#! /bin/bash',0ah
 db ' echo ""',0ah
 db 'echo "enter number for print format"',0ah
 db ' echo ""',0ah
 db ' echo "1=1up 90 columns 65 rows"',0ah
 db ' echo "2=1up 74 columns 55 rows"',0ah
 db ' echo "3=1up 90 columns 63 rows + title"',0ah
 db ' echo "4=2up 71 columns 62 rows"',0ah
 db ' echo "5=2up small font"',0ah
 db ' echo "6=2up 71 columns 60 rows + title"',0ah
 db ' echo "7=landscape small font"',0ah
 db ' echo "8=lanescape medium font"',0ah
 db ' echo "9=landscape big font"',0ah
 db ' echo ""',0ah

 db 'read -sn1 -p "enter number-> " ret',0ah
 db 'echo ""',0ah

 db 'if [ $ret = 1 ]; then',0ah
 db '       # print bold without title',0ah
 db '       enscript $1 --no-header -f Courier-Bold10 -M Letterdj -c ',0ah
 db 'fi',0ah

 db 'if [ $ret = 2 ]; then',0ah
 db '       # print big type without headers',0ah
 db '       enscript $1 --no-header -f Courier-Bold12 -M Letterdj -c',0ah
 db 'fi',0ah

 db 'if [ $ret = 3 ]; then',0ah
 db '       # print  with title',0ah
 db '       enscript $1 --fancy-header -f Courier-Bold10 -M Letterdj -c',0ah
 db 'fi',0ah

 db 'if [ $ret = 4 ]; then',0ah
 db '       # print bold without title',0ah
 db '       enscript $1 --no-header -f Courier-Bold8 -M Letterdj -c -2r',0ah
 db '       echo "2up"',0ah
 db 'fi',0ah

 db 'if [ $ret = 5 ]; then',0ah
 db '       # print bold without headers',0ah
 db '       enscript $1 --no-header -f Courier-Bold10 -M Letterdj -c -2r',0ah
 db '       echo "2upB"',0ah
 db 'fi',0ah

 db 'if [ $ret = 6 ]; then',0ah
 db '       # print bold with title',0ah
 db '       enscript $1 --fancy-header -f Courier-Bold8 -M Letterdj -c -2r',0ah
 db '       echo "2upT"',0ah
 db 'fi',0ah

 db 'if [ $ret = 7 ]; then',0ah
 db '       # print bold without headers',0ah
 db '       enscript $1 --no-header -f Courier-Bold8 -M Letterdj -c -r',0ah
 db '       echo "2upB"',0ah
 db 'fi',0ah

 db 'if [ $ret = 8 ]; then',0ah
 db '       # print bold without headers',0ah
 db '       enscript $1 --no-header -f Courier-Bold10 -M Letterdj -c -r',0ah
 db '       echo "2upB"',0ah
 db 'fi',0ah

 db 'if [ $ret = 9 ]; then',0ah
 db '       # print bold without headers',0ah
 db '       enscript $1 --no-header -f Courier-Bold12 -M Letterdj -c -r',0ah
 db '       echo "2upB"',0ah
 db 'fi',0ah

 db 'exit 0',0ah
print_end:
