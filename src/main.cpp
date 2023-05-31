#include "app/app.hpp"

int main(int argc, char **argv) {
    App app{"Minecraft", 1024, 768};

    app.init();
    app.start();
    app.terminate();

    return 0;
}
