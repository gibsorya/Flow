typedef struct Platform Platform;
struct Platform {

};
static Platform *platform = 0;

#define APP_LOAD(name) void name(Platform *platform_)