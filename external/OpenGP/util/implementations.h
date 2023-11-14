#pragma once

#ifdef OPENGP_HEADERONLY

#ifdef OPENGP_IMPLEMENT_ALL_IN_THIS_FILE
#define OPENGP_IMPLEMENT_IMGUI_IN_THIS_FILE
#define OPENGP_IMPLEMENT_LODEPNG_IN_THIS_FILE
#endif

#ifdef OPENGP_IMPLEMENT_IMGUI_IN_THIS_FILE
#include <OpenGP/external/imgui/imgui.cpp>
#include <OpenGP/external/imgui/imgui_draw.cpp>
#include <OpenGP/external/imgui/ImGuizmo.cpp>
#endif

#ifdef OPENGP_IMPLEMENT_LODEPNG_IN_THIS_FILE
#include <OpenGP/external/LodePNG/lodepng.cpp>
#endif

#endif
