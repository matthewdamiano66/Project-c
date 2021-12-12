
void printChar(char c);
void printString(char* s);
void readString(char* s);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cs int dx);

int main()
{
	char c;
	char line[80];
	char buffer[512];

	printString("Enter a line: ");

	readString(line);
	printString(line);

	readSector(buffer, 30);
	printString(buffer);

	makeInterrupt21();
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);

	while(1)
	{
	}
}

void printString(char* s)
{
	int i;
	for (i = 0; s[i]!='\0'; i++)
	{
		printChar(s[i]);
	}
}

void printChar(char c)
{
	interrupt(0x10,0x0e*0x100+c,0,0,0);
}
void readString(char* line)
{
	int i = 0;
	while(1)
	{
		line[i] = interrupt(0x16,0,0,0,0);
		printChar(line[i]);
		if(line[i] == '\r')
		{
			line[i+1] = '\n';
			printChar('\n');
			break;

		}
		if(line[i] == '\b')
		{
			line[i-1] = interrupt(0x10, 0x0e*0x100+0x20);
			line[i] = interrupt(0x10, 0x0e*0x100+0x8);
			i--;
			continue;
		}
		i++;

	}
}
void readSector(char* buffer, int sector)
{
	interrupt(0x13, 0x2*256+1, buffer, sector+1, 0x80);
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{

	if(ax == 0)
	{
		printString(bx);
	}
	else if(ax == 1)
	{
		readString(bx);
	}
	else if(ax == 2)
	{
		readSector(bx, cx);
	}
	else
	{
		printString(" Function Unknown");
	}
}
