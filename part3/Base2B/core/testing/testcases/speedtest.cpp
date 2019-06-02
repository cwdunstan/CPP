#include "speedtest.h"
#include "coordinate.h"

SpeedTest::SpeedTest() : TestRunner("SpeedTest") {
    stickman = std::make_unique<JumpingStickman>(50);
    stickman->setSprite(":sprites/sprite0.png");
    stickman->setCoordinate(Coordinate(50, 50, 450));
    stickman->setSize("normal");
    stickman->setMoving(true);

    obstacles.push_back(std::move(std::make_unique<speedBoost>(Coordinate(400, 50, 450), 2)));
    obstacles.push_back(std::move(std::make_unique<Bird>(Coordinate(500, 50, 450), 2)));
    obstacles.push_back(std::move(std::make_unique<Bird>(Coordinate(530, 150, 450), 2)));
    obstacles.push_back(std::move(std::make_unique<Bird>(Coordinate(560, 250, 450), 2)));

}

void SpeedTest::update() {
    stickman->update(obstacles);
    if (stickman->isColliding()) {
        stickman->jump();
        stickman->jump();
        stickman->jump();
    }

    for (auto &o : obstacles) {
        o->collisionLogic(*stickman);
        if (o->getCoordinate().getXCoordinate() < 0) {
            status = Status::Passed;
        }
    }


}

void SpeedTest::render(Renderer &renderer) {
    stickman->render(renderer, counter++);

    for (auto &o : obstacles) {
        o->render(renderer, counter);
    }
}
