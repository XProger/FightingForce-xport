#ifndef FF_WIP_H
#define FF_WIP_H
void ff_function_first_use(unsigned int pc,const char *image,const char *function);
/* One entry per process; stage records where the first call occurred. */
#define FF_FUNCTION_MARKER(pc,image) do { static int ff_seen; if(!ff_seen){ff_function_first_use((pc),(image),__FUNCTION__);ff_seen=1;} } while(0)
int ff_wip_options(int *argc,char **argv);
int ff_wip_visit(unsigned int pc,const char *subsystem,const char *function,const char *file,int line,int recoverable,const char *fallback);
__declspec(noreturn) void ff_wip_stop(const char *function,const char *file,int line);
#endif
