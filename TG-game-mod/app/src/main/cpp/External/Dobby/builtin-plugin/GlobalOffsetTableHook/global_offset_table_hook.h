#ifndef DOBBY_GLOBAL_OFFSET_TABLE_HOOK
#define DOBBY_GLOBAL_OFFSET_TABLE_HOOK

#ifdef __cplusplus
extern "C" {
#endif

int DobbyGlobalOffsetTableReplace(char *image_name, char *symbol_name, void *fake_func, void **orig_func);

#ifdef __cplusplus
}
#endif

#endif