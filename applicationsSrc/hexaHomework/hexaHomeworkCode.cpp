#include "hexaHomeworkCode.hpp"

HexaHomeworkCode::HexaHomeworkCode(HexanodesBlock *host):HexanodesBlockCode(host),module(host) {
    // @warning Do not remove block below, as a blockcode with a NULL host might be created
    //  for command line parsing
    if (not host) return;

    // Registers a callback (myGoFunc) to the message of type O
    addMessageEventFunc2(GO_MSG_ID,
                       std::bind(&HexaHomeworkCode::myGoFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myBackFunc) to the message of type C
    addMessageEventFunc2(BACK_MSG_ID,
                       std::bind(&HexaHomeworkCode::myBackFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

    // Registers a callback (myMoveFunc) to the message of type E
    addMessageEventFunc2(MOVE_MSG_ID,
                       std::bind(&HexaHomeworkCode::myMoveFunc,this,
                       std::placeholders::_1, std::placeholders::_2));

}

void HexaHomeworkCode::startup() {
    console << "start " << getId() << "\n";

    if (isInTarget())
        module->setColor(ORANGE);

    if (isLeader)
    {
        module->setColor(BLUE);
        distance = 0;

        if (isInTarget())
        {
            waitingMessage = sendMessageToAllNeighbors("Go", new MessageOf<int>(GO_MSG_ID, distance), 1000, 100, 0);
        }
        else
        {
            module->setColor(YELLOW);
            vector<HexanodesMotion *> tab = HexanodesWorld::getWorld()->getAllMotionsForModule(module);
            vector<HexanodesMotion *>::const_iterator ci = tab.begin();
            while (ci != tab.end() && (*ci)->direction != motionDirection::CW)
            {
                ci++;
            }
            if (ci != tab.end())
            {
                initialPosition = module->position;
                auto orient = (*ci)->getFinalOrientation(module->orientationCode);
                Cell3DPosition destination = (*ci)->getFinalPos(module->position);
                scheduler->schedule(new HexanodesMotionStartEvent(scheduler->now() + 10000, module, destination, orient));
            }
        }
    }
    else
    {
        distance = -1;
    }
}
void HexaHomeworkCode::myGoFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    receivedMove = false;

    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());
    int d = *msg->getData();
    if (parent == nullptr)
    {
        distance = d + 1;
        parent = sender;

        waitingMessage = sendMessageToAllNeighbors("Go", new MessageOf<int>(GO_MSG_ID, distance), 1000, 100, 1, parent);

        if (waitingMessage == 0)
        {
            pair<int, int> test;
            if (target && !isInTarget())
            {
                test = make_pair(module->blockId, distance);
            }
            else
            {
                test = make_pair(-1, -1);
            }

            sendMessage("Back", new MessageOf<pair<int, int>>(BACK_MSG_ID, test), sender, 1000, 100);
        }
    }
    else
    {
        sendMessage("Back2parent", new MessageOf<pair<int, int>>(BACK_MSG_ID, make_pair(-1, -1)), sender, 1000, 100);
    }

}

void HexaHomeworkCode::myBackFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    waitingMessage--;

    MessageOf<pair<int, int>> *msg = static_cast<MessageOf<pair<int, int>> *>(_msg.get());
    pair<int, int> tmp = *msg->getData();
    if (tmp.second != -1)
    {

        if (tmp.second > farthestNode.second)
        {
            farthestNode = tmp;
        }
    }

    if (waitingMessage == 0)
    {

        if (isLeader)
        {
            sendMessageToAllNeighbors("Move", new MessageOf<int>(MOVE_MSG_ID, farthestNode.first), 1000, 100, 0);
            isLeader = false;
            module->setColor(ORANGE);
        }
        else
        {
            if (farthestNode.second < distance && !isInTarget())
                sendMessage("Back", new MessageOf<pair<int, int>>(BACK_MSG_ID, make_pair(module->blockId, distance)), parent, 1000, 100);
            else
                sendMessage("Back", new MessageOf<pair<int, int>>(BACK_MSG_ID, make_pair(farthestNode.first, farthestNode.second)), parent, 1000, 100);
        }
    }

}

void HexaHomeworkCode::myMoveFunc(std::shared_ptr<Message>_msg, P2PNetworkInterface*sender) {
    if (receivedMove)
        return;

    if (isInTarget())
        module->setColor(ORANGE);
    else
        module->setColor(WHITE);

    receivedMove = true;
    MessageOf<int> *msg = static_cast<MessageOf<int> *>(_msg.get());
    int id = *msg->getData();
    waitingMessage = 0;
    farthestNode = make_pair(-1, -1);
    sendMessageToAllNeighbors("Move", new MessageOf<int>(MOVE_MSG_ID, id), 1000, 100, 1, sender);
    if (module->blockId == id)
    {
        module->setColor(GREEN);
        vector<HexanodesMotion *> tab = HexanodesWorld::getWorld()->getAllMotionsForModule(module);
        vector<HexanodesMotion *>::const_iterator ci = tab.begin();
        while (ci != tab.end() && (*ci)->direction != motionDirection::CW)
        {
            ci++;
        }
        if (ci != tab.end())
        {
            initialPosition = module->position;
            auto orient = (*ci)->getFinalOrientation(module->orientationCode);
            Cell3DPosition destination = (*ci)->getFinalPos(module->position);
            scheduler->schedule(new HexanodesMotionStartEvent(scheduler->now() + 10000, module, destination, orient));
        }
    }

    parent = nullptr;
}


void HexaHomeworkCode::parseUserBlockElements(TiXmlElement *config) {
    const char *attr = config->Attribute("leader");
    isLeader = (attr?Simulator::extractBoolFromString(attr):false);
    if (isLeader) {
        std::cout << getId() << " is leader!" << std::endl; // complete with your code
    }
}

void HexaHomeworkCode::onMotionEnd() {
    motionCounter++;
    if (!isClockwise)
    {
        isLeader = true;
        module->setColor(ORANGE);
        nbNeighbors = -1;
        isClockwise = true;
        waitingMessage = sendMessageToAllNeighbors("Go", new MessageOf<int>(GO_MSG_ID, distance), 1000, 100, 0);
        return;
    }

    if (isInTarget() && nbNeighbors > module->getNbNeighbors())
    {
        vector<HexanodesMotion *> tab = HexanodesWorld::getWorld()->getAllMotionsForModule(module);
        vector<HexanodesMotion *>::const_iterator ci = tab.begin();
        while (ci != tab.end() && (*ci)->direction != motionDirection::CCW)
        {
            ci++;
        }
        if (ci != tab.end())
        {
            auto orient = (*ci)->getFinalOrientation(module->orientationCode);
            Cell3DPosition destination = (*ci)->getFinalPos(module->position);
            isClockwise = false;
            scheduler->schedule(new HexanodesMotionStartEvent(scheduler->now() + 10000, module, destination, orient));
        }
        return;
    }
    else
    {
        vector<HexanodesMotion *> tab = HexanodesWorld::getWorld()->getAllMotionsForModule(module);
        vector<HexanodesMotion *>::const_iterator ci = tab.begin();
        while (ci != tab.end() && (*ci)->direction != motionDirection::CW)
        {
            ci++;
        }
        if (ci != tab.end())
        {
            auto orient = (*ci)->getFinalOrientation(module->orientationCode);
            Cell3DPosition destination = (*ci)->getFinalPos(module->position);
            if (isInTarget() && !isPositionInTarget(destination))
            {

                isLeader = true;
                module->setColor(ORANGE);
                nbNeighbors = -1;
                isClockwise = true;
                waitingMessage = sendMessageToAllNeighbors("Go", new MessageOf<int>(GO_MSG_ID, distance), 1000, 100, 0);
                return;
            }
            nbNeighbors = module->getNbNeighbors();
            scheduler->schedule(new HexanodesMotionStartEvent(scheduler->now() + 10000, module, destination, orient));
        }
        if (initialPosition == module->position)
        {
            return;
        }
    }
}

bool HexaHomeworkCode::isInTarget()
{
    if (target && target->isInTarget(module->position))
    {
        return true;
    }
    return false;
}

bool HexaHomeworkCode::isPositionInTarget(Cell3DPosition position)
{
    if (target && target->isInTarget(position))
    {
        return true;
    }
    return false;
}

//isConnector
pair<bool,Cell3DPosition> HexaHomeworkCode::isConnector() {
    int n = 0;
    while (n < HHLattice::MAX_NB_NEIGHBORS &&
            ( !lattice->isFree(module->position + (static_cast<HHLattice *>(lattice))->getNeighborRelativePos(
                    static_cast<HHLattice::Direction>(n)))
                    ||
           !target->isInTarget(module->position + (static_cast<HHLattice *>(lattice))->getNeighborRelativePos(
                   static_cast<HHLattice::Direction>(n))))) {
        n++;
    }
    return (n < HHLattice::MAX_NB_NEIGHBORS?
      make_pair(true,module->position + (static_cast<HHLattice *>(lattice))->getNeighborRelativePos(
              static_cast<HHLattice::Direction>(n)))
              :make_pair(false,Cell3DPosition(0,0,0)));
}

string HexaHomeworkCode::onInterfaceDraw() {
  return "Number of motions: " + to_string(motionCounter);
}
