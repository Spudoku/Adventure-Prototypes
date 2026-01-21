#include <atari.h>
#include <_antic.h>
#include <_atarios.h>
#include <peekpoke.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <joystick.h>
#include <unistd.h>

int main() {
    printf("hello world");
    while (true) {
        sleep(1);
    }
}

// --debug-info -Wl --dbgfile,hello.dbg
// cl65 --debug-info -Wl --dbgfile,test.dbg -t atari -O -g -Ln game.lbl -o test.xex test.c