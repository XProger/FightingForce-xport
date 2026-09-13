#include "ff.h"
#include "wip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <process.h>
#include "platform/win/windows_compat.h"
#include <wincrypt.h>
#pragma comment(lib,"advapi32.lib")
static FILE *journal;
static FILE *function_journal;
static void close_function_journal(void){if(function_journal){fclose(function_journal);function_journal=NULL;}}
void ff_function_first_use(unsigned int pc,const char *image,const char *function){
 if(!function_journal){
  function_journal=fopen("functions-used.jsonl","ab");
  if(!function_journal){fprintf(stderr,"Cannot open functions-used.jsonl\n");abort();}
  atexit(close_function_journal);
  fprintf(function_journal,"{\"event\":\"session\",\"pid\":%d,\"time\":%lld}\n",_getpid(),(long long)time(NULL));
 }
 /* image and function are compile-time identifiers, never external strings. */
 fprintf(function_journal,"{\"event\":\"function_first_use\",\"pid\":%d,\"image\":\"%s\",\"pc\":\"%08X\",\"function\":\"%s\",\"stage\":%u,\"tick\":%u}\n",_getpid(),image,pc,function,ff_u32(0x80093d58),ff_u32(0x80093dd0));
 if(fflush(function_journal)||ferror(function_journal)){fprintf(stderr,"Function journal write failed\n");abort();}
}
static int continue_wip=1;
static unsigned long sequence,tainted;
static int executable_hash(char out[65]){
 char path[MAX_PATH];unsigned char data[16384],digest[32];DWORD size=32,n;size_t count;FILE *f;
 HCRYPTPROV provider=0;HCRYPTHASH hash=0;int ok=0,i;
 n=GetModuleFileNameA(NULL,path,sizeof(path));if(!n||n>=sizeof(path))return 0;
 f=fopen(path,"rb");if(!f)return 0;
 if(!CryptAcquireContextA(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT)||!CryptCreateHash(provider,CALG_SHA_256,0,0,&hash))goto done;
 while((count=fread(data,1,sizeof(data),f))!=0)if(!CryptHashData(hash,data,(DWORD)count,0))goto done;
 if(ferror(f)||!CryptGetHashParam(hash,HP_HASHVAL,digest,&size,0)||size!=32)goto done;
 for(i=0;i<32;i++)sprintf(out+2*i,"%02x",digest[i]);ok=1;
done:
 if(hash)CryptDestroyHash(hash);if(provider)CryptReleaseContext(provider,0);fclose(f);return ok;
}
static void close_journal(void){if(journal){fclose(journal);journal=NULL;}}
static void quoted(const char *s){
 fputc('"',journal);
 for(;*s;s++){unsigned char c=(unsigned char)*s;if(c=='"'||c=='\\')fputc('\\',journal);if(c<32)fprintf(journal,"\\u%04x",c);else fputc(c,journal);}
 fputc('"',journal);
}
__declspec(noreturn) void ff_wip_stop(const char *function,const char *file,int line){
 ff_wip_visit(0,"guard_or_unclassified",function,file,line,0,"abort");
 fprintf(stderr,"Fatal guard or unclassified WIP: %s (%s:%d)\n",function,file,line);
 abort();
}
int ff_wip_options(int *argc,char **argv){
 const char *path=NULL;char hash[65];int i,out=1,debug=0;
 for(i=1;i<*argc;i++){
  if(!strcmp(argv[i],"--wip-continue"))continue_wip=1;
  else if(!strcmp(argv[i],"--debug"))debug=1;
  else if(!strcmp(argv[i],"--wip-log")){
   if(++i>=*argc||!argv[i][0]){fprintf(stderr,"--wip-log requires a file path\n");return 0;}path=argv[i];
  }else argv[out++]=argv[i];
 }
 *argc=out;argv[out]=NULL;
 if(debug)continue_wip=0; /* Strict mode wins regardless of option order. */
 if(!path&&!continue_wip)return 1;
 if(!executable_hash(hash)){fprintf(stderr,"Cannot fingerprint FF.exe for WIP journal\n");return 0;}
 if(!path)path="wip-branches.jsonl";
 journal=fopen(path,"ab");if(!journal){fprintf(stderr,"Cannot open WIP journal: %s\n",path);return 0;}
 atexit(close_journal);
 fprintf(journal,"{\"event\":\"session\",\"pid\":%d,\"time\":%lld,\"continue\":%s,\"build\":",_getpid(),(long long)time(NULL),continue_wip?"true":"false");
 quoted(__DATE__ " " __TIME__);fputs(",\"exe_sha256\":",journal);quoted(hash);fputs("}\n",journal);
 if(fflush(journal)||ferror(journal)){fprintf(stderr,"Cannot write WIP journal\n");return 0;}
 if(continue_wip)fprintf(stderr,"WIP discovery enabled: skipped logic may invalidate later gameplay. Journal: %s\n",path);
 return 1;
}
int ff_wip_visit(unsigned int pc,const char *subsystem,const char *function,const char *file,int line,int recoverable,const char *fallback){
 unsigned int i,actor;int resume=continue_wip&&recoverable;
 if(journal){
  fprintf(journal,"{\"event\":\"wip\",\"pid\":%d,\"sequence\":%lu,\"time\":%lld,\"pc\":\"%08X\",\"subsystem\":",_getpid(),++sequence,(long long)time(NULL),pc);quoted(subsystem);
  fputs(",\"function\":",journal);quoted(function);fputs(",\"file\":",journal);quoted(file);
  fprintf(journal,",\"line\":%d,\"tick\":%u,\"stage\":%u,\"input\":[%u,%u],\"continued\":%s,\"prior_skips\":%lu,\"fallback\":",line,ff_u32(0x80093dd0),ff_u32(0x80093d58),ff_u32(0x80093a10),ff_u32(0x80093a14),resume?"true":"false",tainted);quoted(resume?fallback:"abort");
  fputs(",\"actors\":[",journal);
  for(i=0;i<8;i++){
   actor=0x800b4318+244u*i;
   fprintf(journal,"%s{\"slot\":%u,\"type\":%d,\"state\":%u,\"animation_frame\":%d,\"hp\":%d,\"xyz\":[%d,%d,%d]}",i?",":"",i,ff_s16(actor+52),ff_u32(actor+12),(sint32)ff_u32(actor+4),ff_s16(actor+66),(sint32)ff_u32(actor+40),(sint32)ff_u32(actor+44),(sint32)ff_u32(actor+48));
  }
  fputs("]}\n",journal);
  if(fflush(journal)||ferror(journal)){fprintf(stderr,"WIP journal write failed; stopping to avoid losing evidence\n");abort();}
 }
 if(resume)++tainted;
 return resume;
}
