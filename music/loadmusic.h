#ifndef __LOADMUSIC_H__
#define __LOADMUSIC_H__

unsigned int memopen(char* name);
void memclose(unsigned int handle);
int memread(void* buffer, int size, unsigned int handle);
void memseek(unsigned int handle, int pos, signed char mode);
int memtell(unsigned int handle);
void loadmusic(void);

#endif
