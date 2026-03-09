cls
cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o test.xex gfx.c charmap.c entity.c gamemap.c player.c test.c util_input.c util.c dragon.c items.c util_structs.c displaylist.c camera.c player_missile.c
del *.o