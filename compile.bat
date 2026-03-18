cls
cl65 --debug-info -Wl --dbgfile,test.dbg -C atari_modifed.cfg -t atari  -g -Ln game.lbl -o adventure.xex sound.c camera.c charmap.c displaylist.c entity.c gamemap.c gfx.c items.c player.c main.c util.c util_input.c
del *.o