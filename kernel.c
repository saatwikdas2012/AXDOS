#define bool  _Bool
#define true  1
#define false 0
#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define VGA_MEMORY 0xb8000

int cursor_x = 0;
int cursor_y = 0;
bool shift_pressed = false;
bool ctrl_pressed = false;

volatile unsigned short* video_memory = (volatile unsigned short*)VGA_MEMORY;
short text_color = 0x0f00;
short background_color = 0x0000;

void newline(void);
void clearscreen(void);
void scroll(void);
static inline unsigned char inbyte(unsigned short port);
static inline void outbyte(unsigned short port, unsigned short value);
unsigned char keyboard_read(void);
char scan_char(unsigned char c);
char *scan(char *prompt);
void putchar(char ch);
void print(char *word);
void setstr(char *str1, char *str2);
bool strcmp(char *str1, char *str2);
bool strcnt(char *str1, char *str2);
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
    print(" Alpha 1.0\n");
	print("Type help for a list of commands.\n");
    while(true){
		char *command = scan("AXDOS> ");
		if (strcnt("echo ", command)){
			for (int i = 5; command[i] != '\0'; i++){
				putchar(command[i]);
			}
			putchar('\n');
		} else if (strcmp(command, "ver")){
			print("AXDOS Alpha 1.0\n");
		} else if (strcmp(command, "help")){
			print(" AXDOS - Help & Commands\n =======================\n\n Available Commands:\n\n  help   - Displays this help message.\n  echo   - Displays the text you type.\n  ver    - Displays the current AXDOS version.\n  stop   - Shuts down AXDOS.\n  cls    - Clears the screen.\n  color  - Change your color scheme.\n\n Usage:\n  Type a command and press ENTER to execute it.\n\n =======================\n        AXDOS OS\n");
		} else if (strcmp(command, "stop")){
			print("AXDOS shutting down...\nAXDOS has successfully stopped\nYou may now power off the device.\nThank you for using AXDOS!\n");
			while(true){
				__asm__ __volatile__("hlt");
			}
			break;
		} else if (strcmp(command, "cls")){
			clearscreen();
		} else if (strcmp(command, "color")) {
			print(" AXDOS - Color Settings\n ======================\n\nAvailable Colors:\n\n [01] Black\n [02] Blue\n [03] Green\n [04] Cyan\n [05] Red\n [06] Magenta\n [07] Brown\n [08] Light Gray\n [09] Dark Gray\n [10] Light Blue\n [11] Light Green\n [12] Light Cyan\n [13] Light Red\n [14] Pink\n [15] Yellow\n [16] White\n\n =======================\n        AXDOS OS\n");
			char *newtext;
			char *input = scan("New text color (all lowercase): ");
			setstr(newtext,input);
			set_background_color(scan("New background color (all lowercase): "));
			set_text_color(newtext);
			clearscreen();
		} else if (command[0] == '\0'){
			continue;
		} else {
			print("Command not recognized.\n");
		}
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
		video_memory[i] = (unsigned short)' ' | text_color | background_color;
	}
}

void scroll(void){
	for (int i = 0; i < VGA_HEIGHT; i++){
		for (int j = 0; j < VGA_WIDTH; j++){
			video_memory[i*VGA_WIDTH+j]=video_memory[(i+1)*VGA_WIDTH+j];
		}
	}
	for (int i = 0; i < VGA_WIDTH; i++){
		video_memory[VGA_HEIGHT*VGA_WIDTH+i]=(unsigned short)' ' | text_color | background_color;
	}
	cursor_y = VGA_HEIGHT-1;
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
	if (c == 0x00){return '\0';}
	else if (c == 0x2a || c == 0x36){
		shift_pressed=true;
		return '\0';
	}
	else if (c == 0xaa || c == 0xb6){
		shift_pressed=false;
		return '\0';
	}
	else if (c == 0x1d){
		ctrl_pressed=true;
		return '\0';
	}
	else if (c == 0x9d){
		ctrl_pressed=false;
		return '\0';
	}

	else if (shift_pressed){
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
		else if (c == 0x02){return '!';}
		else if (c == 0x03){return '@';}
		else if (c == 0x04){return '#';}
		else if (c == 0x05){return '%';}
		else if (c == 0x06){return '%';}
		else if (c == 0x07){return '^';}
		else if (c == 0x08){return '&';}
		else if (c == 0x09){return '*';}
		else if (c == 0x0a){return '(';}
		else if (c == 0x0b){return ')';}
		else if (c == 0x0d){return '+';}
		else if (c == 0x29){return '~';}
		else if (c == 0x1a){return '{';}
		else if (c == 0x1b){return '}';}
		else if (c == 0x2b){return '|';}
		else if (c == 0x27){return ':';}
		else if (c == 0x28){return '\"';}
		else if (c == 0x33){return '<';}
		else if (c == 0x34){return '>';}
		else if (c == 0x35){return '?';}
		else if (c == 0x0c){return '_';}
		else if (c == 0x1c){return '\n';}
		else {return '\0';}
	} else if (!shift_pressed){
		if (c == 0x1e){return 'a';}
		else if (c == 0x30){return 'b';}
		else if (c == 0x2e){return 'c';}
		else if (c == 0x20){return 'd';}
		else if (c == 0x12){return 'e';}
		else if (c == 0x21){return 'f';}
		else if (c == 0x22){return 'g';}
		else if (c == 0x23){return 'h';}
		else if (c == 0x17){return 'i';}
		else if (c == 0x24){return 'j';}
		else if (c == 0x25){return 'k';}
		else if (c == 0x26){return 'l';}
		else if (c == 0x32){return 'm';}
		else if (c == 0x31){return 'n';}
		else if (c == 0x18){return 'o';}
		else if (c == 0x19){return 'p';}
		else if (c == 0x10){return 'q';}
		else if (c == 0x13){return 'r';}
		else if (c == 0x1f){return 's';}
		else if (c == 0x14){return 't';}
		else if (c == 0x16){return 'u';}
		else if (c == 0x2f){return 'v';}
		else if (c == 0x11){return 'w';}
		else if (c == 0x2d){return 'x';}
		else if (c == 0x15){return 'y';}
		else if (c == 0x2c){return 'z';}
		else if (c == 0x39){return ' ';}
		else if (c == 0x02){return '1';}
		else if (c == 0x03){return '2';}
		else if (c == 0x04){return '3';}
		else if (c == 0x05){return '4';}
		else if (c == 0x06){return '5';}
		else if (c == 0x07){return '6';}
		else if (c == 0x08){return '7';}
		else if (c == 0x09){return '8';}
		else if (c == 0x0a){return '9';}
		else if (c == 0x0b){return '0';}
		else if (c == 0x0d){return '=';}
		else if (c == 0x29){return '`';}
		else if (c == 0x1a){return '[';}
		else if (c == 0x1b){return ']';}
		else if (c == 0x2b){return '\\';}
		else if (c == 0x27){return ';';}
		else if (c == 0x28){return '\'';}
		else if (c == 0x33){return ',';}
		else if (c == 0x34){return '.';}
		else if (c == 0x35){return '/';}
		else if (c == 0x0c){return '-';}
		else if (c == 0x1c){return '\n';}
		else if (c == 0x0e){return '\b';}
		else {return '\0';}
	} else {return '\0';}
}

char *scan(char *prompt){
	print(prompt);
	char user_in = scan_char(keyboard_read());
	int i = 0;
	static char str[1024];
	while(user_in != '\n' && i < 49){
		user_in = scan_char(keyboard_read());
		if (user_in == '\n'){break;}
		if (user_in == '\0'){continue;}
		if (user_in == '\b'){
			if (i == 0){
				continue;
			}
			putchar(user_in);
			i--;
			str[i] = '\0';
			continue;
		}
		str[i] = user_in;
		putchar(user_in);
		i++;
	}
	putchar('\n');
	str[i] = '\0';
	return str;
}

void putchar(char ch) {
	if (ch == '\0'){
		return;
	}
	if (ch == '\n'){
        newline();
        return;
    }
	if (ch == '\b'){
		if (cursor_x == 0){
			cursor_y--;
			cursor_x = VGA_WIDTH;
		} else{cursor_x--;}
		video_memory[cursor_y * VGA_WIDTH + cursor_x] = (unsigned short)0 | (text_color | background_color);
		return;
	}

    int offset = cursor_y * VGA_WIDTH + cursor_x;
	video_memory[offset] = (unsigned short)ch | (text_color | background_color);

    cursor_x++;
    if (cursor_x >= VGA_WIDTH){
        newline();
    }
	if (cursor_y >= VGA_HEIGHT){
		scroll();
	}
}

void print(char *word) {
    for (int i = 0; word[i] != 0; i++) {
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

void setstr(char *str1, char *str2){
	int i = 0;
	while (str2[i] != 0){
		str1[i] = str2[i];
		i++;
	}
	str1[i] = '\0';
	return;
}

bool strcmp(char *str1, char *str2){
	int i = 0;
	while (str1[i] != 0 || str2[i] != 0){
		if (str1[i] != str2[i]){
			return false;
		}
		i++;
	}
	return str1[i] == str2[i];
}

bool strcnt(char *str1, char *str2){
	int i = 0;
	while (str1[i] != 0){
		if (str1[i] != str2[i]){
			return false;
		}
		i++;
	}
	return true;
}
