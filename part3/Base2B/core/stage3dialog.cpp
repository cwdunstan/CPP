#include "stage3dialog.h"
#include "collision.h"
#include "debugrenderer.h"
#include <QMessageBox>

Stage3Dialog::Stage3Dialog(Game &game, std::unique_ptr<Stickman> stickman, std::unique_ptr<EntityFactory> factory, std::vector<std::pair<std::unique_ptr<Entity>, int>> obstacleLayout, std::vector<level> stageLevels) :
    Dialog(game, std::move(stickman), std::move(factory)), obstacleLayout(std::move(obstacleLayout)), distanceToSpawn(0), nextObstacle(0), stageLevels(std::move(stageLevels)) {

}

void Stage3Dialog::spawnObstacles(unsigned int /*counter*/) {
    // Check if it's time to spawn an obstacle
    if (obstacleLayout.size() == 0 || distanceToSpawn > 0) return;

    auto &e = obstacleLayout[nextObstacle];

    // Check for collisions between next obstacle and current obstacles
    bool isOverlapping = false;
    for (auto &o : obstacles) {
        if (Collision::overlaps(*e.first, *o)) {
            isOverlapping = true;
            break;
        }
    }

    // Only spawn the obstacle if it isn't colliding with anything
    if (!isOverlapping) {
        auto obs = e.first->clone();
        obs->setVelocity(background.getVelocity());
        addObstacle(std::move(obs));
    }

    // Set next obstacle in sequence
    distanceToSpawn = e.second;
    nextObstacle = (nextObstacle + 1) % obstacleLayout.size();
}


void Stage3Dialog::update() {
    stickman->update(obstacles);
    if (!stickman->isColliding() && stickman->isMoving()) {
        hasCollided=false;
        // Reduce distance to next obstacle
        if(stickman->isMovingRight()){
            background.setBackwards(false);
            score.increment();
        }
        if(stickman->isMovingLeft()){
            background.setBackwards(true);
        }
        distanceToSpawn -= background.getVelocity();
        background.update();
        speedUp(counter);
        spawnObstacles(counter);
    }
    QMessageBox msgBox;
     msgBox.setText(QString::number(stageLevels[0].getTemp()));
    if (stickman->isMoving() && stickman->isColliding()) {
        if (!hasCollided) {
            lives.decrement();

            msgBox.exec();
            if (lives.getLives() == 0) {

            }

        }
        hasCollided=true;
    }


    for (auto &c : clouds) {
        c->collisionLogic(*stickman);
    }

    for (auto &o : obstacles) {
        o->collisionLogic(*stickman);
    }
}


void Stage3Dialog::setLevel(level *newLevel){
//    delete currLevel;
//    this->currLevel=newLevel;
}
