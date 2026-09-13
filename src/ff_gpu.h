#ifndef FF_GPU_H
#define FF_GPU_H
#include "psx.h"
void ff_gpu_set_reverse(uint32 enabled);
sint32 ff_gpu_store_image(uint32 rect,uint32 destination);
sint32 ff_gpu_load_rect(uint32 source,sint32 x,sint32 y,sint32 w,sint32 h);
sint32 ff_gpu_load_image(uint32 rect,uint32 source);
sint32 ff_gpu_load_clut(uint32 source,sint32 x,sint32 y);
void ff_gpu_clear_menu_surfaces(void);
void ff_gpu_reset_graph_state(void);
uint32 ff_gpu_set_def_draw_env(uint32 env,sint32 x,sint32 y,sint32 w,sint32 h);
uint32 ff_gpu_set_def_disp_env(uint32 env,sint32 x,sint32 y,sint32 w,sint32 h);
void ff_gpu_begin(void);
void ff_gpu_draw_env(uint32 env,sint32 surface_x,sint32 surface_y);
void ff_gpu_put_draw_env(uint32 env,sint32 surface_x,sint32 surface_y);
void ff_gpu_packet(void *packet);
sint32 ff_gpu_ot(uint32 head);
sint32 ff_gpu_present(void);
void ff_gpu_display_offset(sint32 x,sint32 y);
sint32 ff_gpu_load_vram(const char *path);
void ff_gpu_init_empty(void);
sint32 ff_gpu_save_frame(const char *path);
#endif
