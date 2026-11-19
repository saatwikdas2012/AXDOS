#include <stdbool.h>
#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_MEMORY 0xb8000

int cursor_x = 0;
int cursor_y = 0;

volatile unsigned short* video_memory = (volatile unsigned short*)VGA_MEMORY;
short text_color = 0x0f00;
short background_color = 0x0000;

void newline(void);

void clearscreen(void);

static inline unsigned char inbyte(unsigned short port);

static inline void outbyte(unsigned short port, unsigned short value);

unsigned char keyboard_read(void);

char scan_char(unsigned char c);

void putchar(char ch);

void print(char *word);

bool strcmp(char *str1, char *str2);

void set_text_color(char *color);

void set_background_color(char *color);

void main(void) {
	print("Welcome to ");
	set_background_color("white");
	set_text_color("red");
    print("A");
	set_text_color("cyan");
    print("X");
	set_text_color("red");
    print("D");
	set_text_color("cyan");
    print("O");
	set_text_color("red");
    print("S");
	set_text_color("white");
	set_background_color("black");
    print(" Pre-Alpha 1.0!\n");
    print("Now with new line escape sequence!\n");
	print("AND TEXT INPUT!!!\ntry it: ");
	while(true){
		putchar(scan_char(keyboard_read()));
	}
}

void newline(void){
    cursor_x = 0;
    cursor_y++;
}

void clearscreen(void){
	cursor_x = 0;
	cursor_y = 0;
	for (int i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++){
		video_memory[i] = 0xf20;
	}
}

static inline unsigned char inbyte(unsigned short port){
	unsigned char result;
	__asm__ __volatile__(
		"inb %1, %0"
		: "=a" (result)
		: "d" (port)
	);

	return result;
}

static inline void outbyte(unsigned short port, unsigned short value){
	__asm__ __volatile__(
		"outb %0, %1"
		:
		: "a" (value), "d" (port)
	);
}

unsigned char keyboard_read(void){
	bool status = inbyte(0x64) & 0x01;
	if (status){
		return inbyte(0x60);
	} else{
		return 0x00;
	}
}

char scan_char(unsigned char c){
	if (c == 0x1e){return 'A';}
	else if (c == 0x30){return 'B';}
	else if (c == 0x2e){return 'C';}
	else if (c == 0x20){return 'D';}
	else if (c == 0x12){return 'E';}
	else if (c == 0x21){return 'F';}
	else if (c == 0x22){return 'G';}
	else if (c == 0x23){return 'H';}
	else if (c == 0x17){return 'I';}
	else if (c == 0x24){return 'J';}
	else if (c == 0x25){return 'K';}
	else if (c == 0x26){return 'L';}
	else if (c == 0x32){return 'M';}
	else if (c == 0x31){return 'N';}
	else if (c == 0x18){return 'O';}
	else if (c == 0x19){return 'P';}
	else if (c == 0x10){return 'Q';}
	else if (c == 0x13){return 'R';}
	else if (c == 0x1f){return 'S';}
	else if (c == 0x14){return 'T';}
	else if (c == 0x16){return 'U';}
	else if (c == 0x2f){return 'V';}
	else if (c == 0x11){return 'W';}
	else if (c == 0x2d){return 'X';}
	else if (c == 0x15){return 'Y';}
	else if (c == 0x2c){return 'Z';}
	else if (c == 0x39){return ' ';}
	else {return '\0';}
}

void putchar(char ch) {
	if (ch == '\0'){
		return;
	}
    int offset = cursor_y * VGA_WIDTH + cursor_x;
	video_memory[offset] = (unsigned short)ch | (text_color | background_color);

    cursor_x++;
    if (cursor_x >= VGA_WIDTH){
        newline();
    }
}

void print(char *word) {
    for (int i = 0; word[i] != 0; i++) {
        if (word[i] == '\n'){
            newline();
            continue;
        }
        putchar(word[i]);
    }
}

void set_text_color(char *color){
	if(strcmp(color, "black")){
		text_color = 0x0000;
	} else if(strcmp(color, "blue")){
		text_color = 0x0100;
	} else if(strcmp(color, "green")){
		text_color = 0x0200;
	} else if(strcmp(color, "cyan")){
		text_color = 0x0300;
	} else if(strcmp(color, "red")){
		text_color = 0x0400;
	} else if(strcmp(color, "magenta")){
		text_color = 0x0500;
	} else if(strcmp(color, "brown")){
		text_color = 0x0600;
	} else if(strcmp(color, "light gray")){
		text_color = 0x0700;
	} else if(strcmp(color, "dark gray")){
		text_color = 0x0800;
	} else if(strcmp(color, "light blue")){
		text_color = 0x0900;
	} else if(strcmp(color, "light green")){
		text_color = 0x0a00;
	} else if(strcmp(color, "light cyan")){
		text_color = 0x0b00;
	} else if(strcmp(color, "light red")){
		text_color = 0x0c00;
	} else if(strcmp(color, "pink")){
		text_color = 0x0d00;
	} else if(strcmp(color, "yellow")){
		text_color = 0x0e00;
	} else if(strcmp(color, "white")){
		text_color = 0x0f00;
	} else{
		print("Invalid Argument: Undefined Color");
	}
}

void set_background_color(char *color){
	if(strcmp(color, "black")){
		background_color = 0x0000;
	} else if(strcmp(color, "blue")){
		background_color = 0x1000;
	} else if(strcmp(color, "green")){
		background_color = 0x2000;
	} else if(strcmp(color, "cyan")){
		background_color = 0x3000;
	} else if(strcmp(color, "red")){
		background_color = 0x4000;
	} else if(strcmp(color, "magenta")){
		background_color = 0x5000;
	} else if(strcmp(color, "brown")){
		background_color = 0x6000;
	} else if(strcmp(color, "light gray")){
		background_color = 0x7000;
	} else if(strcmp(color, "dark gray")){
		background_color = 0x8000;
	} else if(strcmp(color, "light blue")){
		background_color = 0x9000;
	} else if(strcmp(color, "light green")){
		background_color = 0xa000;
	} else if(strcmp(color, "light cyan")){
		background_color = 0xb000;
	} else if(strcmp(color, "light red")){
		background_color = 0xc000;
	} else if(strcmp(color, "pink")){
		background_color = 0xd000;
	} else if(strcmp(color, "yellow")){
		background_color = 0xe000;
	} else if(strcmp(color, "white")){
		background_color = 0xf000;
	} else{
		print("Invalid Argument: Undefined Color");
	}
}

bool strcmp(char *str1, char *str2){
	int i = 0;
	while (str1[i] != 0 && str2[i] != 0){
		if (str1[i] != str2[i]){
			return false;
		}
		i++;
	}
	return str1[i] == str2[i];
}
