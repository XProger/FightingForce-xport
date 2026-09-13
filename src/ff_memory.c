#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
uint8 ff_ram[0x200000];
static uint8 ff_scratchpad[1024];
void *ff_ptr(uint32 address,size_t count) {
 uint32 offset;
 if(address>=0x1f800000u&&address<0x1f800400u){offset=address-0x1f800000u;if(count>sizeof(ff_scratchpad)-offset)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return ff_scratchpad+offset;}
 if(address<0x200000u)offset=address;
 else if(address>=0x80000000u&&address<0x80200000u)offset=address-0x80000000u;
 else if(address>=0xa0000000u&&address<0xa0200000u)offset=address-0xa0000000u;
 else { fprintf(stderr,"Unmapped PSX data address %08X\n",address);ff_wip_stop(__FUNCTION__,__FILE__,__LINE__); }
 if(count>sizeof(ff_ram)-offset)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return ff_ram+offset;
}
sint32 ff_load_ram(const char *path) {
 FILE *f=fopen(path,"rb");size_t n;int extra;
 if(!f)return 0;n=fread(ff_ram,1,sizeof(ff_ram),f);extra=fgetc(f);fclose(f);
 return n==sizeof(ff_ram)&&extra==EOF;
}
/* Original GAME.EXE payload and 6CD64 BSS clear, before game initialization. */
sint32 ff_load_game_image(const char *path) {
 uint8 header[2048];uint32 entry,address,size;size_t n;int extra;FILE *f=fopen(path,"rb");
 if(!f)return 0;
 if(fread(header,1,sizeof(header),f)!=sizeof(header)||memcmp(header,"PS-X EXE",8)){fclose(f);return 0;}
 memcpy(&entry,header+16,4);memcpy(&address,header+24,4);memcpy(&size,header+28,4);
 if(entry!=0x8006cd64||address!=0x80010000||size!=0xb1000){fclose(f);return 0;}
 memset(ff_ram,0,sizeof(ff_ram));memset(ff_scratchpad,0,sizeof(ff_scratchpad));
 n=fread(ff_ram+0x10000,1,size,f);extra=fgetc(f);fclose(f);
 if(n!=size||extra!=EOF)return 0;
 memset(ff_ram+0x93f40,0,0x2ce28);return 1;
}
