#include "stage3dialog.h"
#include "collision.h"
#include "debugrenderer.h"
#include <QMessageBox>

Stage3Dialog::Stage3Dialog(Game &game, std::unique_ptr<Stickman> stickman, std::unique_ptr<EntityFactory> factory, std::vector<std::pair<std::unique_ptr<Entity>, int>> obstacleLayout, std::vector<level> stageLevels) :
    Dialog(game, std::move(stickman), std::move(factory)), obstacleLayout(std::move(obstacleLayout)), distanceToSpawn(0), nextObstacle(0), stageLevels(std::move(stageLevels)) {
    currLevel=&this->stageLevels[0];
}

void Stage3Dialog::spawnObstacles(unsigned int /*counter*/) {
    // Check if it's time to spawn an obstacle
    if (currLevel->obstacleLayout.size() == 0 || distanceToSpawn > 0) return;
    if(nextObstacle<obstacleLayout.size()) {
        auto &e = currLevel->obstacleLayout[nextObstacle];

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
            obs->setOffset(e.second);
            addObstacle(std::move(obs) );
        }

        // Set next obstacle in sequence
        distanceToSpawn = e.second;
        nextObstacle++;
    }
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

    if (stickman->isMoving() && stickman->isColliding()) {
        if (!hasCollided) {
            //reset obstacles
            while(obstacles.size()>0){
                obstacles.pop_back();
            }
            distanceToSpawn=0;
            nextObstacle=0;

            //decrement lives
            lives.decrement();

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

