cls
cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o test.xex camera.c charmap.c displaylist.c entity.c gamemap.c gfx.c items.c player.c test.c util.c util_input.c
del *.o