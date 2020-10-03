#include "LedControl.h"
#define R 5
#define L 6
const int lungmax = 82;
LedControl lc = LedControl(4, 2, 3, 1);
int dir = 0;
int lung = 0;
int i;
int c, n, d, u, p;
int pos[lungmax];
int fruit[] {4, 4};
boolean partita = true;
boolean points = false;
boolean vai = true;
int t;
int di = 0;
const int vel = 170;
byte Z[3] {
  B01111100,
  B01000100,
  B01111100
};
byte U[3] {
  B00000000,
  B00001000,
  B01111100
};
byte D[3] {
  B01110100,
  B01010100,
  B01011100
};
byte T[3] {
  B01010100,
  B01010100,
  B01111100
};
byte Q[3] {
  B00011100,
  B00010000,
  B01111100
};
byte C[3] {
  B01011100,
  B01010100,
  B01110100
};
byte S[3] {
  B01111100,
  B01010100,
  B01110100
};
byte E[3] {
  B00000100,
  B00000100,
  B01111100
};
byte O[3] {
  B01111100,
  B01010100,
  B01111100
};
byte N[3] {
  B01011100,
  B01010100,
  B01111100
};

void setup() {
  for (int i = 0; i < lungmax; i++) {
    pos[i] = 0;
  }
  lc.shutdown(0, false);
  lc.setIntensity(0, 0); //8
  lc.clearDisplay(0);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  while (digitalRead(R) == HIGH || digitalRead(L) == HIGH) {}
  randomSeed(analogRead(A0));
  fruit[0] = random (0, 7);
  fruit[1] = random (0, 7);
  lc.setLed(0, fruit[0], fruit[1], true);
}

void loop() {
  if (partita == true) {
    if (di == 1) { /*digitalRead(L) == LOW ||  */
      dir = dir + 1;
      if (dir > 3) {
        dir = 0;
      }
    }
    if (di == -1) { /*digitalRead(R) == LOW ||  */
      dir = dir - 1;
      if (dir < 0) {
        dir = 3;
      }
    }
    for (i = (lungmax - 2) / 2; i > 0; i--) {
      pos[i * 2] = pos [(i - 1) * 2];
      pos[(i * 2) + 1] = pos [((i - 1) * 2) + 1];
    }
    if (dir == 0) {
      pos[0] = pos[0] + 1;
      if (pos[0] > 7) {
        pos[0] = 0;
      }
    }
    if (dir == 2) {
      pos[0] = pos[0] - 1;
      if (pos[0] == -1) {
        pos[0] = 7;
      }
    }
    if (dir == 1) {
      pos[1] = pos[1] + 1;
      if (pos[1] > 7) {
        pos[1] = 0;
      }
    }
    if (dir == 3) {
      pos[1] = pos[1] - 1;
      if (pos[1] == -1) {
        pos[1] = 7;
      }
    }
    lc.setLed(0, (int) pos[0], (int) pos[1], true);
    di = 0;
    for (t = vel; t >= 0; t--) {
      if (digitalRead(L) == HIGH && digitalRead(R) == HIGH) {
        vai = true;
      }
      if (di == 0 && vai == true) {
        if (digitalRead(L) == LOW) {
          di = 1;
          vai = false;
        }
        if (digitalRead(R) == LOW) {
          di = -1;
          vai = false;
        }
      }
      if (t <= vel / 2) {
        lc.setLed(0, fruit[0], fruit[1], false);
      }
      delay (1);
    }
    lc.setLed(0, fruit[0], fruit[1], true);
    lc.setLed(0, pos[lung * 2], pos[lung * 2 + 1], false);
    if (pos[0] == fruit[0] && pos[1] == fruit[1]) {
      lung++;
      fr:
      fruit[0] = random (0, 7);
      fruit[1] = random (0, 7);
      for (i = lung; i >= 0; i--) {
        if (fruit[0] == pos[i * 2] && fruit[1] == pos [i * 2 + 1]) {
          goto fr;
        }
      }
      lc.setLed(0, fruit[0], fruit[1], true);
    }
    if (lung > 40) {
      partita = false;
      points = true;
    }
    for (i = lung; i > 0 ; i--) {
      if (pos[0] == pos[i * 2] && pos[1] == pos[i * 2 + 1]) {
        partita = false;
        points = true;
        while (digitalRead(R) == LOW || digitalRead(L) == LOW) {
          delay(50);
        }
        break;
      }
    }
  }
  else {
    if (points == true) {
      lc.setLed(0, fruit[0], fruit[1], false);
      for (i = lung; i > 0; i--) {
        lc.setLed(0, pos[i * 2], pos[i * 2 + 1], false);
        delay(40);
      }
      for (i = 7; i >= 0; i--) {
        lc.setColumn(0, i, B11111111);
        delay(30);
      }
      for (i = 7; i >= 0; i--) {
        lc.setColumn(0, i, B00000000);
        delay(30);
      }
      num(lung);
      points = false;
    }
    if (digitalRead(R) == LOW && digitalRead(L) == LOW) {
      lc.clearDisplay(0);
      fruit[0] = random (0, 7);
      fruit[1] = random (0, 7);
      lc.setLed(0, fruit[0], fruit[1], true);
      lung = 0;
      pos[0] = 0;
      pos[1] = 0;
      dir = 0;
      partita = true;
    }
  }
}
void num(int x) {
  lc.clearDisplay(0);
  d = x / 10 - (x / 10) % 1;
  u = x - d * 10;
  p = 0;
  for (c = 1; c <= 7; c++, p++) {
    if (c <= 3) {
      n = d;
    }
    if (c == 4) {
      c = 5;
      n = u;
      p = 0;
    }
    switch (n) {
      case 0: {
          lc.setRow(0, c, Z[p]);
          break;
        }
      case 1: {
          lc.setRow(0, c, U[p]);
          break;
        }
      case 2: {
          lc.setRow(0, c, D[p]);
          break;
        }
      case 3: {
          lc.setRow(0, c, T[p]);
          break;
        }
      case 4: {
          lc.setRow(0, c, Q[p]);
          break;
        }
      case 5: {
          lc.setRow(0, c, C[p]);
          break;
        }
      case 6: {
          lc.setRow(0, c, S[p]);
          break;
        }
      case 7: {
          lc.setRow(0, c, E[p]);
          break;
        }
      case 8: {
          lc.setRow(0, c, O[p]);
          break;
        }
      case 9: {
          lc.setRow(0, c, N[p]);
          break;
        }
    }
  }
}
