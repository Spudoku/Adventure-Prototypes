cls
del *.o
cl65 -t atari -g --debug-info  -C atari_modifed.cfg  -Ln game.lbl -o adventure.xex sound.c camera.c charmap.c displaylist.c entity.c gamemap.c gfx.c items.c player.c main.c util.c util_input.c -Wl --dbgfile,adventure.dbg
