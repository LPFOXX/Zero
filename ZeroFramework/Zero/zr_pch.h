
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <utility>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <map>
#include <unordered_map>

#ifdef _MSC_VER
#pragma warning(disable:26451)
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GLM_FORCE_CXX11
#define GLM_FORCE_CTOR_INIT
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
