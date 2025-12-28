#include "player.h"

void Player::Draw() {
    // Draw using pixel coordinates
    int x = (int)(posX);
    int y = (int)(posY);
    int x1 = (int)(posX + halfSizeX);
    int y1 = (int)(posY + halfSizeY);
    PixelDrawRect(x, y, x1, y1, color);
}

void Player::Move(float deltaTime) {
    // Follow the currently computed path (path is set once on click via SetTarget)
    FollowPath(deltaTime);
}

void Player::SetTarget(float x, float y) {
    if(!gridGenerated) {
        GenerateNodeGrid();
        gridGenerated = true;
    }

    path.clear();
    path = GeneratePath(FindClosestNode(posX, posY), FindClosestNode(x, y));
    logger.Log(LOG_INFO, "player node: " + std::to_string(FindClosestNode(posX, posY)->posX) + ", " + std::to_string(FindClosestNode(posX, posY)->posY));
    logger.Log(LOG_INFO, "target node: " + std::to_string(FindClosestNode(x, y)->posX) + ", " + std::to_string(FindClosestNode(x, y)->posY));
}

void Player::FollowPath(float deltaTime) {
    if (path.size() < 2) return;

    const float arrivalThreshold = 0.01f;

    while (path.size() > 1) {
        Node* nextNodeCheck = path[1];
        float dx = nextNodeCheck->posX - posX;
        float dy = nextNodeCheck->posY - posY;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < arrivalThreshold) {
            path.erase(path.begin());
        } else {
            break;
        }
    }

    if (path.size() < 2) return;

    Node* nextNode = path[1];
    float dirX = nextNode->posX - posX;
    float dirY = nextNode->posY - posY;
    float length = sqrt(dirX * dirX + dirY * dirY);

    if (length != 0) {
        dirX /= length;
        dirY /= length;
    }

    posX += dirX * speed * deltaTime;
    posY += dirY * speed * deltaTime;
}