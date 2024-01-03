
#define compose_class_name(prefix, name, postfix) prefix ## _api_ ## name ## postfix
#define settings_class_name(prefix, name) compose_class_name(prefix, name, _settings)
#define impl_class_name(prefix, name) compose_class_name(prefix, name, _impl)



#define Xw(name) X(name, window)

#define X_WINAPI Xw(winapi)
#define X_XLIB Xw(xlib)
#define X_WAYLAND Xw(wayland)

#ifdef _WIN32
#define Xw_WINAPI X_WINAPI
#else
#define Xw_WINAPI
#endif

#ifdef __linux__
#define Xw_XLIB X_XLIB
#else
#define Xw_XLIB
#endif

#ifdef __linux__
#define Xw_WAYLAND X_WAYLAND
#else
#define Xw_WAYLAND
#endif

#define WINDOW_API_XLIST Xw_WAYLAND Xw_XLIB Xw_WINAPI



#define Xg(name) X(name, graphics)

#define X_DIRECTX Xg(directx)
#define X_OPENGL Xg(opengl)
#define X_VULKAN Xg(vulkan)

#define Xg_NONE Xg(none)

#ifdef _WIN32
#define Xg_DIRECTX X_DIRECTX
#else
#define Xg_DIRECTX
#endif

#ifdef LIBKSN_ENABLE_OPENGL
#define Xg_OPENGL X_OPENGL
#else
#define Xg_OPENGL
#endif

#ifdef LIBKSN_ENABLE_VULKAN
#define Xg_VULKAN X_VULKAN
#else
#define Xg_VULKAN
#endif

#define GRAPHICS_API_XLIST Xg_NONE Xg_DIRECTX Xg_OPENGL Xg_VULKAN
