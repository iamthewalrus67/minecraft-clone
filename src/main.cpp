#include "app/app.hpp"

int main(int argc, char **argv) {
    App app{"Minecraft", 1024, 768};
    app.start();

    return 0;
}
