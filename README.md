# About

This is a port of Adventure to the Atari 800+ series using CC65

# Building

Makefile is a WIP, need to manually compile with:
```bash
cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o test.xex *.c -m map.txt
```



