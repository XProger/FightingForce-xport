#include "platform_dummy.h"
#include "cd_toc.h"
#include <string.h>
#include <stdlib.h>
/* WIP BIOS card events: opaque handles, no asynchronous card completion. */
uint32 ff_dummy_card_events[8][5];
uint8 ff_dummy_card_pending[8];
/* WIP card requests deliver the no-media event (spec0100), never success. */
void ff_dummy_card_request_no_media(uint32 group) {
 uint32 i;++ff_dummy_calls[0];for(i=0;i<8;i++)
  if(ff_dummy_card_events[i][0]==(group?0xf0000011u:0xf4000001u)&&ff_dummy_card_events[i][1]==0x100)
   ff_dummy_card_pending[i]=1;
}
sint32 ff_dummy_card_test_event(uint32 handle) {
 uint32 i=handle-0xf1000000u;++ff_dummy_calls[0];
 if(i>=8||!ff_dummy_card_events[i][4]||!ff_dummy_card_pending[i])return 0;
 ff_dummy_card_pending[i]=0;return 1;
}
static uint32 card_event_count;
void ff_dummy_card_events_reset(void){card_event_count=0;memset(ff_dummy_card_events,0,sizeof(ff_dummy_card_events));memset(ff_dummy_card_pending,0,sizeof(ff_dummy_card_pending));}
uint32 ff_dummy_card_open_event(uint32 descriptor,uint32 spec,uint32 mode,uint32 callback) {
 uint32 *e;if(card_event_count>=8)abort();e=ff_dummy_card_events[card_event_count];
 e[0]=descriptor;e[1]=spec;e[2]=mode;e[3]=callback;++ff_dummy_calls[0];
 return 0xf1000000u+card_event_count++;
}
sint32 ff_dummy_card_enable_event(uint32 handle) {
 uint32 i=handle-0xf1000000u;if(i>=card_event_count)return 0;
 ff_dummy_card_events[i][4]=1;++ff_dummy_calls[0];return 1;
}
/* WIP CdGetToc boundary: original disc metadata, no drive operation. */
sint32 ff_dummy_cd_get_toc(uint8 *destination) {
 uint32 i,j;++ff_dummy_calls[1];
 for(i=0;i<28;i++)for(j=0;j<3;j++)destination[4*i+j]=ff_cd_toc[i][j];
 return 27;
}
/* Explicit user-authorized dummy services. Callers must handle unavailable data;
 * do not report successful reads or consume uninitialized destination buffers. */
uint32 ff_dummy_calls[3];
sint32 ff_dummy_cd_search(void *file_info,const char *disc_name) {
 (void)file_info;(void)disc_name;++ff_dummy_calls[1];return 0; /* WIP physical CD directory: no fabricated sector metadata. */
}
uint8 ff_dummy_cd_mix_state[4],ff_dummy_cd_track_state[4];
/* WIP CD commands: retain requested location; synthesize ready status only.
 * No track playback, seek or media access is performed. */
sint32 ff_dummy_cd_control(uint32 command,const uint8 *parameter,uint8 *result) {
 uint32 i;++ff_dummy_calls[1];
 if(command==3&&parameter)for(i=0;i<4;i++)ff_dummy_cd_track_state[i]=parameter[i];
 if(command==1&&result)result[0]=0x80;
 return command==1||command==3||command==14;
}
/* WIP CdControl(9, NULL, NULL): no physical drive to pause. */
sint32 ff_dummy_cd_pause(void){++ff_dummy_calls[1];return 1;}
/* WIP CdMix: native CD audio is absent. Match the SDK's successful command
 * return without claiming that any media was read or any PCM was produced. */
sint32 ff_dummy_cd_mix(uint8 volume){ff_dummy_cd_mix_state[0]=ff_dummy_cd_mix_state[2]=volume;ff_dummy_cd_mix_state[1]=ff_dummy_cd_mix_state[3]=0;++ff_dummy_calls[1];return 1;}
sint32 ff_dummy_memory_card_read(sint32 slot,void *data,uint32 bytes) {
 (void)slot;(void)data;(void)bytes;++ff_dummy_calls[0];return -1;
}
sint32 ff_dummy_cd_read(uint32 sector,void *data,uint32 bytes) {
 (void)sector;(void)data;(void)bytes;++ff_dummy_calls[1];return -1;
}
sint32 ff_dummy_str_decode(const void *data,uint32 bytes) {
 (void)data;(void)bytes;++ff_dummy_calls[2];return 0; /* No decoded frames. */
}

#include <stdio.h>
#include <string.h>
/* WIP67554 boundary: exact extracted assets replace physical CD transfers. */
sint32 ff_dummy_archive_read(const char *disc_name,void *destination,uint32 capacity) {
 const char *path=NULL;char extracted[260];uint32 size=0;FILE *f;size_t got;int extra;
 ++ff_dummy_calls[1];
 if(!strcmp(disc_name,"\\MODFILES\\MODELSFE.WAD;1")){path="assets/MODELSFE.WAD";size=59180;}
 if(!strcmp(disc_name,"\\STGFILES\\STAGEFE.WAD;1")){path="assets/STAGEFE.WAD";size=511180;}
 if(!strcmp(disc_name,"\\FRONT\\LOADING.WAD;1")){path="assets/LOADING.WAD";size=175104;}
 if(!strcmp(disc_name,"\\FRONT\\TITLE.WAD;1")){path="assets/TITLE.WAD";size=175104;}
 if(!strcmp(disc_name,"\\FRONT\\ENGLISH.WAD;1")){path="assets/ENGLISH.WAD";size=3001;}
 if(!strcmp(disc_name,"\\FRONT\\FRENCH.WAD;1")){path="assets/FRENCH.WAD";size=3374;}
 if(!strcmp(disc_name,"\\FRONT\\GERMAN.WAD;1")){path="assets/GERMAN.WAD";size=3469;}
 if(!strcmp(disc_name,"\\SOUND\\SAMPSFE.WAD;1")){path="assets/SAMPSFE.WAD";size=143712;}
 /* Sequence 0 / level 0: exact original assets, verified by
  * tools/extract_gameplay_archives.py and status/gameplay/archives.json. */
 if(!strcmp(disc_name,"\\CUTFILES\\STAGEC1.WAD;1")){path="assets/STAGEC1.WAD";size=720156;}
 if(!strcmp(disc_name,"\\CUTFILES\\MODELSC1.WAD;1")){path="assets/MODELSC1.WAD";size=86964;}
 if(!strcmp(disc_name,"\\STGFILES\\STAGE11.WAD;1")){path="assets/STAGE11.WAD";size=753060;}
 if(!strcmp(disc_name,"\\MODFILES\\MODELS11.WAD;1")){path="assets/MODELS11.WAD";size=69788;}
 if(!strcmp(disc_name,"\\CHAR\\CHARS11.WAD;1")){path="assets/CHARS11.WAD";size=434464;}
 if(!strcmp(disc_name,"\\CHAR\\HAWK.WAD;1")){path="assets/HAWK.WAD";size=226700;}
 if(!strcmp(disc_name,"\\CHAR\\MACE.WAD;1")){path="assets/MACE.WAD";size=252796;}
 if(!strcmp(disc_name,"\\CHAR\\SMASHER.WAD;1")){path="assets/SMASHER.WAD";size=234540;}
 if(!strcmp(disc_name,"\\CHAR\\ALANA.WAD;1")){path="assets/ALANA.WAD";size=255596;}
 if(!strcmp(disc_name,"\\AID\\LEV11.MVS;1")){path="assets/LEV11.MVS";size=1868;}
 if(!strcmp(disc_name,"\\AII\\LEV11.INT;1")){path="assets/LEV11.INT";size=1840;}
 if(!strcmp(disc_name,"\\SOUND\\SAMPS11.WAD;1")){path="assets/SAMPS11.WAD";size=451912;}
 if(!path) {
  /* WIP physical CD: read only an extracted, root-relative ISO file. */
  const char *name=disc_name;size_t n=0,j;long length;
  if(*name=='\\'||*name=='/')++name;
  while(*name&&*name!=';'&&n+1<sizeof(extracted)) {
   unsigned char c=(unsigned char)*name++;
   if(c==':'||c<32)return 0;
   extracted[n++]=c=='\\'?'/':(char)c;
  }
  if(*name&&strcmp(name,";1"))return 0;
  extracted[n]=0;if(!n||extracted[0]=='/')return 0;
  for(j=0;j<n;j++)if((j==0||extracted[j-1]=='/')&&extracted[j]=='.'&&
    (extracted[j+1]=='/'||!extracted[j+1]||(extracted[j+1]=='.'&&(extracted[j+2]=='/'||!extracted[j+2]))))return 0;
  if(strncmp(extracted,"MODFILES/",9)&&strncmp(extracted,"STGFILES/",9)&&strncmp(extracted,"CUTFILES/",9)&&
     strncmp(extracted,"CHAR/",5)&&strncmp(extracted,"SOUND/",6)&&strncmp(extracted,"FRONT/",6)&&
     strncmp(extracted,"AID/",4)&&strncmp(extracted,"AII/",4)&&strncmp(extracted,"DEMOS/",6))return 0;
  f=fopen(extracted,"rb");if(!f)return 0;
  if(fseek(f,0,SEEK_END)||(length=ftell(f))<0||(unsigned long)length>capacity){fclose(f);return 0;}
  size=(uint32)length;if(fseek(f,0,SEEK_SET)){fclose(f);return 0;}
  got=fread(destination,1,size,f);extra=fgetc(f);fclose(f);return got==size&&extra==EOF?(sint32)size:0;
 }
 if(size>capacity){fprintf(stderr,"WIP: unsupported CD archive\n");return 0;}
 f=fopen(path,"rb");if(!f)return 0;got=fread(destination,1,size,f);extra=fgetc(f);fclose(f);
 return got==size&&extra==EOF?(sint32)size:0;
}
