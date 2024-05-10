#include <iostream>

#include "scene.h"

int main() {
    scene s{"../resources/teapot.obj"};
    s.print_triangles();

    return 0;
}
