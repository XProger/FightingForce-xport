/* Differential harness for the actual main-menu callback and its C callees. */
#include "ff.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
static uint8 fixture[0x200000];
static jmp_buf menu_jump;
static uint32 trace[32];
static uint32 ranges[][2]={{0x8007f804,40},{0x8008d490,76},{0x8009272c,260},{0x80093a18,0x1380},{0x801cb000,8192},{0x801d0000,65536},{0x800b50d8,0}};
static sint32 record_effect(sint32 id,sint32 volume,sint32 pitch) {
 uint32 n=trace[3]++;if(n>=9)return -1;trace[4+3*n]=id;trace[5+3*n]=volume;trace[6+3*n]=pitch;return 7;
}
static sint32 record_aux(uint32 f,sint32 a,sint32 b){return record_effect((sint32)f,a,b);}
static sint32 record_attr(uint32 a){return record_effect(0x8007922c,(sint32)a,0);}
static void record_jump(uint32 buffer,sint32 value){trace[1]=buffer;trace[2]=value;longjmp(menu_jump,1);}
extern void ff_audit_end(void);
int ff_audit_menu_mode(int character) {
 FILE *jobs,*out;uint32 j[11];int i;
 if(character==2)ranges[2][1]=384;
 if(character==6)ranges[6][1]=60;
 if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(fixture,ff_ram,sizeof(fixture));
 jobs=fopen(character==6?"../status/menu/preframe-jobs.bin":character==5?"../status/menu/scores-jobs.bin":character==4?"../status/menu/controller-jobs.bin":character==3?"../status/menu/screen-jobs.bin":character==2?"../status/menu/options-jobs.bin":character?"../status/menu/character-jobs.bin":"../status/menu/menu-jobs.bin","rb");out=fopen(character==6?"../status/menu/preframe-native.bin":character==5?"../status/menu/scores-native.bin":character==4?"../status/menu/controller-native.bin":character==3?"../status/menu/screen-native.bin":character==2?"../status/menu/options-native.bin":character?"../status/menu/character-native.bin":"../status/menu/menu-native.bin","wb");if(!jobs||!out)return 4;
 ff_services.menu_aux=record_aux;ff_services.spu_voice_attributes=record_attr;ff_services.play_effect=record_effect;ff_services.game_longjmp=record_jump;
 while(fread(j,sizeof(j),1,jobs)==1) {
  memcpy(ff_ram,fixture,sizeof(fixture));memset(trace,0,sizeof(trace));
  ff_w32(0x8009355c,0x80094748);ff_w32(0x80093560,0x80094770);
  for(i=0;i<2;i++) {
   uint32 p=i?0x80094770:0x80094748;uint8 *pad=(uint8*)ff_ptr(p,8);memset(pad,128,8);
   pad[0]=j[0]>(uint32)i?0:255;pad[1]=0x40;ff_w16(p+2,(uint16)~j[1+i]);
   ff_w32(0x80092790+4u*i,j[3+i]);ff_w16(0x80093a18+2u*i,0);
  }
  ff_w32(0x80092730,j[5]);ff_w32(0x80093dd0,j[6]);ff_w32(0x80092774,j[7]);ff_w32(0x80092778,j[8]);
  ff_w32(0x80093d68,j[9]);ff_w32(0x80092800,j[10]);*(uint8*)ff_ptr(0x80093cf8,1)=0;
  if(character==1){ff_w32(0x8009277c,j[5]);ff_w32(0x80092780,j[6]);ff_w32(0x80092784,j[7]);ff_w32(0x8009272c,j[8]);ff_w32(0x80093d70,j[9]);ff_w32(0x800927ec,0x80050798);ff_w32(0x80092788,0);}
  if(character==2){ff_w32(0x80092850,j[5]);ff_w32(0x80093d70,j[6]);ff_w32(0x80093d6c,j[7]);ff_w32(0x800927b4,j[8]);ff_w32(0x8009284c,j[10]);ff_w32(0x800927ec,0x80050d98);}
  if(character==3){ff_w16(0x80093d38,(uint16)j[5]);ff_w16(0x80093d3a,(uint16)j[6]);ff_w32(0x800927ec,0x80051c98);}
  if(character==4){*(uint8*)ff_ptr(0x80094480,1)=(uint8)j[5];*(uint8*)ff_ptr(0x80094481,1)=(uint8)j[6];ff_w32(0x80093d70,j[7]);ff_w32(0x800927ec,0x800519d4);}
  if(character==5){ff_w32(0x800940b0,j[0]);ff_w32(0x80094288,0);ff_w32(0x8009428c,1);ff_w32(0x800927ec,0x80052624);for(i=0;i<10;i++){ff_w32(0x80093e28+12u*i,j[5]);ff_w32(0x80093e30+12u*i,j[6]);}}
  if(character==6){ff_w32(0x800940b0,j[0]);*(uint8*)ff_ptr(0x80093dd8,1)=(uint8)j[1];ff_w32(0x8008d4c4,j[5]);ff_w32(0x8008d4d4,j[5]);for(i=0;i<30;i++)ff_w16(0x800947f8+2u*i,(uint16)(0x8000+13*i));}
  ff_w32(0x8008d4b0,0x1d0000);ff_w32(0x8008d4b4,0x1cb000);ff_w32(0x8008d4d8,0x801ce000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);
  for(i=0;i<2048;i++)ff_w32(0x801cb000+4u*i,i?0x1cb000+4u*(i-1):0xffffffu);
  ff_w16(0x801ce000,8188);ff_w16(0x801ce008,8184);
  if(setjmp(menu_jump))trace[0]=0x7ffffffe;else trace[0]=(uint32)(character==6?FUN_80058C14():character==5?FUN_80052624():character==4?FUN_800519D4():character==3?FUN_80051C98():character==2?FUN_80050D98():character?FUN_80050798():FUN_80050270());
  fwrite(trace,sizeof(trace),1,out);
  for(i=0;i<sizeof(ranges)/sizeof(ranges[0]);i++)fwrite(ff_ptr(ranges[i][0],ranges[i][1]),1,ranges[i][1],out);
 }
 fclose(jobs);fclose(out);ff_audit_end();return 0;
}
int ff_audit_menu(void){return ff_audit_menu_mode(0);}
int ff_audit_character(void){return ff_audit_menu_mode(1);}

int ff_audit_options(void){return ff_audit_menu_mode(2);}

int ff_audit_screen(void){return ff_audit_menu_mode(3);}

int ff_audit_controller(void){return ff_audit_menu_mode(4);}

int ff_audit_scores(void){return ff_audit_menu_mode(5);}

int ff_audit_preframe(void){return ff_audit_menu_mode(6);}
