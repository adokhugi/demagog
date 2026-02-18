// DEMAGOG V1.1 ist PD (w) by The Real Adok

void char13h(unsigned int letter,unsigned char font,unsigned char col,
	     unsigned int xpos,unsigned char ypos,
	     unsigned char height,unsigned char width,
	     unsigned int zseg,unsigned int zoff);
void pset13h(int x,int y,int col);
void screen (unsigned char nr);
void text13h(unsigned char text[],unsigned char font,unsigned char col,
	     unsigned int xpos,unsigned char ypos,
	     unsigned char height,unsigned char width,
	     unsigned int zseg,unsigned int zoff);

// Ausgabe eines Zeichens
void char13h(unsigned int letter,unsigned char font,unsigned char col,
	     unsigned int xpos,unsigned char ypos,
	     unsigned char height,unsigned char width,
	     unsigned int zseg,unsigned int zoff)
{
  unsigned char temp,i;
  unsigned int  y;

  letter=letter*font;

  for(y=0;y<font;y++)
  {
    asm {
      mov bx,word ptr zoff
      add bx,word ptr letter
      add bx,word ptr y
      mov ax,word ptr zseg
      mov es,ax
      mov al,byte ptr es:[bx]
      mov byte ptr temp,al
    }

    for(i=0;i<=7;i++)
    {
      if(temp==(temp|(1<<(7-i)))) pset13h(xpos+i/width,ypos+y/height,col);
    }
  }
}

// Setzen eines Bildpunkts
void pset13h(int x,int y,int col)
{
  int temp=320*y+x;

  asm {
    mov ax,0a000h
    mov es,ax
    mov bx,word ptr temp
    mov ax,word ptr col
    xor ah,ah
    mov es:[bx],al
  }
}

// Bildschirmmodus ausw„hlen
void screen(unsigned char nr)
{
  asm {
    xor ah,ah
    mov al,byte ptr nr
    int 10h
  }
}

// Text ausgeben
void text13h(unsigned char text[],unsigned char font,unsigned char col,
	     unsigned int xpos,unsigned char ypos,
	     unsigned char height,unsigned char width,
	     unsigned int zseg,unsigned int zoff)
{
  char i=0;

  while(text[i]!='\0')
    char13h(text[i++],font,col,xpos+i*8,ypos,height,width,zseg,zoff);
}

// Demonstration
void main(void)
{
  unsigned int zseg,zoff8,zoff14,zoff16;

  // Zeichens„tze laden
  asm {
    push bp
  // 16-Byte-Zeichensatz und ES laden
    mov ax,1130h
    mov bx,600h
    int 10h
    mov bx,bp
    pop bp
    mov word ptr zoff16,bx
    mov word ptr zseg,es

    push bp
  // 14-Byte-Zeichensatz laden
    mov ax,1130h
    mov bx,200h
    int 10h
    mov bx,bp
    pop bp
    mov word ptr zoff14,bx

    push bp
  // 8-Byte-Zeichensatz laden
    mov ax,1130h
    mov bx,300h
    int 10h
    mov bx,bp
    pop bp
    mov word ptr zoff8,bx
  }

  screen(0x13);               // Modus 320x200x256

  // Ein wenig Text ausgeben...
  text13h("Hallo Welt! Hallo Freunde!",16,15,0,0,1,1,zseg,zoff16);
  text13h("Das ist jetzt im 'schmalen' Modus.",16,10,15,15,1,2,zseg,zoff16);
  text13h("nun ein biáchen unleserlich...",16,13,20,30,2,1,zseg,zoff16);
  text13h("...JETZT TOTAL",16,14,50,70,2,2,zseg,zoff16);
  text13h("Das ist der 14-Byte-Zeichensatz.",14,11,60,100,1,1,zseg,zoff14);
  text13h("Und das sind 8 Byte.",8,12,20,120,1,1,zseg,zoff8);
  text13h("Dies entstand durch šberlauf.",8,6,150,160,1,1,zseg,zoff8);
  text13h("--- Ende mit ESC! ---",14,13,80,184,1,2,zseg,zoff14);

  while(inp(0x60)!=1);        // Auf ESC warten
  screen(3);                  // Textmodus
}
