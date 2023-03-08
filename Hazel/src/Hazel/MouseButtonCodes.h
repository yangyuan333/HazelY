#pragma once

// 此处需要利用premake定义平台相关的宏，进行控制
// 类似windows平台上与glfw适配，其余平台与类似的库适配
// 因为Input类是基类，它会派生出与平台相关的类，此时项目只会编译特定平台代码，从而保证了代码执行一致性。

// From glfw3.h
#define HZ_MOUSE_BUTTON_1         0
#define HZ_MOUSE_BUTTON_2         1
#define HZ_MOUSE_BUTTON_3         2
#define HZ_MOUSE_BUTTON_4         3
#define HZ_MOUSE_BUTTON_5         4
#define HZ_MOUSE_BUTTON_6         5
#define HZ_MOUSE_BUTTON_7         6
#define HZ_MOUSE_BUTTON_8         7
#define HZ_MOUSE_BUTTON_LAST      HZ_MOUSE_BUTTON_8
#define HZ_MOUSE_BUTTON_LEFT      HZ_MOUSE_BUTTON_1
#define HZ_MOUSE_BUTTON_RIGHT     HZ_MOUSE_BUTTON_2
#define HZ_MOUSE_BUTTON_MIDDLE    HZ_MOUSE_BUTTON_3