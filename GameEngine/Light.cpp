#include "Light.h"

Light::Light() : id(0), type("Point"), attenuation(1.0f, 0.09f, 0.032f), transform(0, "Light") {}
